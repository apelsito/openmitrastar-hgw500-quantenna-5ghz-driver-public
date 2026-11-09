
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
typedef uint32_t u32;typedef uint8_t u8;
#define TOPAZ_HBM_LOCAL_CPU 0
#define TOPAZ_HBM_POOL_REQ(cpu) (0x110U+4U*(cpu))
#define TOPAZ_HBM_POOL_DATA(cpu) (0x140U+4U*(cpu))
#define TOPAZ_HBM_POOL_NUM(pool) ((pool)<<2)
#define TOPAZ_HBM_DONE (1U<<31)
#define TOPAZ_HBM_RELEASE_BUF 1U
#define TOPAZ_HBM_REQUEST_BUF 2U
static unsigned irq,phase,pre_reads,post_reads,pre_done_on,post_done_on,writes,requests,releases,data_reads,relaxes;
static uint32_t data_value;
static void save(unsigned long*f){*f=irq;irq=1;}
static void restore(unsigned long f){irq=f;}
#define local_irq_save(f) save(&(f))
#define local_irq_restore(f) restore(f)
static void cpu_relax(void){relaxes++;}
static uint32_t qtn_mproc_sync_mem_read(uint32_t address){
 if(address==0x140U){data_reads++;return data_value;}
 assert(address==0x110U);unsigned n;if(!phase){n=++pre_reads;return pre_done_on&&n>=pre_done_on?TOPAZ_HBM_DONE:0;}n=++post_reads;return post_done_on&&n>=post_done_on?TOPAZ_HBM_DONE:0;
}
static void qtn_mproc_sync_mem_write_wmb(uint32_t address,uint32_t value){assert(irq);writes++;if(address==0x140U){data_value=value;return;}assert(address==0x110U);if(value&1)releases++;if(value&2)requests++;phase=1;}

typedef int8_t int8_t;
#define TOPAZ_HBM_BUF_WMAC_RX_POOL 1
#define TOPAZ_HBM_BUF_EMAC_RX_POOL 0
#define TOPAZ_HBM_BUF_MAGIC_CHK_ALLPOOL 1
#define HBM_HR_OFFSET_ENQ_CNT 1
#define HBM_HR_OFFSET_FREE_CNT 2
static u32 counts[4];static int magic_bad,magic_handle;
static int8_t topaz_hbm_payload_get_pool_bus(void*p){(void)p;return 1;}
static void*bus_to_virt(uintptr_t p){return(void*)p;}
static u32*topaz_hbm_buf_get_meta(void*p){(void)p;return counts+3;}
static u32 arc_read_uncached_32(u32*p){return*p;}
static void arc_write_uncached_32(u32*p,u32 v){*p=v;}
static int hbm_buf_check_buf_magic(u32*p){(void)p;return magic_bad;}
static int topaz_hbm_handle_buf_err(u32*p,int pool){(void)p;(void)pool;return magic_handle;}
static void*topaz_hbm_payload_store_align_bus(void*p,int pool,int aligned){(void)pool;(void)aligned;return p;}
#define topaz_hbm_local_irq_save local_irq_save
#define topaz_hbm_local_irq_restore local_irq_restore
static void qtn_pipeline_drain(void){}
static void qtn_mproc_sync_log(const char*p){(void)p;}
#define unlikely(v) (v)
static int __topaz_hbm_is_done(void)
{
	return qtn_mproc_sync_mem_read(TOPAZ_HBM_POOL_REQ(TOPAZ_HBM_LOCAL_CPU)) & TOPAZ_HBM_DONE;
}
static void __topaz_hbm_release_buf(void *buf, uint8_t pool)
{
	/* assumes previous operations are complete */
	qtn_mproc_sync_mem_write_wmb(TOPAZ_HBM_POOL_DATA(TOPAZ_HBM_LOCAL_CPU),
			(unsigned long) buf);
	qtn_mproc_sync_mem_write_wmb(TOPAZ_HBM_POOL_REQ(TOPAZ_HBM_LOCAL_CPU),
			TOPAZ_HBM_POOL_NUM(pool) | TOPAZ_HBM_RELEASE_BUF);
}
static void __topaz_hbm_request_start(uint8_t pool)
{
	qtn_mproc_sync_mem_write_wmb(TOPAZ_HBM_POOL_REQ(TOPAZ_HBM_LOCAL_CPU),
			TOPAZ_HBM_POOL_NUM(pool) | TOPAZ_HBM_REQUEST_BUF);
}
static void * __topaz_hbm_rd_buf(uint8_t pool)
{
	/* must be preceded by __topaz_hbm_rd_req, then polling on __topaz_hbm_is_done */
	return (void *) qtn_mproc_sync_mem_read(TOPAZ_HBM_POOL_DATA(TOPAZ_HBM_LOCAL_CPU));
}/* SOURCE-ONLY requestor0 operation token; not a replacement voidHBM API.
 * Pre-admission refusal leaves payload/refcounts untouched. After REQUEST/RELEASE
 * writes a timeout latches quarantine and forbids reuse, release, reset or retry.
 * MMIO stall latency and foreignprocessorwrites remain outside this read budget.
 */
