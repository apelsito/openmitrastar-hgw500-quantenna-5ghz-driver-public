
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#undef EINPROGRESS
#define EINPROGRESS 115 /* Linux ARC errno; hostMinGW value differs. */
#include "openmitrastar_vopt481_recovered.h"
#include "openmitrastar_vopt482_state.h"
typedef uint32_t u32;typedef uint8_t u8;
typedef unsigned spinlock_t;
struct work_struct{void(*fn)(struct work_struct*);unsigned queued;};
struct timer_list{unsigned pending;};
struct ieee80211com;struct ieee80211vap;struct ieee80211_node;
struct node_table{struct ieee80211_node*nodes[8];unsigned count;};
struct ieee80211_channel{int8_t ic_maxpower_normal;};
struct net_device{unsigned unused;};
struct ieee80211vap{struct ieee80211com*iv_ic;struct ieee80211_node*iv_bss;unsigned iv_opmode;unsigned char iv_myaddr[6];struct net_device*iv_dev;};
struct ieee80211_node{struct ieee80211vap*ni_vap;unsigned char ni_macaddr[6];u32 ni_qtn_flags;};
struct ieee80211com{unsigned ic_opmode;struct om_vopt_state om_vopt;struct om_vopt482_state om_vopt482;struct work_struct om_vopt_work;spinlock_t om_vopt_lock;bool om_vopt_stopping,om_vopt_pending,om_vopt_running,om_vopt_destroying;int om_vopt_last_error;int(*om_vopt_vsp_get)(struct ieee80211com*,unsigned,u32*);int(*om_vopt_vsp_set)(struct ieee80211com*,unsigned,unsigned);unsigned ic_flags,ic_flags_ext;struct{struct{unsigned ocac_chan_ieee,ocac_enable;}ocac_cfg;}ic_ocac;struct ieee80211_channel*ic_curchan;struct node_table ic_sta;void*ic_setparam;void(*ic_iterate_nodes)(struct node_table*,void(*)(void*,struct ieee80211_node*),void*,int);struct ieee80211vap*ic_vaps;u8 ic_pppc_select_enable,ic_airfair;struct timer_list ic_inact;};
#define container_of(p,t,m) ((t*)((char*)(p)-offsetof(t,m)))
#define IEEE80211_M_HOSTAP 6
#define IEEE80211_ADDR_LEN 6
#define IEEE80211_CHAN_ANYC ((void*)0xffff)
#define IEEE80211_INACT_WAIT 5
#define HZ 200
#define READ_ONCE(v) (v)
#define EXPORT_SYMBOL(v)
#define TAILQ_FIRST(h) (*(h))
#define TAILQ_EMPTY(h) (!*(h))
static unsigned locks,rtlock,try_success=1,scheduled,canceled,timerjoins,params[500],reassocs,iterations;static int param_values[500],rc482,scs_rc,ocac_rc,irq_flag,interrupt_flag;static const struct work_struct*current;
static unsigned long jiffies;
#define spin_lock_irqsave(l,f) do{(f)=0;(*(l))++;locks++;}while(0)
#define spin_unlock_irqrestore(l,f) do{(void)(f);assert(*(l));(*(l))--;locks--;}while(0)
#define spin_lock_init(l) (*(l)=0)
#define INIT_WORK(w,f) do{(w)->fn=(f);(w)->queued=0;}while(0)
static int rtnl_trylock(void){assert(!locks);if(!try_success)return 0;rtlock++;return 1;}
static void rtnl_unlock(void){assert(rtlock);rtlock--;}
static unsigned rtnl_is_locked(void){return rtlock;}
static unsigned in_interrupt(void){return interrupt_flag;}
static unsigned irqs_disabled(void){return irq_flag;}
static int schedule_work(struct work_struct*w){scheduled++;if(w->queued)return 0;w->queued=1;return 1;}
static void cancel_work_sync(struct work_struct*w){assert(!locks);w->queued=0;canceled++;}
static void del_timer_sync(struct timer_list*t){assert(!locks);t->pending=0;timerjoins++;}
static void mod_timer(struct timer_list*t,unsigned long when){(void)when;t->pending=1;}
static const struct work_struct*current_work(void){return current;}
int om_qdrv_vopt482_pending(const struct om_vopt482_state*s){return s&&(s->processing||s->quarantined);}
int om_qdrv_vopt482(struct ieee80211vap*v,struct om_vopt482_state*s,int val){(void)v;params[482]++;param_values[482]=val;if(rc482==-EINPROGRESS){s->processing=s->quarantined=1;s->payload=(void*)1;}return rc482;}
static void ieee80211_param_to_qdrv(struct ieee80211vap*v,unsigned command,int val,void*d,unsigned n){(void)v;(void)d;(void)n;assert(command<500);params[command]++;param_values[command]=val;}
static int ieee80211_param_scs_set(struct net_device*d,struct ieee80211vap*v,u32 value){(void)d;(void)v;param_values[147]=value;return scs_rc;}
static int ieee80211_param_ocac_set(struct net_device*d,struct ieee80211vap*v,u32 value){(void)d;v->iv_ic->ic_ocac.ocac_cfg.ocac_enable=(value>>16)==1;return ocac_rc;}
static void ieee80211_wireless_reassoc(struct ieee80211vap*v,unsigned a,unsigned b){(void)v;assert(!a&&!b);reassocs++;}
static void iterator(struct node_table*t,void(*fn)(void*,struct ieee80211_node*),void*a,int ignore){assert(ignore==0);iterations++;for(unsigned i=0;i<t->count;i++)fn(a,t->nodes[i]);}
/* OEM derived reconstruction. Callback errors are ignored where OEM ignores
 * them; adapter/provider availability remains an independent acceptance gate. */
