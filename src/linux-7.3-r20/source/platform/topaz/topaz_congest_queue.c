/**
 * Copyright (c) 2012-2013 Quantenna Communications, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 **/

#include <linux/module.h>
#include "openmitrastar_tqe_bounded_ownership.h"
#include <linux/proc_fs.h>
#include <linux/io.h>

#include <linux/netdevice.h>
#include <qtn/topaz_tqe.h>
#include <qtn/topaz_tqe_cpuif.h>
#include <qtn/topaz_hbm_cpuif.h>
#include <qtn/topaz_hbm.h>
#include <qtn/topaz_congest_queue.h>

#define TOPAZ_DEF_TASKLET_BUDGET	32
#define TOPAZ_DEF_CONGEST_TIMEOUT	(2 * HZ)

#define TOPAZ_HBM_POOL_THRESHOLD	2048

/* SOURCE-ONLY provider owns bank state and all retained descriptions.
 * No recovery/release/reset API: first ownership pins both modules until reboot.
 * Entry count is bounded; saturated retention never overwrites previous records.
 * A missing slot is counted explicitly, not reported as complete ownership proof.
 */
#ifdef CONFIG_MODULE_FORCE_UNLOAD
#error "Bounded ownership provider requires forced module unload disabled"
#endif
#if defined(CONFIG_SMP) || defined(CONFIG_PREEMPT) || defined(CONFIG_PREEMPT_VOLUNTARY) || defined(CONFIG_TOPAZ_PCIE_TARGET)
#error "Bounded ownership provider requires UP/PREEMPT_NONE/non-PCIe"
#endif
#define OMCGQ_HELD_MAX 256U
struct omcgq_held_record {
	union topaz_tqe_cpuif_ppctl descriptor;
	u32 references;
	bool published;
};
static struct omcgq_held_record omcgq_held[OMCGQ_HELD_MAX];
static u32 omcgq_held_count;
static u32 omcgq_unrecorded;
static u32 omcgq_generation;
static bool omcgq_reserved;
static bool omcgq_quarantined;
static bool omcgq_retained;
static struct module *omcgq_owner;

int omcgq_bank_quarantined(void) { return READ_ONCE(omcgq_quarantined); }
EXPORT_SYMBOL(omcgq_bank_quarantined);
int omcgq_bank_retained(void) { return READ_ONCE(omcgq_retained); }
EXPORT_SYMBOL(omcgq_bank_retained);

static bool omcgq_token_matches(const struct omcgq_bank_token *token)
{
	return token && omcgq_reserved && token->generation &&
		token->generation == omcgq_generation && token->owner == omcgq_owner;
}

/* Typed batch reserves one PAYLOAD owner, not one descriptor per output.
 * Source fanout is bounded to128WMAC nodes plus7otherports; expectedrefs stay
 * attached to this one owner across partial submission or queue retention.
 */
#define OMCGQ_BATCH_FANOUT_MAX 135U
struct omcgq_batch_state {
	struct omcgq_batch_token token;
	u32 expected;
	u32 completed;
	u32 published;
	u32 ledger_slot;
	bool active;
};
static struct omcgq_batch_state omcgq_batch;
static u32 omcgq_batch_generation;

int omcgq_bank_acquire(struct module *owner, struct omcgq_bank_token *token)
{
	unsigned long flags;
	int acquired = 0;
	if (!token || !owner)
		return 0;
	token->generation = 0;
	token->owner = NULL;
	local_irq_save(flags);
	if (omcgq_batch.active || omcgq_reserved || omcgq_quarantined ||
	    (omcgq_owner && omcgq_owner != owner) || omcgq_generation == ~0U)
		goto out;
	if (!omcgq_retained) {
		/* Acquire lifetime references BEFORE any skb/HBM mutation or TXSTART.
		 * Never rely on returning early from module_exit to prevent unloading. */
		if (!try_module_get(owner))
			goto out;
		if (!try_module_get(THIS_MODULE)) {
			module_put(owner);
			goto out;
		}
		omcgq_owner = owner;
		omcgq_retained = true;
	}
	omcgq_generation++;
	omcgq_reserved = true;
	token->generation = omcgq_generation;
	token->owner = owner;
	acquired = 1;
out:
	local_irq_restore(flags);
	return acquired;
}
EXPORT_SYMBOL(omcgq_bank_acquire);