#define OMHBM_READ_MAX 1024U
struct omhbm_token { u32 generation; };
enum omhbm_result { OMHBM_REFUSED=0, OMHBM_OK=1, OMHBM_PUBLISHED_UNKNOWN=2 };
static u32 omhbm_generation;
static bool omhbm_reserved;
static bool omhbm_quarantined;
static bool omhbm_pending_release;
static void *omhbm_pending_buffer;
static u8 omhbm_pending_pool;

static bool omhbm_token_valid(const struct omhbm_token *token)
{
	return token && token->generation && token->generation == omhbm_generation &&
		omhbm_reserved && !omhbm_quarantined;
}

static int omhbm_admit(struct omhbm_token *token)
{
	unsigned long flags;
	u32 count;
	int admitted=0;
	if (!token)
		return 0;
	token->generation=0;
	local_irq_save(flags);
	if (omhbm_reserved || omhbm_quarantined || omhbm_generation==~0U)
		goto out;
	for (count=0;count<OMHBM_READ_MAX;count++) {
		if (__topaz_hbm_is_done()) {
			omhbm_generation++;
			omhbm_reserved=true;
			token->generation=omhbm_generation;
			admitted=1;
			goto out;
		}
		cpu_relax();
	}
	/* Existingpendingoperation belongs to somebodyelse; neverresetit. */
	omhbm_quarantined=true;
out:
	local_irq_restore(flags);
	return admitted;
}

static int omhbm_cancel(const struct omhbm_token *token)
{
	unsigned long flags;
	int cancelled=0;
	local_irq_save(flags);
	if (omhbm_token_valid(token)) { omhbm_reserved=false;cancelled=1; }
	local_irq_restore(flags);
	return cancelled;
}

static enum omhbm_result omhbm_wait_owned(const struct omhbm_token *token)
{
	u32 count;
	if (!omhbm_token_valid(token))
		return OMHBM_PUBLISHED_UNKNOWN;
	for (count=0;count<OMHBM_READ_MAX;count++) {
		if (__topaz_hbm_is_done())
			return OMHBM_OK;
		cpu_relax();
	}
	omhbm_quarantined=true;
	omhbm_reserved=false;
	return OMHBM_PUBLISHED_UNKNOWN;
}

static enum omhbm_result omhbm_request(const struct omhbm_token *token,u8 pool,void **buffer)
{
	unsigned long flags;
	enum omhbm_result result;
	if (!buffer)
		return OMHBM_REFUSED;
	*buffer=NULL;
	local_irq_save(flags);
	if (!omhbm_token_valid(token)) { result=OMHBM_REFUSED;goto out; }
	omhbm_pending_release=false;omhbm_pending_pool=pool;omhbm_pending_buffer=NULL;
	__topaz_hbm_request_start(pool);
	result=omhbm_wait_owned(token);
	if (result==OMHBM_OK) { *buffer=__topaz_hbm_rd_buf(pool);omhbm_reserved=false; }
out:
	local_irq_restore(flags);
	return result;
}

