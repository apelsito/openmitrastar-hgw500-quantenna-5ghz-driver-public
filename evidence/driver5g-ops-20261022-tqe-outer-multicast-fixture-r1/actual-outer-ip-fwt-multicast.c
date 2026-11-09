
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
 if(address==0xe1034038U){phase(8);if(foreign_inflight&&foreign_kind==1)collisions++;publications++;bank_writes[0]++;last_status=0;postsubmit=1;phase(9);}
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
}
#define TOPAZ_TQE_FIRST_PORT 0
#define TOPAZ_TQE_MUC_PORT 5
#define TOPAZ_FWT_SW_NODE_BITMAP_SIZE 4
#define TOPAZ_BITS_PER_WD 32
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#define unlikely(x) (x)
#define printk_ratelimit() 0
#define printk(...) ((void)0)
#define KERN_CRIT ""
struct ether_header{uint8_t ether_dhost[6],ether_shost[6];uint16_t ether_type;};
struct topaz_fwt_sw_mcast_entry{uint32_t node_bitmap[4];uint8_t port_bitmap;unsigned flood_forward;};
static unsigned ec_writes,ec_value,fc_value,cache_ops;
static void tqe_buf_set_refcounts(void*p,int enq,int free_){(void)p;ec_writes++;ec_value=enq;fc_value=free_;assert(omcgq_batch.active&&omcgq_retained&&omcgq_held_count==1);}
static void flush_and_inv_dcache_range(uintptr_t a,uintptr_t b){assert(b>=a);cache_ops++;}
static int topaz_fwt_sw_mcast_port_has_nodes(uint8_t port){return port==2;}
static uint8_t topaz_fwt_sw_count_bits(uint32_t x)
{
	uint8_t bits_set = 0;

	while (x) {
		bits_set++;
		x &= x - 1;
	}

	return bits_set;
}
static int topaz_fwt_sw_mcast_node_is_set(const struct topaz_fwt_sw_mcast_entry *const e,
						const uint8_t port, const uint8_t node)
{
	if (port == TOPAZ_TQE_WMAC_PORT) {
		return (e->node_bitmap[node / TOPAZ_BITS_PER_WD] &
				(1 << (node % TOPAZ_BITS_PER_WD)));
	}
	return 0;
}
static uint32_t topaz_fwt_sw_mcast_enqueues(const struct topaz_fwt_sw_mcast_entry *const e,
		uint8_t port_bitmap, const uint8_t in_port, const uint8_t in_node)
{
	uint32_t enqueues = 0;
	uint8_t i;

	/* Exclude input port. If WMAC, the port doesn't contribute, only nodes. */
	port_bitmap &= ~((1 << in_port) | (1 << TOPAZ_TQE_WMAC_PORT));
	enqueues += topaz_fwt_sw_count_bits(port_bitmap);

	/* add wmac nodes */
	for (i = 0; i < ARRAY_SIZE(e->node_bitmap) ; i++) {
		enqueues += topaz_fwt_sw_count_bits(e->node_bitmap[i]);
	}

	/* must exclude the input node */
	if (topaz_fwt_sw_mcast_node_is_set(e, in_port, in_node)) {
		--enqueues;
	}

	return enqueues;
}
static uint32_t topaz_fwt_sw_mcast_do_per_node(
	void (*handler)(const void *token1, void *token2, uint8_t node, uint8_t port, uint8_t tid),
	const struct topaz_fwt_sw_mcast_entry *mcast_ent,
	const void *token1, void *token2, uint8_t in_node, uint8_t port, uint8_t tid)
{
	uint8_t node;
	uint8_t node_cnt = 0;
	uint32_t bitmap;
	uint8_t i;
	uint8_t j;

	for (i = 0; i < TOPAZ_FWT_SW_NODE_BITMAP_SIZE; i++) {
		bitmap = mcast_ent->node_bitmap[i];
		j = 0;
		while (bitmap) {
			if (bitmap & 0x1) {
				node = (i * TOPAZ_BITS_PER_WD) + j;
				if ((in_node == 0) || (node != in_node)) {
					handler(token1, token2, port, node, tid);
					node_cnt++;
				}
			}
			bitmap >>= 1;
			j++;
		}
	}

	return node_cnt;
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
	/* Existing normal queues cannot encode this batch token. The first source
	 * variant intentionally avoids requeue/retry and retains on any refusal. */
	if (result != NET_XMIT_SUCCESS && !omtqe_lhost_quarantined()) {
		if (omcgq_batch_acquire(&context->batch, &token))
			(void)omcgq_bank_quarantine(&token, NULL, false);
	}
	(void)omcgq_batch_complete(&context->batch, result == NET_XMIT_SUCCESS);
}
static uint32_t __sram_text tqe_push_mc_ports(void *queue,
		const struct topaz_fwt_sw_mcast_entry *mcast_ent_shared,
		const union topaz_tqe_cpuif_descr *desc, uint8_t tid, uint8_t in_node,
		uint32_t header_access_bytes)
{
	struct topaz_fwt_sw_mcast_entry mcast_ent;
	enum topaz_tqe_port in_port = desc->data.in_port;
	struct omtqe_mcast_context context;
	union topaz_tqe_cpuif_ppctl payload_owner;
	uint32_t push_count;
	uint32_t pushes = 0;
	uint8_t port = TOPAZ_TQE_FIRST_PORT;
	void *buf_bus_rx = desc->data.pkt;
	void *buf_virt_rx = bus_to_virt((unsigned long) buf_bus_rx);
	const struct ether_header *eh = buf_virt_rx;
	int offset = desc->data.buff_ptr_offset;

	/* On a quarantined bank, refuse before creating multicast refcounts. */
	if (omtqe_lhost_quarantined())
		return 0;
	mcast_ent = *mcast_ent_shared;

	/* The MuC handles snooped multicast directly */
	if (in_port == TOPAZ_TQE_WMAC_PORT || in_port == TOPAZ_TQE_MUC_PORT) {
		if (printk_ratelimit())
			printk("%s: mcast pkt from mac t=%04x d=%pM s=%pM\n", __FUNCTION__,
				eh->ether_type, eh->ether_dhost, eh->ether_shost);
		return 0;
	}

	/* find the expected enqueue count and set the HBM buffer reference count */
	push_count = topaz_fwt_sw_mcast_enqueues(&mcast_ent, mcast_ent.port_bitmap,
						in_port, in_node);
	if (unlikely(!push_count)) {
		return 0;
	}

	memset(&payload_owner, 0, sizeof(payload_owner));
	payload_owner.data.pkt = desc->data.pkt;
	payload_owner.data.length = desc->data.length;
	payload_owner.data.buff_ptr_offset = desc->data.buff_ptr_offset;
	context.queue = queue;
	if (!omcgq_batch_admit(THIS_MODULE, &payload_owner, push_count, &context.batch))
		return 0;
	tqe_buf_set_refcounts((uint8_t *)buf_virt_rx + offset, push_count, 0);

	flush_and_inv_dcache_range((unsigned long)buf_virt_rx, (unsigned long)(buf_virt_rx + header_access_bytes));

	/* push this packet to the tqe for each port/node */
	while (mcast_ent.port_bitmap) {
		if (mcast_ent.port_bitmap & 0x1) {
			if (topaz_fwt_sw_mcast_port_has_nodes(port)) {
				pushes += topaz_fwt_sw_mcast_do_per_node(omtqe_push_mcast_batch,
							&mcast_ent, desc, &context, in_node, port, tid);
			} else {
				if (port != in_port)  {
					omtqe_push_mcast_batch(desc, &context, port, 0, 0);
					++pushes;
				}
			}
		}
		mcast_ent.port_bitmap >>= 1;
		port++;
	}

	if (unlikely(pushes != push_count)) {
		printk(KERN_CRIT "%s: pushes %u push_count %u, buffer leak imminent\n",
				__FUNCTION__, pushes, push_count);
	}

	(void)omcgq_batch_finish(&context.batch);
	return push_count;
}
static void run_kind(unsigned k){(void)k;}
static void clear(void){memset(omcgq_held,0,sizeof(omcgq_held));omcgq_held_count=omcgq_unrecorded=omcgq_generation=omcgq_batch_generation=0;omcgq_reserved=omcgq_quarantined=omcgq_retained=false;omcgq_owner=NULL;memset(&omcgq_batch,0,sizeof(omcgq_batch));provider.refs=consumer.refs=0;provider.deny=consumer.deny=false;irq_disabled=publications=collisions=queue_calls=drops=waits=pending_kind=scheduled_kind=inject_phase=injected=foreign_kind=foreign_inflight=0;ready_reads=complete_reads=postsubmit=vlan_denied=egress_calls=ec_writes=ec_value=fc_value=cache_ops=0;ready_on=complete_on=1;vlan_enabled=0;}