int omcgq_bank_token_valid(const struct omcgq_bank_token *token)
{
	unsigned long flags;
	int valid;
	local_irq_save(flags);
	valid = !omcgq_quarantined && omcgq_token_matches(token);
	local_irq_restore(flags);
	return valid;
}
EXPORT_SYMBOL(omcgq_bank_token_valid);

int omcgq_bank_cancel(const struct omcgq_bank_token *token)
{
	unsigned long flags;
	int cancelled = 0;
	local_irq_save(flags);
	if (omcgq_token_matches(token)) {
		omcgq_reserved = false;
		cancelled = 1;
	}
	local_irq_restore(flags);
	return cancelled;
}
EXPORT_SYMBOL(omcgq_bank_cancel);

static int omcgq_record_held(const union topaz_tqe_cpuif_ppctl *desc, bool published)
{
	u32 i;
	if (!desc)
		return 1; /* known prepublication refusal can have no consumed payload */
	for (i = 0; i < omcgq_held_count; i++) {
		if (omcgq_held[i].published == published &&
		    !memcmp(&omcgq_held[i].descriptor, desc, sizeof(*desc))) {
			if (omcgq_held[i].references != ~0U)
				omcgq_held[i].references++;
			else if (omcgq_unrecorded != ~0U)
				omcgq_unrecorded++;
			return 1;
		}
	}
	if (omcgq_held_count == OMCGQ_HELD_MAX) {
		if (omcgq_unrecorded != ~0U)
			omcgq_unrecorded++;
		return 0;
	}
	omcgq_held[omcgq_held_count].descriptor = *desc;
	omcgq_held[omcgq_held_count].references = 1;
	omcgq_held[omcgq_held_count].published = published;
	omcgq_held_count++;
	return 1;
}

int omcgq_bank_quarantine(const struct omcgq_bank_token *token,
	const union topaz_tqe_cpuif_ppctl *desc, bool published)
{
	unsigned long flags;
	int recorded = 0;
	local_irq_save(flags);
	if (omcgq_token_matches(token)) {
		omcgq_quarantined = true;
		omcgq_reserved = false;
		recorded = omcgq_batch.active ? 1 : omcgq_record_held(desc, published);
		if (omcgq_batch.active)
			omcgq_held[omcgq_batch.ledger_slot].published |= published;
	}
	local_irq_restore(flags);
	return recorded;
}
EXPORT_SYMBOL(omcgq_bank_quarantine);

int omcgq_bank_retain(const union topaz_tqe_cpuif_ppctl *desc, bool published)
{
	unsigned long flags;
	int recorded = 0;
	local_irq_save(flags);
	if (omcgq_retained && omcgq_quarantined)
		recorded = omcgq_record_held(desc, published);
	local_irq_restore(flags);
	return recorded;
}
EXPORT_SYMBOL(omcgq_bank_retain);

/* Fixed typed markers: one multicastbatch<=135 pendingoutputs.
 * Original queue ring owns each payloadref; markers persist token, neverstack.
 */
static bool omcgq_batch_matches(const struct omcgq_batch_token *batch);
struct omcgq_queue_marker {
	struct topaz_congest_q_desc *queue;
	u32 ring_index;
	struct omcgq_batch_token token;
	bool active;
};
static struct omcgq_queue_marker omcgq_queue_markers[OMCGQ_BATCH_FANOUT_MAX];
static u32 omcgq_queue_pending;
static omcgq_batch_xmit_fn omcgq_typed_xmit;
static struct module *omcgq_typed_owner;
static struct topaz_congest_queue *omcgq_credit_queue;
static u32 omcgq_queue_reserved;
static struct omcgq_queue_marker *omcgq_retry_marker;
static bool omcgq_queue_processing;

int omcgq_batch_retry_acquire(const struct omcgq_batch_token *batch,
	struct omcgq_bank_token *token)
{
	unsigned long flags;
	int acquired = 0;
	if (!token)
		return 0;
	token->generation = 0;
	token->owner = NULL;
	local_irq_save(flags);
	if (!omcgq_retry_marker || !omcgq_retry_marker->active ||
	    !omcgq_batch_matches(batch) || omcgq_reserved || omcgq_quarantined ||
	    omcgq_generation == ~0U ||
	    omcgq_retry_marker->token.generation != batch->generation ||
	    omcgq_retry_marker->token.owner != batch->owner)
		goto out;
	omcgq_generation++;
	omcgq_reserved = true;
	token->generation = omcgq_generation;
	token->owner = batch->owner;
	acquired = 1;
out:
	local_irq_restore(flags);
	return acquired;
}
EXPORT_SYMBOL(omcgq_batch_retry_acquire);