#include "openmitrastar_vopt481_recovered.h"
typedef char om_vopt_u32_is_four[(sizeof(om_vopt_u32)==4)?1:-1];
void om_vopt_initialize(struct om_vopt_state *state,unsigned pppc,unsigned airfair)
{
    struct om_vopt_state zero={0};
    *state=zero;
    state->saved_probe=1;
    state->saved_pppc=(om_vopt_u8)pppc;
    state->saved_airfair=(om_vopt_u8)airfair;
    state->live_pppc=(om_vopt_u8)pppc;
    state->live_airfair=(om_vopt_u8)airfair;
}
int om_vopt_set(struct om_vopt_state *state,unsigned opmode,om_vopt_u32 value)
{
    /* Signed OEM bounds0..2; uint comparison rejects every negative encoding. */
    if(!state||opmode!=6||value>2)return -95;
    state->policy=(om_vopt_u8)value;return 0;
}
int om_vopt_get(const struct om_vopt_state *state,unsigned opmode,int *output)
{
    if(!state||!output)return -22;
    *output=opmode==6?((state->policy&15U)<<4)|state->effective:0;
    return 0;
}
static int om_vopt_byte_value(unsigned value)
{
    /* OEM ldb.x sign extends its saved byte before param_to_qdrv. */
    return value<128? (int)value:(int)value-256;
}
int om_vopt_apply(struct om_vopt_state *state,unsigned effective,
    const struct om_vopt_environment *environment,const struct om_vopt_provider *provider)
{
    om_vopt_u32 value;int om482_rc;
    if(!state||!environment||!provider||!provider->parameter||
       !provider->scs||!provider->ocac||!provider->reassociate)return -95;
    /* Provider availability checked before mutation. Original consumer assumes
     * these callbacks exist. This refusal is a declared source adapter guard. */
    state->effective=(om_vopt_u8)effective;
    if(!state->effective){
        if(state->saved_txbf){
            provider->parameter(provider->context,78,(int)state->saved_txbf);
            provider->parameter(provider->context,68,17);
        }
        if(state->saved_pppc){
            provider->parameter(provider->context,158,(int)state->saved_pppc);
            state->live_pppc=state->saved_pppc;
        }
        if(state->saved_scs){
            value=state->saved_scs|0x10000U;
            if(!provider->scs(provider->context,value))
                provider->parameter(provider->context,147,(int)value);
        }
        if(state->saved_ocac){
            value=environment->ocac_channel&65535U;value|=0x10000U;
            provider->ocac(provider->context,value);
        }
        if(state->saved_bgscan&&provider->background_set)
            provider->background_set(provider->context,0,1);
        if(state->saved_probe)
            provider->parameter(provider->context,195,(int)state->saved_probe);
        if(state->saved_airfair){
            state->live_airfair=state->saved_airfair;
            provider->parameter(provider->context,288,(int)state->saved_airfair);
        }
        om482_rc=provider->parameter(provider->context,482,om_vopt_byte_value((unsigned)environment->mode_value&255U));
        if(om482_rc==-115)return om482_rc;
        state->live_duplicate_mu=0;
        provider->parameter(provider->context,414,0);
    }else{
        provider->parameter(provider->context,68,16);
        provider->parameter(provider->context,78,0);
        if(!(environment->flags&0x40000000U)||!(environment->flags_ext&0x800000U))
            provider->parameter(provider->context,158,0);
        state->live_pppc=0;
        provider->parameter(provider->context,195,0);
        state->live_airfair=0;
        provider->parameter(provider->context,288,0);
        value=0x10000U;
        if(!provider->scs(provider->context,value))
            provider->parameter(provider->context,147,(int)value);
        value=0x20000U;
        provider->ocac(provider->context,value);
        if(provider->background_get&&provider->background_set&&
           !provider->background_get(provider->context,0,&value)){
            state->saved_bgscan=value;
            provider->background_set(provider->context,0,0);
        }
        om482_rc=provider->parameter(provider->context,482,0);
        if(om482_rc==-115)return om482_rc;
        state->live_duplicate_mu=3;
        provider->parameter(provider->context,414,3);
    }
    provider->reassociate(provider->context,0,0);
    return 0;
}
void om_vopt_count_peer(const struct om_vopt_peer *peer,om_vopt_u32 *total,om_vopt_u32 *qualified)
{
    if(!peer||!total||!qualified||peer->self_bss||peer->self_address)return;
    ++*total;if(peer->flags&0x800U)++*qualified;
}
int om_vopt_tick(struct om_vopt_state *state,om_vopt_u32 *global_backoff,
    om_vopt_u32 total,om_vopt_u32 qualified,const struct om_vopt_environment *environment,
    const struct om_vopt_provider *provider)
{
    int result;
    if(!state||!global_backoff)return -22;
    if(state->policy==state->effective)return 0;
    if(state->policy==0||state->policy==1){
        result=om_vopt_apply(state,state->policy,environment,provider);
        if(!result)*global_backoff=0;return result;
    }
    if(state->policy==2){
        if(*global_backoff){--*global_backoff;return 0;}
        if(total==qualified&&state->effective!=1)
            return om_vopt_apply(state,1,environment,provider);
        if(total!=qualified&&state->effective!=0){
            result=om_vopt_apply(state,0,environment,provider);
            if(!result)*global_backoff=10;return result;
        }
    }
    return 0;
}

