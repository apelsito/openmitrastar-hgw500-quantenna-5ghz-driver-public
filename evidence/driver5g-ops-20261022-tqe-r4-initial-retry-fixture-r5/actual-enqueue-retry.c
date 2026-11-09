
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
typedef uint8_t uint8; typedef uint32_t uint32; typedef uint16_t uint16;
#define __sram_text
#define EXPORT_SYMBOL(x)
#define __iomem
#define TOPAZ_TQE_LOCAL_CPU 4
#define TOPAZ_TQE_LHOST_PORT 4
#define TOPAZ_TQE_WMAC_PORT 2
#define TOPAZ_TQE_CPUIF_TXSTART(x) (0xe1034038U+0x1000U*(x))
#define TOPAZ_TQE_CPUIF_TX_START_NOT_SUCCESS 0x40000000U
#define NET_XMIT_CN 2
#define NET_XMIT_SUCCESS 0
#define TOPAZ_HBM_EMAC_TX_DONE_POOL 2
#define TOPAZ_TQE_PORT_IS_EMAC(p) ((p)<2)
union topaz_tqe_cpuif_descr { struct { void *pkt; unsigned length; int buff_ptr_offset; int in_port; } data; };
union topaz_tqe_cpuif_ppctl { struct { void *pkt; unsigned length; int buff_ptr_offset,buff_pool_num,out_port; } data; struct { uint32_t ppctl0,ppctl1,ppctl2,ppctl3,ppctl4,ppctl5; } raw; };
typedef struct { int out_port,out_node,portal; } fwt_db_entry;
struct qtn_vlan_dev { int unused; };
struct { void (*fn)(void *,uint8_t); void *ctx; } update_multicast;
static struct qtn_vlan_dev *vport_tbl_lhost[8];
static int vlan_enabled,g_dscp_flag,g_dscp_value[2];
static unsigned irq_disabled,irq_saves,irq_restores,publications,collisions,queue_calls,drops,waits;
static unsigned pending_kind,scheduled_kind,inject_phase,injected,foreign_kind,foreign_inflight;
static uint32_t last_status;
static unsigned bank_writes[4],writes_in_transaction;
static union topaz_tqe_cpuif_descr incoming={.data={.pkt=(void*)64,.length=64}};
static fwt_db_entry fwt={.out_port=1,.out_node=3,.portal=1};
static void run_kind(unsigned kind);static unsigned postsubmit;
static void phase(unsigned at){
 if(!injected&&inject_phase==at){
  injected=1;
  if(foreign_kind){foreign_inflight=1;bank_writes[foreign_kind==1?0:1]++;}
  else if(irq_disabled)pending_kind=scheduled_kind;
  else run_kind(scheduled_kind);
 }
}
static void irq_save(unsigned long *flags){*flags=irq_disabled;irq_disabled=1;irq_saves++;}
static void irq_restore(unsigned long flags){irq_disabled=(unsigned)flags;irq_restores++;if(!irq_disabled&&pending_kind){unsigned k=pending_kind;pending_kind=0;run_kind(k);}}
#define local_irq_save(f) irq_save(&(f))
#define local_irq_restore(f) irq_restore((f))
static int ready_state(void);static int topaz_tqe_cpuif_tx_nready(void){return ready_state();}
static void topaz_tqe_wait(void){waits++;while(topaz_tqe_cpuif_tx_nready());}
#define TOPAZ_TQE_PCIE_PORT 3
#define TOPAZ_TQE_CPUIF_PPCTL0(n) (0xe1034020U+0x1000U*(n))
#define TOPAZ_TQE_CPUIF_PPCTL1(n) (0xe1034024U+0x1000U*(n))
#define TOPAZ_TQE_CPUIF_PPCTL2(n) (0xe1034028U+0x1000U*(n))
#define TOPAZ_TQE_CPUIF_PPCTL3(n) (0xe103402cU+0x1000U*(n))
#define TOPAZ_TQE_CPUIF_PPCTL4(n) (0xe1034030U+0x1000U*(n))
#define TOPAZ_TQE_CPUIF_PPCTL5(n) (0xe1034034U+0x1000U*(n))
#define TOPAZ_TQE_CPUIF_TX_START_NREADY 1
enum topaz_tqe_port { FIXTURE_PORT_ENUM = 0 };
static void qtn_mproc_sync_mem_write(uint32_t address,uint32_t value){
 (void)value;assert(irq_disabled);assert(address>=0xe1034020U&&address<=0xe1034038U);
 if(address==0xe1034038U){phase(8);if(foreign_inflight&&foreign_kind==1)collisions++;publications++;bank_writes[0]++;postsubmit=1;phase(9);}
 else{phase(2+(address-0xe1034020U)/4);bank_writes[0]++;writes_in_transaction++;}
}
static void qtn_mproc_sync_mem_write_wmb(uint32_t address,uint32_t value){qtn_mproc_sync_mem_write(address,value);}
static uint32_t qtn_mproc_sync_mem_read(uint32_t address){assert(address==0xe1034038U);phase(10);return last_status;}
static void wmb(void){phase(11);}
static unsigned vlan_denied,egress_calls;static int qtn_vlan_egress(struct qtn_vlan_dev*d,int n,void*b){(void)d;(void)n;(void)b;egress_calls++;return!vlan_denied;}
static void *bus_to_virt(uintptr_t a){return(void*)a;}
static uintptr_t virt_to_bus(const void*a){return(uintptr_t)a;}
static long topaz_hbm_payload_buff_ptr_offset_bus(void*a,int pool,void*n){(void)a;(void)pool;(void)n;return 64;}
static void tqe_rx_pkt_drop(const union topaz_tqe_cpuif_descr*d){(void)d;drops++;}
static void topaz_tqe_vlan_gettid(void*a,uint8_t*b,uint8_t*c){(void)a;*b=0;*c=0;}
static void topaz_tqe_cpuif_ppctl_init(union topaz_tqe_cpuif_ppctl*p,int port,uint8_t*node,int count,int tid,int portal,int alloc,int pool,int free_,int user){(void)node;(void)count;(void)tid;(void)portal;(void)alloc;(void)free_;(void)user;memset(p,0,sizeof(*p));p->data.out_port=port;p->data.buff_pool_num=pool;}
static void topaz_tqe_congest_queue_process(const union topaz_tqe_cpuif_descr*d,void*q,uint8_t n,uint8_t tid,union topaz_tqe_cpuif_ppctl*p,uint8_t unicast){(void)d;(void)q;(void)n;(void)tid;(void)p;(void)unicast;queue_calls++;}

