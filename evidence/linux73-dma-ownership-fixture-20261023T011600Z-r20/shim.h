
#ifndef OM_DMA_FIXTURE_H
#define OM_DMA_FIXTURE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#define __iomem
#define GFP_KERNEL 0
#define EXPORT_SYMBOL(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_LICENSE(x)
#define DEFINE_SPINLOCK(x) int x
#define spin_lock_irqsave(lock, flags) ((void)(lock), (flags)=0)
#define spin_unlock_irqrestore(lock, flags) ((void)(lock), (void)(flags))
#define check_mul_overflow(a,b,p) __builtin_mul_overflow((a),(b),(p))
#define check_add_overflow(a,b,p) __builtin_add_overflow((a),(b),(p))
#define is_power_of_2(x) ((x) && !((x)&((x)-1)))
struct list_head {struct list_head *next,*prev;};
#define LIST_HEAD(name) struct list_head name={&name,&name}
#define container_of(ptr,type,member) ((type *)((char *)(ptr)-offsetof(type,member)))
#define list_for_each_entry(pos,head,member) \
 for(struct list_head *fixture_it=(head)->next; \
 fixture_it!=(head) && (((pos)=container_of(fixture_it,__typeof__(*(pos)),member)),1); \
 fixture_it=fixture_it->next)
static void list_add(struct list_head *n,struct list_head *head){n->next=head->next;n->prev=head;head->next->prev=n;head->next=n;}
static void list_del(struct list_head *n){n->next->prev=n->prev;n->prev->next=n->next;}
typedef struct {uint32_t desc_count;void *descs;unsigned long aligned_vdescs,unaligned_vdescs,descs_dma_addr;} aligned_dma_descs;
static int fail_record,fail_raw,fail_map,record_live,raw_live,mapping_live,dram_free,sram_free,unmaps;
static void (*fixture_hook)(void);
static void *kzalloc(size_t n,int g){(void)g;if(fail_record){fail_record=0;return NULL;}void *p=calloc(1,n);if(p)record_live++;return p;}
static void *kmalloc(size_t n,int g){(void)g;if(fail_raw){fail_raw=0;return NULL;}if(n>1048576)return NULL;void *p=malloc(n);if(p)raw_live++;if(fixture_hook){void(*h)(void)=fixture_hook;fixture_hook=NULL;h();}return p;}
static void *heap_sram_alloc(unsigned long n){return kmalloc(n,0);}
static void heap_sram_free(void *p){if(p){sram_free++;raw_live--;free(p);}}
/* Determine record-vs-raw by size tracked using list member containment test. */
static void *record_ptrs[64];
static unsigned record_count;
static void *fixture_record_alloc(size_t n,int g){void *p=kzalloc(n,g);if(p){assert(record_count<64);record_ptrs[record_count++]=p;}return p;}
#define kzalloc fixture_record_alloc
static void kfree(void *p){if(!p)return;for(unsigned i=0;i<record_count;i++){if(record_ptrs[i]==p){record_ptrs[i]=NULL;record_live--;free(p);return;}}dram_free++;raw_live--;free(p);}
static void *ioremap_nocache(unsigned long a,size_t n){(void)a;if(fail_map){fail_map=0;return NULL;}void *p=calloc(1,n);if(p)mapping_live++;return p;}
static void iounmap(void *p){assert(p);unmaps++;mapping_live--;free(p);}
static void flush_dcache_range(unsigned long a,unsigned long b){assert(b>=a);}
static unsigned long align_val_up(unsigned long a,unsigned long align){return (a+align-1)&~(align-1);}
static unsigned long virt_to_bus(void *p){return (unsigned long)p;}
#endif
