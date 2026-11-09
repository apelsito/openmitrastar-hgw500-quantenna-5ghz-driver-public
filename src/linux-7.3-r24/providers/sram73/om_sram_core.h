/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef OM_SRAM_CORE_H
#define OM_SRAM_CORE_H
#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/string.h>
#else
#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint32_t u32;
#endif
#define OM_SRAM_MAX_LIVE 128
#define OM_SRAM_MAX_OCCUPIED 16
struct om_sram_range { u32 start,end; }; /* offsets in the same physical SRAM */
struct om_sram_contract {
 u32 enabled,reserved_exclusive,complete_occupancy_map;
 u32 cpu_base,bus_base,uncached_base,total_bytes;
 u32 start,end,kernel_occupied_end,noccupied;
 struct om_sram_range occupied[OM_SRAM_MAX_OCCUPIED];
};
struct memblock { struct memblock *next; unsigned long length; };
struct om_sram_live { void *pointer; unsigned long bytes; };
struct om_sram_core {
 struct memblock memlist;
 unsigned char *begin,*end;
 struct om_sram_live live[OM_SRAM_MAX_LIVE];
 u32 active,live_count;
};
int om_sram_core_init(struct om_sram_core *,void *mapped,u32 mapped_bytes,
 const struct om_sram_contract *);
void *om_sram_core_alloc(struct om_sram_core *,unsigned long bytes);
int om_sram_core_free(struct om_sram_core *,void *pointer);
int om_sram_core_close(struct om_sram_core *);
#endif