static enum omhbm_result omhbm_release(const struct omhbm_token *token,u8 pool,void *buffer)
{
	unsigned long flags;
	enum omhbm_result result;
	local_irq_save(flags);
	if (!omhbm_token_valid(token)) { result=OMHBM_REFUSED;goto out; }
	omhbm_pending_release=true;omhbm_pending_pool=pool;omhbm_pending_buffer=buffer;
	__topaz_hbm_release_buf(buffer,pool);
	result=omhbm_wait_owned(token);
	if (result==OMHBM_OK) { omhbm_reserved=false;omhbm_pending_buffer=NULL; }
out:
	local_irq_restore(flags);
	return result;
}
/* SOURCE-ONLY last-reference helper; exact originalrecyclemetadata preserved.
 * TypedAPIonly: anypublishedUNKNOWN mustdisable/retain alloriginalcallers before
 * integration; originalvoid APIs bypassing quarantine are notqualified.
 */
static enum omhbm_result omhbm_final_ref_preserve_abi(void *const buf_bus)
{
	const int8_t pool=topaz_hbm_payload_get_pool_bus(buf_bus);
	uint32_t *buffer,*meta,*enqueuep,*freep;
	uint32_t ec,fc;
	struct omhbm_token token;
	void *aligned;
	unsigned long flags;
	enum omhbm_result result;
	if (pool!=TOPAZ_HBM_BUF_WMAC_RX_POOL && pool!=TOPAZ_HBM_BUF_EMAC_RX_POOL)
		return OMHBM_REFUSED;
	buffer=bus_to_virt((uintptr_t)buf_bus);meta=topaz_hbm_buf_get_meta(buffer);
	if (!meta)
		return OMHBM_REFUSED;
	enqueuep=meta-HBM_HR_OFFSET_ENQ_CNT;freep=meta-HBM_HR_OFFSET_FREE_CNT;
	local_irq_save(flags);
	if (omhbm_quarantined) { result=OMHBM_REFUSED;goto out; }
	ec=arc_read_uncached_32(enqueuep);fc=arc_read_uncached_32(freep);
	if (!ec || fc>=ec || fc==~0U) { result=OMHBM_REFUSED;goto out; }
	if (fc+1<ec) {
		arc_write_uncached_32(freep,fc+1);result=OMHBM_OK;goto out;
	}
	if (!omhbm_admit(&token)) { result=OMHBM_REFUSED;goto out; }
	if (TOPAZ_HBM_BUF_MAGIC_CHK_ALLPOOL || pool==TOPAZ_HBM_BUF_WMAC_RX_POOL) {
		int bad=hbm_buf_check_buf_magic(buffer);
		if (bad && !topaz_hbm_handle_buf_err(buffer,pool)) {
			(void)omhbm_cancel(&token);result=OMHBM_REFUSED;goto out;
		}
	}
	aligned=topaz_hbm_payload_store_align_bus(buf_bus,pool,1);
	/* KeeporiginalrecycleABI. Admission is alreadyheld, noearlierbankwaitcan
	 * happen afterthiscounterreset. Postpublication timeout cannotrollitback. */
	if (ec!=1) {
		arc_write_uncached_32(enqueuep,1);
		arc_write_uncached_32(freep,0);
	}
	result=omhbm_release(&token,(u8)pool,aligned);
out:
	local_irq_restore(flags);
	return result;
}
static void __topaz_hbm_wait(void)
{
	unsigned int timeouts = 0;
	unsigned int timeout_reached = 0;

	while (1) {
		int i;
		if (__topaz_hbm_is_done()) {
			return;
		}

		/* busy wait until buffer is available */
		for (i = 0; i < 10; i++) {
#if defined(AUC_BUILD) && defined(_ARC)
			/*
			 * This is a workaround for MetaWare C Compiler v7.4.0
			 * bug in Zero-Delay Loop code generation for ARC 600 family cores.
			 * Without it the LP_START register will be written to, two
			 * instruction before the end address of the loop, but at least three
			 * instructions are required according to the ARC ISA Programmer's
			 * Reference.
			 */
			_nop();
			_nop();
			_nop();
#else
			qtn_pipeline_drain();
#endif
		}

		if (unlikely(timeout_reached == 0 && timeouts++ == 1000)) {
			timeout_reached = 1;
			qtn_mproc_sync_log("__topaz_hbm_wait timeout");
		}
	}

	if (unlikely(timeout_reached)) {
		qtn_mproc_sync_log("__topaz_hbm_wait succeeded");
	}
}
static void __topaz_hbm_put_buf(void *buf, uint8_t pool)
{
	__topaz_hbm_wait();
	__topaz_hbm_release_buf(buf, pool);
}
static void topaz_hbm_put_buf(void *buf, uint8_t pool)

