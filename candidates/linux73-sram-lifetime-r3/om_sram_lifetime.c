/* SPDX-License-Identifier: GPL-2.0-only */
/* Source-only lifecycle bookkeeping. Call before/after exact observed writes;
 * this core neither performs MMIO nor proves a complete firmware writer map.
 * A trusted kernel provider still must own mapping/reservation and serialize
 * these events with allocation/free. No hook is connected to R20 here. */
#include "om_sram_lifetime.h"
#define EINVAL_ 22
#define EBUSY_ 16
#define EOVERFLOW_ 75
static int live_pointer(struct om_sram_lifetime *l,void *p,u32 base,u32 bytes)
{
 u32 i;unsigned long offset,userbytes;
 if(!l||!l->core||!l->core->active||!p)return 0;
 for(i=0;i<OM_SRAM_MAX_LIVE;i++)if(l->core->live[i].pointer==p)break;
 if(i==OM_SRAM_MAX_LIVE)return 0;
 offset=(unsigned char*)p-l->core->begin;
 userbytes=l->core->live[i].bytes-sizeof(struct memblock);
 if(base<l->contract.bus_base+l->contract.start+offset)return 0;
 offset=base-(l->contract.bus_base+l->contract.start+offset);
 return offset<=userbytes&&bytes<=userbytes-offset;
}
int om_sram_lifetime_bind(struct om_sram_lifetime *l,struct om_sram_core *core,
 const struct om_sram_contract *c)
{
 if(!l||!core||!c||!core->active||l->generation||
    c->enabled!=1||c->reserved_exclusive!=1||c->complete_occupancy_map!=1||
    c->start>=c->end||(unsigned long)(core->end-core->begin)!=c->end-c->start)return-EINVAL_;
 memset(l,0,sizeof(*l));l->core=core;l->contract=*c;l->generation=1;return 0;
}
void *om_sram_lifetime_alloc(struct om_sram_lifetime *l,unsigned long bytes)
{
 if(!l||!l->generation||l->pending_reset_mask||l->clear_end)return 0;
 return om_sram_core_alloc(l->core,bytes);
}
int om_sram_lifetime_free(struct om_sram_lifetime *l,void *pointer)
{
 if(!l||!l->generation||l->pending_reset_mask||l->clear_end)return-EINVAL_;
 if(pointer&&l->ring.allocation==pointer)return-EBUSY_;
 return om_sram_core_free(l->core,pointer);
}
int om_sram_ring_published(struct om_sram_lifetime *l,void *allocation,
 u32 bus_base,u32 count,u32 descriptor_bytes,u32 observed_base,u32 observed_count)
{
 u32 bytes;
 if(!l||!l->generation||l->pending_reset_mask||l->clear_end||
    l->ring.allocation||!count||!descriptor_bytes)return-EINVAL_;
 if(count>0xffffffffU/descriptor_bytes)return-EOVERFLOW_;
 bytes=count*descriptor_bytes;
 if(bus_base!=observed_base||count!=observed_count||
    !live_pointer(l,allocation,bus_base,bytes))return-EINVAL_;
 l->ring=(struct om_sram_ring_binding){allocation,bus_base,count,descriptor_bytes};return 0;
}
int om_sram_ring_unpublished(struct om_sram_lifetime *l,u32 base,u32 count,u32 csr)
{
 if(!l||!l->ring.allocation||base||count||!(csr&(1U<<31)))return-EINVAL_;
 memset(&l->ring,0,sizeof(l->ring));return 0;
}
int om_sram_reset_prepare(struct om_sram_lifetime *l,u32 exact_mask)
{
 if(!l||!l->core||!l->core->active||l->pending_reset_mask||l->clear_end)return-EINVAL_;
 /* Only observed R20 ARC mask, optionally SRAM bit for a future exact caller.
  * A domain reset with live owned memory cannot silently orphan consumers. */
 if(exact_mask!=OM_R20_ARC_RESET_MASK&&
    exact_mask!=(OM_R20_ARC_RESET_MASK|OM_SRAM_RESET_BIT))return-EINVAL_;
 if(l->ring.allocation||l->core->live_count)return-EBUSY_;
 if(l->generation==0xffffffffU)return-EOVERFLOW_;
 if(exact_mask&OM_SRAM_RESET_BIT){int rc=om_sram_core_close(l->core);if(rc)return rc;}
 l->pending_reset_mask=exact_mask;l->reset_asserted=0;return 0;
}
int om_sram_reset_asserted(struct om_sram_lifetime *l,u32 mask,u32 vector)
{
 if(!l||!l->pending_reset_mask||mask!=l->pending_reset_mask||
    vector!=l->pending_reset_mask)return-EINVAL_;
 l->reset_asserted=1;return 0;
}
int om_sram_reset_released(struct om_sram_lifetime *l,u32 mask,u32 vector)
{
 u32 destructive;
 if(!l||!l->reset_asserted||mask||vector!=l->pending_reset_mask)return-EINVAL_;
 destructive=l->pending_reset_mask&OM_SRAM_RESET_BIT;
 /* Physical restore must reinit core through its exact mapping/contract first.
  * This function never manufactures that proof from register values. */
 if(destructive&&(!l->core->active||l->core->live_count))return-EBUSY_;
 l->generation++;l->pending_reset_mask=0;l->reset_asserted=0;return 0;
}
int om_sram_clear_prepare(struct om_sram_lifetime *l,u32 host_start,u32 bytes)
{
 u32 start,end;
 if(!l||!l->core||!l->core->active||l->pending_reset_mask||l->clear_end||
    !bytes||host_start<l->contract.cpu_base)return-EINVAL_;
 start=host_start-l->contract.cpu_base;
 if(start>=l->contract.total_bytes||bytes>l->contract.total_bytes-start)return-EINVAL_;
 end=start+bytes;
 /* R20 clearsram ranges >=0x20000 do not overlap proposed12000..20000.
  * Never admit a direct write into the owned heap, even if currently empty. */
 if(start<l->contract.end&&end>l->contract.start)return-EBUSY_;
 l->clear_start=start;l->clear_end=end;return 0;
}
int om_sram_clear_complete(struct om_sram_lifetime *l,u32 host_start,u32 bytes)
{
 if(!l||!l->clear_end||host_start!=l->contract.cpu_base+l->clear_start||
    bytes!=l->clear_end-l->clear_start)return-EINVAL_;
 l->clear_start=l->clear_end=0;return 0;
}
