
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
typedef uint8_t u8;typedef uint16_t u16;typedef uint32_t u32;
#define __iomem
#define GFP_KERNEL 0
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*(x)))
#define pr_info(...) ((void)0)
#define pr_err(...) ((void)0)
#define DBGPRINTF_E(...) ((void)0)
#define CONFIG_ARC_MUC_STACK_INIT 0x8803ffc0U
#define CONFIG_ARC_MUC_STACK_SIZE 2048U
#define RUBY_SRAM_BEGIN 0x88000000U
#define RUBY_SRAM_SIZE 0x80000U
#define CONFIG_ARC_MUC_SRAM_B1_BASE 0x20000U
#define CONFIG_ARC_MUC_SRAM_B1_SIZE 0x10000U
#define CONFIG_ARC_MUC_SRAM_B2_BASE 0x30000U
#define CONFIG_ARC_MUC_SRAM_B2_SIZE 0xffc0U
#define RUBY_CRUMBS_ADDR 0x8803ffc0U
#define RUBY_CRUMBS_SIZE 0x40U
#define RUBY_DRAM_BEGIN 0x80000000U
#define RUBY_MAX_DRAM_SIZE 0x8000000U
#define CONFIG_ARC_MUC_BASE 0xe0000U
#define CONFIG_ARC_MUC_SIZE 0x2c1000U
#define CONFIG_ARC_DSP_BASE 0x20000U
#define CONFIG_ARC_DSP_SIZE 0xc0000U
#define HEADER_CORE_DUMP 0xa5c3U
struct qdrv_cb{u32 resources;};struct device{struct qdrv_cb*cb;};
static struct qdrv_cb*dev_get_drvdata(struct device*d){return d?d->cb:NULL;}
static char*qdrv_crash_log;static uint32_t qdrv_crash_log_len;
struct om73_sram_writer_token{uint64_t generation;u32 cpu;};
struct om73_sram_reset_token{uint64_t generation;u32 mask;};
static unsigned held,begins,ends,checks,maps,unmaps,clears,reads,copyreads;
static unsigned deny_check,fail_map,fail_alloc;static int begin_error,end_error;
static u16 log_magic,log_size;static unsigned core_begins,hw_resets,boot_failures;
static int reset_begin_error,reset_end_error,core_begin_error;static unsigned reset_held;
static int om73_sram_writer_begin(unsigned cpu,struct om73_sram_writer_token*t){assert(!held&&!reset_held);begins++;if(begin_error)return begin_error;held=1;t->cpu=cpu;t->generation=1;return 0;}
static int om73_sram_writer_end(struct om73_sram_writer_token*t){assert(held&&t->generation==1);ends++;held=0;return end_error;}
static int om73_sram_writer_check(const struct om73_sram_writer_token*t,unsigned long a,size_t n){assert(held&&t->generation==1&&a&&n);checks++;return checks==deny_check?-EBUSY:0;}
static void*ioremap_nocache(unsigned long p,size_t n){assert(held&&p&&n&&checks==6);maps++;if(maps==fail_map)return NULL;void*b=calloc(1,n);assert(b);if(p==CONFIG_ARC_MUC_STACK_INIT-CONFIG_ARC_MUC_STACK_SIZE){memcpy(b,&log_magic,2);memcpy((char*)b+2,&log_size,2);if(log_size<=n-4)for(unsigned i=0;i<log_size;i++)((char*)b)[4+i]=(char)i;}return b;}
static void iounmap(void*p){assert(held&&p);unmaps++;free(p);}
static u16 readw(void*p){assert(held&&p);u16 v;memcpy(&v,p,2);reads++;return v;}
static void memset_io(void*p,int v,size_t n){assert(held&&p&&checks==6);clears++;memset(p,v,n);}
static void memcpy_fromio(void*d,const void*s,size_t n){assert(held);copyreads++;memcpy(d,s,n);}
static void*kmalloc(size_t n,int f){(void)f;return fail_alloc?NULL:malloc(n?n:1);}
static int om73_sram_reset_begin(u32 m,struct om73_sram_reset_token*t){assert(m==0x4180&&!held&&!reset_held);if(reset_begin_error)return reset_begin_error;reset_held=1;t->mask=m;t->generation=1;return 0;}
static int om73_sram_reset_end(struct om73_sram_reset_token*t){assert(reset_held&&t->mask==0x4180&&t->generation==1);reset_held=0;return reset_end_error;}
static int qdrv_f355_core_begin(void){assert(reset_held);core_begins++;return core_begin_error;}
static void hal_reset(void){assert(reset_held);hw_resets++;}
static void qdrv_f355_boot_failure(void){assert(!reset_held);boot_failures++;}
struct qdrv_clearsram_region{const char*name;u32 start,size,allowed_start,allowed_size;};
static int qdrv_clearsram_range_valid(const struct qdrv_clearsram_region *region)
{
	u32 end;
	u32 allowed_end;

	if (!region || !region->size || !region->allowed_size)
		return 0;
	if (region->start < region->allowed_start)
		return 0;
	if (region->start > (~0U - (region->size - 1)))
		return 0;
	if (region->allowed_start > (~0U - (region->allowed_size - 1)))
		return 0;
	end = region->start + region->size - 1;
	allowed_end = region->allowed_start + region->allowed_size - 1;

	return end <= allowed_end;
}
static int qdrv_clearsram_zero_region(const struct qdrv_clearsram_region *region)
{
	void __iomem *mapped;

	if (!qdrv_clearsram_range_valid(region)) {
		pr_err("OMQDRVCLR stage=range-fail name=%s start=%#x size=%#x allowed_start=%#x allowed_size=%#x\n",
			region ? region->name : "null", region ? region->start : 0,
			region ? region->size : 0, region ? region->allowed_start : 0,
			region ? region->allowed_size : 0);
		return -ERANGE;
	}

	pr_info("OMQDRVCLR stage=range-pass name=%s start=%#x size=%#x\n",
		region->name, region->start, region->size);
	mapped = ioremap_nocache(region->start, region->size);
	if (!mapped) {
		pr_err("OMQDRVCLR stage=map-fail name=%s start=%#x size=%#x\n",
			region->name, region->start, region->size);
		return -ENOMEM;
	}
	pr_info("OMQDRVCLR stage=map-pass name=%s mapped=%px\n",
		region->name, mapped);
	memset_io(mapped, 0, region->size);
	pr_info("OMQDRVCLR stage=clear-pass name=%s bytes=%u\n",
		region->name, region->size);
	iounmap(mapped);

	return 0;
}
static int qdrv_clearsram_capture_log(const struct qdrv_clearsram_region *stack)
{
	void __iomem *mapped;
	char *copy;
	u16 bytes;
	int ret = 0;
	if (qdrv_crash_log)
		return 0;
	mapped = ioremap_nocache(stack->start, stack->size);
	if (!mapped)
		return -ENOMEM;
	if (readw(mapped) != HEADER_CORE_DUMP)
		goto out;
	bytes = readw((u8 __iomem *)mapped + sizeof(u16));
	if (bytes > stack->size - 2 * sizeof(u16)) {
		ret = -ERANGE;
		goto out;
	}
	copy = kmalloc(bytes, GFP_KERNEL);
	if (!copy) {
		ret = -ENOMEM;
		goto out;
	}
	memcpy_fromio(copy, (u8 __iomem *)mapped + 2 * sizeof(u16), bytes);
	qdrv_crash_log_len = bytes;
	qdrv_crash_log = copy;
out:
	iounmap(mapped);
	return ret;
}
static int qdrv_command_clearsram_owned(struct device *dev, int argc, char *argv[],
                                        const struct om73_sram_writer_token *writer)
{
	struct qdrv_cb *qcb;
	u32 stack_start = CONFIG_ARC_MUC_STACK_INIT - CONFIG_ARC_MUC_STACK_SIZE;
	int i;
	int ret;
	const struct qdrv_clearsram_region stack_region = {
		.name = "muc-stack",
		.start = CONFIG_ARC_MUC_STACK_INIT - CONFIG_ARC_MUC_STACK_SIZE,
		.size = CONFIG_ARC_MUC_STACK_SIZE,
		.allowed_start = RUBY_SRAM_BEGIN,
		.allowed_size = RUBY_SRAM_SIZE,
	};
	const struct qdrv_clearsram_region regions[] = {
		{ "muc-sram-b1", RUBY_SRAM_BEGIN + CONFIG_ARC_MUC_SRAM_B1_BASE,
			CONFIG_ARC_MUC_SRAM_B1_SIZE, RUBY_SRAM_BEGIN, RUBY_SRAM_SIZE },
		{ "muc-sram-b2", RUBY_SRAM_BEGIN + CONFIG_ARC_MUC_SRAM_B2_BASE,
			CONFIG_ARC_MUC_SRAM_B2_SIZE, RUBY_SRAM_BEGIN, RUBY_SRAM_SIZE },
		{ "crumbs", RUBY_CRUMBS_ADDR, RUBY_CRUMBS_SIZE,
			RUBY_SRAM_BEGIN, RUBY_SRAM_SIZE },
		{ "muc-dram", RUBY_DRAM_BEGIN + CONFIG_ARC_MUC_BASE,
			CONFIG_ARC_MUC_SIZE, RUBY_DRAM_BEGIN, RUBY_MAX_DRAM_SIZE },
		{ "dsp-dram", RUBY_DRAM_BEGIN + CONFIG_ARC_DSP_BASE,
			CONFIG_ARC_DSP_SIZE, RUBY_DRAM_BEGIN, RUBY_MAX_DRAM_SIZE },
	};

	qcb = dev_get_drvdata(dev);
	pr_info("OMQDRVCLR stage=enter argc=%d qcb=%px resources=%#x rf=blocked vap=blocked\n",
		argc, qcb, qcb ? qcb->resources : 0);
	if (!qcb) {
		pr_err("OMQDRVCLR stage=reject reason=qcb-null\n");
		return -ENODEV;
	}
	if (argc != 1) {
		pr_err("OMQDRVCLR stage=reject reason=arguments argc=%d\n", argc);
		return -EINVAL;
	}
	if (qcb->resources != 0) {
		pr_err("OMQDRVCLR stage=reject reason=resources-held resources=%#x\n",
			qcb->resources);
		return -EBUSY;
	}
	if (!qdrv_clearsram_range_valid(&stack_region)) {
		pr_err("OMQDRVCLR stage=reject reason=stack-range start=%#x size=%#x\n",
			stack_region.start, stack_region.size);
		return -ERANGE;
	}
	for (i = 0; i < ARRAY_SIZE(regions); i++) {
		if (!qdrv_clearsram_range_valid(&regions[i])) {
			pr_err("OMQDRVCLR stage=reject reason=region-range name=%s start=%#x size=%#x\n",
				regions[i].name, regions[i].start, regions[i].size);
			return -ERANGE;
		}
	}
	pr_info("OMQDRVCLR stage=preflight-pass regions=%u stack_start=%#x stack_size=%#x\n",
		(unsigned int)ARRAY_SIZE(regions), stack_start,
		CONFIG_ARC_MUC_STACK_SIZE);
	ret = om73_sram_writer_check(writer, stack_region.start, stack_region.size);
	if (ret)
		return ret;
	for (i = 0; i < ARRAY_SIZE(regions); i++) {
		ret = om73_sram_writer_check(writer, regions[i].start, regions[i].size);
		if (ret)
			return ret;
	}
	/* Copy out any crash log, let the parse function deal with validity of the buffer */
	ret = qdrv_clearsram_capture_log(&stack_region);
	if (ret)
		return ret;

	/* Fill only the five OEM firmware regions after every range has passed. */
	for (i = 0; i < ARRAY_SIZE(regions); i++) {
		ret = qdrv_clearsram_zero_region(&regions[i]);
		if (ret)
			return ret;
	}
	pr_info("OMQDRVCLR stage=complete regions=%u rf=blocked vap=blocked\n",
		(unsigned int)ARRAY_SIZE(regions));

	return 0;
}
static int qdrv_command_clearsram(struct device *dev, int argc, char *argv[])
{
	struct om73_sram_writer_token writer;
	int ret, end_ret;
	ret = om73_sram_writer_begin(0, &writer);
	if (ret)
		return ret;
	ret = qdrv_command_clearsram_owned(dev, argc, argv, &writer);
	end_ret = om73_sram_writer_end(&writer);
	return ret ? ret : end_ret;
}static int qdrv_soc_admitted_core_reset(void)
{
	struct om73_sram_reset_token reset;
	int ret, end_ret;
	/* This software lease does not prove NETSS/TQE/firmware quiescence.
	 * It must reject live pool owners before lifetime promotion or MMIO. */
	ret = om73_sram_reset_begin(0x4180U, &reset);
	if (ret)
		return ret;
	ret = qdrv_f355_core_begin();
	if (!ret)
		hal_reset();
	end_ret = om73_sram_reset_end(&reset);
	if (!ret && end_ret) {
		qdrv_f355_boot_failure();
		ret = end_ret;
	}
	return ret;
}
static void reset(void){assert(!held&&!reset_held);free(qdrv_crash_log);qdrv_crash_log=NULL;qdrv_crash_log_len=0;begins=ends=checks=maps=unmaps=clears=reads=copyreads=0;deny_check=fail_map=fail_alloc=0;begin_error=end_error=0;log_magic=log_size=0;reset_begin_error=reset_end_error=core_begin_error=0;core_begins=hw_resets=boot_failures=0;}
int main(void){unsigned cases=0;struct qdrv_cb cb={0};struct device dev={&cb};
 for(unsigned bad=0;bad<3;bad++){reset();cb.resources=bad==2?1:0;int rc=qdrv_command_clearsram(bad==0?NULL:&dev,bad==1?2:1,NULL);assert(rc==(bad==0?-ENODEV:bad==1?-EINVAL:-EBUSY)&&begins==1&&ends==1&&!checks&&!maps);cases++;}cb.resources=0;
 for(unsigned denied=1;denied<=6;denied++){reset();deny_check=denied;assert(qdrv_command_clearsram(&dev,1,NULL)==-EBUSY&&checks==denied&&!maps&&ends==1);cases++;}
 reset();begin_error=-EBUSY;assert(qdrv_command_clearsram(&dev,1,NULL)==-EBUSY&&!maps&&!checks&&!ends);cases++;
 reset();assert(!qdrv_command_clearsram(&dev,1,NULL)&&begins==1&&ends==1&&checks==6&&maps==6&&unmaps==6&&clears==5&&!qdrv_crash_log&&reads==1);cases++;
 for(unsigned m=1;m<=6;m++){reset();fail_map=m;assert(qdrv_command_clearsram(&dev,1,NULL)==-ENOMEM&&maps==m&&unmaps==m-1&&ends==1&&clears==(m>1?m-2:0));cases++;}
 for(unsigned length=0;length<=2044;length+=511){reset();log_magic=HEADER_CORE_DUMP;log_size=length;assert(!qdrv_command_clearsram(&dev,1,NULL)&&qdrv_crash_log&&qdrv_crash_log_len==length&&copyreads==1&&reads==2&&maps==6&&unmaps==6);for(unsigned i=0;i<length;i++)assert(qdrv_crash_log[i]==(char)i);cases++;}
 reset();log_magic=HEADER_CORE_DUMP;log_size=2045;assert(qdrv_command_clearsram(&dev,1,NULL)==-ERANGE&&maps==1&&unmaps==1&&!clears&&!qdrv_crash_log&&!qdrv_crash_log_len);cases++;
 reset();log_magic=HEADER_CORE_DUMP;log_size=32;fail_alloc=1;assert(qdrv_command_clearsram(&dev,1,NULL)==-ENOMEM&&maps==1&&unmaps==1&&!clears&&!qdrv_crash_log&&!qdrv_crash_log_len);cases++;
 reset();qdrv_crash_log=malloc(8);qdrv_crash_log_len=8;assert(!qdrv_command_clearsram(&dev,1,NULL)&&maps==5&&unmaps==5&&!reads&&!copyreads&&clears==5&&qdrv_crash_log_len==8);cases++;
 reset();end_error=-EPERM;assert(qdrv_command_clearsram(&dev,1,NULL)==-EPERM&&clears==5&&ends==1);cases++;
 reset();fail_map=2;end_error=-EPERM;assert(qdrv_command_clearsram(&dev,1,NULL)==-ENOMEM&&ends==1);cases++;
 struct qdrv_clearsram_region ranges[]={{"zero",1,0,1,16},{"under",0,1,1,16},{"overflow",UINT32_MAX,2,1,UINT32_MAX},{"over-allow",17,1,1,16},{"allowed-overflow",1,1,UINT32_MAX,2}};
 for(unsigned i=0;i<5;i++){assert(!qdrv_clearsram_range_valid(&ranges[i]));cases++;}
 reset();reset_begin_error=-EBUSY;assert(qdrv_soc_admitted_core_reset()==-EBUSY&&!core_begins&&!hw_resets&&!boot_failures);cases++;
 reset();core_begin_error=-EIO;assert(qdrv_soc_admitted_core_reset()==-EIO&&core_begins==1&&!hw_resets&&!boot_failures&&!reset_held);cases++;
 reset();assert(!qdrv_soc_admitted_core_reset()&&core_begins==1&&hw_resets==1&&!boot_failures&&!reset_held);cases++;
 reset();reset_end_error=-EPERM;assert(qdrv_soc_admitted_core_reset()==-EPERM&&core_begins==1&&hw_resets==1&&boot_failures==1&&!reset_held);cases++;
 reset();printf("{\"cases\":%u,\"clear_regions\":5,\"preflight_spans\":6,\"reset_software_only\":true}\n",cases);return 0;}
