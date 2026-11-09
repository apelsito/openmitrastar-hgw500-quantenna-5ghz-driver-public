
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#define EXPORT_SYMBOL(x)
#define READ_ONCE(x) (x)
typedef uint32_t u32;
struct module {unsigned refs;bool deny;};
static struct module provider,consumer,foreign;
#define THIS_MODULE (&provider)
static bool try_module_get(struct module*m){if(m->deny)return false;m->refs++;return true;}
static void module_put(struct module*m){assert(m->refs);m->refs--;}
static unsigned irqstate;
static void irqsave(unsigned long*f){*f=irqstate;irqstate=1;}
static void irqrestore(unsigned long f){irqstate=f;}
#define local_irq_save(f) irqsave(&(f))
#define local_irq_restore(f) irqrestore(f)
union topaz_tqe_cpuif_ppctl{u32 raw[6];};
struct omcgq_bank_token {u32 generation;struct module*owner;};
struct omcgq_batch_token {u32 generation;struct module*owner;};
struct topaz_congest_queue{int congest_tx;};
static struct topaz_congest_queue*g_congest_queue_ptr;
static unsigned tasklet_kills,queue_frees;
static void tasklet_kill(int*p){(void)p;tasklet_kills++;}
static void kfree(void*p){(void)p;queue_frees++;}
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
		accepted = 1;
	}
	local_irq_restore(flags);
	return accepted;
}
EXPORT_SYMBOL(omcgq_batch_finish);


static void reset_state(void){memset(omcgq_held,0,sizeof(omcgq_held));omcgq_held_count=omcgq_unrecorded=omcgq_generation=0;omcgq_reserved=omcgq_quarantined=omcgq_retained=false;omcgq_owner=NULL;memset(&omcgq_batch,0,sizeof(omcgq_batch));omcgq_batch_generation=0;consumer.refs=provider.refs=foreign.refs=0;consumer.deny=provider.deny=false;irqstate=0;}
int main(void){unsigned cases=0;struct omcgq_batch_token b,bad;struct omcgq_bank_token bank,normal;union topaz_tqe_cpuif_ppctl payload={{1,2,3,4,5,6}};
for(unsigned held=0;held<=256;held++)for(unsigned refs=0;refs<=136;refs++){
 reset_state();omcgq_held_count=held;int expected=held<256&&refs>0&&refs<=135;assert(omcgq_batch_admit(&consumer,&payload,refs,&b)==expected);
 if(expected){assert(consumer.refs==1&&provider.refs==1&&omcgq_held_count==held+1&&omcgq_held[held].references==refs);assert(!omcgq_bank_acquire(&consumer,&normal));assert(!omcgq_batch_admit(&consumer,&payload,1,&bad));assert(omcgq_batch_valid(&b));bad=b;bad.owner=&foreign;assert(!omcgq_batch_acquire(&bad,&bank));assert(omcgq_batch_acquire(&b,&bank));assert(omcgq_bank_cancel(&bank));for(unsigned n=0;n<refs;n++)assert(omcgq_batch_complete(&b,true));assert(!omcgq_batch_complete(&b,true));assert(omcgq_batch_finish(&b)&&omcgq_held_count==held&&!omcgq_batch.active);}
 assert(!irqstate);cases++;
}
reset_state();assert(omcgq_batch_admit(&consumer,&payload,135,&b));assert(omcgq_batch_acquire(&b,&bank));assert(omcgq_bank_quarantine(&bank,&payload,true));assert(omcgq_held_count==1);for(unsigned n=0;n<135;n++)assert(omcgq_batch_complete(&b,n==0));assert(omcgq_batch_finish(&b)&&omcgq_held_count==1&&omcgq_held[0].references==135&&omcgq_held[0].published);cases++;
assert(!omcgq_batch_admit(&consumer,&payload,1,&bad)&&!omcgq_bank_acquire(&consumer,&normal));cases++;
reset_state();provider.deny=true;assert(!omcgq_batch_admit(&consumer,&payload,1,&b)&&!consumer.refs&&!provider.refs&&!omcgq_held_count);cases++;
printf("{\"cases\":%u}\n",cases);return 0;}
