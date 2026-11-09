#ifndef OPENMITRASTAR_QDRV_COMPAT_H
#define OPENMITRASTAR_QDRV_COMPAT_H

#include <asm/cacheflush.h>
#include <asm/board/gpio.h>
#include <asm/hardware.h>
#include <common/ruby_mem.h>
#include <linux/etherdevice.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/preempt.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/openmitrastar_bridge.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/unaligned.h>

/* Removed legacy APIs, preserving strlcpy return length rather than strscpy's
 * negative truncation result. Inputs obey the existing C-string contract. */
static inline size_t om_linux73_strlcpy(char *dst, const char *src, size_t size)
{
    size_t len = strlen(src);
    if (size) {
        size_t copy = len < size - 1 ? len : size - 1;
        memcpy(dst, src, copy);
        dst[copy] = '\0';
    }
    return len;
}
#define strlcpy om_linux73_strlcpy
#ifndef PDE_DATA
#define PDE_DATA(inode) pde_data(inode)
#endif


void *heap_sram_alloc(unsigned long size);
void heap_sram_free(void *ptr);
u32 chip_id(void);

#ifndef ARC_DCACHE_LINE_LEN
#define ARC_DCACHE_LINE_LEN ARC_DCACHE_LINE_LENGTH
#endif

/* The OEM linker placed selected hot paths in SRAM. Stage 1 keeps them in text. */
#ifndef __sram_text
#define __sram_text
#endif

/* Stage 1 uses the normal Linux bridge path, not Quantenna's private hooks. */
#undef CONFIG_BRIDGE
#undef CONFIG_BRIDGE_MODULE

/* IPv6 stays enabled in Linux; only qdrv's private NDISC hooks are omitted. */
#if defined(OPENMITRASTAR_QDRV_NO_IPV6_OFFLOAD) && !defined(OPENMITRASTAR_NATIVE_IPV6_PROVIDER)
#undef CONFIG_IPV6
#endif

#ifndef BR_SUBPORT_MAP
#define BR_SUBPORT_MAP(port) ((port) | 0x8000U)
#endif
#ifndef BR_SUBPORT_UNMAP
#define BR_SUBPORT_UNMAP(port) ((port) & 0x7fffU)
#endif

#ifndef read_new_aux_reg
#define read_new_aux_reg(reg) read_aux_reg(reg)
#endif

/* ARC ioremap() already creates the device mapping expected by this driver. */
#ifndef ioremap_nocache
#define ioremap_nocache ioremap
#endif

#ifndef writel_or
#define writel_or(bits, addr) writel(readl(addr) | (bits), (addr))
#endif
#ifndef writel_and
#define writel_and(bits, addr) writel(readl(addr) & (bits), (addr))
#endif

#ifndef arc_write_uncached_16
#define arc_write_uncached_16(ptr, data) \
	__asm__ __volatile__("stw.di %0, [%1]" : : "r"(data), "r"(ptr))
#endif

#ifndef compare_ether_addr
#define compare_ether_addr(addr1, addr2) (!ether_addr_equal((addr1), (addr2)))
#endif

#ifndef strict_strtoul
#define strict_strtoul kstrtoul
#endif

#ifndef ipv6_addr_copy
#define ipv6_addr_copy(dst, src) (*(dst) = *(src))
#endif

#ifndef __skb_append
#define __skb_append(old, new, list) __skb_queue_after((list), (old), (new))
#endif

#ifndef LL_ALLOCATED_SPACE
#define LL_ALLOCATED_SPACE(dev) \
	((((dev)->hard_header_len + (dev)->needed_headroom + \
	   (dev)->needed_tailroom) & ~(HH_DATA_MOD - 1)) + HH_DATA_MOD)
#endif

int qdrv_compat_open(const char *path, int flags, umode_t mode);
ssize_t qdrv_compat_read(int handle, void *buffer, size_t count);
ssize_t qdrv_compat_write(int handle, const void *buffer, size_t count);
int qdrv_compat_close(int handle);

#ifndef QTN_FLAG_WIFI_DEVICE
#define QTN_FLAG_WIFI_DEVICE          0x80000000U
#define QTN_FLAG_INTRA_BSS_ISOLATE    0x40000000U
#define QTN_FLAG_BSS_ISOLATE          0x20000000U
#endif
#define QTN_FLAG_WIFI_INTRA_BSS \
	(QTN_FLAG_WIFI_DEVICE | QTN_FLAG_INTRA_BSS_ISOLATE)