int omcgq_batch_queue_reserve(struct topaz_congest_queue *queue, u32 expected)
{
	unsigned long flags;
	int admitted = 0;
	if (!queue || !expected || expected > OMCGQ_BATCH_FANOUT_MAX)
		return 0;
	local_irq_save(flags);
	if (omcgq_queue_reserved || omcgq_queue_pending || omcgq_batch.active ||
	    omcgq_quarantined || queue->total_qlen > TOPAZ_CONGEST_TOTAL_PKT_MAX ||
	    expected > TOPAZ_CONGEST_TOTAL_PKT_MAX - queue->total_qlen)
		goto out;
	omcgq_credit_queue = queue;
	omcgq_queue_reserved = expected;
	admitted = 1;
out:
	local_irq_restore(flags);
	return admitted;
}
EXPORT_SYMBOL(omcgq_batch_queue_reserve);

void omcgq_batch_queue_cancel_reserve(void)
{
	unsigned long flags;
	local_irq_save(flags);
	if (!omcgq_batch.active && !omcgq_queue_pending) {
		omcgq_queue_reserved = 0;
		omcgq_credit_queue = NULL;
	}
	local_irq_restore(flags);
}
EXPORT_SYMBOL(omcgq_batch_queue_cancel_reserve);

int omcgq_batch_has_queued(void) { return READ_ONCE(omcgq_queue_pending) != 0; }
EXPORT_SYMBOL(omcgq_batch_has_queued);

int omcgq_batch_xmit_register(struct module *owner, omcgq_batch_xmit_fn fn)
{
	unsigned long flags;
	int registered = 0;
	if (!owner || !fn)
		return 0;
	local_irq_save(flags);
	if (!omcgq_typed_xmit && !omcgq_retained && !omcgq_batch.active) {
		/* A stored function pointer is already a lifetime dependency. Pin both
		 * modules before registration can outlive a failed batch admission. */
		if (!try_module_get(owner))
			goto out;
		if (!try_module_get(THIS_MODULE)) {
			module_put(owner);
			goto out;
		}
		omcgq_owner = owner;
		omcgq_retained = true;
		omcgq_typed_xmit = fn;
		omcgq_typed_owner = owner;
		registered = 1;
	} else if (owner == omcgq_typed_owner && fn == omcgq_typed_xmit) {
		registered = 1;
	}
out:
	local_irq_restore(flags);
	return registered;
}
EXPORT_SYMBOL(omcgq_batch_xmit_register);

int omcgq_batch_queue_admit(const struct omcgq_batch_token *batch,
	struct topaz_congest_q_desc *queue, union topaz_tqe_cpuif_ppctl *ppctl)
{
	unsigned long flags;
	u32 slot;
	u32 ring;
	int admitted = 0;
	if (!queue || !ppctl)
		return 0;
	local_irq_save(flags);
	if (!omcgq_batch_matches(batch) || omcgq_quarantined ||
	    omcgq_queue_pending == OMCGQ_BATCH_FANOUT_MAX ||
	    queue->congest_queue != omcgq_credit_queue || !omcgq_queue_reserved ||
	    omcgq_batch.completed >= omcgq_batch.expected)
		goto out;
	for (slot = 0; slot < OMCGQ_BATCH_FANOUT_MAX; slot++)
		if (!omcgq_queue_markers[slot].active)
			break;
	if (slot == OMCGQ_BATCH_FANOUT_MAX)
		goto out;
	ring = queue->tail;
	/* Convert a pre-reserved global credit before the ordinary enqueue. */
	omcgq_queue_reserved--;
	if (topaz_congest_enqueue(queue, ppctl) != NET_XMIT_SUCCESS) {
		omcgq_queue_reserved++;
		goto out;
	}
	omcgq_queue_markers[slot].queue = queue;
	omcgq_queue_markers[slot].ring_index = ring;
	omcgq_queue_markers[slot].token = *batch;
	omcgq_queue_markers[slot].active = true;
	omcgq_queue_pending++;
	omcgq_batch.completed++;
	admitted = 1;
out:
	local_irq_restore(flags);
	return admitted;
}
EXPORT_SYMBOL(omcgq_batch_queue_admit);

