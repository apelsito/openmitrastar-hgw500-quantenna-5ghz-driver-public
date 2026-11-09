
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
typedef uint8_t u8;typedef uint16_t u16;typedef uint32_t u32;typedef uint32_t __le32;
#define __packed __attribute__((packed))
#define le32_to_cpu(x) (x)
#define U32_MAX UINT32_MAX
#define KERN_ERR ""
#define printk(...) ((void)0)
#define DBGPRINTF(...) ((void)0)
#define QDRV_BLD_VER 0x25040059U
#define QDRV_FWINFO_VADDR 0xffff0000U
#define QDRV_FWINFO_TYPE_RELEASE 1U
#define ELFMAG "\177ELF"
#define SELFMAG 4
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define ELFCLASS32 1
#define ELFDATA2LSB 1
#define EV_CURRENT 1
#define ET_EXEC 2
#define EM_ARCOMPACT 93
#define PT_LOAD 1
typedef struct{unsigned char e_ident[16];uint16_t e_type,e_machine;uint32_t e_version,e_entry,e_phoff,e_shoff,e_flags;uint16_t e_ehsize,e_phentsize,e_phnum,e_shentsize,e_shnum,e_shstrndx;}Elf32_Ehdr;
typedef struct{uint32_t p_type,p_offset,p_vaddr,p_paddr,p_filesz,p_memsz,p_flags,p_align;}Elf32_Phdr;
struct qdrv_fwinfo{char magic[8];__le32 type,version;}__packed;
enum qdrv_fw_cpu{qdrv_fw_muc,qdrv_fw_auc,qdrv_fw_dsp};
#define RUBY_DSP_XYMEM_BEGIN 0xd0000000U
#define RUBY_DSP_XYMEM_END 0xdfffffffU
#define RUBY_SRAM_BEGIN 0x88000000U
#define RUBY_SRAM_SIZE 0x80000U
#define RUBY_DRAM_BEGIN 0x80000000U
#define RUBY_MAX_DRAM_SIZE 0x8000000U
#define TOPAZ_AUC_IMEM_ADDR 0xe5000000U
#define TOPAZ_AUC_IMEM_SIZE 0x8000U
#define TOPAZ_AUC_DMEM_ADDR 0xe5101000U
#define TOPAZ_AUC_DMEM_SIZE 0x3000U
#define RUBY_BAD_VIRT_ADDR NULL
#define __in_mem_range(a,s,n) ((a)>=(s)&&(a)-(s)<(n))
static void*bus_to_virt(unsigned long a){if(a>=0x80000000U&&a-0x80000000U<0x80000U)return(void*)(a+0x8000000U);if(a<0x8000000U)return(void*)(a+0x80000000U);if(a<0xc0000000U)return NULL;return(void*)a;}
static unsigned long muc_to_lhost(unsigned long a){if(a>=0x60000000U&&a-0x60000000U<0x80000U)a-=0x60000000U-0x80000000U;return(unsigned long)bus_to_virt(a);}
static unsigned long virt_to_phys(void*p){return(unsigned long)p;}
static unsigned maps,unmaps,copies,zeros,fail_map_at;static size_t mapping_size;
static unsigned active;
static unsigned checks,first_map_checks;
static void*ioremap_nocache(unsigned long a,size_t n){assert(a&&n&&n<0x1000000U);assert(active);if(!maps)first_map_checks=checks;maps++;if(fail_map_at==maps)return NULL;mapping_size=n;void*p=malloc(n);assert(p);memset(p,0xa5,n);return p;}
static void iounmap(void*p){assert(p);unmaps++;free(p);}
static void qdrv_fw_auc_memcpy(void*d,const void*s,unsigned long n,unsigned long a){(void)a;assert(n<=mapping_size);copies++;memcpy(d,s,n);}
static void qdrv_fw_auc_memzero(void*d,unsigned long n,unsigned long a){(void)a;assert(n<=mapping_size);zeros++;memset(d,0,n);}

