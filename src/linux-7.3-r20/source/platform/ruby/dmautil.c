/*
 * Copyright (c) Quantenna Communications, Inc. 2012
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <qtn/dmautil.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/overflow.h>
#include <linux/log2.h>
#include <asm/io.h>
#include <asm/cacheflush.h>

/* Host-only allocation ownership. The shared descriptor/wire ABI is unchanged.
 * The caller serializes allocation/free for each descriptor owner. */
struct om_dma_allocation {
	struct list_head link;
	aligned_dma_descs *owner;
	void *raw;
	void __iomem *mapped;
	bool is_sram;
};

static LIST_HEAD(om_dma_allocations);
static DEFINE_SPINLOCK(om_dma_allocations_lock);

static struct om_dma_allocation *om_dma_find_owner(aligned_dma_descs *owner)
{
	struct om_dma_allocation *record;

	list_for_each_entry(record, &om_dma_allocations, link) {
		if (record->owner == owner)
			return record;
	}
	return NULL;
}

static void om_dma_release(struct om_dma_allocation *record)
{
	if (record->mapped)
		iounmap(record->mapped);
	if (record->raw) {
		if (record->is_sram)
			heap_sram_free(record->raw);
		else
			kfree(record->raw);
	}
	kfree(record);
}

int dmautil_aligned_dma_desc_alloc(aligned_dma_descs *d,
		unsigned int desc_size, unsigned int desc_count,
		unsigned int align, bool is_sram)
{
	struct om_dma_allocation *record;
	aligned_dma_descs allocated = { 0 };
	size_t remap_size, alloc_size;
	unsigned long flags;

	if (!d || !desc_size || !desc_count || !is_power_of_2(align))
		return -EINVAL;
	if (check_mul_overflow((size_t)desc_size, (size_t)desc_count, &remap_size) ||
		check_add_overflow(remap_size, (size_t)align - 1, &alloc_size))
		return -EOVERFLOW;

	spin_lock_irqsave(&om_dma_allocations_lock, flags);
	if (om_dma_find_owner(d)) {
		spin_unlock_irqrestore(&om_dma_allocations_lock, flags);
		return -EBUSY;
	}
	spin_unlock_irqrestore(&om_dma_allocations_lock, flags);

	record = kzalloc(sizeof(*record), GFP_KERNEL);
	if (!record)
		return -ENOMEM;
	record->owner = d;
	record->is_sram = is_sram;
	record->raw = is_sram ? heap_sram_alloc(alloc_size) : kmalloc(alloc_size, GFP_KERNEL);
	if (!record->raw) {
		om_dma_release(record);
		return -ENOMEM;
	}

	allocated.desc_count = desc_count;
	allocated.unaligned_vdescs = (unsigned long)record->raw;
	flush_dcache_range(allocated.unaligned_vdescs, allocated.unaligned_vdescs + alloc_size);
	allocated.aligned_vdescs = align_val_up(allocated.unaligned_vdescs, align);
	allocated.descs_dma_addr = virt_to_bus((void *)allocated.aligned_vdescs);
	record->mapped = ioremap_nocache(allocated.aligned_vdescs, remap_size);
	if (!record->mapped) {
		om_dma_release(record);
		return -ENOMEM;
	}
	allocated.descs = record->mapped;
	memset(allocated.descs, 0, remap_size);

	spin_lock_irqsave(&om_dma_allocations_lock, flags);
	/* Also reject an allocation committed while this one was preparing. */
	if (om_dma_find_owner(d)) {
		spin_unlock_irqrestore(&om_dma_allocations_lock, flags);
		om_dma_release(record);
		return -EBUSY;
	}
	*d = allocated;
	list_add(&record->link, &om_dma_allocations);
	spin_unlock_irqrestore(&om_dma_allocations_lock, flags);
	return 0;
}
EXPORT_SYMBOL(dmautil_aligned_dma_desc_alloc);

void dmautil_aligned_dma_desc_free(aligned_dma_descs *d)
{
	struct om_dma_allocation *record;
	unsigned long flags;

	if (!d)
		return;
	spin_lock_irqsave(&om_dma_allocations_lock, flags);
	record = om_dma_find_owner(d);
	if (record) {
		list_del(&record->link);
		memset(d, 0, sizeof(*d));
	}
	spin_unlock_irqrestore(&om_dma_allocations_lock, flags);
	/* Unknown/repeated frees never unmap or free an inferred address. */
	if (record)
		om_dma_release(record);
}
EXPORT_SYMBOL(dmautil_aligned_dma_desc_free);

MODULE_DESCRIPTION("Quantenna owned aligned DMA descriptor allocator");
MODULE_LICENSE("GPL");
