/* SPDX-License-Identifier: GPL-2.0-only */
/* Real Linux 7.3 resource owner; no automatic registration or physical action.
 * The SRAM is RAM-like storage accessed through a single forced-uncached MMU
 * mapping. Core metadata uses normal C loads/stores on that mapping only;
 * device-visible records use io_map plus explicit physical/bus translations.
 */
#include <linux/errno.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/log2.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/overflow.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <asm/page.h>
#include "om73_sram_backend.h"

enum om73_sram_state { OM73_CLOSED, OM73_OPENING, OM73_ACTIVE, OM73_CLOSING };
struct om73_sram_owned {
 const void *owner;
 void *raw;
 struct om73_sram_allocation allocation;
};

static DEFINE_MUTEX(om73_sram_lifecycle);
static DEFINE_SPINLOCK(om73_sram_lock);
static struct om_sram_core om73_core;
static struct om_sram_contract om73_arena;
static struct om73_sram_owned om73_owned[OM_SRAM_MAX_LIVE];
static enum om73_sram_state om73_state;
static struct resource *om73_lease;
static void __iomem *om73_map;
static u32 om73_generation;
static struct om73_sram_writer_token *om73_writer_token;
static struct task_struct *om73_writer_task;
static u64 om73_writer_generation;
static u32 om73_writer_cpu;
static int om73_writer_reentrant(void);


static bool om73_pin_present(const u8 hash[32])
{
 u32 i;
 for (i = 0; i < 32; i++)
  if (hash[i])
   return true;
 return false;
}

static int om73_validate_board(const struct om73_sram_board_contract *board)
{
 const struct om_sram_contract *c;
 u32 i;
 if (!board)
  return -EINVAL;
 c = &board->arena;
 if (c->enabled != 1 || c->reserved_exclusive != 1 || c->complete_occupancy_map != 1 ||
     c->cpu_base != 0x88000000U || c->bus_base != 0x80000000U ||
     c->uncached_base != 0x60000000U || c->total_bytes != 0x80000U ||
     !c->kernel_occupied_end || c->start < c->kernel_occupied_end ||
     c->start >= c->end || c->end > c->total_bytes ||
     !IS_ALIGNED(c->start, PAGE_SIZE) || !IS_ALIGNED(c->end, PAGE_SIZE) ||
     c->noccupied > OM_SRAM_MAX_OCCUPIED ||
     !om73_pin_present(board->kernel_occupancy_sha256) ||
     !om73_pin_present(board->selected_carriers_sha256) ||
     !om73_pin_present(board->complete_writers_map_sha256))
  return -EINVAL;
 for (i = 0; i < c->noccupied; i++) {
  const struct om_sram_range *r = &c->occupied[i];
  if (r->start >= r->end || r->end > c->total_bytes ||
      (c->start < r->end && c->end > r->start))
   return -EINVAL;
 }
 return 0;
}

int om73_sram_acquire(const struct om73_sram_board_contract *board)
{
 struct om_sram_contract arena;
 struct resource *lease;
 void __iomem *mapping;
 unsigned long flags;
 resource_size_t physical, length;
 int error;

 error = om73_writer_reentrant();
 if (error)
  return error;
 error = om73_validate_board(board);
 if (error)
  return error;
 /* Snapshot the admitted immutable board contract before any resource write. */
 arena = board->arena;
 physical = (resource_size_t)arena.cpu_base + arena.start;
 length = arena.end - arena.start;
 mutex_lock(&om73_sram_lifecycle);
 spin_lock_irqsave(&om73_sram_lock, flags);
 if (om73_state != OM73_CLOSED) {
  spin_unlock_irqrestore(&om73_sram_lock, flags);
  mutex_unlock(&om73_sram_lifecycle);
  return -EBUSY;
 }
 om73_state = OM73_OPENING;
 spin_unlock_irqrestore(&om73_sram_lock, flags);

 /* If built as a module, keep its code/metadata alive for the arena lifetime. */
 if (!try_module_get(THIS_MODULE)) {
  error = -ENODEV;
  goto closed;
 }

 lease = request_mem_region_exclusive(physical, length, "openmitrastar-sram73");
 if (!lease) {
  error = -EBUSY;
  goto unpin_closed;
 }
 mapping = ioremap(physical, length);
 if (!mapping) {
  release_mem_region(physical, length);
  error = -ENOMEM;
  goto unpin_closed;
 }
 /* SRAM is RAM-like memory, never MMIO registers. This is the only __force
  * cast used by the recovered core; no cached/identity alias is dereferenced. */
 error = om_sram_core_init(&om73_core, (__force void *)mapping, length, &arena);
 if (error) {
  iounmap(mapping);
  release_mem_region(physical, length);
  goto unpin_closed;
 }
 spin_lock_irqsave(&om73_sram_lock, flags);
 om73_arena = arena;
 om73_lease = lease;
 om73_map = mapping;
 memset(om73_owned, 0, sizeof(om73_owned));
 om73_state = OM73_ACTIVE;
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 mutex_unlock(&om73_sram_lifecycle);
 return 0;

unpin_closed:
 module_put(THIS_MODULE);
closed:
 spin_lock_irqsave(&om73_sram_lock, flags);
 om73_state = OM73_CLOSED;
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 mutex_unlock(&om73_sram_lifecycle);
 return error;
}
EXPORT_SYMBOL_GPL(om73_sram_acquire);

