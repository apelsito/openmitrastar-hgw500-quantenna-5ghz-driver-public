/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef OM_SRAM_LIFETIME_H
#define OM_SRAM_LIFETIME_H
#include "om_sram_core.h"
#define OM_SRAM_RESET_BIT (1U<<3)
#define OM_NETSS_RESET_BIT (1U<<7)
#define OM_R20_ARC_RESET_MASK ((1U<<7)|(1U<<8)|(1U<<14))
struct om_sram_ring_binding { void *allocation;u32 bus_base,count,descriptor_bytes; };
struct om_sram_lifetime {
 struct om_sram_core *core;
 struct om_sram_contract contract;
 struct om_sram_ring_binding ring;
 u32 generation,pending_reset_mask,reset_asserted,clear_start,clear_end;
};
int om_sram_lifetime_bind(struct om_sram_lifetime *,struct om_sram_core *,
 const struct om_sram_contract *);
void *om_sram_lifetime_alloc(struct om_sram_lifetime *,unsigned long bytes);
int om_sram_lifetime_free(struct om_sram_lifetime *,void *pointer);
int om_sram_ring_published(struct om_sram_lifetime *,void *allocation,
 u32 bus_base,u32 count,u32 descriptor_bytes,u32 observed_base,u32 observed_count);
int om_sram_ring_unpublished(struct om_sram_lifetime *,u32 observed_base,
 u32 observed_count,u32 observed_csr);
int om_sram_reset_prepare(struct om_sram_lifetime *,u32 exact_mask);
int om_sram_reset_asserted(struct om_sram_lifetime *,u32 observed_mask,u32 observed_vector);
int om_sram_reset_released(struct om_sram_lifetime *,u32 observed_mask,u32 observed_vector);
int om_sram_clear_prepare(struct om_sram_lifetime *,u32 host_start,u32 bytes);
int om_sram_clear_complete(struct om_sram_lifetime *,u32 host_start,u32 bytes);
#endif
