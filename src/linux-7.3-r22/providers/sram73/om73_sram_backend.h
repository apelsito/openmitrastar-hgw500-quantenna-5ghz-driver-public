/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef OM73_SRAM_BACKEND_H
#define OM73_SRAM_BACKEND_H
#include <linux/types.h>
#include <linux/ioport.h>
#include "om_sram_core.h"

/* Only a compiled kernel board caller may admit an arena. These pins identify
 * evidence verified by that caller; nonzero hashes alone never prove occupancy.
 * No module parameter, userspace ioctl, DT yes flag, or guessed free gap may
 * construct this admission contract. Registration is absent by default. */
struct om73_sram_board_contract {
 struct om_sram_contract arena;
 u8 kernel_occupancy_sha256[32];
 u8 selected_carriers_sha256[32];
 u8 complete_writers_map_sha256[32];
};

struct om73_sram_allocation {
 phys_addr_t cpu_phys;
 dma_addr_t bus;
 void __iomem *io_map;
 size_t size;
 u32 extent_offset; /* raw allocation offset, including alignment slack */
 u32 extent_bytes;  /* raw requested bytes, excludes OEM core block header */
 u32 generation;
};

/* acquire/close may sleep; call them only from serialized process context.
 * acquire owns the exclusive iomem resource lease and one NC mapping. The
 * mapping is never returned as an identity CPU/bus pointer. No DRAM fallback.
 * alloc/free/query perform no sleeping operations and use irqsave locking.
 * Callers serialize operations for each owner and quiesce devices before free.
 */
int om73_sram_acquire(const struct om73_sram_board_contract *board);
int om73_sram_close(void);
int om73_sram_alloc(const void *owner, size_t bytes, unsigned int alignment,
                    struct om73_sram_allocation *out);
int om73_sram_query(const void *owner, struct om73_sram_allocation *out);
int om73_sram_free(const void *owner);
#endif