#define QTN_FLAG_WIFI_BSS_ISOLATE \
	(QTN_FLAG_WIFI_DEVICE | QTN_FLAG_BSS_ISOLATE)
#define QTN_FLAG_IS_INTRA_BSS(flags) \
	(((flags) & QTN_FLAG_WIFI_INTRA_BSS) == QTN_FLAG_WIFI_INTRA_BSS)
#define QTN_FLAG_IS_BSS_ISOLATE(flags) \
	(((flags) & QTN_FLAG_WIFI_BSS_ISOLATE) == QTN_FLAG_WIFI_BSS_ISOLATE)

/* Entropy sampling was removed from request_irq(); no interrupt semantics. */
#ifndef IRQF_SAMPLE_RANDOM
#define IRQF_SAMPLE_RANDOM 0
#endif

#define QTN_SKB_ALLOC_TRACE 0
#define QTN_SKB_ALLOC_TRACE_ARGNAMES(file, func, line)
#define QTN_SKB_ALLOC_TRACE_ARGS
#define QTN_SKB_ALLOC_TRACE_ARGVARS
#define QTN_SKB_ALLOC_TRACE_ARGSRC

static inline void openmitrastar_flush_dcache_range(unsigned long start,
		unsigned long end)
{
	dma_cache_wback(virt_to_phys((void *)start), end - start);
}

static inline void openmitrastar_flush_inv_dcache_range(unsigned long start,
		unsigned long end)
{
	dma_cache_wback_inv(virt_to_phys((void *)start), end - start);
}

static inline void openmitrastar_inv_dcache_range(unsigned long start,
		unsigned long end)
{
	dma_cache_inv(virt_to_phys((void *)start), end - start);
}

#define flush_dcache_range openmitrastar_flush_dcache_range
#define flush_and_inv_dcache_range openmitrastar_flush_inv_dcache_range
#define inv_dcache_range openmitrastar_inv_dcache_range


/* Linux7.3removed in_irq alias: same hardIRQcount contract, not softIRQ. */
#ifndef in_irq
#define in_irq in_hardirq
#endif
/* Preserve original strncpy copy/pad/truncation semantics for historical ABI
 * buffers; replacing all callers with strscpy would alter nonterminated fields. */
static inline char *om_linux73_strncpy(char *destination,const char *source,size_t count)
{
    size_t copied=strnlen(source,count);
    memcpy(destination,source,copied);
    if(copied<count)memset(destination+copied,0,count-copied);
    return destination;
}
#define strncpy om_linux73_strncpy

/* Exact Linux7.3 renamed timer API compatibility; no timing policy changes. */
#ifndef from_timer
#define from_timer timer_container_of
#endif
#ifndef del_timer
#define del_timer timer_delete
#endif
#ifndef del_timer_sync
#define del_timer_sync timer_delete_sync
#endif
/* OEM register constants are CPU I/O addresses. Preserve MMIO ordering while
 * explicitly expressing pointer typing to modern Linux ARC accessors. */
#undef readl
#define readl(address) ({ u32 om_value=readl_relaxed((const volatile void __iomem *)(uintptr_t)(address)); __iormb(); om_value; })
#undef writel
#define writel(value,address) ({ __iowmb(); writel_relaxed((value),(volatile void __iomem *)(uintptr_t)(address)); })

/* Preserve optionalexternalI2Csensorunavailability when kernelbusisabsent.
 * These guards do not emulate a sensor or report thermal acceptance. */
#if !IS_ENABLED(CONFIG_I2C)
static inline struct i2c_adapter *om73_i2c_get_adapter(int nr){(void)nr;return NULL;}
static inline void om73_i2c_put_adapter(struct i2c_adapter *a){(void)a;}
static inline struct i2c_client *om73_i2c_new_client_device(struct i2c_adapter *a,const struct i2c_board_info *i){(void)a;(void)i;return ERR_PTR(-EOPNOTSUPP);}
static inline void om73_i2c_unregister_device(struct i2c_client *c){(void)c;}
#define i2c_get_adapter om73_i2c_get_adapter
#define i2c_put_adapter om73_i2c_put_adapter
#define i2c_new_client_device om73_i2c_new_client_device
#define i2c_unregister_device om73_i2c_unregister_device
#endif
static inline int om73_gpio_export(unsigned gpio,bool direction_may_change)
{
    struct gpio_desc *desc=gpio_to_desc(gpio);
    if(!desc)return -ENODEV;
    return gpiod_export(desc,direction_may_change);
}
#define gpio_export om73_gpio_export
#endif
