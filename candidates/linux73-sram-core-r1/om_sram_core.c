/* SPDX-License-Identifier: GPL-2.0-only */
/* Offline core: OEM2.6.35 qtn_memget/free first-fit split/coalescing derived
 * here; caller must serialize init/alloc/free/close. No hardware provider exists.
 * Admission requires an exclusive kernel reservation and a complete occupied
 * region map. User module parameters must never stand in for those proofs. */
#include "om_sram_core.h"
#define OM_EINVAL 22
#define OM_EBUSY 16
#define OM_ENOENT 2
#define OM_EOVERFLOW 75
#define OM_ENOSPC 28
#define roundmb(x) ((void *)((7UL+(unsigned long)(x))&~7UL))

int om_sram_core_init(struct om_sram_core *core,void *mapped,u32 mapped_bytes,
 const struct om_sram_contract *c)
{
 unsigned long p=(unsigned long)mapped;
 u32 i;
 if(!core||!c||!mapped)return-OM_EINVAL;
 if(core->active||core->live_count)return-OM_EBUSY;
 if(c->enabled!=1||c->reserved_exclusive!=1||c->complete_occupancy_map!=1)
  return-OM_EINVAL;
 /* The observed Linux/OEM flip mapping is the only prototype profile. */
 if(c->cpu_base!=0x88000000U||c->bus_base!=0x80000000U||
    c->uncached_base!=0x60000000U||c->total_bytes!=0x80000U)
  return-OM_EINVAL;
 if(!c->kernel_occupied_end||c->noccupied>OM_SRAM_MAX_OCCUPIED||
    c->start<c->kernel_occupied_end||c->start>=c->end||c->end>c->total_bytes||
    (c->start&7)||(c->end&7)||c->end-c->start<sizeof(struct memblock)||
    mapped_bytes!=c->end-c->start||(p&7)||p>~0UL-mapped_bytes)
  return-OM_EINVAL;
 for(i=0;i<c->noccupied;i++){
  if(c->occupied[i].start>=c->occupied[i].end||
     c->occupied[i].end>c->total_bytes)return-OM_EINVAL;
  if(c->start<c->occupied[i].end&&c->end>c->occupied[i].start)
   return-OM_EINVAL;
 }
 memset(core,0,sizeof(*core));core->begin=mapped;core->end=core->begin+mapped_bytes;
 core->memlist.next=(struct memblock*)mapped;core->memlist.length=mapped_bytes;
 core->memlist.next->next=0;core->memlist.next->length=mapped_bytes;
 core->active=1;return 0;
}
static void *om_sram_memget(struct om_sram_core *core,unsigned long nbytes)
{
	struct memblock *memlist = &core->memlist;
	register struct memblock *prev, *curr, *leftover;

	if(0 == nbytes)
	{
		return NULL;
	}

	/* Round to multiple of memblock size   */
	nbytes = (unsigned long) roundmb(nbytes);


	prev = memlist;
	curr = memlist->next;

	while(curr != NULL)
	{
		if(curr->length == nbytes)
		{
			prev->next = curr->next;
			memlist->length -= nbytes;

			goto return_curr;
		}
		else if(curr->length > nbytes)
		{
			/* Split block into two */
			leftover = (struct memblock *)((unsigned long) curr + nbytes);
			prev->next = leftover;
			leftover->next = curr->next;
			leftover->length = curr->length - nbytes;
			memlist->length -= nbytes;

			goto return_curr;
		}

		prev = curr;
		curr = curr->next;
	}

	return NULL;

return_curr:
	return (void *)curr;
}

static int om_sram_memfree(struct om_sram_core *core,void *memptr, unsigned long nbytes)
{
	struct memblock *memlist = &core->memlist;
	register struct memblock *block, *next, *prev;
	unsigned long top;

	block = (struct memblock *) memptr;
	nbytes = (unsigned long) roundmb(nbytes);


	prev = memlist;
	next = memlist->next;
	while((next != NULL) && (next < block))
	{
		prev = next;
		next = next->next;
	}

	/* Find top of previous memblock */
	if(prev == memlist)
	{
		top = (unsigned long) NULL;
	}
	else
	{
		top = (unsigned long) prev + prev->length;
	}

	/* Make sure block is not overlapping on prev or next blocks */
	if ((top > (unsigned long) block)
		|| ((next != NULL) && ((unsigned long) block + nbytes) > (unsigned long)next))
	{
			return -1;
	}

	memlist->length += nbytes;

	/* Coalesce with previous block if adjacent */
	if(top == (unsigned long) block)
	{
		prev->length += nbytes;
		block = prev;
	}
	else
	{
		block->next = next;
		block->length = nbytes;
		prev->next = block;
	}

	/* coalesce with next block if adjacent */
	if(next && ((unsigned long) block + block->length) == (unsigned long) next)
	{
		block->length += next->length;
		block->next = next->next;
	}


	return 0;
}
void *om_sram_core_alloc(struct om_sram_core *core,unsigned long bytes)
{
 struct memblock *block;unsigned long n;u32 slot;
 if(!core||!core->active||!bytes||bytes>~0UL-sizeof(*block)-7)return 0;
 for(slot=0;slot<OM_SRAM_MAX_LIVE;slot++)if(!core->live[slot].pointer)break;
 if(slot==OM_SRAM_MAX_LIVE)return 0;
 n=(bytes+sizeof(*block)+7)&~7UL;
 block=om_sram_memget(core,n);if(!block)return 0;
 block->next=block;block->length=n;
 core->live[slot].pointer=block+1;core->live[slot].bytes=n;core->live_count++;
 return block+1;
}
int om_sram_core_free(struct om_sram_core *core,void *pointer)
{
 struct memblock *block;u32 slot;unsigned long n;
 if(!core||!core->active)return-OM_EINVAL;
 if(!pointer)return 0;
 /* Match owned identity before reading any caller-supplied address. */
 for(slot=0;slot<OM_SRAM_MAX_LIVE;slot++)if(core->live[slot].pointer==pointer)break;
 if(slot==OM_SRAM_MAX_LIVE)return-OM_ENOENT;
 block=(struct memblock*)pointer-1;n=core->live[slot].bytes;
 if((unsigned char*)block<core->begin||(unsigned char*)block>=core->end||
    n>(unsigned long)(core->end-(unsigned char*)block)||
    block->next!=block||block->length!=n)return-OM_EINVAL;
 if(om_sram_memfree(core,block,n))return-OM_EINVAL;
 core->live[slot].pointer=0;core->live[slot].bytes=0;core->live_count--;return 0;
}
int om_sram_core_close(struct om_sram_core *core)
{
 if(!core||!core->active)return-OM_EINVAL;
 if(core->live_count)return-OM_EBUSY;
 memset(core,0,sizeof(*core));return 0;
}