static struct omcgq_queue_marker *omcgq_typed_head(struct topaz_congest_q_desc *queue)
{
	u32 slot;
	for (slot = 0; slot < OMCGQ_BATCH_FANOUT_MAX; slot++)
		if (omcgq_queue_markers[slot].active &&
		    omcgq_queue_markers[slot].queue == queue &&
		    omcgq_queue_markers[slot].ring_index == queue->head)
			return &omcgq_queue_markers[slot];
	return NULL;
}

static void omcgq_typed_success(struct omcgq_queue_marker *marker)
{
	if (!marker || !marker->active)
		return;
	marker->active = false;
	if (omcgq_queue_pending)
		omcgq_queue_pending--;
	omcgq_batch.published++;
	if (!omcgq_queue_pending && omcgq_batch.completed == omcgq_batch.expected &&
	    omcgq_batch.published == omcgq_batch.expected && !omcgq_reserved) {
		memset(&omcgq_held[omcgq_batch.ledger_slot], 0, sizeof(omcgq_held[0]));
		omcgq_held_count--;
		omcgq_batch.active = false;
		omcgq_queue_reserved = 0;
		omcgq_credit_queue = NULL;
	}
}

static bool omcgq_batch_matches(const struct omcgq_batch_token *batch)
{
	return batch && omcgq_batch.active && batch->generation &&
		batch->generation == omcgq_batch.token.generation &&
		batch->owner == omcgq_batch.token.owner;
}

int omcgq_batch_admit(struct module *owner, const union topaz_tqe_cpuif_ppctl *payload,
	u32 expected, struct omcgq_batch_token *batch)
{
	unsigned long flags;
	int accepted = 0;
	if (!owner || !payload || !batch || !expected || expected > OMCGQ_BATCH_FANOUT_MAX)
		return 0;
	batch->generation = 0;
	batch->owner = NULL;
	local_irq_save(flags);
	if (omcgq_batch.active || omcgq_reserved || omcgq_quarantined ||
	    omcgq_held_count == OMCGQ_HELD_MAX || omcgq_batch_generation == ~0U ||
	    (omcgq_owner && omcgq_owner != owner))
		goto out;
	/* Pin before caller changes the payload's ec/freecount ownership. */
	if (!omcgq_retained) {
		if (!try_module_get(owner))
			goto out;
		if (!try_module_get(THIS_MODULE)) {
			module_put(owner);
			goto out;
		}
		omcgq_owner = owner;
		omcgq_retained = true;
	}
	omcgq_batch_generation++;
	omcgq_batch.token.generation = omcgq_batch_generation;
	omcgq_batch.token.owner = owner;
	omcgq_batch.expected = expected;
	omcgq_batch.completed = 0;
	omcgq_batch.published = 0;
	omcgq_batch.ledger_slot = omcgq_held_count;
	omcgq_batch.active = true;
	/* A fixed slot is reserved now; timeout will never need new capacity. */
	omcgq_held[omcgq_held_count].descriptor = *payload;
	omcgq_held[omcgq_held_count].references = expected;
	omcgq_held[omcgq_held_count].published = false;
	omcgq_held_count++;
	*batch = omcgq_batch.token;
	accepted = 1;
out:
	local_irq_restore(flags);
	return accepted;
}
EXPORT_SYMBOL(omcgq_batch_admit);

int omcgq_batch_valid(const struct omcgq_batch_token *batch)
{
	unsigned long flags;
	int valid;
	local_irq_save(flags);
	valid = omcgq_batch_matches(batch);
	local_irq_restore(flags);
	return valid;
}
EXPORT_SYMBOL(omcgq_batch_valid);

int omcgq_batch_acquire(const struct omcgq_batch_token *batch,
	struct omcgq_bank_token *token)
{
	unsigned long flags;
	int acquired = 0;
	if (!token)
		return 0;
	token->generation = 0;
	token->owner = NULL;
	local_irq_save(flags);
	if (!omcgq_batch_matches(batch) || omcgq_batch.completed >= omcgq_batch.expected ||
	    omcgq_reserved || omcgq_quarantined ||
	    omcgq_generation == ~0U)
		goto out;
	omcgq_generation++;
	omcgq_reserved = true;
	token->generation = omcgq_generation;
	token->owner = batch->owner;
	acquired = 1;
out:
	local_irq_restore(flags);
	return acquired;
}
EXPORT_SYMBOL(omcgq_batch_acquire);

