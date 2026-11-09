
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
static void*ioremap_nocache(unsigned long a,size_t n){assert(a&&n&&n<0x1000000U);assert(active);maps++;if(fail_map_at==maps)return NULL;mapping_size=n;void*p=malloc(n);assert(p);memset(p,0xa5,n);return p;}
static void iounmap(void*p){assert(p);unmaps++;free(p);}
static void qdrv_fw_auc_memcpy(void*d,const void*s,unsigned long n,unsigned long a){(void)a;assert(n<=mapping_size);copies++;memcpy(d,s,n);}
static void qdrv_fw_auc_memzero(void*d,unsigned long n,unsigned long a){(void)a;assert(n<=mapping_size);zeros++;memset(d,0,n);}

#include <openssl/sha.h>
#define SHA256_DIGEST_SIZE 32
typedef uint64_t u64;typedef unsigned long phys_addr_t;
struct device{unsigned unused;};struct firmware{size_t size;const u8*data;};
struct om73_sram_writer_token{u64 generation;u32 cpu;};
static const u8*requested_data;static size_t requested_size;static struct firmware request;
static unsigned requests,releases,begins,ends,hashes,checks,fail_request,fail_begin,fail_end,fail_check_at;
static phys_addr_t pool_start,pool_end;static struct om73_sram_writer_token*token_identity;static u64 token_generation;
static int request_firmware(const struct firmware**fw,const char*name,struct device*d){(void)d;assert(name);requests++;if(fail_request)return-ENOENT;request=(struct firmware){requested_size,requested_data};*fw=&request;return 0;}
static void release_firmware(const struct firmware*fw){assert(fw==&request);releases++;}
static int om73_sram_writer_begin(unsigned cpu,struct om73_sram_writer_token*t){begins++;if(fail_begin)return-EBUSY;if(active)return-EBUSY;assert(t&&cpu<=2);active=1;token_identity=t;t->generation=++token_generation;t->cpu=cpu;return 0;}
static int om73_sram_writer_check(const struct om73_sram_writer_token*t,phys_addr_t start,size_t n){assert(active&&t==token_identity&&t->generation==token_generation&&n);checks++;if(fail_check_at==checks)return-EBUSY;if(pool_end&&start<pool_end&&start+n>pool_start)return-EBUSY;return 0;}
static int om73_sram_writer_end(struct om73_sram_writer_token*t){assert(active&&t==token_identity&&t->generation==token_generation);ends++;if(fail_end)return-EPERM;active=0;token_identity=NULL;memset(t,0,sizeof(*t));return 0;}
static void sha256(const u8*data,size_t n,u8 out[32]){assert(active);hashes++;assert(SHA256(data,n,out));}
static void reset_pipeline(void){maps=unmaps=copies=zeros=fail_map_at=0;requests=releases=begins=ends=hashes=checks=active=fail_request=fail_begin=fail_end=fail_check_at=0;pool_start=pool_end=0;token_identity=NULL;}
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
int qdrv_fw_load(struct device *dev,
		char *firmware,
		uint32_t *start_addr,
		enum qdrv_fw_cpu cpu)
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
static char*read_input(const char*name,size_t*size){FILE*f=fopen(name,"rb");assert(f);assert(!fseek(f,0,SEEK_END));long n=ftell(f);assert(n>0);rewind(f);char*b=malloc(n);assert(b&&fread(b,1,n,f)==(size_t)n);fclose(f);*size=n;return b;}
static unsigned cases;
static void reject_load(char*b,size_t n,int cpu){reset_pipeline();requested_data=(void*)b;requested_size=n;u32 entry=0xdeadbeef;int rc=qdrv_fw_load(NULL,"selected",&entry,cpu);assert(rc<0&&entry==0xdeadbeef&&!maps&&!unmaps&&!copies&&!zeros);assert(requests==1&&releases==1&&begins==1&&ends==1&&!active);cases++;}
int main(int argc,char**argv){assert(argc==4);for(int cpu=0;cpu<3;cpu++){size_t n;char*b=read_input(argv[cpu+1],&n);Elf32_Ehdr h;assert(!qdrv_fw_preflight(b,n,cpu,&h));Elf32_Phdr*ph=(void*)(b+52);unsigned expected_checks=0;size_t cursor=52+(size_t)h.e_phnum*32;for(unsigned i=0;i<h.e_phnum;i++){unsigned long a;int x=qdrv_fw_check_info(&ph[i],b+cursor);if(x==0&&!qdrv_fw_segment_address(&ph[i],cpu,&a))expected_checks++;cursor+=ph[i].p_filesz;}
reset_pipeline();requested_data=(void*)b;requested_size=n;u32 entry=0xdeadbeef;assert(qdrv_fw_load(NULL,"selected",&entry,cpu)==h.e_phnum&&entry==h.e_entry&&requests==1&&releases==1&&begins==1&&ends==1&&hashes==1&&checks==expected_checks&&maps==unmaps&&!active);unsigned expected_maps=maps;cases++;
reset_pipeline();requested_data=(void*)b;requested_size=n;pool_start=0x88012000;pool_end=0x88020000;entry=0xdeadbeef;assert(qdrv_fw_load(NULL,"selected",&entry,cpu)>0&&entry==h.e_entry&&maps==expected_maps&&checks==expected_checks&&!active);cases++;
char*m=malloc(n+1);assert(m);unsigned positions[]={0,4,5,6,16,18,20,24,28,40,42,44,52,68,84};for(unsigned i=0;i<sizeof(positions)/sizeof(*positions);i++){memcpy(m,b,n);m[positions[i]]^=1;reject_load(m,n,cpu);}for(unsigned i=0;i<h.e_phnum;i++)if(ph[i].p_filesz){for(unsigned bit=0;bit<8;bit++){memcpy(m,b,n);m[ph[i].p_offset+(ph[i].p_filesz/2)]^=1U<<bit;reject_load(m,n,cpu);}}for(unsigned bit=0;bit<8;bit++){memcpy(m,b,n);m[n-1]^=1U<<bit;reject_load(m,n,cpu);}if(cpu==2)for(unsigned byte=n-28;byte<n;byte++){memcpy(m,b,n);m[byte]^=1;reject_load(m,n,cpu);}reject_load(b,n-1,cpu);memcpy(m,b,n);m[n]=0;reject_load(m,n+1,cpu);
for(int wrong=0;wrong<3;wrong++)if(wrong!=cpu)reject_load(b,n,wrong);
for(unsigned stage=0;stage<6;stage++){reset_pipeline();requested_data=(void*)b;requested_size=n;entry=0xdeadbeef;if(stage==0)fail_request=1;if(stage==1)fail_begin=1;if(stage==2)fail_end=1;if(stage==3)fail_check_at=expected_checks;if(stage==4)fail_map_at=1;if(stage==5)fail_map_at=expected_maps;int rc=qdrv_fw_load(NULL,"selected",&entry,cpu);assert(rc<0&&entry==0xdeadbeef);if(stage==0)assert(requests==1&&!releases&&!begins&&!maps);if(stage==1)assert(releases==1&&begins==1&&!ends&&!hashes&&!maps);if(stage==2)assert(ends==1&&maps==expected_maps&&active);if(stage==3)assert(checks==expected_checks&&!maps&&!copies&&!zeros&&!active&&releases==1&&ends==1);if(stage>=4)assert(maps==fail_map_at&&unmaps+1==maps&&releases==1&&ends==1&&!active);cases++;}
if(cpu==0){reset_pipeline();requested_data=(void*)b;requested_size=n;pool_start=0x8803efc8;pool_end=pool_start+8;entry=0xdeadbeef;assert(qdrv_fw_load(NULL,"selected",&entry,cpu)==-EBUSY&&entry==0xdeadbeef&&!maps&&checks>1&&releases==1&&ends==1&&!active);cases++;}
reset_pipeline();requested_data=(void*)b;requested_size=n;assert(qdrv_fw_load(NULL,NULL,&entry,cpu)==-EINVAL&&!requests);assert(qdrv_fw_load(NULL,"selected",NULL,cpu)==-EINVAL&&!requests);assert(qdrv_fw_load(NULL,"selected",&entry,4)==-EINVAL&&!requests);cases++;free(m);free(b);
}
printf("{\"full_load_pipeline_cases\":%u,\"exact_carriers\":3,\"hash_backend\":\"OpenSSL SHA256 host\",\"last_segment_overlap_before_first_map\":true}\n",cases);return 0;}
