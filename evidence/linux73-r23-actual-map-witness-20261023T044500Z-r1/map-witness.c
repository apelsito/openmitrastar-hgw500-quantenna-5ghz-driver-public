#include <linux/elf.h>
const u32 om_r23_map[]={EM_ARCOMPACT,RUBY_SRAM_BEGIN,RUBY_SRAM_BUS_BEGIN,RUBY_SRAM_NOCACHE_BEGIN,RUBY_SRAM_SIZE,RUBY_DRAM_BEGIN,RUBY_DRAM_BUS_BEGIN,RUBY_MAX_DRAM_SIZE,TOPAZ_AUC_IMEM_ADDR,TOPAZ_AUC_IMEM_SIZE,TOPAZ_AUC_DMEM_ADDR,TOPAZ_AUC_DMEM_SIZE,RUBY_DSP_XYMEM_BEGIN,RUBY_DSP_XYMEM_END};
unsigned long om_r23_muc_translate(unsigned long a){return virt_to_phys((void *)muc_to_lhost(a));}
unsigned long om_r23_bus_translate(unsigned long a){return virt_to_phys(bus_to_virt(a));}