int omcgq_batch_complete(const struct omcgq_batch_token *batch, bool published)
{
	unsigned long flags;
	int accepted = 0;
	local_irq_save(flags);
	if (omcgq_batch_matches(batch) && omcgq_batch.completed < omcgq_batch.expected) {
		omcgq_batch.completed++;
		if (published)
			omcgq_batch.published++;
		else
			omcgq_quarantined = true; /* Never recycle owned refs after refusal. */
		omcgq_held[omcgq_batch.ledger_slot].published |= published;
		accepted = 1;
	}
	local_irq_restore(flags);
	return accepted;
}
EXPORT_SYMBOL(omcgq_batch_complete);

int omcgq_batch_finish(const struct omcgq_batch_token *batch)
{
	unsigned long flags;
	int accepted = 0;
	local_irq_save(flags);
	if (omcgq_batch_matches(batch)) {
		if (omcgq_queue_pending && !omcgq_quarantined) {
			accepted = 1;
			goto out; /* queued tokens remain valid untillastretry succeeds */
		}
		/* Successful batch transfers all expected refs; recycle only its unused
		 * catalog slot. On quarantine retain the one owner with ALLexpectedrefs. */
		if (!omcgq_quarantined && !omcgq_reserved &&
		    omcgq_batch.completed == omcgq_batch.expected &&
		    omcgq_batch.published == omcgq_batch.expected) {
			memset(&omcgq_held[omcgq_batch.ledger_slot], 0, sizeof(omcgq_held[0]));
			omcgq_held_count--;
		} else {
			/* Missing callback/unfinished bank must remain a held owner and
			 * refuse all later writers, even when no poll timeout was observed. */
			omcgq_quarantined = true;
		}
		omcgq_batch.active = false;
		if (!omcgq_queue_pending) {
			omcgq_queue_reserved = 0;
			omcgq_credit_queue = NULL;
		}
		accepted = 1;
	}
out:
	local_irq_restore(flags);
	return accepted;
}
EXPORT_SYMBOL(omcgq_batch_finish);

struct topaz_congest_queue *g_congest_queue_ptr = NULL;

void topaz_congest_dump(struct topaz_congest_queue *queue)
{
	struct topaz_congest_q_desc *ptr;
	int i;

	for (i = 0; i < TOPAZ_CONGEST_QUEUE_NUM; i++) {
		ptr = &queue->queues[i];

		if (ptr->valid == 0)
			continue;

		printk("Queue Number: %4d\t", i);
		printk("qlen: %4d head: %4d tail: %4d\t", ptr->qlen, ptr->head, ptr->tail);
		printk("node_id: %4d tid: %4d\n", ptr->node_id, ptr->tid);
	}

	printk("Dump queue length logs:\n");
	for (i = 0; i < TOPAZ_CONGEST_PKT_MAX; i++) {
		printk("%20d:\t%d\n", i, queue->logs[i]);
	}
}
EXPORT_SYMBOL(topaz_congest_dump);

struct reg_congest_stats {
	void (*fn)(void *ctx, uint32_t type, uint8_t q_num, uint32_t q_value);
	void *ctx;
};

struct reg_congest_stats pktlog_update_cgq_stats;
void reg_congest_queue_stats(void (*fn)(void *, uint32_t, uint8_t, uint32_t), void *ctx)
{
	pktlog_update_cgq_stats.fn = fn;
	pktlog_update_cgq_stats.ctx = ctx;
}
EXPORT_SYMBOL(reg_congest_queue_stats);

struct topaz_congest_queue* topaz_congest_queue_get(void)
{
	return g_congest_queue_ptr;
}
EXPORT_SYMBOL(topaz_congest_queue_get);

inline int get_active_tid_num(void)
{
	volatile struct shared_params* sp = qtn_mproc_sync_shared_params_get();
	if (likely(sp)) {
		return sp->active_tid_num;
	} else {
		return 0;
	}
}

void topaz_congest_set_unicast_queue_count(uint32_t qnum)
{
	if (qnum <= TOPAZ_CONGEST_QUEUE_NUM)
		topaz_congest_queue_get()->max_unicast_qcount = qnum;
}
EXPORT_SYMBOL(topaz_congest_set_unicast_queue_count);