static struct om73_sram_owned *om73_find_owner(const void *owner)
{
 u32 i;
 for (i = 0; i < OM_SRAM_MAX_LIVE; i++)
  if (om73_owned[i].owner == owner)
   return &om73_owned[i];
 return NULL;
}

int om73_sram_alloc(const void *owner, size_t bytes, unsigned int alignment,
                    struct om73_sram_allocation *out)
{
 struct om73_sram_owned *record = NULL;
 unsigned long flags, raw_address, aligned_address;
 phys_addr_t raw_physical, aligned_physical;
 size_t extent;
 u32 i, offset;
 void *raw;
 int error = -ENOMEM;

 if (!owner || !out || !bytes || !is_power_of_2(alignment) || alignment > PAGE_SIZE)
  return -EINVAL;
 if (check_add_overflow(bytes, (size_t)alignment - 1, &extent) || extent > U32_MAX)
  return -EOVERFLOW;
 spin_lock_irqsave(&om73_sram_lock, flags);
 if (om73_state != OM73_ACTIVE) {
  error = -ENODEV;
  goto done;
 }
 if (om73_find_owner(owner)) {
  error = -EBUSY;
  goto done;
 }
 for (i = 0; i < OM_SRAM_MAX_LIVE; i++)
  if (!om73_owned[i].owner) {
   record = &om73_owned[i];
   break;
  }
 if (!record)
  goto done;
 raw = om_sram_core_alloc(&om73_core, extent);
 if (!raw)
  goto done;
 raw_address = (unsigned long)raw;
 raw_physical = (phys_addr_t)om73_arena.cpu_base + om73_arena.start +
                (raw_address - (unsigned long)om73_map);
 aligned_physical = ALIGN(raw_physical, alignment);
 aligned_address = raw_address + (aligned_physical - raw_physical);
 offset = (u32)(aligned_address - (unsigned long)om73_map);
 if (offset > om73_arena.end - om73_arena.start ||
     bytes > om73_arena.end - om73_arena.start - offset) {
  om_sram_core_free(&om73_core, raw);
  error = -EINVAL;
  goto done;
 }
 record->raw = raw;
 record->owner = owner;
 record->allocation.cpu_phys = (phys_addr_t)om73_arena.cpu_base + om73_arena.start + offset;
 record->allocation.bus = (dma_addr_t)om73_arena.bus_base + om73_arena.start + offset;
 record->allocation.io_map = (__force void __iomem *)aligned_address;
 record->allocation.size = bytes;
 record->allocation.extent_offset = (u32)(raw_address - (unsigned long)om73_map);
 record->allocation.extent_bytes = extent;
 if (!++om73_generation)
  ++om73_generation;
 record->allocation.generation = om73_generation;
 *out = record->allocation;
 error = 0;
done:
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 return error;
}
EXPORT_SYMBOL_GPL(om73_sram_alloc);

int om73_sram_query(const void *owner, struct om73_sram_allocation *out)
{
 struct om73_sram_owned *record;
 unsigned long flags;
 int error = 0;
 if (!owner || !out)
  return -EINVAL;
 spin_lock_irqsave(&om73_sram_lock, flags);
 if (om73_state != OM73_ACTIVE)
  error = -ENODEV;
 else if (!(record = om73_find_owner(owner)))
  error = -ENOENT;
 else
  *out = record->allocation;
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 return error;
}
EXPORT_SYMBOL_GPL(om73_sram_query);

int om73_sram_free(const void *owner)
{
 struct om73_sram_owned *record;
 unsigned long flags;
 int error;
 if (!owner)
  return -EINVAL;
 spin_lock_irqsave(&om73_sram_lock, flags);
 if (om73_state != OM73_ACTIVE)
  error = -ENODEV;
 else if (!(record = om73_find_owner(owner)))
  error = -ENOENT;
 else {
  error = om_sram_core_free(&om73_core, record->raw);
  if (!error)
   memset(record, 0, sizeof(*record));
 }
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 return error;
}
EXPORT_SYMBOL_GPL(om73_sram_free);