/* OEM481 Linux worker adapter. Timer onlyqueues; callbacks run underRTNL.
 * Exact coreR2 is separately preserved; this variant stops on retained482. */
static u32 om_vopt_global_backoff;
struct om_vopt_work_context { struct ieee80211com *ic;struct ieee80211vap *vap; };
static int om_vopt_kernel_parameter(void *context,unsigned command,int value)
{
    struct om_vopt_work_context *ctx=context;
    if(command==482)return om_qdrv_vopt482(ctx->vap,&ctx->ic->om_vopt482,value);
    ieee80211_param_to_qdrv(ctx->vap,command,value,NULL,0);return 0;
}
static int om_vopt_kernel_scs(void *context,u32 value)
{
    struct om_vopt_work_context *ctx=context;
    return ieee80211_param_scs_set(ctx->vap->iv_dev,ctx->vap,value);
}
static int om_vopt_kernel_ocac(void *context,u32 value)
{
    struct om_vopt_work_context *ctx=context;
    return ieee80211_param_ocac_set(ctx->vap->iv_dev,ctx->vap,value);
}
static int om_vopt_kernel_vsp_get(void *context,unsigned index,u32 *value)
{
    struct om_vopt_work_context *ctx=context;
    return ctx->ic->om_vopt_vsp_get(ctx->ic,index,value);
}
static int om_vopt_kernel_vsp_set(void *context,unsigned index,unsigned value)
{
    struct om_vopt_work_context *ctx=context;
    return ctx->ic->om_vopt_vsp_set(ctx->ic,index,value);
}
static void om_vopt_kernel_reassociate(void *context,unsigned first,unsigned second)
{
    struct om_vopt_work_context *ctx=context;
    ieee80211_wireless_reassoc(ctx->vap,first,second);
}
static void om_vopt_kernel_peer(void *arg,struct ieee80211_node *ni)
{
    struct om_vopt_peer peer;
    u32 *counts=arg;
    peer.self_bss=ni==ni->ni_vap->iv_bss;
    peer.self_address=!memcmp(ni->ni_macaddr,ni->ni_vap->iv_myaddr,IEEE80211_ADDR_LEN);
    peer.flags=READ_ONCE(ni->ni_qtn_flags);
    om_vopt_count_peer(&peer,&counts[0],&counts[1]);
}
static void om_vopt_kernel_work(struct work_struct *work)
{
    struct ieee80211com *ic=container_of(work,struct ieee80211com,om_vopt_work);
    struct ieee80211vap *vap;
    struct om_vopt_work_context context;
    struct om_vopt_environment environment;
    struct om_vopt_provider provider;
    unsigned long flags;
    u32 counts[2]={0,0};
    int rc=0;
    /* Never block on RTNL: a VAPdelete caller may hold it during syncjoin. */
    if(!rtnl_trylock())goto no_rtnl;
    spin_lock_irqsave(&ic->om_vopt_lock,flags);
    if(ic->om_vopt_stopping||ic->om_vopt482.quarantined){
        spin_unlock_irqrestore(&ic->om_vopt_lock,flags);rtnl_unlock();goto no_rtnl;
    }
    ic->om_vopt_running=true;
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);
    vap=TAILQ_FIRST(&ic->ic_vaps);
    if(!vap||vap->iv_opmode!=IEEE80211_M_HOSTAP||!vap->iv_bss||
       !ic->ic_setparam||!ic->ic_iterate_nodes){rc=-EAGAIN;goto done;}
    context.ic=ic;context.vap=vap;
    memset(&provider,0,sizeof(provider));provider.context=&context;
    provider.parameter=om_vopt_kernel_parameter;provider.scs=om_vopt_kernel_scs;
    provider.ocac=om_vopt_kernel_ocac;provider.reassociate=om_vopt_kernel_reassociate;
    if(ic->om_vopt_vsp_get&&ic->om_vopt_vsp_set){
        provider.background_get=om_vopt_kernel_vsp_get;
        provider.background_set=om_vopt_kernel_vsp_set;
    }
    environment.flags=ic->ic_flags;environment.flags_ext=ic->ic_flags_ext;
    environment.ocac_channel=ic->ic_ocac.ocac_cfg.ocac_chan_ieee;
    environment.mode_value=ic->ic_curchan&&ic->ic_curchan!=IEEE80211_CHAN_ANYC?
        (int)(signed char)ic->ic_curchan->ic_maxpower_normal:0;
    ic->ic_iterate_nodes(&ic->ic_sta,om_vopt_kernel_peer,counts,0);
    rc=om_vopt_tick(&ic->om_vopt,&om_vopt_global_backoff,counts[0],counts[1],
        &environment,&provider);
    ic->ic_pppc_select_enable=ic->om_vopt.live_pppc;
    ic->ic_airfair=ic->om_vopt.live_airfair;