struct topaz_congest_q_desc* topaz_congest_alloc_unicast_queue(struct topaz_congest_queue *congest_queue,
														uint32_t node_id,
														uint32_t tid)
{
	struct topaz_congest_q_desc* unicast_queue;

	if (get_active_tid_num() > congest_queue->max_unicast_qcount)
		return NULL;

	if (congest_queue->unicast_qcount >= congest_queue->max_unicast_qcount)
		return NULL;

	unicast_queue = topaz_congest_alloc_queue(congest_queue, node_id, tid);

	if (unicast_queue == NULL)
		return NULL;

	unicast_queue->is_unicast = 1;
	congest_queue->unicast_qcount ++;

	return unicast_queue;
}
EXPORT_SYMBOL(topaz_congest_alloc_unicast_queue);

struct topaz_congest_q_desc* topaz_congest_alloc_queue(struct topaz_congest_queue *congest_queue, uint32_t node_id,	uint32_t tid)
{
	struct topaz_congest_q_desc *queue;
	int i;

	if (congest_queue->total_qlen >= TOPAZ_CONGEST_TOTAL_PKT_MAX)
		return NULL;

	if (topaz_hbm_pool_available(TOPAZ_HBM_BUF_EMAC_RX_POOL) <= TOPAZ_HBM_POOL_THRESHOLD)
		return NULL;

	for (i = 0; i < TOPAZ_CONGEST_QUEUE_NUM; i++) {
		queue = &congest_queue->queues[i];

		if (queue->valid == 0) {
			queue->valid = 1;

			queue->node_id = node_id;
			queue->tid = tid;
			queue->head = 0;
			queue->tail = 0;
			queue->qlen = 0;
			queue->index = i;
			queue->last_retry_success = 1;
			queue->retry_timeout = jiffies + queue->congest_queue->congest_timeout;
			queue->is_unicast = 0;

			congest_queue->ptrs[node_id][tid] = queue;

			return queue;
		}
	}

	return NULL;
}
EXPORT_SYMBOL(topaz_congest_alloc_queue);

__attribute__((section(".sram.text"))) int topaz_congest_enqueue(struct topaz_congest_q_desc* queue, union topaz_tqe_cpuif_ppctl *ppctl)
{
	struct topaz_congest_q_elem *ptr;
	uint32_t index;
	int8_t pool;
	int ret = 0;

	if (queue->congest_queue == omcgq_credit_queue &&
	    queue->congest_queue->total_qlen + omcgq_queue_reserved >= TOPAZ_CONGEST_TOTAL_PKT_MAX)
		return NET_XMIT_CN;
	if ((queue->qlen >= TOPAZ_CONGEST_PKT_MAX) ||
			(queue->congest_queue->total_qlen >= TOPAZ_CONGEST_TOTAL_PKT_MAX)) {
		queue->congest_enq_fail++;
		ret = NET_XMIT_CN;
		goto make_stats;
	}

	pool = topaz_hbm_payload_get_pool_bus(ppctl->data.pkt);
	if (topaz_hbm_pool_available(pool) <= TOPAZ_HBM_POOL_THRESHOLD) {
		queue->congest_enq_fail++;
		ret = NET_XMIT_CN;
		goto make_stats;
	}

	queue->congest_queue->logs[queue->qlen]++;

	index = queue->tail;
	ptr = &queue->elems[index];

	ptr->ppctl.raw.ppctl0 = ppctl->raw.ppctl0;
	ptr->ppctl.raw.ppctl1 = ppctl->raw.ppctl1;
	ptr->ppctl.raw.ppctl2 = ppctl->raw.ppctl2;
	ptr->ppctl.raw.ppctl3 = ppctl->raw.ppctl3;
	ptr->ppctl.raw.ppctl4 = ppctl->raw.ppctl4;
	ptr->ppctl.raw.ppctl5 = ppctl->raw.ppctl5;

	if (++index == TOPAZ_CONGEST_PKT_MAX)
		index = 0;

	queue->tail = index;
	queue->qlen++;
	queue->congest_queue->total_qlen++;

make_stats:
	if (pktlog_update_cgq_stats.fn) {
		pktlog_update_cgq_stats.fn(pktlog_update_cgq_stats.ctx,
					TOPAZ_CONGEST_QUEUE_STATS_QLEN,
					queue->index,
					queue->qlen);
		pktlog_update_cgq_stats.fn(pktlog_update_cgq_stats.ctx,
					TOPAZ_CONGEST_QUEUE_STATS_ENQFAIL,
					queue->index,
					queue->congest_enq_fail);
	}

	return ret;
}
EXPORT_SYMBOL(topaz_congest_enqueue);