#include <openssl/sha.h>
#define SHA256_DIGEST_SIZE 32
typedef uint64_t u64;typedef unsigned long phys_addr_t;
struct device{unsigned unused;};struct firmware{size_t size;const u8*data;};
struct om73_sram_writer_token{u64 generation;u32 cpu;};
static const u8*requested_data;static size_t requested_size;static struct firmware request;
static unsigned requests,releases,begins,ends,hashes,fail_request,fail_begin,fail_end,fail_check_at;
static phys_addr_t pool_start,pool_end;static struct om73_sram_writer_token*token_identity;static u64 token_generation;
static int request_firmware(const struct firmware**fw,const char*name,struct device*d){(void)d;assert(name);requests++;if(fail_request)return-ENOENT;request=(struct firmware){requested_size,requested_data};*fw=&request;return 0;}
static void release_firmware(const struct firmware*fw){assert(fw==&request);releases++;}
static int om73_sram_writer_begin(unsigned cpu,struct om73_sram_writer_token*t){begins++;if(fail_begin)return-EBUSY;if(active)return-EBUSY;assert(t&&cpu<=2);active=1;token_identity=t;t->generation=++token_generation;t->cpu=cpu;return 0;}
static int om73_sram_writer_check(const struct om73_sram_writer_token*t,phys_addr_t start,size_t n){assert(active&&t==token_identity&&t->generation==token_generation&&n);checks++;if(fail_check_at==checks)return-EBUSY;if(pool_end&&start<pool_end&&start+n>pool_start)return-EBUSY;return 0;}
static int om73_sram_writer_end(struct om73_sram_writer_token*t){assert(active&&t==token_identity&&t->generation==token_generation);ends++;if(fail_end)return-EPERM;active=0;token_identity=NULL;memset(t,0,sizeof(*t));return 0;}
static void sha256(const u8*data,size_t n,u8 out[32]){assert(active);hashes++;assert(SHA256(data,n,out));}
static void reset_pipeline(void){maps=unmaps=copies=zeros=fail_map_at=0;requests=releases=begins=ends=hashes=checks=active=fail_request=fail_begin=fail_end=fail_check_at=0;pool_start=pool_end=0;token_identity=NULL;}

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
#define DBGPRINTF_E(...) ((void)0)
#define DBGPRINTF(...) ((void)0)
#define CONFIG_ARC_AUC_BASE 0x472000U
#define CONFIG_ARC_AUC_SIZE 0x1ca000U
#define CONFIG_ARC_AUC_SRAM_BASE 0x40000U
#define CONFIG_ARC_AUC_SRAM_SIZE 0x30000U
typedef int(*qdrv_fw_prepare_fn)(const struct om73_sram_writer_token*,void*);
static unsigned enabled,registered,prepare_entered;
static unsigned required_preflight_checks=8;
struct ieee80211com{unsigned unused;};struct qdrv_wlan{struct ieee80211com ic;};
struct qdrv_cb{struct device*dev;char*auc_firmware;struct{void*data;}macs[1];};
static struct{struct{unsigned auc_config;}auc;}shared;
static unsigned global_auc_config=7;
#define qtn_mproc_sync_shared_params_get() (&shared)
static void hal_enable_auc(void){enabled++;assert(!active);}
static void qdrv_auc_update_multicast_stats(void*c,uint8_t n){(void)c;(void)n;}
static void tqe_reg_multicast_tx_stats(void(*fn)(void*,uint8_t),void*c){assert(fn==qdrv_auc_update_multicast_stats&&c);registered++;assert(enabled);}
/* SPDX-License-Identifier: GPL-2.0-only */
/* Exact selected OEM carriers. Update through a new reviewed source
 * revision; these hashes prove identity, not authenticity or RF safety. */