done:
    spin_lock_irqsave(&ic->om_vopt_lock,flags);
    ic->om_vopt_last_error=rc;ic->om_vopt_running=false;
    if(rc==-EINPROGRESS)ic->om_vopt_stopping=true;
    ic->om_vopt_pending=false;
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);rtnl_unlock();return;
no_rtnl:
    spin_lock_irqsave(&ic->om_vopt_lock,flags);ic->om_vopt_pending=false;
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);
}
void om_ieee80211_vopt_initialize(struct ieee80211com *ic)
{
    spin_lock_init(&ic->om_vopt_lock);INIT_WORK(&ic->om_vopt_work,om_vopt_kernel_work);
    om_vopt_initialize(&ic->om_vopt,ic->ic_pppc_select_enable,ic->ic_airfair);
    memset(&ic->om_vopt482,0,sizeof(ic->om_vopt482));
    ic->om_vopt_stopping=false;ic->om_vopt_pending=false;ic->om_vopt_running=false;
    ic->om_vopt_destroying=false;
    ic->om_vopt_last_error=0;
}
EXPORT_SYMBOL(om_ieee80211_vopt_initialize);
void om_ieee80211_vopt_queue(struct ieee80211com *ic)
{
    unsigned long flags;
    spin_lock_irqsave(&ic->om_vopt_lock,flags);
    if(!ic->om_vopt_stopping&&!ic->om_vopt_pending&&!ic->om_vopt482.quarantined){
        ic->om_vopt_pending=true;
        if(!schedule_work(&ic->om_vopt_work))ic->om_vopt_pending=false;
    }
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);
}
EXPORT_SYMBOL(om_ieee80211_vopt_queue);
int om_ieee80211_vopt_quiesce(struct ieee80211com *ic)
{
    unsigned long flags;
    if(current_work()==&ic->om_vopt_work)return -EBUSY;
    spin_lock_irqsave(&ic->om_vopt_lock,flags);ic->om_vopt_stopping=true;
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);
    del_timer_sync(&ic->ic_inact);cancel_work_sync(&ic->om_vopt_work);
    return om_qdrv_vopt482_pending(&ic->om_vopt482)?-EINPROGRESS:0;
}
EXPORT_SYMBOL(om_ieee80211_vopt_quiesce);
int om_ieee80211_vopt_detach_begin(struct ieee80211com *ic)
{
    unsigned long flags;
    spin_lock_irqsave(&ic->om_vopt_lock,flags);ic->om_vopt_destroying=true;
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);
    return om_ieee80211_vopt_quiesce(ic);
}
EXPORT_SYMBOL(om_ieee80211_vopt_detach_begin);
void om_ieee80211_vopt_resume(struct ieee80211com *ic)
{
    unsigned long flags;
    spin_lock_irqsave(&ic->om_vopt_lock,flags);
    if(!ic->om_vopt_destroying&&!om_qdrv_vopt482_pending(&ic->om_vopt482)&&
       !TAILQ_EMPTY(&ic->ic_vaps)){
        ic->om_vopt_stopping=false;
        mod_timer(&ic->ic_inact,jiffies+IEEE80211_INACT_WAIT*HZ);
    }
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);
}
EXPORT_SYMBOL(om_ieee80211_vopt_resume);
static int om_vopt_kernel_policy(struct ieee80211vap *vap,int *value,int write)
{
    struct ieee80211com *ic=vap->iv_ic;unsigned long flags;int rc;
    if(!value)return -EINVAL;
    spin_lock_irqsave(&ic->om_vopt_lock,flags);
    if(ic->om_vopt_stopping||ic->om_vopt_running){rc=-EBUSY;goto out;}
    rc=write?om_vopt_set(&ic->om_vopt,ic->ic_opmode,(u32)*value):
        om_vopt_get(&ic->om_vopt,ic->ic_opmode,value);
out:spin_unlock_irqrestore(&ic->om_vopt_lock,flags);return rc;
}
static int om_vopt_kernel_write_ready(struct ieee80211com *ic,unsigned command)
{
    unsigned long flags;int result=0;
    if(command!=481&&command!=78&&command!=195&&command!=147&&command!=158&&
       command!=288&&command!=232&&command!=375)return 0;
    /* VOPT worker mutates its core outside the spinlock while RTNL-owned.
     * Admit user writes only under the same process/lifetime serialization. */
    if(!rtnl_is_locked()||in_interrupt()||irqs_disabled())return -EBUSY;
    spin_lock_irqsave(&ic->om_vopt_lock,flags);
    if(ic->om_vopt_running||ic->om_vopt_destroying||ic->om_vopt482.quarantined)result=-EBUSY;
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);return result;
}
static void om_vopt_kernel_backup(struct ieee80211com *ic,unsigned command,u32 value)
{
    unsigned long flags;
    spin_lock_irqsave(&ic->om_vopt_lock,flags);
    switch(command){
    case 78:ic->om_vopt.saved_txbf=(u8)value;break;
    case 195:ic->om_vopt.saved_probe=(u8)value;break;
    case 147:ic->om_vopt.saved_scs=value&65535U;break;
    case 158:ic->om_vopt.saved_pppc=(u8)value;ic->om_vopt.live_pppc=(u8)value;break;
    case 288:ic->om_vopt.saved_airfair=(u8)value;ic->om_vopt.live_airfair=(u8)value;break;
    case 232:case 375:ic->om_vopt.saved_ocac=ic->ic_ocac.ocac_cfg.ocac_enable;break;
    }
    spin_unlock_irqrestore(&ic->om_vopt_lock,flags);
}