void topaz_congest_release_queue(struct topaz_congest_queue *congest_queue, uint32_t node_id, uint32_t tid)
{
	struct topaz_congest_q_desc *queue;

	queue = congest_queue->ptrs[node_id][tid];

	BUG_ON(queue->qlen != 0);

	queue->node_id = 0;
	queue->tid = 0;
	queue->head = 0;
	queue->tail = 0;
	queue->qlen = 0;
	queue->last_retry_success = 1;

	queue->valid = 0;

	congest_queue->ptrs[node_id][tid] = NULL;

	if (queue->is_unicast) {
		queue->is_unicast = 0;
		if(congest_queue->unicast_qcount > 0)
			congest_queue->unicast_qcount --;
	}
}
EXPORT_SYMBOL(topaz_congest_release_queue);

inline static union topaz_tqe_cpuif_ppctl *topaz_congest_peek(struct topaz_congest_q_desc *queue)
{
	if (queue->qlen == 0)
		return NULL;

	return &queue->elems[queue->head].ppctl;
}

inline static void topaz_congest_dequeue(struct topaz_congest_q_desc *queue)
{
	if (++queue->head == TOPAZ_CONGEST_PKT_MAX)
		queue->head = 0;

	queue->qlen--;
	queue->congest_queue->total_qlen--;
	if (pktlog_update_cgq_stats.fn)
		pktlog_update_cgq_stats.fn(pktlog_update_cgq_stats.ctx, TOPAZ_CONGEST_QUEUE_STATS_QLEN, queue->index, queue->qlen);

	/* Initial status setting */
	queue->last_retry_success = 1;

	if (queue->qlen == 0)
		topaz_congest_release_queue(queue->congest_queue, queue->node_id, queue->tid);
}

void topaz_hbm_congest_queue_put_buf(const union topaz_tqe_cpuif_ppctl *ppctl)
{
	uint32_t flags;
	uint8_t *buf = ppctl->data.pkt + ppctl->data.buff_ptr_offset;

	local_irq_save(flags);
	topaz_hbm_filter_txdone_buf(buf);
	local_irq_restore(flags);
}
EXPORT_SYMBOL(topaz_hbm_congest_queue_put_buf);

noinline static void topaz_congest_queue_clear(struct topaz_congest_q_desc *queue)
{
	struct topaz_congest_q_elem *ptr;

	if (omcgq_bank_quarantined())
		return; /* no HBM release while a bank output is uncertain */
	if (omcgq_typed_head(queue)) {
		omcgq_quarantined = true;
		return; /* no unboundedHBMcleanup of typedowner */
	}
	queue->congest_drop += queue->qlen;

	while(queue->qlen) {
		if (omcgq_typed_head(queue)) {
			omcgq_quarantined = true;
			return; /* precedinguntypedhead mayhaveadvancedonto typedowner */
		}
		ptr = &queue->elems[queue->head];
		topaz_hbm_congest_queue_put_buf(&ptr->ppctl);
		topaz_congest_dequeue(queue);
	}
}

__attribute__((section(".sram.text"))) int topaz_congest_queue_xmit(struct topaz_congest_q_desc *queue, uint32_t budget)
{
	union topaz_tqe_cpuif_ppctl *pp_cntl;
	struct topaz_congest_queue *congest_queue = queue->congest_queue;
	int re_sched = 0;
	struct omcgq_queue_marker *typed;
	unsigned long retry_irq_flags;
	int ret;

	if (omcgq_bank_quarantined())
		return 0; /* retain queue and stop retry scheduling */
	local_irq_save(retry_irq_flags);
	if (omcgq_queue_processing) {
		local_irq_restore(retry_irq_flags);
		return 1; /* defer nestedqueueconsumer beforetouching samehead */
	}
	omcgq_queue_processing = true;
	local_irq_restore(retry_irq_flags);
	congest_queue->xmit_entry++;

	if (queue->last_retry_success == 0) {
		if (time_after(jiffies, queue->retry_timeout)) {
			/* PNPT queue very likely is gone */
			topaz_congest_queue_clear(queue);
			goto release_processing;
		}
	}

	while (1) {
		pp_cntl = topaz_congest_peek(queue);
		if (!pp_cntl)
			break;

		congest_queue->cnt_retries++;

		typed = omcgq_typed_head(queue);
		if (typed && omcgq_typed_xmit) {
			local_irq_save(retry_irq_flags);
			omcgq_retry_marker = typed;
			ret = omcgq_typed_xmit(&typed->token, pp_cntl);
			omcgq_retry_marker = NULL;
			local_irq_restore(retry_irq_flags);
		} else {
			ret = congest_queue->xmit_func(pp_cntl);
		}
		if (ret == OMTQE_OWNERSHIP_UNCERTAIN || ret == OMTQE_OWNERSHIP_REFUSED)
			goto release_processing; /* keep head: NEVER retry, dequeue or free uncertain ownership */

		if (ret == NET_XMIT_CN) {
			queue->last_retry_success = 0;
			re_sched = 1;
			break;
		}

		queue->retry_timeout = jiffies + congest_queue->congest_timeout;
		queue->congest_xmit++;

		/* Typed owner/credit/head state is committed atomically. */
		if (typed) {
			local_irq_save(retry_irq_flags);
			omcgq_typed_success(typed);
			topaz_congest_dequeue(queue);
			local_irq_restore(retry_irq_flags);
		} else {
			topaz_congest_dequeue(queue);
		}

		if (--budget == 0)
			break;
	}

	if (budget == 0)
		re_sched = 1;

release_processing:
	local_irq_save(retry_irq_flags);
	omcgq_queue_processing = false;
	local_irq_restore(retry_irq_flags);
	return re_sched;
}
EXPORT_SYMBOL(topaz_congest_queue_xmit);