#ifndef OM73_FW_PROFILES_H
#define OM73_FW_PROFILES_H
struct om73_fw_profile { size_t bytes; u8 sha256[32]; };
static const struct om73_fw_profile om73_fw_profiles[] = {
    { 1094927, { 0x71, 0xdc, 0xd3, 0x28, 0x49, 0xf1, 0x22, 0xc0, 0x93, 0x97, 0x49, 0x09, 0x14, 0x22, 0x7a, 0xf6, 0xf7, 0x7e, 0x51, 0xc3, 0x4d, 0x15, 0x61, 0x50, 0x28, 0xfa, 0xa3, 0xd0, 0xfa, 0xbb, 0x0f, 0xe4 } }, /* MuC */
    { 33342, { 0xc1, 0x00, 0x18, 0xf4, 0xfe, 0x38, 0x4d, 0x75, 0x3a, 0x80, 0x0b, 0xbb, 0xcd, 0x5e, 0xf3, 0xc8, 0x5c, 0x56, 0x8d, 0x08, 0x52, 0xc2, 0x21, 0x00, 0x90, 0x63, 0x28, 0x2d, 0x30, 0xfd, 0x85, 0xf6 } }, /* AuC_nomU */
    { 246244, { 0x87, 0xc8, 0x82, 0x91, 0x0d, 0x87, 0xcd, 0x29, 0xba, 0x20, 0x04, 0xc4, 0x15, 0x4e, 0x92, 0x40, 0x29, 0x1a, 0xaa, 0xd6, 0xae, 0xdb, 0x0f, 0xf4, 0xd9, 0x0c, 0x7a, 0x55, 0x70, 0xbe, 0x79, 0xfd } }, /* DSP */
};
#endif
static int qdrv_fw_check_info(const Elf32_Phdr *phdr, const char *data)
{
	const struct qdrv_fwinfo *info;
	u32 type;
	u32 version;

	if (phdr->p_vaddr != QDRV_FWINFO_VADDR)
		return 0;

	if (phdr->p_filesz < sizeof(*info)) {
		printk(KERN_ERR "qdrv: truncated FWINFO segment (%u bytes)\n",
			phdr->p_filesz);
		return -EINVAL;
	}

	info = (const struct qdrv_fwinfo *)data;
	if (memcmp(info->magic, "FWINFO", 6) != 0) {
		printk(KERN_ERR "qdrv: invalid FWINFO magic\n");
		return -EINVAL;
	}

	type = le32_to_cpu(info->type);
	version = le32_to_cpu(info->version);
	if (type != QDRV_FWINFO_TYPE_RELEASE) {
		printk(KERN_ERR "qdrv: unsupported FWINFO type 0x%x\n", type);
		return -EINVAL;
	}
	if (version != QDRV_BLD_VER) {
		printk(KERN_ERR
			"qdrv: firmware version 0x%x does not match host 0x%x\n",
			version, QDRV_BLD_VER);
		return -EPROTO;
	}

	return 1;
}
static int qdrv_fw_physical_range(unsigned long address, u32 bytes,
                                 enum qdrv_fw_cpu cpu)
{
    unsigned long end;
    if (!bytes || address > U32_MAX - (bytes - 1))
        return -ERANGE;
    end = address + bytes - 1;
    if (__in_mem_range(address, RUBY_SRAM_BEGIN, RUBY_SRAM_SIZE) &&
        __in_mem_range(end, RUBY_SRAM_BEGIN, RUBY_SRAM_SIZE))
        return 0;
    if (__in_mem_range(address, RUBY_DRAM_BEGIN, RUBY_MAX_DRAM_SIZE) &&
        __in_mem_range(end, RUBY_DRAM_BEGIN, RUBY_MAX_DRAM_SIZE))
        return 0;
    if (cpu == qdrv_fw_auc &&
        ((__in_mem_range(address, TOPAZ_AUC_IMEM_ADDR, TOPAZ_AUC_IMEM_SIZE) &&
          __in_mem_range(end, TOPAZ_AUC_IMEM_ADDR, TOPAZ_AUC_IMEM_SIZE)) ||
         (__in_mem_range(address, TOPAZ_AUC_DMEM_ADDR, TOPAZ_AUC_DMEM_SIZE) &&
          __in_mem_range(end, TOPAZ_AUC_DMEM_ADDR, TOPAZ_AUC_DMEM_SIZE))))
        return 0;
    return -ERANGE;
}
static int qdrv_fw_segment_address(const Elf32_Phdr *phdr,
                                   enum qdrv_fw_cpu cpu,
                                   unsigned long *address)
{
    unsigned long source;
    void *host;
    if (cpu == qdrv_fw_dsp && phdr->p_vaddr >= RUBY_DSP_XYMEM_BEGIN &&
        phdr->p_vaddr <= RUBY_DSP_XYMEM_END) {
        if (!phdr->p_memsz || phdr->p_memsz - 1 > RUBY_DSP_XYMEM_END - phdr->p_vaddr)
            return -ERANGE;
        return 1; /* Retain the DSP X/Y skip; this loader does not write it. */
    }
    if (cpu < qdrv_fw_muc || cpu > qdrv_fw_dsp)
        return -EINVAL;
    source = cpu == qdrv_fw_dsp ? phdr->p_vaddr :
             (phdr->p_paddr ? phdr->p_paddr : phdr->p_vaddr);
    host = cpu == qdrv_fw_muc ? (void *)muc_to_lhost(source) : bus_to_virt(source);
    if (host == RUBY_BAD_VIRT_ADDR)
        return -ERANGE;
    *address = virt_to_phys(host);
    return qdrv_fw_physical_range(*address, phdr->p_memsz, cpu);
}
static int qdrv_fw_preflight(const char *data, size_t size,
                             enum qdrv_fw_cpu cpu, Elf32_Ehdr *header)
{
    Elf32_Phdr phdr;
    size_t cursor, table_bytes;
    unsigned long address;
    u32 i;
    int ret;
    if (!data || !header || size < sizeof(*header) ||
        cpu < qdrv_fw_muc || cpu > qdrv_fw_dsp)
        return -EINVAL;
    memcpy(header, data, sizeof(*header));
    if (memcmp(header->e_ident, ELFMAG, SELFMAG) ||
        header->e_ident[EI_CLASS] != ELFCLASS32 ||
        header->e_ident[EI_DATA] != ELFDATA2LSB ||
        header->e_ident[EI_VERSION] != EV_CURRENT ||
        header->e_version != EV_CURRENT || header->e_type != ET_EXEC ||
        header->e_machine != EM_ARCOMPACT ||
        header->e_ehsize != sizeof(*header) ||
        header->e_phoff != sizeof(*header) ||
        header->e_phentsize != sizeof(phdr) || !header->e_phnum)
        return -ENOEXEC;
    table_bytes = (size_t)header->e_phnum * sizeof(phdr);
    if (table_bytes > size - sizeof(*header))
        return -EINVAL;
    cursor = sizeof(*header) + table_bytes;
    for (i = 0; i < header->e_phnum; i++) {
        memcpy(&phdr, data + sizeof(*header) + i * sizeof(phdr), sizeof(phdr));
        if (phdr.p_type != PT_LOAD || phdr.p_filesz > phdr.p_memsz ||
            phdr.p_filesz > size - cursor || phdr.p_offset > size ||
            (phdr.p_filesz && phdr.p_offset != cursor))
            return -EINVAL;
        ret = qdrv_fw_check_info(&phdr, data + cursor);
        if (ret < 0)
            return ret;
        if (!ret) {
            ret = qdrv_fw_segment_address(&phdr, cpu, &address);
            if (ret < 0)
                return ret;
        }
        cursor += phdr.p_filesz;
    }
    /* Entry is processor-relative, while p_paddr may hold a load copy address.
     * Validate it independently without demanding it match the load-copy VA. */
    memset(&phdr, 0, sizeof(phdr));
    phdr.p_vaddr = header->e_entry;
    phdr.p_memsz = 1;
    ret = qdrv_fw_segment_address(&phdr, cpu, &address);
    return ret ? (ret < 0 ? ret : -ERANGE) : 0;
}
static int qdrv_fw_writer_preflight(const char *data, size_t size,
                                   enum qdrv_fw_cpu cpu,
                                   const struct om73_sram_writer_token *writer)
{
    Elf32_Ehdr header;
    Elf32_Phdr phdr;
    size_t cursor;
    unsigned long address;
    u8 digest[SHA256_DIGEST_SIZE];
    u32 i;
    int ret;
    if (!data || cpu < qdrv_fw_muc || cpu > qdrv_fw_dsp ||
        size != om73_fw_profiles[cpu].bytes)
        return -EKEYREJECTED;
    sha256((const u8 *)data, size, digest);
    if (memcmp(digest, om73_fw_profiles[cpu].sha256, sizeof(digest)))
        return -EKEYREJECTED;
    ret = qdrv_fw_preflight(data, size, cpu, &header);
    if (ret)
        return ret;
    cursor = sizeof(header) + (size_t)header.e_phnum * sizeof(phdr);
    for (i = 0; i < header.e_phnum; i++) {
        memcpy(&phdr, data + sizeof(header) + i * sizeof(phdr), sizeof(phdr));
        ret = qdrv_fw_check_info(&phdr, data + cursor);
        if (!ret) {
            ret = qdrv_fw_segment_address(&phdr, cpu, &address);
            if (!ret) {
                ret = om73_sram_writer_check(writer, address, phdr.p_memsz);
                if (ret)
                    return ret;
            } else if (ret < 0) {
                return ret;
            }
        }
        cursor += phdr.p_filesz;
    }
    return 0;
}
static int qdrv_fw_install_segment(const Elf32_Phdr *phdr,
		const char *data,
		enum qdrv_fw_cpu cpu)
{
	uint8_t *vaddr;
	unsigned long paddr;