int om73_sram_close(void)
{
 void __iomem *mapping;
 resource_size_t physical, length;
 unsigned long flags;
 int error;
 error = om73_writer_reentrant();
 if (error)
  return error;
 mutex_lock(&om73_sram_lifecycle);
 spin_lock_irqsave(&om73_sram_lock, flags);
 if (om73_state != OM73_ACTIVE) {
  error = -ENODEV;
  goto locked_done;
 }
 error = om_sram_core_close(&om73_core);
 if (error)
  goto locked_done;
 om73_state = OM73_CLOSING;
 mapping = om73_map;
 physical = om73_lease->start;
 length = resource_size(om73_lease);
 spin_unlock_irqrestore(&om73_sram_lock, flags);

 iounmap(mapping);
 release_mem_region(physical, length);
 spin_lock_irqsave(&om73_sram_lock, flags);
 om73_map = NULL;
 om73_lease = NULL;
 memset(&om73_arena, 0, sizeof(om73_arena));
 om73_state = OM73_CLOSED;
locked_done:
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 mutex_unlock(&om73_sram_lifecycle);
 if (!error)
  module_put(THIS_MODULE);
 return error;
}
EXPORT_SYMBOL_GPL(om73_sram_close);


static int om73_writer_reentrant(void)
{
 unsigned long flags;
 bool nested;
 spin_lock_irqsave(&om73_sram_lock, flags);
 nested = om73_writer_task == current;
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 return nested ? -EBUSY : 0;
}

int om73_sram_writer_begin(unsigned int cpu, struct om73_sram_writer_token *token)
{
 unsigned long flags;
 int error;
 if (!token || cpu > 2)
  return -EINVAL;
 error = om73_writer_reentrant();
 if (error)
  return error;
 /* All three processor writers share this mutex with pool acquire/close.
  * It is held by this task through hashes, preflight, map, copy and cleanup. */
 mutex_lock(&om73_sram_lifecycle);
 if (!try_module_get(THIS_MODULE)) {
  mutex_unlock(&om73_sram_lifecycle);
  return -ENODEV;
 }
 spin_lock_irqsave(&om73_sram_lock, flags);
 if (om73_writer_generation == U64_MAX) {
  spin_unlock_irqrestore(&om73_sram_lock, flags);
  mutex_unlock(&om73_sram_lifecycle);
  module_put(THIS_MODULE);
  return -EOVERFLOW;
 }
 token->generation = ++om73_writer_generation;
 token->cpu = cpu;
 om73_writer_cpu = cpu;
 om73_writer_token = token;
 om73_writer_task = current;
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 return 0;
}
EXPORT_SYMBOL_GPL(om73_sram_writer_begin);

static bool om73_writer_matches(const struct om73_sram_writer_token *token)
{
 return token && token == om73_writer_token && om73_writer_task == current &&
        token->generation == om73_writer_generation && token->cpu <= 2 &&
        token->cpu == om73_writer_cpu;
}

int om73_sram_writer_check(const struct om73_sram_writer_token *token,
                          phys_addr_t physical, size_t size)
{
 unsigned long flags;
 phys_addr_t end, arena_start, arena_end;
 int error = 0;
 if (!size || physical > U32_MAX || size - 1 > U32_MAX - physical)
  return -ERANGE;
 end = physical + size - 1;
 spin_lock_irqsave(&om73_sram_lock, flags);
 if (!om73_writer_matches(token))
  error = -EPERM;
 else if (om73_state == OM73_ACTIVE) {
  arena_start = (phys_addr_t)om73_arena.cpu_base + om73_arena.start;
  arena_end = (phys_addr_t)om73_arena.cpu_base + om73_arena.end - 1;
  /* The free list itself lives here even when live_count is zero. */
  if (physical <= arena_end && end >= arena_start)
   error = -EBUSY;
 }
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 return error;
}
EXPORT_SYMBOL_GPL(om73_sram_writer_check);

int om73_sram_writer_end(struct om73_sram_writer_token *token)
{
 unsigned long flags;
 spin_lock_irqsave(&om73_sram_lock, flags);
 if (!om73_writer_matches(token)) {
  spin_unlock_irqrestore(&om73_sram_lock, flags);
  return -EPERM;
 }
 om73_writer_task = NULL;
 om73_writer_token = NULL;
 memset(token, 0, sizeof(*token));
 spin_unlock_irqrestore(&om73_sram_lock, flags);
 mutex_unlock(&om73_sram_lifecycle);
 module_put(THIS_MODULE);
 return 0;
}
EXPORT_SYMBOL_GPL(om73_sram_writer_end);

/* Built-in provider is preferred. No module_init/module parameters admit an
 * arena; board registration and hardware resource proofs remain separate. */
MODULE_DESCRIPTION("OpenMitraStar typed exclusive SRAM73 backend");
MODULE_LICENSE("GPL");