#define READ_ONCE(x) (x)
#define OMTQE_OWNERSHIP_UNCERTAIN 0x40
#define OMTQE_OWNERSHIP_REFUSED 0x41
#define OMTQE_READY_READ_MAX 1024U
#define OMTQE_COMPLETE_READ_MAX 1024U
typedef uint32_t u32;
struct module{unsigned refs;bool deny;};
static struct module provider,consumer,foreign;
#define THIS_MODULE (&consumer)
static bool try_module_get(struct module*m){if(m->deny)return false;m->refs++;return true;}
static void module_put(struct module*m){assert(m->refs);m->refs--;}
struct omcgq_bank_token{u32 generation;struct module*owner;};
struct omcgq_batch_token{u32 generation;struct module*owner;};
static unsigned ready_reads,complete_reads,ready_on,complete_on,postsubmit,vlan_denied,egress_calls;
static int ready_state(void){if(postsubmit){complete_reads++;return!complete_on||complete_reads<complete_on;}ready_reads++;return!ready_on||ready_reads<ready_on;}
static void cpu_relax(void){}

#define TOPAZ_CONGEST_TOTAL_PKT_MAX 2048U
#define time_after(a,b) ((a)>(b))
typedef int(*omcgq_batch_xmit_fn)(const struct omcgq_batch_token*,union topaz_tqe_cpuif_ppctl*);
struct topaz_congest_queue;
struct topaz_congest_q_desc{union topaz_tqe_cpuif_ppctl elems[4];unsigned tail,head,qlen,last_retry_success,retry_timeout,congest_drop,congest_xmit;struct topaz_congest_queue*congest_queue;};
struct topaz_congest_queue{unsigned total_qlen,xmit_entry,cnt_retries,congest_timeout;int congest_tx;int(*xmit_func)(union topaz_tqe_cpuif_ppctl*);};
static struct topaz_congest_q_desc queue;
static unsigned jiffies,dequeues,schedules;
static int topaz_congest_enqueue(struct topaz_congest_q_desc*q,union topaz_tqe_cpuif_ppctl*p){if(q->qlen==4)return 2;q->elems[q->tail]=*p;q->tail=(q->tail+1)%4;q->qlen++;q->congest_queue->total_qlen++;return 0;}
static union topaz_tqe_cpuif_ppctl*topaz_congest_peek(struct topaz_congest_q_desc*q){return q->qlen?&q->elems[q->head]:NULL;}
static void topaz_congest_dequeue(struct topaz_congest_q_desc*q){assert(q->qlen);q->qlen--;q->head=(q->head+1)%4;q->congest_queue->total_qlen--;dequeues++;}
static void topaz_congest_queue_clear(struct topaz_congest_q_desc*q){(void)q;assert(0);}
static int topaz_queue_congested(struct topaz_congest_queue*q,unsigned n,unsigned tid){(void)q;(void)n;(void)tid;return queue.qlen!=0;}
static struct topaz_congest_q_desc*topaz_get_congest_queue(struct topaz_congest_queue*q,unsigned n,unsigned tid){(void)n;(void)tid;queue.congest_queue=q;return&queue;}
static struct topaz_congest_q_desc*topaz_congest_alloc_queue(struct topaz_congest_queue*q,unsigned n,unsigned tid){return topaz_get_congest_queue(q,n,tid);}
static void tasklet_schedule(int*p){(void)p;schedules++;}
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
		if (!try_module_get(&provider)) {
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
		if (!try_module_get(&provider)) {
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
		if (!try_module_get(&provider)) {
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

static int topaz_tqe_cpuif_port_to_num(enum topaz_tqe_port port)
{
	if (port == TOPAZ_TQE_PCIE_PORT) {
		return 4;
	} else {
		return port - TOPAZ_TQE_LHOST_PORT;
	}
}
static int __topaz_tqe_cpuif_ppctl_write(enum topaz_tqe_port port, const union topaz_tqe_cpuif_ppctl *ctl)
{
	int num = topaz_tqe_cpuif_port_to_num(port);
	qtn_mproc_sync_mem_write(TOPAZ_TQE_CPUIF_PPCTL0(num), ctl->raw.ppctl0);
	qtn_mproc_sync_mem_write(TOPAZ_TQE_CPUIF_PPCTL1(num), ctl->raw.ppctl1);
	qtn_mproc_sync_mem_write(TOPAZ_TQE_CPUIF_PPCTL2(num), ctl->raw.ppctl2);
	qtn_mproc_sync_mem_write(TOPAZ_TQE_CPUIF_PPCTL3(num), ctl->raw.ppctl3);
	qtn_mproc_sync_mem_write(TOPAZ_TQE_CPUIF_PPCTL4(num), ctl->raw.ppctl4);
	qtn_mproc_sync_mem_write_wmb(TOPAZ_TQE_CPUIF_PPCTL5(num), ctl->raw.ppctl5);
	return num;
}
static void __topaz_tqe_cpuif_tx_start(enum topaz_tqe_port port, const union topaz_tqe_cpuif_ppctl *ctl)
{
	int num = __topaz_tqe_cpuif_ppctl_write(port, ctl);
	qtn_mproc_sync_mem_write(TOPAZ_TQE_CPUIF_TXSTART(num), TOPAZ_TQE_CPUIF_TX_START_NREADY);
}

static void topaz_tqe_cpuif_tx_start(const union topaz_tqe_cpuif_ppctl *ctl)
{
	__topaz_tqe_cpuif_tx_start(TOPAZ_TQE_LOCAL_CPU, ctl);
}struct omtqe_mcast_context{void*queue;struct omcgq_batch_token batch;};
static int omtqe_lhost_quarantined(void) { return omcgq_bank_quarantined(); }
static int omtqe_lhost_submit_reserved(const struct omcgq_bank_token *token,
	const union topaz_tqe_cpuif_ppctl *ppctl)
{
	unsigned long flags;
	unsigned int count;
	int result = OMTQE_OWNERSHIP_UNCERTAIN;
	local_irq_save(flags);
	if (!omcgq_bank_token_valid(token)) {
		(void)omcgq_bank_retain(ppctl, false);
		goto out;
	}
	topaz_tqe_cpuif_tx_start(ppctl);
	wmb();
	for (count = 0; count < OMTQE_COMPLETE_READ_MAX; count++) {
		if (!topaz_tqe_cpuif_tx_nready()) {
			result = (qtn_mproc_sync_mem_read(TOPAZ_TQE_CPUIF_TXSTART(0)) &
				TOPAZ_TQE_CPUIF_TX_START_NOT_SUCCESS) ? NET_XMIT_CN : NET_XMIT_SUCCESS;
			(void)omcgq_bank_cancel(token);
			goto out;
		}
		cpu_relax();
	}
	(void)omcgq_bank_quarantine(token, ppctl, true);
out:
	local_irq_restore(flags);
	return result;
}
static int omtqe_typed_queue_xmit(const struct omcgq_batch_token *batch,
	union topaz_tqe_cpuif_ppctl *ppctl)
{
	struct omcgq_bank_token token;
	unsigned long flags;
	unsigned int count;
	int ready = 0;
	if (!omcgq_batch_retry_acquire(batch, &token))
		return OMTQE_OWNERSHIP_REFUSED;
	local_irq_save(flags);
	for (count = 0; count < OMTQE_READY_READ_MAX; count++) {
		if (!topaz_tqe_cpuif_tx_nready()) { ready = 1; break; }
		cpu_relax();
	}
	if (!ready)
		(void)omcgq_bank_quarantine(&token, NULL, false);
	local_irq_restore(flags);
	return ready ? omtqe_lhost_submit_reserved(&token, ppctl) : OMTQE_OWNERSHIP_REFUSED;
}
static void omtqe_push_mcast_batch(const void *token1, void *token2,
	uint8_t port, uint8_t node, uint8_t tid)
{
	const union topaz_tqe_cpuif_descr *desc = token1;
	struct omtqe_mcast_context *context = token2;
	union topaz_tqe_cpuif_ppctl ppctl;
	struct omcgq_bank_token token;
	int result = OMTQE_OWNERSHIP_REFUSED;
	if (!context || !omcgq_batch_valid(&context->batch))
		return;
	topaz_tqe_cpuif_ppctl_init(&ppctl, port, &node, 1, tid,
		1, 1, TOPAZ_HBM_EMAC_TX_DONE_POOL, context->queue ? 0 : 1, 0);
	ppctl.data.pkt = desc->data.pkt;
	ppctl.data.length = desc->data.length;
	ppctl.data.buff_ptr_offset = desc->data.buff_ptr_offset;
	if (!omtqe_lhost_quarantined() && vlan_enabled && TOPAZ_TQE_PORT_IS_EMAC(port)) {
		struct qtn_vlan_dev *vdev = vport_tbl_lhost[port];
		if (!qtn_vlan_egress(vdev, 0, bus_to_virt((uintptr_t)desc->data.pkt))) {
			/* Retain this batch owner instead of invoking the legacy unbounded
			 * HBM final-ref cleanup. Refused policy is explicit, never forwarded. */
			if (omcgq_batch_acquire(&context->batch, &token))
				(void)omcgq_bank_quarantine(&token, NULL, false);
			(void)omcgq_batch_complete(&context->batch, false);
			return;
		}
	}
	if (!omtqe_lhost_quarantined() && omcgq_batch_acquire(&context->batch, &token)) {
		/* Batch-owned admission: bounded poll BEFORE publishing, no normal
		 * callback can borrow this token or consume another ledger slot. */
		unsigned long flags;
		unsigned int count;
		int ready = 0;
		local_irq_save(flags);
		for (count = 0; count < OMTQE_READY_READ_MAX; count++) {
			if (!topaz_tqe_cpuif_tx_nready()) { ready = 1; break; }
			cpu_relax();
		}
		if (!ready)
			(void)omcgq_bank_quarantine(&token, NULL, false);
		local_irq_restore(flags);
		if (ready)
			result = omtqe_lhost_submit_reserved(&token, &ppctl);
	}
	if (result == NET_XMIT_CN && context->queue) {
		struct topaz_congest_queue *cgq = context->queue;
		struct topaz_congest_q_desc *queued = topaz_queue_congested(cgq, node, tid) ? topaz_get_congest_queue(cgq, node, tid) : topaz_congest_alloc_queue(cgq, node, tid);
		if (queued && omcgq_batch_queue_admit(&context->batch, queued, &ppctl)) {
			tasklet_schedule(&cgq->congest_tx);
			return;
		}
	}
	/* Existing normal queues cannot encode this batch token. The first source
	 * variant intentionally avoids requeue/retry and retains on any refusal. */
	if (result != NET_XMIT_SUCCESS && !omtqe_lhost_quarantined()) {
		if (omcgq_batch_acquire(&context->batch, &token))
			(void)omcgq_bank_quarantine(&token, NULL, false);
	}
	(void)omcgq_batch_complete(&context->batch, result == NET_XMIT_SUCCESS);
}__attribute__((section(".sram.text"))) int topaz_congest_queue_xmit(struct topaz_congest_q_desc *queue, uint32_t budget)
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
static void run_kind(unsigned k){(void)k;}
static int ordinary(union topaz_tqe_cpuif_ppctl*p){(void)p;assert(0);return 0;}
static void clear(void){memset(omcgq_held,0,sizeof(omcgq_held));memset(&omcgq_batch,0,sizeof(omcgq_batch));memset(omcgq_queue_markers,0,sizeof(omcgq_queue_markers));memset(&queue,0,sizeof(queue));omcgq_held_count=omcgq_unrecorded=omcgq_generation=omcgq_batch_generation=omcgq_queue_pending=omcgq_queue_reserved=0;omcgq_reserved=omcgq_quarantined=omcgq_retained=false;omcgq_owner=omcgq_typed_owner=NULL;omcgq_typed_xmit=NULL;omcgq_credit_queue=NULL;omcgq_retry_marker=NULL;provider.refs=consumer.refs=0;provider.deny=consumer.deny=false;irq_disabled=publications=collisions=queue_calls=drops=waits=pending_kind=scheduled_kind=inject_phase=injected=foreign_kind=foreign_inflight=0;ready_reads=complete_reads=postsubmit=vlan_denied=egress_calls=schedules=dequeues=0;ready_on=complete_on=1;last_status=0;vlan_enabled=0;}
int main(void){unsigned cases=0;union topaz_tqe_cpuif_ppctl payload={0};struct omtqe_mcast_context context;struct topaz_congest_queue cgq;
payload.data.pkt=(void*)64;payload.data.length=64;
clear();memset(&cgq,0,sizeof(cgq));cgq.xmit_func=ordinary;context.queue=&cgq;assert(omcgq_batch_xmit_register(&consumer,omtqe_typed_queue_xmit));assert(omcgq_batch_queue_reserve(&cgq,1)&&omcgq_batch_admit(&consumer,&payload,1,&context.batch));last_status=0x40000000U;omtqe_push_mcast_batch(&incoming,&context,1,3,0);
assert(publications==1&&queue.qlen==1&&omcgq_queue_pending==1&&omcgq_batch.completed==1&&omcgq_batch.published==0&&!omcgq_quarantined&&schedules==1);assert(omcgq_batch_finish(&context.batch)&&omcgq_batch.active);cases++;
last_status=0;postsubmit=0;ready_reads=complete_reads=0;queue.last_retry_success=1;assert(topaz_congest_queue_xmit(&queue,1)==1);assert(publications==2&&queue.qlen==0&&dequeues==1&&!omcgq_queue_pending&&!omcgq_batch.active&&omcgq_held_count==0&&!irq_disabled);cases++;
clear();memset(&cgq,0,sizeof(cgq));cgq.xmit_func=ordinary;context.queue=&cgq;assert(omcgq_batch_xmit_register(&consumer,omtqe_typed_queue_xmit));assert(omcgq_batch_queue_reserve(&cgq,1)&&omcgq_batch_admit(&consumer,&payload,1,&context.batch));last_status=0x40000000U;omtqe_push_mcast_batch(&incoming,&context,1,3,0);assert(omcgq_batch_finish(&context.batch));postsubmit=0;complete_on=0;queue.last_retry_success=1;assert(topaz_congest_queue_xmit(&queue,1)==0);assert(publications==2&&queue.qlen==1&&omcgq_queue_pending==1&&omcgq_quarantined&&omcgq_held_count==1&&!dequeues&&!irq_disabled);cases++;
printf("{\"cases\":%u}\n",cases);return 0;}