	int ret = qdrv_fw_segment_address(phdr, cpu, &paddr);
	if (ret)
		return ret > 0 ? 0 : ret;

	DBGPRINTF(DBG_LL_INFO, QDRV_LF_TRACE, "ELF header p_vaddr=%p p_paddr=%p, "
				 "remapping to 0x%lx filesz %d memsz %d\n",
			(void*)phdr->p_vaddr, (void*)phdr->p_paddr,
			paddr, phdr->p_filesz, phdr->p_memsz);

	vaddr = ioremap_nocache(paddr, phdr->p_memsz);
	if (!vaddr)
		return -ENOMEM;

	/* Copy data and clear BSS */
	if (cpu == qdrv_fw_auc) {
		qdrv_fw_auc_memcpy(vaddr, data, phdr->p_filesz, paddr);
		qdrv_fw_auc_memzero(vaddr + phdr->p_filesz, phdr->p_memsz - phdr->p_filesz, paddr);
	} else {
		memcpy(vaddr, data, phdr->p_filesz);
		memset(vaddr + phdr->p_filesz, 0, phdr->p_memsz - phdr->p_filesz);
	}

	iounmap(vaddr);
	return 0;
}
static int qdrv_fw_install(const char *data, size_t size,
                           uint32_t *start_addr, enum qdrv_fw_cpu cpu)
{
	Elf32_Ehdr header;
	Elf32_Phdr phdr;
	size_t cursor;
	u32 i;
	int ret;

	if (!start_addr)
		return -EINVAL;
	ret = qdrv_fw_preflight(data, size, cpu, &header);
	if (ret)
		return ret;
	cursor = sizeof(header) + (size_t)header.e_phnum * sizeof(phdr);
	for (i = 0; i < header.e_phnum; i++) {
		memcpy(&phdr, data + sizeof(header) + i * sizeof(phdr), sizeof(phdr));
		ret = qdrv_fw_check_info(&phdr, data + cursor);
		if (!ret) {
			ret = qdrv_fw_install_segment(&phdr, data + cursor, cpu);
			if (ret)
				return ret;
		}
		cursor += phdr.p_filesz;
	}
	/* Publish entry only after every segment was successfully installed. */
	*start_addr = header.e_entry;
	return header.e_phnum;
}
int qdrv_fw_load_prepared(struct device *dev,
		char *firmware,
		uint32_t *start_addr,
		enum qdrv_fw_cpu cpu, qdrv_fw_prepare_fn prepare, void *context)
{
	const struct firmware *fw;
	struct om73_sram_writer_token writer;
	u32 loaded_entry;
	int ret, end_ret;

