
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
static int vlan_enabled,g_dscp_flag,g_dscp_value[2],vlan_denied;static unsigned egress_calls;
static unsigned irq_disabled,irq_saves,irq_restores,publications,collisions,queue_calls,drops,waits;
static unsigned pending_kind,scheduled_kind,inject_phase,injected,foreign_kind,foreign_inflight;
static uint32_t last_status;
static unsigned bank_writes[4],writes_in_transaction;
static union topaz_tqe_cpuif_descr incoming={.data={.pkt=(void*)64,.length=64}};
static fwt_db_entry fwt={.out_port=1,.out_node=3,.portal=1};
static void run_kind(unsigned kind);
static void reset(void);
static unsigned postsubmit;
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
static int ready_state(void);
static int topaz_tqe_cpuif_tx_nready(void){return ready_state();}
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
static int status_state(uint32_t address);
static uint32_t qtn_mproc_sync_mem_read(uint32_t address){return status_state(address);}
static void wmb(void){phase(11);}
static int qtn_vlan_egress(struct qtn_vlan_dev*d,int n,void*b){(void)d;(void)n;(void)b;egress_calls++;return !vlan_denied;}
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
#define NETDEV_TX_OK 0
#define NETDEV_TX_BUSY 16
#define TOPAZ_HBM_BUF_EMAC_RX_SIZE 4096
#define TOPAZ_HBM_BUF_EMAC_RX_POOL 0
#define TOPAZ_HBM_OWNER_LH_TX_TQE 1
#define OPENMITRASTAR_QDRV_V89_PAIR 1
#define M_VLAN_TAGGED 1
#define M_FLAG_ISSET(s,f) ((s)->flags&(f))
#define QVLAN_PKTCTRL_LEN 4
#define unlikely(x) (x)
#define KERN_ERR ""
#define printk(...) ((void)0)
#define printk_ratelimit() 0
struct shinfo { int dataref; };
struct sk_buff { unsigned len; void *data; int users;struct shinfo sh;int hbm_no_free,freed,flags; };
#define skb_shinfo(s) (&(s)->sh)
#define refcount_read(p) (*(p))
#define atomic_read(p) (*(p))
static unsigned ready_reads,complete_reads,ready_on,complete_on,completed_status,postsubmit;
static unsigned free_count,alloc_count,hbm_available,relaxes,queue_dequeues,queue_frees,queue_retries;


static unsigned char original_buf[128],copy_buf[128];
static int direct_pool;
static void cpu_relax(void){relaxes++;}
static int ready_state(void){
 phase(1);
 if(postsubmit){complete_reads++;return !complete_on||complete_reads<complete_on;}
 ready_reads++;return !ready_on||ready_reads<ready_on;
}
static int status_state(uint32_t address){assert(address==0xe1034038U);return completed_status;}
static void *topaz_hbm_get_payload_virt(int pool){(void)pool;alloc_count++;return hbm_available?copy_buf:NULL;}
static int topaz_hbm_payload_get_pool_bus(void*p){(void)p;return direct_pool;}
static int topaz_hbm_pool_valid(int p){return p>=0&&p<4;}
static void topaz_hbm_flush_skb_cache(struct sk_buff*s){(void)s;}
static void topaz_hbm_filter_txdone_pool(void){}
static void topaz_hbm_debug_stamp(void*a,int b,unsigned c){(void)a;(void)b;(void)c;}
static void *qtn_vlan_get_info(void*p){return p;}
static void *align_buf_cache(void*p){return p;}
static size_t align_buf_cache_size(void*p,size_t n){(void)p;return n;}
static void flush_and_inv_dcache_range(uintptr_t a,uintptr_t b){assert(b>=a);}
static void kfree_skb(struct sk_buff*s){assert(!s->freed);s->freed=1;free_count++;}
#define dev_kfree_skb(s) kfree_skb(s)

