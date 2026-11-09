
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
static void*ioremap_nocache(unsigned long a,size_t n){assert(a&&n&&n<0x1000000U);maps++;if(fail_map_at==maps)return NULL;mapping_size=n;void*p=malloc(n);assert(p);memset(p,0xa5,n);return p;}
static void iounmap(void*p){assert(p);unmaps++;free(p);}
static void qdrv_fw_auc_memcpy(void*d,const void*s,unsigned long n,unsigned long a){(void)a;assert(n<=mapping_size);copies++;memcpy(d,s,n);}
static void qdrv_fw_auc_memzero(void*d,unsigned long n,unsigned long a){(void)a;assert(n<=mapping_size);zeros++;memset(d,0,n);}
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
static char*read_carrier(const char*name,size_t*size){FILE*f=fopen(name,"rb");assert(f);assert(!fseek(f,0,SEEK_END));long n=ftell(f);assert(n>0);rewind(f);char*b=malloc((size_t)n);assert(b&&fread(b,1,(size_t)n,f)==(size_t)n);fclose(f);*size=n;return b;}
static void reset(void){assert(maps==unmaps||fail_map_at);maps=unmaps=copies=zeros=fail_map_at=0;}
static void reject(char*b,size_t n,int cpu,unsigned*cases){reset();uint32_t start=0xdeadbeef;int rc=qdrv_fw_install(b,n,&start,cpu);assert(rc<0&&start==0xdeadbeef&&!maps&&!unmaps&&!copies&&!zeros);(*cases)++;}
int main(int argc,char**argv){assert(argc==4);_Static_assert(sizeof(size_t)==4,"32bit");unsigned cases=0;
for(int cpu=0;cpu<3;cpu++){size_t n;char*b=read_carrier(argv[cpu+1],&n);Elf32_Ehdr h;assert(!qdrv_fw_preflight(b,n,cpu,&h));Elf32_Phdr*ph=(void*)(b+52);uint32_t start=0xdeadbeef;reset();assert(qdrv_fw_install(b,n,&start,cpu)==h.e_phnum&&start==h.e_entry&&maps==unmaps);unsigned expected_maps=maps;cases++;
 for(size_t cut=0;cut<52+(size_t)h.e_phnum*32;cut++)reject(b,cut,cpu,&cases);
 /* Every short nonempty payload is rejected before any destination map. */
 size_t last=52+(size_t)h.e_phnum*32;for(unsigned i=0;i<h.e_phnum;i++)last+=ph[i].p_filesz;
 for(size_t cut=last>128?last-128:0;cut<last;cut++)reject(b,cut,cpu,&cases);
 char*m=malloc(n);assert(m);unsigned offsets[]={0,4,5,6,16,18,20,28,40,42,44};
 for(unsigned i=0;i<sizeof(offsets)/sizeof(*offsets);i++){memcpy(m,b,n);m[offsets[i]]^=0x7f;reject(m,n,cpu,&cases);}
 memcpy(m,b,n);((Elf32_Ehdr*)m)->e_entry=0xe0000040;reject(m,n,cpu,&cases);
 unsigned payload_index=h.e_phnum-1;while(!ph[payload_index].p_filesz||ph[payload_index].p_vaddr==QDRV_FWINFO_VADDR)payload_index--;
 unsigned fields[]={0,4,16,20};for(unsigned i=0;i<4;i++){memcpy(m,b,n);uint32_t*p=(void*)(m+52+payload_index*32+fields[i]);*p=i==0?2:i==1?0xffffffffU:i==2?0xffffffffU:0;reject(m,n,cpu,&cases);}
 memcpy(m,b,n);Elf32_Phdr*p=(void*)(m+52+payload_index*32);p->p_vaddr=0xc0000000;p->p_paddr=0xc0000000;reject(m,n,cpu,&cases);
 memcpy(m,b,n);p=(void*)(m+52+payload_index*32);p->p_vaddr=0x8007ffff;p->p_paddr=0x8007ffff;p->p_filesz=0;p->p_memsz=2;reject(m,n,cpu,&cases);
 for(unsigned i=0;i<h.e_phnum;i++)if(ph[i].p_vaddr==QDRV_FWINFO_VADDR){for(unsigned j=0;j<3;j++){memcpy(m,b,n);struct qdrv_fwinfo*info=(void*)(m+ph[i].p_offset);if(j==0)info->magic[0]='X';if(j==1)info->type=2;if(j==2)info->version++;reject(m,n,cpu,&cases);}}
 reset();fail_map_at=1;start=0xdeadbeef;assert(qdrv_fw_install(b,n,&start,cpu)==-ENOMEM&&start==0xdeadbeef&&maps==1&&!unmaps);cases++;
 if(expected_maps>1){reset();fail_map_at=2;start=0xdeadbeef;assert(qdrv_fw_install(b,n,&start,cpu)==-ENOMEM&&start==0xdeadbeef&&maps==2&&unmaps==1);cases++;}
 reset();assert(qdrv_fw_install(b,n,NULL,cpu)==-EINVAL&&!maps);cases++;reject(b,n,4,&cases);free(m);free(b);
}
printf("{\"cases\":%u,\"carriers\":3,\"preflight_error_mapping_calls\":0}\n",cases);return 0;}