#define likely(x) (x)
#define KERN_WARNING ""
#define ETH_P_IP 0x0800
#define ETH_P_IPV6 0x86dd
#define VLAN_HLEN 4
#define __constant_htons(x) ((uint16_t)(((x)>>8)|((x)<<8)))
static int qtn_ether_type_is_vlan(uint16_t type){return type==__constant_htons(0x8100)||type==__constant_htons(0x88a8);}
typedef int(*fwt_false_miss_cb)(unsigned,uint8_t);
static unsigned lookup_calls,hook_calls,miss_calls,lookup_timeout,lookup_valid,lookup_falsemiss,hook_present,hook_null,node_result;
static const void *hook_ip;
static uint16_t hook_type;
union topaz_fwt_lookup{struct{unsigned trig:1,unused:7,hash_addr:10,unused2:2,entry_addr:11,valid:1;}data;};
static union topaz_fwt_lookup topaz_fwt_hw_lookup_wait_be(const uint8_t*mac,int*timeout,uint8_t*miss){(void)mac;lookup_calls++;*timeout=lookup_timeout;*miss=lookup_falsemiss;union topaz_fwt_lookup r={0};r.data.valid=lookup_valid;r.data.entry_addr=3;return r;}
static int false_miss(unsigned entry,uint8_t value){assert(entry==3&&value==lookup_falsemiss);miss_calls++;return 0;}
static const struct topaz_fwt_sw_mcast_entry *lookup_hook(unsigned entry,const void*ip,uint16_t type);
static const struct topaz_fwt_sw_mcast_entry *(*g_tqe_fwt_get_mcast_hook)(unsigned,const void*,uint16_t);
static fwt_false_miss_cb g_tqe_fwt_false_miss_hook;
static int tqe_rx_get_node_id(const struct ether_header*eh){(void)eh;return node_result;}
union qtn_ipv4_addr {
	uint32_t ip32;
	uint16_t ip16[2];
	uint8_t ip8[4];
};

