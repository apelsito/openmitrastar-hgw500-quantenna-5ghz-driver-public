
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
typedef uint32_t u32;
#define EXPORT_SYMBOL(x)
#define READ_ONCE(x) (x)
#define __attribute__(x)
#define NET_XMIT_SUCCESS 0
#define NET_XMIT_CN 2
#define OMTQE_OWNERSHIP_UNCERTAIN 0x40
#define OMTQE_OWNERSHIP_REFUSED 0x41
#define TOPAZ_CONGEST_TOTAL_PKT_MAX 2048U
#define time_after(a,b) ((a)>(b))
struct module{unsigned refs;bool deny;};static struct module provider,owner;
#define THIS_MODULE (&provider)
static bool try_module_get(struct module*m){if(m->deny)return false;m->refs++;return true;}
static void module_put(struct module*m){assert(m->refs);m->refs--;}
static unsigned irq;
static void save(unsigned long*f){*f=irq;irq=1;}
static void restore(unsigned long f){irq=f;}
#define local_irq_save(f) save(&(f))
#define local_irq_restore(f) restore(f)
union topaz_tqe_cpuif_ppctl{u32 words[6];};
struct omcgq_bank_token{u32 generation;struct module*owner;};
struct omcgq_batch_token{u32 generation;struct module*owner;};
typedef int(*omcgq_batch_xmit_fn)(const struct omcgq_batch_token*,union topaz_tqe_cpuif_ppctl*);
struct topaz_congest_queue;
struct topaz_congest_q_desc{union topaz_tqe_cpuif_ppctl elems[4];u32 tail,head,qlen,last_retry_success,retry_timeout,congest_drop,congest_xmit;struct topaz_congest_queue*congest_queue;};
struct topaz_congest_queue{u32 total_qlen,xmit_entry,cnt_retries,congest_timeout;int(*xmit_func)(union topaz_tqe_cpuif_ppctl*);};
static unsigned jiffies,callback_result,callback_calls,dequeues,clears;
static struct topaz_congest_q_desc *nested_queue;
static unsigned nested_arrival,nested_result;
static int topaz_congest_queue_xmit(struct topaz_congest_q_desc*,uint32_t);
static int topaz_congest_enqueue(struct topaz_congest_q_desc*q,union topaz_tqe_cpuif_ppctl*p){if(q->qlen==4)return 2;q->elems[q->tail]=*p;q->tail=(q->tail+1)%4;q->qlen++;q->congest_queue->total_qlen++;return 0;}
static union topaz_tqe_cpuif_ppctl*topaz_congest_peek(struct topaz_congest_q_desc*q){return q->qlen?&q->elems[q->head]:NULL;}
static void topaz_congest_dequeue(struct topaz_congest_q_desc*q){assert(q->qlen);q->qlen--;q->head=(q->head+1)%4;q->congest_queue->total_qlen--;dequeues++;}
static void topaz_congest_queue_clear(struct topaz_congest_q_desc*q){(void)q;clears++;}
static int typed(const struct omcgq_batch_token*b,union topaz_tqe_cpuif_ppctl*p);
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
static int typed(const struct omcgq_batch_token*b,union topaz_tqe_cpuif_ppctl*p){(void)p;assert(omcgq_batch_valid(b));struct omcgq_bank_token acquired;assert(omcgq_batch_retry_acquire(b,&acquired));callback_calls++;if(nested_arrival){nested_arrival=0;nested_result=topaz_congest_queue_xmit(nested_queue,1);assert(callback_calls==1);}if(callback_result==OMTQE_OWNERSHIP_UNCERTAIN)assert(omcgq_bank_quarantine(&acquired,p,true));else assert(omcgq_bank_cancel(&acquired));return callback_result;}
static int ordinary(union topaz_tqe_cpuif_ppctl*p){(void)p;assert(0);return 0;}
static void reset(void){memset(omcgq_held,0,sizeof(omcgq_held));memset(&omcgq_batch,0,sizeof(omcgq_batch));memset(omcgq_queue_markers,0,sizeof(omcgq_queue_markers));omcgq_held_count=omcgq_unrecorded=omcgq_generation=omcgq_batch_generation=omcgq_queue_pending=omcgq_queue_reserved=0;omcgq_retained=omcgq_reserved=omcgq_quarantined=false;omcgq_owner=omcgq_typed_owner=NULL;omcgq_typed_xmit=NULL;omcgq_credit_queue=NULL;owner.refs=provider.refs=0;callback_calls=callback_result=dequeues=clears=irq=0;}
int main(void){unsigned cases=0;struct topaz_congest_queue cgq;struct topaz_congest_q_desc q;union topaz_tqe_cpuif_ppctl p={{1,2,3,4,5,6}};struct omcgq_batch_token b;
reset();memset(&cgq,0,sizeof(cgq));memset(&q,0,sizeof(q));q.congest_queue=&cgq;cgq.xmit_func=ordinary;assert(omcgq_batch_xmit_register(&owner,typed));assert(omcgq_batch_queue_reserve(&cgq,1));assert(omcgq_batch_admit(&owner,&p,1,&b));assert(omcgq_batch_queue_admit(&b,&q,&p)&&q.qlen==1&&omcgq_queue_pending==1);assert(omcgq_batch_finish(&b)&&omcgq_batch.active);struct omcgq_bank_token wrong;assert(!omcgq_batch_acquire(&b,&wrong)&&!omcgq_batch_retry_acquire(&b,&wrong));cases++;
callback_result=NET_XMIT_CN;assert(topaz_congest_queue_xmit(&q,1)==1&&q.qlen==1&&callback_calls==1&&!dequeues&&omcgq_queue_pending==1);cases++;
callback_result=0;q.last_retry_success=1;assert(topaz_congest_queue_xmit(&q,1)==1&&q.qlen==0&&callback_calls==2&&dequeues==1&&!omcgq_queue_pending&&!omcgq_batch.active&&omcgq_held_count==0);cases++;
reset();memset(&cgq,0,sizeof(cgq));cgq.total_qlen=2048;assert(!omcgq_batch_queue_reserve(&cgq,1)&&!omcgq_queue_reserved&&!owner.refs);cases++;
reset();memset(&cgq,0,sizeof(cgq));assert(omcgq_batch_queue_reserve(&cgq,1)&&omcgq_batch_admit(&owner,&p,1,&b));assert(omcgq_batch_complete(&b,true)&&omcgq_batch_finish(&b)&&!omcgq_queue_reserved&&!omcgq_credit_queue);cases++;
reset();memset(&cgq,0,sizeof(cgq));memset(&q,0,sizeof(q));q.congest_queue=&cgq;cgq.xmit_func=ordinary;assert(omcgq_batch_xmit_register(&owner,typed));assert(omcgq_batch_queue_reserve(&cgq,1));assert(omcgq_batch_admit(&owner,&p,1,&b));assert(omcgq_batch_queue_admit(&b,&q,&p));assert(omcgq_batch_finish(&b));callback_result=OMTQE_OWNERSHIP_UNCERTAIN;assert(topaz_congest_queue_xmit(&q,1)==0&&q.qlen==1&&omcgq_queue_pending==1&&omcgq_quarantined&&!dequeues);cases++;
reset();owner.deny=true;assert(!omcgq_batch_xmit_register(&owner,typed)&&!omcgq_typed_xmit&&!provider.refs&&!owner.refs);owner.deny=false;cases++;
reset();provider.deny=true;assert(!omcgq_batch_xmit_register(&owner,typed)&&!omcgq_typed_xmit&&!provider.refs&&!owner.refs);provider.deny=false;cases++;
reset();assert(omcgq_batch_xmit_register(&owner,typed)&&provider.refs==1&&owner.refs==1);assert(omcgq_batch_xmit_register(&owner,typed)&&provider.refs==1&&owner.refs==1);cases++;
reset();memset(&cgq,0,sizeof(cgq));memset(&q,0,sizeof(q));q.congest_queue=&cgq;cgq.xmit_func=ordinary;assert(omcgq_batch_xmit_register(&owner,typed)&&omcgq_batch_queue_reserve(&cgq,1)&&omcgq_batch_admit(&owner,&p,1,&b)&&omcgq_batch_queue_admit(&b,&q,&p));assert(omcgq_batch_finish(&b));nested_queue=&q;nested_arrival=1;callback_result=0;assert(topaz_congest_queue_xmit(&q,1)==1&&nested_result==1&&callback_calls==1&&dequeues==1&&q.qlen==0&&!omcgq_queue_processing);cases++;
printf("{\"cases\":%u}\n",cases);return 0;}