__attribute__((section(".sram.text"))) void congest_tx_tasklet(unsigned long data)
{
	struct topaz_congest_queue *congest_queue = (struct topaz_congest_queue *)data;
	struct topaz_congest_q_desc *queue;
	int re_sched = 0;
	int ret;
	int i;

	congest_queue->func_entry++;

	for (i = 0; i < TOPAZ_CONGEST_QUEUE_NUM; i++) {
		queue = &congest_queue->queues[i];

		if (queue->valid == 0)
			continue;

		ret = topaz_congest_queue_xmit(queue, congest_queue->tasklet_budget);
		if (ret == 1)
			re_sched = 1;
	}

	if (re_sched == 1) {
		tasklet_schedule(&congest_queue->congest_tx);
	}
}
EXPORT_SYMBOL(congest_tx_tasklet);

struct topaz_congest_queue* topaz_congest_queue_init(void)
{
	struct topaz_congest_queue *queue;
	int i;

	queue = kmalloc(sizeof(struct topaz_congest_queue), GFP_KERNEL | __GFP_ZERO);
	if (queue == NULL) {
		printk(KERN_ERR"Out of memory\n");
		return NULL;
	}
	g_congest_queue_ptr = queue;

	for (i = 0; i < TOPAZ_CONGEST_QUEUE_NUM; i++) {
		queue->queues[i].congest_queue = queue;
		queue->queues[i].last_retry_success = 1;
	}
	queue->tasklet_budget = TOPAZ_DEF_TASKLET_BUDGET;
	queue->congest_timeout = TOPAZ_DEF_CONGEST_TIMEOUT;
	tasklet_init(&queue->congest_tx, congest_tx_tasklet, (unsigned long)queue);
	queue->xmit_func = NULL;
	queue->tasklet_extra_proc = NULL;
	queue->max_unicast_qcount = TOPAZ_CONGEST_MAX_UNICAST_QCOUNT;

	return queue;
}
EXPORT_SYMBOL(topaz_congest_queue_init);

void topaz_congest_queue_exit(struct topaz_congest_queue* queue)
{
	if (omcgq_bank_retained())
		return; /* retain container/tasklet context; modules pinned to reboot */
	if (queue == NULL)
		return;
	tasklet_kill(&queue->congest_tx);
	if (g_congest_queue_ptr == queue)
		g_congest_queue_ptr = NULL;
	kfree(queue);
}
EXPORT_SYMBOL(topaz_congest_queue_exit);

static int __init topaz_congest_queue_module_init(void)
{
	pr_info("OMCGQ stage=module-ready allocation=deferred mmio=none\n");
	return 0;
}

static void __exit topaz_congest_queue_module_exit(void)
{
	if (g_congest_queue_ptr != NULL)
		topaz_congest_queue_exit(g_congest_queue_ptr);
	pr_info("OMCGQ stage=module-exit\n");
}

module_init(topaz_congest_queue_module_init);
module_exit(topaz_congest_queue_module_exit);

MODULE_DESCRIPTION("CONGEST QUEUE");
MODULE_AUTHOR("Quantenna");
MODULE_LICENSE("GPL");