struct qtn_ipv4 {
	uint8_t	vers_ihl;
	uint8_t dscp;
	uint16_t length;
	uint16_t ident;
	uint16_t flags:3,
		 fragoffset:13;
	uint8_t ttl;
	uint8_t proto;
	uint16_t csum;
	union qtn_ipv4_addr srcip;
	union qtn_ipv4_addr dstip;
	uint32_t opt[0];
};

union qtn_ipv6_addr {
	uint64_t ip64[2];
	uint32_t ip32[4];
	uint16_t ip16[8];
	uint8_t ip8[16];
};

struct qtn_ipv6 {
	uint16_t vers_tclass_flowlabel[2];
	uint16_t length;
	uint8_t next_hdr;
	uint8_t hop_limit;
	union qtn_ipv6_addr srcip;
	union qtn_ipv6_addr dstip;
};


#define QTN_IP_PROTO_IGMP 2
static inline int iputil_mac_is_v6_multicast(const uint8_t *mac)
{
	const char ipmc6_addr[] = {0x33, 0x33};

	return mac[0] == ipmc6_addr[0] &&
		mac[1] == ipmc6_addr[1];
}static inline int iputil_mac_is_v4_multicast(const uint8_t *mac)
{
	const char ipmc4_addr[] = {0x01, 0x00, 0x5E};

	return mac[0] == ipmc4_addr[0] &&
		mac[1] == ipmc4_addr[1] &&
		mac[2] == ipmc4_addr[2];
}const uint16_t *tqe_rx_ether_type_skip_vlan(const struct ether_header *eh, uint32_t len)
{
	const uint16_t *ether_type = &eh->ether_type;

	if (len < sizeof(struct ether_header)) {
		return NULL;
	}

	while(qtn_ether_type_is_vlan(*ether_type)) {
		if (len < sizeof(struct ether_header) + VLAN_HLEN) {
			return NULL;
		}
		ether_type += VLAN_HLEN / sizeof(*ether_type);
		len -= VLAN_HLEN;
	}

	return ether_type;
}int __sram_text tqe_rx_multicast(void *congest_queue, const union topaz_tqe_cpuif_descr *desc)
{
	int timeout;
	union topaz_fwt_lookup fwt_lu;
	const struct topaz_fwt_sw_mcast_entry *mcast_ent = NULL;
	const struct ether_header *eh = bus_to_virt((uintptr_t) desc->data.pkt);
	const enum topaz_tqe_port in_port = desc->data.in_port;
	const void *ipaddr = NULL;
	uint8_t tid = 0;
	const uint16_t *ether_type = tqe_rx_ether_type_skip_vlan(eh, desc->data.length);
	const void *iphdr = NULL;
	uint8_t vlan_index;
	uint8_t in_node = 0;
	uint32_t header_access_bytes = 0;
	uint32_t ether_payload_length = 0;
	uint8_t false_miss = 0;

	if (unlikely(!ether_type)) {
		printk(KERN_WARNING "%s: malformed packet in_port %u\n", __FUNCTION__, in_port);
		return 0;
	}

	iphdr = ether_type + 1;
	ether_payload_length = desc->data.length - ((char *)iphdr - (char *)eh);
	header_access_bytes = iphdr - (const void *)eh;

	/* FIXME: this won't work for 802.3 frames */
	if (*ether_type == __constant_htons(ETH_P_IP)
			&& iputil_mac_is_v4_multicast(eh->ether_dhost)
			&& (ether_payload_length >= sizeof(struct qtn_ipv4))) {
		const struct qtn_ipv4 *ipv4 = (const struct qtn_ipv4 *)iphdr;
		/* do not accelerate IGMP */
		if (ipv4->proto == QTN_IP_PROTO_IGMP) {
			return 0;
		}
		ipaddr = &ipv4->dstip;

		/* Option field doesn't take into account because they are not accessed */
		header_access_bytes += sizeof (struct qtn_ipv4);
	} else if (*ether_type == __constant_htons(ETH_P_IPV6)
			&& iputil_mac_is_v6_multicast(eh->ether_dhost)
			&& (ether_payload_length >= sizeof(struct qtn_ipv6))) {
		const struct qtn_ipv6 *ipv6 = (const struct qtn_ipv6 *)iphdr;
		ipaddr = &ipv6->dstip;

		header_access_bytes += sizeof (struct qtn_ipv6);
	}

	if (ipaddr) {
		topaz_tqe_vlan_gettid(desc->data.pkt, &tid, &vlan_index);
		fwt_lu = topaz_fwt_hw_lookup_wait_be(eh->ether_dhost, &timeout, &false_miss);
		if (fwt_lu.data.valid && !timeout) {
#ifndef TOPAZ_DISABLE_FWT_WAR
			if (unlikely(false_miss && g_tqe_fwt_false_miss_hook))
				g_tqe_fwt_false_miss_hook(fwt_lu.data.entry_addr, false_miss);
#endif

			if (g_tqe_fwt_get_mcast_hook)
				mcast_ent = g_tqe_fwt_get_mcast_hook(fwt_lu.data.entry_addr,
						ipaddr, *ether_type);

			if (mcast_ent) {
				if ((mcast_ent->flood_forward) && (in_port == TOPAZ_TQE_MUC_PORT)) {
					in_node = tqe_rx_get_node_id(eh);
					if (in_node == 0)
						return 0;
				}
				return tqe_push_mc_ports(congest_queue, mcast_ent, desc, tid,
								in_node, header_access_bytes);
			}
		}
	}

	return 0;
}
static struct topaz_fwt_sw_mcast_entry group;
static const struct topaz_fwt_sw_mcast_entry *lookup_hook(unsigned entry,const void*ip,uint16_t type){assert(entry==3);hook_calls++;hook_ip=ip;hook_type=type;return hook_null?NULL:&group;}
static unsigned char packet[160];
static union topaz_tqe_cpuif_descr make_packet(int ipv6,int vlan,unsigned length){
 memset(packet,0,sizeof(packet));struct ether_header*eh=(void*)packet;eh->ether_dhost[0]=ipv6?0x33:1;eh->ether_dhost[1]=ipv6?0x33:0;eh->ether_dhost[2]=ipv6?0:0x5e;
 eh->ether_type=__constant_htons(vlan?0x8100:(ipv6?ETH_P_IPV6:ETH_P_IP));unsigned offset=14;if(vlan){*(uint16_t*)(packet+16)=__constant_htons(ipv6?ETH_P_IPV6:ETH_P_IP);offset=18;}
 if(ipv6){struct qtn_ipv6*ip=(void*)(packet+offset);ip->next_hdr=17;}else{struct qtn_ipv4*ip=(void*)(packet+offset);ip->proto=17;}
 union topaz_tqe_cpuif_descr d={.data={.pkt=packet,.length=length,.in_port=0}};return d;
}
static void clear_outer(void){clear();lookup_calls=hook_calls=miss_calls=lookup_timeout=lookup_falsemiss=hook_null=0;lookup_valid=1;node_result=1;g_tqe_fwt_get_mcast_hook=lookup_hook;g_tqe_fwt_false_miss_hook=false_miss;memset(&group,0,sizeof(group));group.port_bitmap=2;}
int main(void){unsigned cases=0;union topaz_tqe_cpuif_descr d;
for(unsigned len=0;len<14;len++){clear_outer();d=make_packet(0,0,len);assert(tqe_rx_multicast(NULL,&d)==0&&!lookup_calls&&!ec_writes&&!publications);cases++;}
for(unsigned len=14;len<34;len++){clear_outer();d=make_packet(0,0,len);assert(tqe_rx_multicast(NULL,&d)==0&&!lookup_calls&&!ec_writes);cases++;}
for(unsigned len=14;len<54;len++){clear_outer();d=make_packet(1,0,len);assert(tqe_rx_multicast(NULL,&d)==0&&!lookup_calls&&!ec_writes);cases++;}
for(unsigned type=0;type<2;type++)for(unsigned vlan=0;vlan<2;vlan++){
 clear_outer();d=make_packet(type,vlan,type?(vlan?58:54):(vlan?38:34));assert(tqe_rx_multicast(NULL,&d)==1&&lookup_calls==1&&hook_calls==1&&ec_value==1&&publications==1&&omcgq_held_count==0);cases++;
 clear_outer();d=make_packet(type,vlan,64);lookup_timeout=1;assert(tqe_rx_multicast(NULL,&d)==0&&lookup_calls==1&&!hook_calls&&!ec_writes);cases++;
 clear_outer();d=make_packet(type,vlan,64);lookup_valid=0;assert(tqe_rx_multicast(NULL,&d)==0&&lookup_calls==1&&!hook_calls&&!ec_writes);cases++;
 clear_outer();d=make_packet(type,vlan,64);hook_null=1;assert(tqe_rx_multicast(NULL,&d)==0&&hook_calls==1&&!ec_writes);cases++;
 clear_outer();d=make_packet(type,vlan,64);g_tqe_fwt_get_mcast_hook=NULL;assert(tqe_rx_multicast(NULL,&d)==0&&!hook_calls&&!ec_writes);cases++;
 clear_outer();d=make_packet(type,vlan,64);lookup_falsemiss=2;assert(tqe_rx_multicast(NULL,&d)==1&&miss_calls==1&&publications==1);cases++;
}
clear_outer();d=make_packet(0,0,34);((struct qtn_ipv4*)(packet+14))->proto=2;assert(tqe_rx_multicast(NULL,&d)==0&&!lookup_calls&&!ec_writes);cases++;
clear_outer();d=make_packet(0,0,34);packet[0]=2;assert(tqe_rx_multicast(NULL,&d)==0&&!lookup_calls);cases++;
clear_outer();d=make_packet(0,1,17);assert(tqe_rx_multicast(NULL,&d)==0&&!lookup_calls);cases++;
clear_outer();d=make_packet(0,0,34);omcgq_held_count=256;assert(tqe_rx_multicast(NULL,&d)==0&&lookup_calls==1&&!ec_writes&&!publications);cases++;
clear_outer();d=make_packet(0,0,34);complete_on=0;assert(tqe_rx_multicast(NULL,&d)==1&&publications==1&&ec_value==1&&omcgq_held_count==1&&omcgq_quarantined);cases++;
clear_outer();d=make_packet(0,0,34);d.data.in_port=5;group.flood_forward=1;node_result=0;assert(tqe_rx_multicast(NULL,&d)==0&&!ec_writes);cases++;
clear_outer();d=make_packet(0,0,34);d.data.in_port=5;group.flood_forward=1;node_result=3;assert(tqe_rx_multicast(NULL,&d)==0&&!ec_writes);cases++;
printf("{\"cases\":%u}\n",cases);return 0;}