static struct ieee80211com ic;static struct ieee80211vap vap;static struct ieee80211_node self,peer;static struct ieee80211_channel chan;static struct net_device dev;
static void clear(void){memset(&ic,0,sizeof(ic));memset(&vap,0,sizeof(vap));memset(&self,0,sizeof(self));memset(&peer,0,sizeof(peer));memset(params,0,sizeof(params));memset(param_values,0,sizeof(param_values));locks=rtlock=scheduled=canceled=timerjoins=reassocs=iterations=irq_flag=interrupt_flag=0;try_success=1;rc482=scs_rc=ocac_rc=0;current=NULL;ic.ic_opmode=6;vap.iv_ic=&ic;vap.iv_opmode=6;vap.iv_bss=&self;vap.iv_dev=&dev;self.ni_vap=peer.ni_vap=&vap;peer.ni_macaddr[0]=1;peer.ni_qtn_flags=0x800;ic.ic_vaps=&vap;ic.ic_setparam=(void*)1;ic.ic_iterate_nodes=iterator;ic.ic_sta.nodes[0]=&self;ic.ic_sta.nodes[1]=&peer;ic.ic_sta.count=2;chan.ic_maxpower_normal=-127;ic.ic_curchan=&chan;ic.ic_pppc_select_enable=ic.ic_airfair=1;om_ieee80211_vopt_initialize(&ic);om_vopt_global_backoff=0;}
static void run(void){ic.om_vopt_work.queued=0;current=&ic.om_vopt_work;ic.om_vopt_work.fn(&ic.om_vopt_work);current=NULL;}
int main(void){unsigned cases=0;int value;
clear();assert(ic.om_vopt.saved_probe==1&&ic.om_vopt.saved_pppc==1&&ic.om_vopt.saved_airfair==1&&!ic.om_vopt_stopping&&!ic.om_vopt_pending);om_ieee80211_vopt_queue(&ic);om_ieee80211_vopt_queue(&ic);assert(scheduled==1&&ic.om_vopt_pending&&!params[482]);run();assert(!ic.om_vopt_pending&&!params[482]&&!reassocs&&!locks&&!rtlock);cases++;
clear();value=2;assert(!om_vopt_kernel_policy(&vap,&value,1));om_ieee80211_vopt_queue(&ic);run();assert(ic.om_vopt.effective==1&&params[482]==1&&params[414]==1&&reassocs==1&&iterations==1&&!ic.om_vopt_pending);cases++;
clear();value=1;assert(!om_vopt_kernel_policy(&vap,&value,1));rc482=-EINPROGRESS;om_ieee80211_vopt_queue(&ic);run();assert(ic.om_vopt.effective==1&&params[482]==1&&!params[414]&&!reassocs&&ic.om_vopt_stopping&&ic.om_vopt_last_error==-EINPROGRESS);assert(om_ieee80211_vopt_quiesce(&ic)==-EINPROGRESS);scheduled=0;om_ieee80211_vopt_queue(&ic);assert(!scheduled);cases++;
clear();value=1;assert(!om_vopt_kernel_policy(&vap,&value,1));try_success=0;om_ieee80211_vopt_queue(&ic);run();assert(!params[482]&&!ic.om_vopt_pending&&!ic.om_vopt_running);try_success=1;om_ieee80211_vopt_queue(&ic);run();assert(params[482]==1);cases++;
clear();om_ieee80211_vopt_queue(&ic);assert(!om_ieee80211_vopt_quiesce(&ic)&&ic.om_vopt_stopping&&!ic.om_vopt_work.queued&&timerjoins==1&&canceled==1);om_ieee80211_vopt_resume(&ic);assert(!ic.om_vopt_stopping&&ic.ic_inact.pending);cases++;
clear();om_ieee80211_vopt_queue(&ic);assert(!om_ieee80211_vopt_detach_begin(&ic)&&ic.om_vopt_destroying);om_ieee80211_vopt_resume(&ic);assert(ic.om_vopt_stopping&&!ic.ic_inact.pending);cases++;
clear();current=&ic.om_vopt_work;assert(om_ieee80211_vopt_quiesce(&ic)==-EBUSY&&!timerjoins&&!canceled);current=NULL;cases++;
clear();om_vopt_kernel_backup(&ic,78,255);om_vopt_kernel_backup(&ic,195,128);om_vopt_kernel_backup(&ic,147,0x12345678);om_vopt_kernel_backup(&ic,158,256);om_vopt_kernel_backup(&ic,288,255);ic.ic_ocac.ocac_cfg.ocac_enable=1;om_vopt_kernel_backup(&ic,232,0);assert(ic.om_vopt.saved_txbf==255&&ic.om_vopt.saved_probe==128&&ic.om_vopt.saved_scs==0x5678&&!ic.om_vopt.saved_pppc&&ic.om_vopt.saved_airfair==255&&ic.om_vopt.saved_ocac);cases++;
clear();ic.om_vopt_running=true;value=1;assert(om_vopt_kernel_policy(&vap,&value,1)==-EBUSY);cases++;
/* No survivingVAP leaves stop latched; a newconstructedVAP resumes timer. */
clear();assert(!om_ieee80211_vopt_quiesce(&ic));ic.ic_vaps=NULL;om_ieee80211_vopt_resume(&ic);assert(ic.om_vopt_stopping&&!ic.ic_inact.pending);ic.ic_vaps=&vap;om_ieee80211_vopt_resume(&ic);assert(!ic.om_vopt_stopping&&ic.ic_inact.pending);cases++;
/* Actualrestorecallback values are unsignedsaved bytes exceptchannel482. */
clear();ic.om_vopt.effective=1;ic.om_vopt.policy=0;ic.om_vopt.saved_txbf=255;ic.om_vopt.saved_probe=128;ic.om_vopt.saved_pppc=255;ic.om_vopt.saved_airfair=255;ic.om_vopt.saved_scs=3;om_ieee80211_vopt_queue(&ic);run();assert(param_values[78]==255&&param_values[195]==128&&param_values[158]==255&&param_values[288]==255&&param_values[482]==-127&&param_values[147]==0x10003&&reassocs==1);cases++;
/* Errorsotherthanunknown482 are faithfullyignored thenreassociation occurs. */
clear();ic.om_vopt.policy=1;rc482=-EIO;scs_rc=ocac_rc=-EIO;om_ieee80211_vopt_queue(&ic);run();assert(params[482]==1&&params[414]==1&&reassocs==1&&!ic.om_vopt_stopping&&!ic.om_vopt_last_error);cases++;
/* Global backoff is shared betweentwoICs, notreset bysecondICconstructor. */
clear();ic.om_vopt.policy=2;ic.om_vopt.effective=1;peer.ni_qtn_flags=0;om_ieee80211_vopt_queue(&ic);run();assert(om_vopt_global_backoff==10&&ic.om_vopt.effective==0);struct ieee80211com second=ic;struct ieee80211vap secondvap=vap;struct ieee80211_node secondpeer=peer,secondself=self;secondvap.iv_ic=&second;secondvap.iv_bss=&secondself;secondpeer.ni_vap=secondself.ni_vap=&secondvap;secondpeer.ni_qtn_flags=0x800;second.ic_vaps=&secondvap;second.ic_sta.nodes[0]=&secondself;second.ic_sta.nodes[1]=&secondpeer;om_ieee80211_vopt_initialize(&second);second.om_vopt.policy=2;assert(om_vopt_global_backoff==10);om_ieee80211_vopt_queue(&second);second.om_vopt_work.queued=0;second.om_vopt_work.fn(&second.om_vopt_work);assert(om_vopt_global_backoff==9&&second.om_vopt.effective==0);cases++;
clear();assert(om_vopt_kernel_write_ready(&ic,78)==-EBUSY);rtlock=1;assert(!om_vopt_kernel_write_ready(&ic,78));ic.om_vopt_running=true;assert(om_vopt_kernel_write_ready(&ic,78)==-EBUSY);assert(!om_vopt_kernel_write_ready(&ic,999));rtlock=0;cases++;
/* OEM481 admission/getter belongs to radioICmode, even mixedVAPmode. */
for(unsigned radio=0;radio<16;radio++)for(unsigned vmode=0;vmode<16;vmode++){clear();ic.ic_opmode=radio;vap.iv_opmode=vmode;value=1;int r=om_vopt_kernel_policy(&vap,&value,1);assert(r==(radio==6?0:-95));ic.om_vopt.policy=2;ic.om_vopt.effective=1;value=-1;assert(!om_vopt_kernel_policy(&vap,&value,0)&&value==(radio==6?33:0));cases++;}
printf("{\"cases\":%u,\"actualworker_queue_policy_quiesce_resume\":true,\"unknown482_stops414_reassoc\":true,\"allcount_includingnonblacklist\":true,\"RTNLtrylock_nojoin_deadlock\":true}\n",cases);return 0;}