{
	unsigned long flags;

	topaz_hbm_local_irq_save(flags);
	__topaz_hbm_put_buf(buf, pool);
	topaz_hbm_local_irq_restore(flags);
}
static void clear(void){omhbm_generation=0;omhbm_reserved=omhbm_quarantined=omhbm_pending_release=false;omhbm_pending_buffer=NULL;omhbm_pending_pool=0;irq=phase=pre_reads=post_reads=writes=requests=releases=data_reads=relaxes=0;pre_done_on=post_done_on=1;data_value=0x1234;magic_bad=magic_handle=0;}
int main(void){unsigned cases=0;void*buf=(void*)0x4444;
for(unsigned ec=1;ec<=135;ec++)for(unsigned fc=0;fc<ec;fc++){
 clear();counts[2]=ec;counts[1]=fc;pre_done_on=0;
 enum omhbm_result r=omhbm_final_ref_preserve_abi(buf);
 if(fc+1<ec)assert(r==OMHBM_OK&&counts[1]==fc+1&&!pre_reads&&!writes);
 else assert(r==OMHBM_REFUSED&&counts[1]==fc&&counts[2]==ec&&pre_reads==1024&&!writes);
 assert(!irq);cases++;
}
for(unsigned ec=1;ec<=135;ec++)for(unsigned done=0;done<=1025;done++){
 clear();counts[2]=ec;counts[1]=ec-1;post_done_on=done;
 enum omhbm_result r=omhbm_final_ref_preserve_abi(buf);
 assert(pre_reads==1&&counts[2]==1&&counts[1]==0&&releases==1&&writes==2&&!data_reads&&!irq);
 if(done&&done<=1024)assert(r==OMHBM_OK&&post_reads==done&&!omhbm_quarantined&&!omhbm_pending_buffer);
 else {assert(r==OMHBM_PUBLISHED_UNKNOWN&&post_reads==1024&&omhbm_quarantined&&omhbm_pending_buffer==buf&&omhbm_pending_pool==1);unsigned n=writes;assert(omhbm_final_ref_preserve_abi(buf)==OMHBM_REFUSED&&writes==n);}
 cases++;
}
clear();counts[2]=7;counts[1]=6;magic_bad=1;assert(omhbm_final_ref_preserve_abi(buf)==OMHBM_REFUSED&&!writes&&!omhbm_reserved&&counts[2]==7&&counts[1]==6&&!irq);cases++;
/* Actual original weak body bypasses the typed latch. Readiness may arrive
 * later; waiting never authorizes republishing the retained unknownrelease. */
clear();counts[2]=3;counts[1]=2;post_done_on=0;assert(omhbm_final_ref_preserve_abi(buf)==OMHBM_PUBLISHED_UNKNOWN);assert(writes==2&&releases==1&&omhbm_quarantined);
post_done_on=post_reads+1;topaz_hbm_put_buf(buf,1);assert(writes==4&&releases==2&&omhbm_quarantined&&omhbm_pending_buffer==buf&&!irq);cases++;
printf("{\"cases\":%u,\"original_void_bypass_reproduced\":true}\n",cases);return 0;}
