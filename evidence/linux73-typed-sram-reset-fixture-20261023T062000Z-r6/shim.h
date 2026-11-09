
#ifndef OM73_TEST_SHIM
#define OM73_TEST_SHIM
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#define EINVAL 22
#define EBUSY 16
#define ENODEV 19
#define ENOMEM 12
#define ENOENT 2
#define EOVERFLOW 75
#define EPERM 1
#define ERANGE 34
typedef uint8_t u8;
typedef uint32_t u32;
typedef unsigned long phys_addr_t;
typedef unsigned long dma_addr_t;
typedef unsigned long resource_size_t;
#define __iomem
#define __force
#define __KERNEL__
#define U32_MAX UINT32_MAX
#define PAGE_SIZE 8192u
#define IS_ALIGNED(x,a) (!((x)&((a)-1)))
#define ALIGN(x,a) (((x)+((a)-1))&~((__typeof__(x))(a)-1))
#define is_power_of_2(x) ((x)&&!((x)&((x)-1)))
#define check_add_overflow(a,b,p) __builtin_add_overflow(a,b,p)
#define EXPORT_SYMBOL_GPL(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_LICENSE(x)
#define THIS_MODULE ((void *)1)

#include <pthread.h>
#include <stdatomic.h>
#include <unistd.h>
#include <sched.h>
#define U64_MAX UINT64_MAX
typedef uint64_t u64;
struct task_struct { int identity; };
static _Thread_local struct task_struct fixture_task;
#define current (&fixture_task)
#define DEFINE_MUTEX(name) pthread_mutex_t name=PTHREAD_MUTEX_INITIALIZER
#define DEFINE_SPINLOCK(name) pthread_mutex_t name=PTHREAD_MUTEX_INITIALIZER
static _Thread_local unsigned spin_depth,mutex_depth;
static void lock_spin(pthread_mutex_t *lock,unsigned long *flags){*flags=0;assert(!pthread_mutex_lock(lock));spin_depth++;}
static void unlock_spin(pthread_mutex_t *lock,unsigned long flags){(void)flags;assert(spin_depth);spin_depth--;assert(!pthread_mutex_unlock(lock));}
#define spin_lock_irqsave(lock,flags) lock_spin((lock),&(flags))
#define spin_unlock_irqrestore(lock,flags) unlock_spin((lock),(flags))
static void mutex_lock(pthread_mutex_t *lock){assert(!spin_depth);assert(!pthread_mutex_lock(lock));mutex_depth++;}
static void mutex_unlock(pthread_mutex_t *lock){assert(mutex_depth);mutex_depth--;assert(!pthread_mutex_unlock(lock));}
static int module_live,fail_module,fail_resource,fail_mapping;
static unsigned request_calls,release_calls,map_calls,unmap_calls;
static int resource_live,mapping_live;
static unsigned char storage[0x10000] __attribute__((aligned(8192)));
struct resource {resource_size_t start,end;};
static struct resource resource;
static bool try_module_get(void *m){(void)m;assert(!spin_depth);if(fail_module)return false;module_live++;return true;}
static void module_put(void *m){(void)m;assert(!spin_depth);assert(module_live>0);module_live--;}
static struct resource *request_mem_region_exclusive(resource_size_t start,resource_size_t size,const char *name){
 (void)name;assert(!spin_depth);request_calls++;if(fail_resource||resource_live)return NULL;
 assert(start>=0x88000000ul&&start+size<=0x88080000ul);resource=(struct resource){start,start+size-1};resource_live++;return &resource;
}
static void release_mem_region(resource_size_t start,resource_size_t size){assert(!spin_depth);assert(resource_live);assert(start==resource.start&&size==resource.end-resource.start+1);resource_live--;release_calls++;}
static void *ioremap(resource_size_t start,unsigned long size){assert(!spin_depth);assert(resource_live);assert(start==resource.start&&size<=sizeof(storage));map_calls++;if(fail_mapping)return NULL;mapping_live++;return storage;}
static void iounmap(void *p){assert(!spin_depth);assert(p==storage&&mapping_live);mapping_live--;unmap_calls++;}
static resource_size_t resource_size(struct resource *r){return r->end-r->start+1;}
#endif