typedef uint32_t u32;
struct module {unsigned refs;bool deny;};
static struct module om_provider,om_consumer;
#define THIS_MODULE (&om_consumer)
static bool try_module_get(struct module*m){if(m->deny)return false;m->refs++;return true;}
static void module_put(struct module*m){assert(m->refs);m->refs--;}
struct omcgq_bank_token{uint32_t generation;struct module *owner;};
struct omcgq_batch_token{uint32_t generation;struct module *owner;};
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
		if (!try_module_get(&om_provider)) {
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
		if (!try_module_get(&om_provider)) {
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
}static int omtqe_lhost_quarantined(void) { return omcgq_bank_quarantined(); }
static int omtqe_lhost_reserve(struct omcgq_bank_token *token)
{
	unsigned long flags;
	unsigned int count;
	int ready = 0;
	if (!omcgq_bank_acquire(THIS_MODULE, token))
		return 0;
	local_irq_save(flags);
	for (count = 0; count < OMTQE_READY_READ_MAX; count++) {
		if (!topaz_tqe_cpuif_tx_nready()) {
			ready = 1;
			break;
		}
		cpu_relax();
	}
	if (!ready)
		(void)omcgq_bank_quarantine(token, NULL, false);
	local_irq_restore(flags);
	return ready;
}
static void omtqe_lhost_cancel_reservation(const struct omcgq_bank_token *token)
{
	(void)omcgq_bank_cancel(token);
}
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
static int omtqe_lhost_submit(const union topaz_tqe_cpuif_ppctl *ppctl)
{
	struct omcgq_bank_token token;
	if (!omtqe_lhost_reserve(&token)) {
		(void)omcgq_bank_retain(ppctl, false);
		return OMTQE_OWNERSHIP_REFUSED;
	}
	return omtqe_lhost_submit_reserved(&token, ppctl);
}
int topaz_tqe_xmit(union topaz_tqe_cpuif_ppctl *pp_cntl)
{
	return omtqe_lhost_submit(pp_cntl);
}
static void __sram_text tqe_push_mcast(const void *token1, void *token2,
					uint8_t port, uint8_t node, uint8_t tid)
{
	const union topaz_tqe_cpuif_descr *desc = token1;
	union topaz_tqe_cpuif_ppctl ppctl;
	const uint8_t portal = 1;
	const uint16_t misc_user = 0;
	void *queue = token2;
	uint8_t tqe_free = queue ? 0 : 1;
	struct qtn_vlan_dev *vdev;

	if (omtqe_lhost_quarantined()) {
		topaz_tqe_cpuif_ppctl_init(&ppctl, port, &node, 1, tid,
			portal, 1, TOPAZ_HBM_EMAC_TX_DONE_POOL, tqe_free, misc_user);
		ppctl.data.pkt = desc->data.pkt;
		ppctl.data.length = desc->data.length;
		ppctl.data.buff_ptr_offset = desc->data.buff_ptr_offset;
		(void)omcgq_bank_retain(&ppctl, false);
		return;
	}
	/*
	 * VLAN egress check -- multicast forwarding
	 * for out_port==WMAC, AuC handles egress
	 */
	if (vlan_enabled && TOPAZ_TQE_PORT_IS_EMAC(port)) {
		vdev = vport_tbl_lhost[port];
		if (!qtn_vlan_egress(vdev, 0, bus_to_virt((uintptr_t)desc->data.pkt))) {
			tqe_rx_pkt_drop(desc);
			return;
		}
	}

	topaz_tqe_cpuif_ppctl_init(&ppctl,
			port, &node, 1, tid,
			portal, 1, TOPAZ_HBM_EMAC_TX_DONE_POOL, tqe_free, misc_user);

	ppctl.data.pkt = desc->data.pkt;
	ppctl.data.length = desc->data.length;
	ppctl.data.buff_ptr_offset = desc->data.buff_ptr_offset;

	if (queue) {
		topaz_tqe_congest_queue_process(desc, queue, node, tid, &ppctl, 0);
	} else {
		(void)omtqe_lhost_submit(&ppctl);
		/* Existing void/consumed return retains pre-owned payload on refusal. */
	}

	if (port == TOPAZ_TQE_WMAC_PORT && update_multicast.fn)
		update_multicast.fn(update_multicast.ctx, node);
}
static int topaz_swfwd_tqe_xmit(const fwt_db_entry *fwt_ent,
				const union topaz_tqe_cpuif_descr *desc,
				void *queue)
{
	uint8_t port;
	uint8_t node;
	uint16_t misc_user;
	uint8_t tid = 0;
	union topaz_tqe_cpuif_ppctl ctl;
	uint8_t portal;
	uint8_t vlan_index;
	uint8_t tqe_free = queue ? 0 : 1;
	struct qtn_vlan_dev *vdev;

	if (omtqe_lhost_quarantined())
		return 0; /* no publication; ordinary RX caller still owns descriptor */
	if (fwt_ent->out_port == TOPAZ_TQE_LHOST_PORT)
		return 0;

	/*
	 * VLAN egress check -- unicast forwarding
	 * for out_port==WMAC, AuC handles egress
	 */
	if (vlan_enabled && TOPAZ_TQE_PORT_IS_EMAC(fwt_ent->out_port)) {
		vdev = vport_tbl_lhost[fwt_ent->out_port];
		if (!qtn_vlan_egress(vdev, 0, bus_to_virt((uintptr_t)desc->data.pkt))) {
			tqe_rx_pkt_drop(desc);
			return 1;
		}
	}

	if (TOPAZ_TQE_PORT_IS_EMAC(desc->data.in_port)) {
		if(g_dscp_flag){
			tid = (g_dscp_value[desc->data.in_port] & 0xFF);
		} else {
			topaz_tqe_vlan_gettid(desc->data.pkt, &tid, &vlan_index);
		}
	} else {
		topaz_tqe_vlan_gettid(desc->data.pkt, &tid, &vlan_index);
	}

	port = fwt_ent->out_port;
	node = fwt_ent->out_node;
	portal = fwt_ent->portal;
	misc_user = 0;
	topaz_tqe_cpuif_ppctl_init(&ctl,
			port, &node, 1, tid,
			portal, 1, 0, tqe_free, misc_user);

	ctl.data.pkt = (void *)desc->data.pkt;
	ctl.data.buff_ptr_offset = desc->data.buff_ptr_offset;
	ctl.data.length = desc->data.length;
	ctl.data.buff_pool_num = TOPAZ_HBM_EMAC_TX_DONE_POOL;

	if (queue) {
		topaz_tqe_congest_queue_process(desc, queue, node, tid, &ctl, 1);
	} else {
		(void)omtqe_lhost_submit(&ctl);
		/* Existing void/consumed return retains pre-owned payload on refusal. */
	}

	return 1;
}
static int tqe_tx_buf(const struct omcgq_bank_token *token, union topaz_tqe_cpuif_ppctl *ppctl,
		void __iomem *virt_buf, unsigned long data_len, int8_t pool)
{
	const uintptr_t bus_data_start = virt_to_bus(virt_buf);
	const long buff_ptr_offset = topaz_hbm_payload_buff_ptr_offset_bus((void *)bus_data_start, pool, NULL);

	ppctl->data.pkt = (void *) bus_data_start;
	ppctl->data.buff_ptr_offset = buff_ptr_offset;
	ppctl->data.length = data_len;
	/* always free to txdone pool */
	ppctl->data.buff_pool_num = TOPAZ_HBM_EMAC_TX_DONE_POOL;

	(void)omtqe_lhost_submit_reserved(token, ppctl);
	/* skb was consumed: postsubmit uncertainty remains retained, never BUSY. */
	return 0;
}
int tqe_tx(union topaz_tqe_cpuif_ppctl *ppctl, struct sk_buff *skb)
{
	struct omcgq_bank_token token;
	unsigned int data_len = skb->len;
	void *buf_virt = skb->data;
	void *buf_bus = (void *) virt_to_bus(buf_virt);
	void *buf_virt_vlan;
	int8_t pool = topaz_hbm_payload_get_pool_bus(buf_bus);
	const bool hbm_can_use = !vlan_enabled &&
		topaz_hbm_pool_valid(pool) &&
		(refcount_read(&skb->users) == 1) &&
		(atomic_read(&skb_shinfo(skb)->dataref) == 1);

	/* Admission precedes skb/HBM mutation. A refused skb remains caller-owned. */
	if (!omtqe_lhost_reserve(&token))
		return NETDEV_TX_BUSY;

	if (hbm_can_use) {
		/*
		 * skb is otherwise unused; clear to send out tqe.
		 * Set flag such that payload isn't returned to the hbm on free
		 */
		skb->hbm_no_free = 1;

		topaz_hbm_flush_skb_cache(skb);
	} else {
		void *hbm_buf_virt;
		uintptr_t flush_start;
		size_t flush_size;

		if (data_len < TOPAZ_HBM_BUF_EMAC_RX_SIZE) {
			pool = TOPAZ_HBM_BUF_EMAC_RX_POOL;
		} else {
			/*
			 * requested impossibly large transmission
			 */
			if (printk_ratelimit()) {
				printk(KERN_ERR "%s: requested oversize transmission"
						" (%u bytes) to port %d\n",
						__FUNCTION__, data_len, ppctl->data.out_port);
			}
			kfree_skb(skb);
			omtqe_lhost_cancel_reservation(&token);
			return NETDEV_TX_OK;
		}

		/*
		 * The OEM custom skb allocator reclaimed completed TQE payloads from
		 * EMAC_TX_DONE.  The 5.10 copy-out path has no allocator destructor, so
		 * reclaim before consuming another pool buffer.
		 */
#ifdef OPENMITRASTAR_QDRV_V89_PAIR
		topaz_hbm_filter_txdone_pool();
#endif
		hbm_buf_virt = topaz_hbm_get_payload_virt(pool);
		if (unlikely(!hbm_buf_virt)) {
			/* Prepublication: skb/HBM ownership is unchanged. */
			omtqe_lhost_cancel_reservation(&token);
			return NETDEV_TX_BUSY;
		}

		topaz_hbm_debug_stamp(hbm_buf_virt, TOPAZ_HBM_OWNER_LH_TX_TQE, data_len);

		memcpy(hbm_buf_virt, buf_virt, data_len);
		buf_virt = hbm_buf_virt;

		if (M_FLAG_ISSET(skb, M_VLAN_TAGGED)) {
			buf_virt_vlan = qtn_vlan_get_info(buf_virt);
			memcpy(buf_virt_vlan, (uint8_t *)qtn_vlan_get_info(skb->data),
					QVLAN_PKTCTRL_LEN);
			flush_start = (uintptr_t) align_buf_cache(buf_virt_vlan);
			flush_size = align_buf_cache_size(buf_virt_vlan, data_len + QVLAN_PKTCTRL_LEN);
		} else {
			flush_start = (uintptr_t) align_buf_cache(buf_virt);
			flush_size = align_buf_cache_size(buf_virt, data_len);
		}

		flush_and_inv_dcache_range(flush_start, flush_start + flush_size);
	}
	dev_kfree_skb(skb);

	tqe_tx_buf(&token, ppctl, buf_virt, data_len, pool);

	return NETDEV_TX_OK;
}
static unsigned cases;
static void clear_all(void){
 reset();omcgq_reserved=omcgq_quarantined=omcgq_retained=false;omcgq_generation=omcgq_held_count=omcgq_unrecorded=0;omcgq_owner=NULL;om_provider.refs=om_consumer.refs=0;
 memset(omcgq_held,0,sizeof(omcgq_held));
 ready_reads=complete_reads=free_count=alloc_count=relaxes=postsubmit=completed_status=0;
 ready_on=complete_on=1;hbm_available=1;direct_pool=-1;queue_dequeues=queue_frees=queue_retries=0;
}
static struct sk_buff fresh_skb(void){struct sk_buff s={.len=64,.data=original_buf,.users=1,.sh={1}};return s;}
static void assert_quarantined_no_future_submit(void){
 union topaz_tqe_cpuif_ppctl p={0};unsigned before=publications;
 assert(topaz_tqe_xmit(&p)==OMTQE_OWNERSHIP_REFUSED);
 assert(publications==before&&!irq_disabled&&!omcgq_reserved);
}
static void run_kind(unsigned kind){
 union topaz_tqe_cpuif_ppctl ctl={0};
 if(kind==1){int r=topaz_tqe_xmit(&ctl);assert(r==NET_XMIT_SUCCESS||r==OMTQE_OWNERSHIP_REFUSED);}
 else if(kind==2)tqe_push_mcast(&incoming,NULL,1,3,0);
 else if(kind==3)(void)topaz_swfwd_tqe_xmit(&fwt,&incoming,NULL);
 else if(kind==4){struct sk_buff s=fresh_skb();(void)tqe_tx(&ctl,&s);}
}
static void reset(void){
 irq_disabled=irq_saves=irq_restores=publications=collisions=queue_calls=drops=waits=0;
 pending_kind=scheduled_kind=inject_phase=injected=foreign_kind=foreign_inflight=0;
 memset(bank_writes,0,sizeof(bank_writes));last_status=0;
}
int main(void){
 union topaz_tqe_cpuif_ppctl p={0};struct sk_buff s;int r;
 clear_all();ready_on=0;r=topaz_tqe_xmit(&p);assert(r==OMTQE_OWNERSHIP_REFUSED&&ready_reads==1024&&publications==0&&omcgq_quarantined&&!irq_disabled);assert_quarantined_no_future_submit();cases++;
 clear_all();ready_on=1024;assert(topaz_tqe_xmit(&p)==0&&ready_reads==1024&&complete_reads==1&&publications==1&&!irq_disabled);cases++;
 clear_all();ready_on=1025;assert(topaz_tqe_xmit(&p)==OMTQE_OWNERSHIP_REFUSED&&publications==0);cases++;
 clear_all();complete_on=0;r=topaz_tqe_xmit(&p);assert(r==OMTQE_OWNERSHIP_UNCERTAIN&&publications==1&&complete_reads==1024&&omcgq_held[0].published&&omcgq_quarantined&&!irq_disabled);assert_quarantined_no_future_submit();cases++;
 clear_all();complete_on=1024;assert(topaz_tqe_xmit(&p)==0&&complete_reads==1024&&!omcgq_quarantined);cases++;
 clear_all();complete_on=1025;assert(topaz_tqe_xmit(&p)==OMTQE_OWNERSHIP_UNCERTAIN);cases++;
 clear_all();completed_status=0x40000000U;assert(topaz_tqe_xmit(&p)==NET_XMIT_CN&&!omcgq_quarantined&&complete_reads==1);cases++;
 clear_all();ready_on=0;s=fresh_skb();assert(tqe_tx(&p,&s)==NETDEV_TX_BUSY&&!s.freed&&!s.hbm_no_free&&!alloc_count);cases++;
 clear_all();hbm_available=0;s=fresh_skb();assert(tqe_tx(&p,&s)==NETDEV_TX_BUSY&&!s.freed&&!omcgq_reserved&&!omcgq_quarantined);cases++;
 clear_all();s=fresh_skb();s.len=4096;assert(tqe_tx(&p,&s)==NETDEV_TX_OK&&s.freed&&!publications&&!omcgq_reserved);cases++;
 clear_all();complete_on=0;s=fresh_skb();assert(tqe_tx(&p,&s)==NETDEV_TX_OK&&s.freed&&free_count==1&&publications==1&&omcgq_quarantined);assert_quarantined_no_future_submit();cases++;
 clear_all();direct_pool=1;complete_on=0;s=fresh_skb();assert(tqe_tx(&p,&s)==0&&s.freed&&s.hbm_no_free&&free_count==1&&alloc_count==0);cases++;
 clear_all();complete_on=0;tqe_push_mcast(&incoming,NULL,1,3,0);assert(publications==1&&omcgq_quarantined);tqe_push_mcast(&incoming,NULL,1,4,0);assert(publications==1);cases++;
 clear_all();complete_on=0;assert(topaz_swfwd_tqe_xmit(&fwt,&incoming,NULL)==1&&publications==1&&omcgq_quarantined);assert(topaz_swfwd_tqe_xmit(&fwt,&incoming,NULL)==0&&publications==1);cases++;
 for(unsigned first=1;first<=4;first++)for(unsigned intr=1;intr<=4;intr++)for(unsigned at=1;at<=8;at++){
  clear_all();scheduled_kind=intr;inject_phase=at;run_kind(first);
  assert(!irq_disabled&&!omcgq_reserved&&collisions==0&&publications<=2);cases++;
 }
 clear_all();irq_disabled=1;assert(topaz_tqe_xmit(&p)==0&&irq_disabled==1);cases++;
 clear_all();inject_phase=3;foreign_kind=1;assert(topaz_tqe_xmit(&p)==0&&collisions==1);cases++;

 clear_all();complete_on=0;assert(topaz_tqe_xmit(&p)==OMTQE_OWNERSHIP_UNCERTAIN);union topaz_tqe_cpuif_ppctl held_first=omcgq_held[0].descriptor;unsigned held_before=omcgq_held_count;vlan_enabled=1;vlan_denied=1;egress_calls=0;drops=0;
 tqe_push_mcast(&incoming,NULL,1,3,0);assert(egress_calls==0&&drops==0&&publications==1&&omcgq_held_count==held_before+1&&!memcmp(&held_first,&omcgq_held[0].descriptor,sizeof(held_first)));cases++;
 tqe_push_mcast(&incoming,(void*)1,1,4,0);assert(egress_calls==0&&drops==0&&publications==1&&!memcmp(&held_first,&omcgq_held[0].descriptor,sizeof(held_first)));cases++;
 vlan_enabled=0;vlan_denied=0;
 printf("{\"executed_cases\":%u}\n",cases);return 0;
}