	if (!start_addr || !firmware || cpu < qdrv_fw_muc || cpu > qdrv_fw_dsp)
		return -EINVAL;
	ret = request_firmware(&fw, firmware, dev);
	if (ret)
		return ret;
	ret = om73_sram_writer_begin(cpu, &writer);
	if (ret)
		goto release;
	ret = qdrv_fw_writer_preflight((const char *)fw->data, fw->size, cpu, &writer);
	if (!ret && prepare) {
		ret = prepare(&writer, context);
		if (ret > 0)
			ret = -EPROTO;
	}
	if (!ret)
		ret = qdrv_fw_install((const char *)fw->data, fw->size, &loaded_entry, cpu);
	end_ret = om73_sram_writer_end(&writer);
	if (ret > 0 && end_ret)
		ret = end_ret;
	if (ret > 0)
		*start_addr = loaded_entry;
release:
	release_firmware(fw);
	return ret;
}
int qdrv_fw_load_auc_prepared(struct device *dev, char *firmware, uint32_t *start,
                             qdrv_fw_prepare_fn prepare, void *context)
{
    if (!prepare)
        return -EINVAL;
    return qdrv_fw_load_prepared(dev, firmware, start, qdrv_fw_auc, prepare, context);
}
static int auc_clear_addr_range(unsigned long physaddr, unsigned long size)
{
	void *vaddr = ioremap_nocache(physaddr, size);
	if (!vaddr) {
		DBGPRINTF_E("0x%lx, 0x%lx cannot be mapped\n", physaddr, size);
		return -ENOMEM;
	}
	qdrv_fw_auc_memzero(vaddr, size, physaddr);
	iounmap(vaddr);
	return 0;
}
static int auc_clear_mem(const struct om73_sram_writer_token *writer, void *context)
{
	static const struct { unsigned long address, size; } regions[] = {
		{ TOPAZ_AUC_IMEM_ADDR, TOPAZ_AUC_IMEM_SIZE },
		{ TOPAZ_AUC_DMEM_ADDR, TOPAZ_AUC_DMEM_SIZE },
		{ RUBY_DRAM_BEGIN + CONFIG_ARC_AUC_BASE, CONFIG_ARC_AUC_SIZE },
		{ RUBY_SRAM_BEGIN + CONFIG_ARC_AUC_SRAM_BASE, CONFIG_ARC_AUC_SRAM_SIZE },
	};
	unsigned int i;
	int ret;
	(void)context;
	/* Admission of every clear span precedes the first clear mapping. */
	for (i = 0; i < ARRAY_SIZE(regions); i++) {
		ret = om73_sram_writer_check(writer, regions[i].address, regions[i].size);
		if (ret)
			return ret;
	}
	for (i = 0; i < ARRAY_SIZE(regions); i++) {
		ret = auc_clear_addr_range(regions[i].address, regions[i].size);
		if (ret)
			return ret;
	}
	return 0;
}
int qdrv_auc_init(struct qdrv_cb *qcb)
{
	u32 auc_start_addr = 0;
	struct qdrv_wlan *qw = qcb->macs[0].data;

	DBGPRINTF(DBG_LL_ALL, QDRV_LF_TRACE, "-->Enter");

	qtn_mproc_sync_shared_params_get()->auc.auc_config = global_auc_config;

	/* Hashes/load destinations and all clear spans share one writer lease. */
	if (qdrv_fw_load_auc_prepared(qcb->dev, qcb->auc_firmware, &auc_start_addr,
			auc_clear_mem, NULL) < 0) {
		DBGPRINTF_E("AuC load firmware failed\n");
		DBGPRINTF(DBG_LL_ALL, QDRV_LF_TRACE, "<--Exit\n");
		return -1;
	}

	DBGPRINTF(DBG_LL_INFO, QDRV_LF_DSP, "Firmware start address is %x\n", auc_start_addr);

	hal_enable_auc();

	tqe_reg_multicast_tx_stats(qdrv_auc_update_multicast_stats, &qw->ic);

	DBGPRINTF(DBG_LL_ALL, QDRV_LF_TRACE, "<--Exit\n");

	return 0;
}
static int positive_prepare(const struct om73_sram_writer_token*w,void*c){(void)c;assert(active&&w==token_identity);prepare_entered++;return 1;}
static int negative_prepare(const struct om73_sram_writer_token*w,void*c){(void)c;assert(active&&w==token_identity);prepare_entered++;return-EIO;}
static char*read_carrier(const char*name,size_t*n){FILE*f=fopen(name,"rb");assert(f&&!fseek(f,0,SEEK_END));long size=ftell(f);assert(size>0);rewind(f);char*b=malloc(size);assert(b&&fread(b,1,size,f)==(size_t)size);fclose(f);*n=size;return b;}
static void reset_auc(void){reset_pipeline();enabled=registered=prepare_entered=first_map_checks=0;}
static unsigned cases;
int main(int argc,char**argv){assert(argc==2);size_t n;char*b=read_carrier(argv[1],&n);struct qdrv_wlan qw={0};struct qdrv_cb qcb={NULL,"selected",{{&qw}}};u32 entry;char*m=malloc(n);assert(m);
/* Normal init proves the prepare callback and install share onelease. */
reset_auc();requested_data=(void*)b;requested_size=n;assert(!qdrv_auc_init(&qcb)&&enabled==1&&registered==1&&requests==1&&releases==1&&begins==1&&ends==1&&hashes==1&&!active);assert(first_map_checks==required_preflight_checks+4&&maps==12&&unmaps==12);cases++;
/* Exact profile rejection occurs before clearcallback/maps/core-enable. */
unsigned mutate[]={0,4,5,6,16,18,20,24,28,40,42,44,52,340,355,356,3118,3822,30474};
for(unsigned i=0;i<ARRAY_SIZE(mutate);i++){memcpy(m,b,n);m[mutate[i]]^=1;reset_auc();requested_data=(void*)m;requested_size=n;assert(qdrv_auc_init(&qcb)==-1&&!enabled&&!registered&&!maps&&!zeros&&hashes==1&&ends==1&&releases==1);cases++;}
reset_auc();requested_data=(void*)b;requested_size=n-1;entry=0xdeadbeef;assert(qdrv_fw_load_auc_prepared(NULL,"selected",&entry,auc_clear_mem,NULL)==-EKEYREJECTED&&entry==0xdeadbeef&&!hashes&&!maps&&ends==1);cases++;
/* Last clearspan overlapsactiveheap: all4spanschecked BEFOREfirstclear map. */
reset_auc();requested_data=(void*)b;requested_size=n;pool_start=0x88040000;pool_end=0x88040008;entry=0xdeadbeef;int rc=qdrv_fw_load_auc_prepared(NULL,"selected",&entry,auc_clear_mem,NULL);assert(rc==-EBUSY&&entry==0xdeadbeef&&!maps&&!enabled);cases++;
/* Fail specificallyonfourthpreparecheckafterallloadsegments checked. */
reset_auc();requested_data=(void*)b;requested_size=n;fail_check_at=required_preflight_checks+4;entry=0xdeadbeef;assert(qdrv_fw_load_auc_prepared(NULL,"selected",&entry,auc_clear_mem,NULL)==-EBUSY&&entry==0xdeadbeef&&!maps&&checks==required_preflight_checks+4&&!enabled);cases++;
/* Maprefusal for eachclear: noinstallmaps/enable; entry remainsprivate. */
for(unsigned fail=1;fail<=4;fail++){reset_auc();requested_data=(void*)b;requested_size=n;fail_map_at=fail;entry=0xdeadbeef;assert(qdrv_fw_load_auc_prepared(NULL,"selected",&entry,auc_clear_mem,NULL)==-ENOMEM&&entry==0xdeadbeef&&maps==fail&&unmaps==fail-1&&ends==1&&releases==1&&!active&&!enabled);cases++;}
/* Actualinit propagatespreparemappingfail andmustneverenableAuC. */
for(unsigned fail=1;fail<=12;fail++){reset_auc();requested_data=(void*)b;requested_size=n;fail_map_at=fail;assert(qdrv_auc_init(&qcb)==-1&&!enabled&&!registered&&maps==fail&&ends==1&&releases==1&&!active);cases++;}
/* Callbackreturncontract: positiveunexpected cannotpublishuninitializedentry. */
reset_auc();requested_data=(void*)b;requested_size=n;entry=0xdeadbeef;assert(qdrv_fw_load_auc_prepared(NULL,"selected",&entry,positive_prepare,NULL)==-EPROTO&&entry==0xdeadbeef&&prepare_entered==1&&!maps&&ends==1);cases++;
reset_auc();requested_data=(void*)b;requested_size=n;entry=0xdeadbeef;assert(qdrv_fw_load_auc_prepared(NULL,"selected",&entry,negative_prepare,NULL)==-EIO&&entry==0xdeadbeef&&prepare_entered==1&&!maps&&ends==1);cases++;
reset_auc();requested_data=(void*)b;requested_size=n;entry=0xdeadbeef;assert(qdrv_fw_load_auc_prepared(NULL,"selected",&entry,NULL,NULL)==-EINVAL&&entry==0xdeadbeef&&!requests&&!maps);cases++;
for(unsigned failure=0;failure<3;failure++){reset_auc();requested_data=(void*)b;requested_size=n;fail_request=failure==0;fail_begin=failure==1;fail_end=failure==2;entry=0xdeadbeef;assert(qdrv_fw_load_auc_prepared(NULL,"selected",&entry,auc_clear_mem,NULL)<0&&entry==0xdeadbeef&&!enabled);if(failure<2)assert(!maps);else assert(maps==12&&active&&ends==1);cases++;}
reset_auc();requested_data=(void*)b;requested_size=n;pool_start=0x88012000;pool_end=0x88020000;entry=0xdeadbeef;assert(qdrv_fw_load_auc_prepared(NULL,"selected",&entry,auc_clear_mem,NULL)>0&&entry==0xe5000000&&maps==12&&first_map_checks==12&&!active);cases++;
free(b);free(m);printf("{\"actual_AuC_prepared_pipeline_cases\":%u,\"clear_span_checks_before_first_map\":4,\"loader_destinations_checked_first\":8,\"positive_callback_rejected\":true,\"hash_backend\":\"OpenSSL SHA256 host\"}\n",cases);return 0;}
