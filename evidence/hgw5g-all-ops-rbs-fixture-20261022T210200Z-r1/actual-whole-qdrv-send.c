#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "openmitrastar_rbsdfs_recovered.h"
typedef uint32_t u32;typedef uint8_t u8;typedef int spinlock_t;
#define EINVAL 22
#define EBUSY 16
#define EAGAIN 11
#define EOPNOTSUPP 95
#define EACCES 13
#define HZ 200
#define EXPORT_SYMBOL(x)
#define IEEE80211_M_HOSTAP 6
#define IEEE80211_S_RUN 4
#define IEEE80211_CHAN_DFS_CAC_IN_PROGRESS 0x1000000U
#define IEEE80211_CHAN_ANYC ((struct ieee80211_channel*)(uintptr_t)0xffff)
#define container_of(p,t,m) ((t*)((char*)(p)-offsetof(t,m)))
#define from_timer(v,t,m) container_of(t,struct om_rbsdfs_kernel_state,m)
#define time_before(a,b) ((int32_t)((uint32_t)(a)-(uint32_t)(b))<0)
#define time_after_eq(a,b) (!time_before(a,b))
static unsigned locks,arms,joins;static uint32_t jiffies;static unsigned expiration_order;
struct timer_list {void(*fn)(struct timer_list*);uint32_t expires;unsigned pending;};
#define spin_lock_init(p) (*(p)=0)
#define spin_lock_irqsave(p,f) do{assert(!locks);*(p)=1;locks++;f=0;}while(0)
#define spin_unlock_irqrestore(p,f) do{assert(locks==1);*(p)=0;locks--;(void)f;}while(0)
static void timer_setup(struct timer_list*t,void(*fn)(struct timer_list*),int f){memset(t,0,sizeof(*t));t->fn=fn;}
static int mod_timer(struct timer_list*t,unsigned long expiry){t->expires=(u32)expiry;t->pending=1;arms++;return 0;}
static int del_timer_sync(struct timer_list*t){assert(!locks);t->pending=0;joins++;return 0;}
struct ieee80211com;struct ieee80211vap;struct ieee80211_node;struct sk_buff;
struct om_rbsdfs_kernel_provider {
    struct om_rbsdfs_frame_hooks frame;
    int (*open_window)(void *,unsigned role,u32 channel,u32 deadline);
    int (*close_window)(void *,u32 channel);
};
struct om_rbsdfs_kernel_state {
    struct om_rbsdfs_state core;
    struct om_rbsdfs_frame_state frame;
    struct timer_list timer;
    spinlock_t lock;
    const struct om_rbsdfs_kernel_provider *provider;
    struct ieee80211_node *retained_node;
    u32 channel,deadline;
    u8 initialized,stopping,window_active,quarantined;
};
int om_qdrv_rbsdfs_claim_lifetime(const char *operation);
void om_ieee80211_rbsdfs_init(struct ieee80211com *ic);
int om_ieee80211_rbsdfs_stop(struct ieee80211com *ic);
int om_ieee80211_rbsdfs_resume(struct ieee80211com *ic);
int om_ieee80211_rbsdfs_policy(struct ieee80211com *ic,unsigned op,u32 *value,int write);
int om_ieee80211_rbsdfs_radar(struct ieee80211com *ic,int during_cac,u32 channel);
int om_ieee80211_rbsdfs_mbs(struct ieee80211vap *vap,int during_cac,u32 channel);
/* 0=ordinary unchanged enqueue, 1=provider consumed frame+node,
 * 2=provider publication unknown retained by state, negative=drop both. */
int om_ieee80211_rbsdfs_send(struct ieee80211com *ic,struct ieee80211_node *ni,
    struct sk_buff *skb,int is_mgmt);


struct ieee80211_channel {u32 ic_flags;u8 ic_ieee;};
struct ieee80211_node {unsigned refs;struct ieee80211vap*ni_vap;unsigned ni_node_idx;};
struct net_device {unsigned dummy;};
struct sk_buff {unsigned live;struct net_device*dev;unsigned dest_port,priority;struct{void*ni;unsigned encap,flags;}cb;};
struct ieee80211vap {struct ieee80211com*iv_ic;int iv_opmode,iv_state;};
struct ieee80211com {struct om_rbsdfs_kernel_state om_rbsdfs;unsigned ic_extender_role;struct{unsigned sta_dfs_strict_mode,allow_measurement_report;}sta_dfs_info;
 struct ieee80211_channel chan,*ic_curchan;
 int(*ic_chan_compare_equality)(struct ieee80211com*,struct ieee80211_channel*,struct ieee80211_channel*);
 int(*ic_get_chan_availability_status_by_chan_num)(struct ieee80211com*,struct ieee80211_channel*);};
static unsigned claims,node_frees,skb_frees,queued;static int claim_result;
int om_qdrv_rbsdfs_claim_lifetime(const char*name){assert(locks==1);claims++;return claim_result;}
static void ieee80211_free_node(struct ieee80211_node*ni){assert(!locks&&ni->refs);ni->refs--;node_frees++;}
static unsigned finds,equalities,statusreads;static int equalresult=1,statusfirst=4,statussecond=8,wds_rbs;
static struct ieee80211_channel*ieee80211_find_channel_by_ieee(struct ieee80211com*ic,u32 c){finds++;return c==ic->chan.ic_ieee?&ic->chan:NULL;}
static int equal_current(struct ieee80211com*ic,struct ieee80211_channel*a,struct ieee80211_channel*b){assert(locks&&b==&ic->chan);equalities++;return equalresult;}
static int channel_status(struct ieee80211com*ic,struct ieee80211_channel*c){assert(locks&&c==ic->ic_curchan);return statusreads++?statussecond:statusfirst;}
static int ieee80211_scs_is_wds_rbs_node(struct ieee80211com*ic){return wds_rbs;}
struct fake {unsigned opened,closed,validated,granted,sent,revoked;int open,close,valid,grant,send,revoke;unsigned role;u32 channel,deadline;};
static struct fake fake;
static int open_window(void*x,unsigned role,u32 c,u32 d){struct fake*f=x;assert(locks);assert(claims);f->opened++;f->role=role;f->channel=c;f->deadline=d;return f->open;}
static int close_window(void*x,u32 c){struct fake*f=x;assert(locks);f->closed++;assert(c==f->channel);return f->close;}
static int validate_frame(void*x,const void*frame,unsigned role,u32 c){struct fake*f=x;assert(locks&&frame);f->validated++;assert(role==f->role&&c==f->channel);return f->valid;}
static int grant_frame(void*x,const void*frame){struct fake*f=x;assert(locks&&frame);assert(claims>=2);f->granted++;return f->grant;}
static int send_frame(void*x,const void*frame){struct fake*f=x;struct sk_buff*skb=(void*)frame;assert(locks&&frame);f->sent++;if(f->send==1){assert(skb->live);skb->live=0;skb_frees++;}return f->send;}
static int revoke_frame(void*x,const void*frame){struct fake*f=x;assert(locks&&frame);f->revoked++;return f->revoke;}
static const struct om_rbsdfs_kernel_provider provider={{&fake,validate_frame,grant_frame,send_frame,revoke_frame},open_window,close_window};
static void setup(struct ieee80211com*ic){claims=node_frees=skb_frees=queued=0;claim_result=0;memset(ic,0,sizeof(*ic));om_ieee80211_rbsdfs_init(ic);ic->chan.ic_ieee=52;ic->ic_curchan=&ic->chan;ic->ic_chan_compare_equality=equal_current;ic->ic_get_chan_availability_status_by_chan_num=channel_status;ic->ic_extender_role=2;memset(&fake,0,sizeof(fake));fake.open=fake.close=fake.valid=fake.grant=fake.send=fake.revoke=1;finds=equalities=statusreads=arms=joins=0;equalresult=1;statusfirst=4;statussecond=8;wds_rbs=0;}
#include "openmitrastar_rbsdfs_recovered.h"
void om_rbsdfs_init(struct om_rbsdfs_state *s)
{ if(s){s->measurement_in_cac=0;s->allow_rbs_report=0;s->allow_mbs_csa=0;s->end_tx_ms=500;} }
int om_rbsdfs_set466(struct om_rbsdfs_state *s,omrbs_u32 v)
{ if(!s)return -22;s->measurement_in_cac=v!=0;return 0; }
int om_rbsdfs_get466(const struct om_rbsdfs_state *s,omrbs_u32 *v)
{ if(!s||!v)return -22;*v=s->measurement_in_cac;return 0; }
int om_rbsdfs_set468(struct om_rbsdfs_state *s,omrbs_u32 v)
{ if(!s||(omrbs_u32)(v-1)>999U)return -22;s->end_tx_ms=v;return 0; }
int om_rbsdfs_get468(const struct om_rbsdfs_state *s,omrbs_u32 *v)
{ if(!s||!v)return -22;*v=s->end_tx_ms;return 0; }
int om_rbsdfs_oem_radar_window(struct om_rbsdfs_state *s,unsigned role,
    int during_cac,omrbs_u32 channel,omrbs_u32 now,omrbs_u32 hz,
    const struct om_rbsdfs_oem_hooks *h)
{
    omrbs_u32 ticks;
    if(!s||!h)return -22;
    if(role!=2||!during_cac||!s->measurement_in_cac)return 0;
    if(!h->enable_tx||!h->arm_timer||!hz||hz>0xffffffffU/1000U||
        (omrbs_u32)(s->end_tx_ms-1)>999U)return -22;
    /* Actual OEM assigns allowance then enables TX then arms its close timer.
     * floor(ms*HZ/1000), unsigned wrapping deadline, HZ200 observed OEM build. */
    s->allow_rbs_report=1;h->enable_tx(h->context);
    ticks=s->end_tx_ms*hz/1000U;
    h->arm_timer(h->context,channel,now+ticks);return 1;
}
int om_rbsdfs_oem_mbs_window(struct om_rbsdfs_state *s,int wds_is_rbs,
    int vap_hostap_run,int during_cac,const struct om_rbsdfs_oem_hooks *h)
{
    if(!s||!h)return -22;
    if(wds_is_rbs||!vap_hostap_run||!during_cac||!s->measurement_in_cac)return 0;
    if(!h->enable_tx)return -22;
    s->allow_mbs_csa=1;h->enable_tx(h->context);return 1;
}
int om_rbsdfs_oem_send_permitted(struct om_rbsdfs_state *s,unsigned role,
    const struct om_rbsdfs_oem_hooks *h)
{
    omrbs_u8 *allow;
    unsigned status;
    if(!s||!h)return -22;
    if(!s->measurement_in_cac||(role!=1&&role!=2))return 1;
    allow=role==2?&s->allow_rbs_report:&s->allow_mbs_csa;
    if(*allow){*allow=0;return 1;}
    if(!h->channel_status)return -22;
    /* OEM calls status twice along the short-circuit branch. */
    status=h->channel_status(h->context);if(status==4)return 0;
    status=h->channel_status(h->context);return status!=8;
}
int om_rbsdfs_oem_expire(omrbs_u32 channel,const struct om_rbsdfs_oem_hooks *h)
{
    void *found;
    if(!h||!h->find_channel||!h->equal_current||!h->disable_tx)return -22;
    found=h->find_channel(h->context,channel);
    if(!found)return -22; /* Adapter input guard, not an OEM null-proof claim. */
    if(h->equal_current(h->context,found)){h->disable_tx(h->context);return 1;}
    return 0;
}
enum om_rbsdfs_frame_result om_rbsdfs_exact_frame_window(struct om_rbsdfs_frame_state *s,
    const void *frame,unsigned role,omrbs_u32 channel,omrbs_u32 deadline,
    const struct om_rbsdfs_frame_hooks *h)
{
    int result;
    if(!s||!frame||!h||!h->validate_measurement||!h->grant_frame||
        !h->send_frame||!h->revoke_frame||s->active||s->quarantined||
        s->generation==0xffffffffU||(role!=1&&role!=2))return OMRBS_REFUSED;
    if(h->validate_measurement(h->context,frame,role,channel)!=1)return OMRBS_REFUSED;
    /* Caller serializes and pins state/frame/provider before this admission.
     * No module integration exists until provider lifetime/pins are proven. */
    s->generation++;s->frame=frame;s->channel=channel;s->deadline=deadline;s->active=1;
    result=h->grant_frame(h->context,frame);
    if(result==0){s->active=0;s->frame=0;return OMRBS_REFUSED;}
    if(result!=1){s->quarantined=1;return OMRBS_PUBLISHED_UNKNOWN;}
    result=h->send_frame(h->context,frame);
    if(result!=0&&result!=1){
        (void)h->revoke_frame(h->context,frame);s->quarantined=1;
        return OMRBS_PUBLISHED_UNKNOWN;
    }
    if(h->revoke_frame(h->context,frame)!=1){s->quarantined=1;return OMRBS_PUBLISHED_UNKNOWN;}
    s->active=0;s->frame=0;
    return result==1?OMRBS_COMPLETE:OMRBS_REFUSED;
}

/* Linux bridge: exact OEM core is included separately without modifications.
 * The adapter adds lifetime/serialization/refusal; it never provides raw TX.
 * Qualification stays blocked until a real immutable exact-frame provider and
 * its asynchronous ownership/completion/revoke contract are recovered.
 */
struct om_rbsdfs_kernel_call {
    struct ieee80211com *ic;
    unsigned role;
    u32 channel,deadline;
    int result;
};
static int om_rbsdfs_kernel_ready(struct om_rbsdfs_kernel_state *s)
{
    const struct om_rbsdfs_kernel_provider *p=s->provider;
    return p&&p->open_window&&p->close_window&&p->frame.validate_measurement&&
        p->frame.grant_frame&&p->frame.send_frame&&p->frame.revoke_frame;
}
static void om_rbsdfs_kernel_open(void *arg)
{
    struct om_rbsdfs_kernel_call *c=arg;
    struct om_rbsdfs_kernel_state *s=&c->ic->om_rbsdfs;
    c->result=s->provider->open_window(s->provider->frame.context,c->role,c->channel,c->deadline);
    if(c->result==1){s->window_active=1;s->channel=c->channel;s->deadline=c->deadline;}
    else if(c->result!=0)s->quarantined=1;
}
static void om_rbsdfs_kernel_arm(void *arg,u32 channel,u32 deadline)
{
    struct om_rbsdfs_kernel_call *c=arg;
    struct om_rbsdfs_kernel_state *s=&c->ic->om_rbsdfs;
    if(c->result==1){s->channel=channel;s->deadline=deadline;mod_timer(&s->timer,(unsigned long)deadline);}
}
static void *om_rbsdfs_kernel_find(void *arg,u32 channel)
{
    struct om_rbsdfs_kernel_call *c=arg;
    return ieee80211_find_channel_by_ieee(c->ic,channel);
}
static int om_rbsdfs_kernel_equal(void *arg,void *channel)
{
    struct om_rbsdfs_kernel_call *c=arg;
    struct ieee80211com *ic=c->ic;
    return ic->ic_curchan&&ic->ic_curchan!=IEEE80211_CHAN_ANYC&&
        ic->ic_chan_compare_equality&&ic->ic_chan_compare_equality(ic,ic->ic_curchan,channel);
}
static void om_rbsdfs_kernel_close(void *arg)
{
    struct om_rbsdfs_kernel_call *c=arg;
    struct om_rbsdfs_kernel_state *s=&c->ic->om_rbsdfs;
    c->result=s->provider->close_window(s->provider->frame.context,c->channel);
    if(c->result==1)s->window_active=0;else s->quarantined=1;
}
static unsigned om_rbsdfs_kernel_status(void *arg)
{
    struct om_rbsdfs_kernel_call *c=arg;
    struct ieee80211com *ic=c->ic;
    if(!ic->ic_curchan||ic->ic_curchan==IEEE80211_CHAN_ANYC||
       !ic->ic_get_chan_availability_status_by_chan_num)return 8;
    return ic->ic_get_chan_availability_status_by_chan_num(ic,ic->ic_curchan);
}
static struct om_rbsdfs_oem_hooks om_rbsdfs_kernel_hooks(struct om_rbsdfs_kernel_call *c)
{
    struct om_rbsdfs_oem_hooks h={c,om_rbsdfs_kernel_open,om_rbsdfs_kernel_close,
        om_rbsdfs_kernel_arm,om_rbsdfs_kernel_find,om_rbsdfs_kernel_equal,om_rbsdfs_kernel_status};
    return h;
}
static void om_rbsdfs_kernel_timer(struct timer_list *timer)
{
    struct om_rbsdfs_kernel_state *s=from_timer(s,timer,timer);
    struct ieee80211com *ic=container_of(s,struct ieee80211com,om_rbsdfs);
    struct om_rbsdfs_kernel_call c={ic,0,0,0,0};
    struct om_rbsdfs_oem_hooks h=om_rbsdfs_kernel_hooks(&c);
    unsigned long flags;
    spin_lock_irqsave(&s->lock,flags);
    if(!s->initialized||s->stopping||!s->window_active||!om_rbsdfs_kernel_ready(s))goto out;
    if(time_before(jiffies,(unsigned long)s->deadline)){mod_timer(&s->timer,(unsigned long)s->deadline);goto out;}
    c.channel=s->channel;
    /* Faithful expiry finds stored channel and disables only if current equal.
     * close_window never changes radio TX/CAC/radar; stale-channel bookkeeping
     * is then revoked by this narrower adapter even when OEM returned zero. */
    if(om_rbsdfs_oem_expire(s->channel,&h)==0)om_rbsdfs_kernel_close(&c);
    if(c.result!=1)s->quarantined=1;
out:spin_unlock_irqrestore(&s->lock,flags);
}
void om_ieee80211_rbsdfs_init(struct ieee80211com *ic)
{
    struct om_rbsdfs_kernel_state *s=&ic->om_rbsdfs;
    memset(s,0,sizeof(*s));spin_lock_init(&s->lock);om_rbsdfs_init(&s->core);
    timer_setup(&s->timer,om_rbsdfs_kernel_timer,0);
    s->provider=NULL;s->initialized=1;
}
EXPORT_SYMBOL(om_ieee80211_rbsdfs_init);
int om_ieee80211_rbsdfs_stop(struct ieee80211com *ic)
{
    struct om_rbsdfs_kernel_state *s=&ic->om_rbsdfs;
    unsigned long flags;int result=0;
    spin_lock_irqsave(&s->lock,flags);
    if(!s->initialized){spin_unlock_irqrestore(&s->lock,flags);return 0;}
    s->stopping=1;
    spin_unlock_irqrestore(&s->lock,flags);
    /* Join timer after producer stop, never while holding callback lock. */
    del_timer_sync(&s->timer);
    spin_lock_irqsave(&s->lock,flags);
    if(s->quarantined||s->frame.quarantined||s->frame.active)result=-EBUSY;
    else if(s->window_active){
        if(!om_rbsdfs_kernel_ready(s)||s->provider->close_window(s->provider->frame.context,s->channel)!=1){s->quarantined=1;result=-EBUSY;}
        else s->window_active=0;
    }
    if(!result){s->core.allow_rbs_report=0;s->core.allow_mbs_csa=0;}
    spin_unlock_irqrestore(&s->lock,flags);return result;
}
EXPORT_SYMBOL(om_ieee80211_rbsdfs_stop);
int om_ieee80211_rbsdfs_resume(struct ieee80211com *ic)
{
    struct om_rbsdfs_kernel_state *s=&ic->om_rbsdfs;unsigned long flags;int result=0;
    spin_lock_irqsave(&s->lock,flags);
    if(!s->initialized)result=-EAGAIN;
    else if(s->quarantined||s->frame.quarantined||s->frame.active||s->window_active)result=-EBUSY;
    else{s->stopping=0;s->core.allow_rbs_report=0;s->core.allow_mbs_csa=0;}
    spin_unlock_irqrestore(&s->lock,flags);return result;
}
EXPORT_SYMBOL(om_ieee80211_rbsdfs_resume);
int om_ieee80211_rbsdfs_policy(struct ieee80211com *ic,unsigned op,u32 *v,int write)
{
    struct om_rbsdfs_kernel_state *s=&ic->om_rbsdfs;unsigned long flags;int result;
    if(!v)return -EINVAL;
    spin_lock_irqsave(&s->lock,flags);
    if(!s->initialized)result=-EAGAIN;
    else if(s->stopping||s->quarantined)result=-EBUSY;
    else if(op==466)result=write?om_rbsdfs_set466(&s->core,*v):om_rbsdfs_get466(&s->core,v);
    else if(op==468)result=write?om_rbsdfs_set468(&s->core,*v):om_rbsdfs_get468(&s->core,v);
    else result=-EINVAL;
    spin_unlock_irqrestore(&s->lock,flags);return result;
}
EXPORT_SYMBOL(om_ieee80211_rbsdfs_policy);
static int om_rbsdfs_kernel_producer(struct ieee80211com *ic,unsigned role,int wds_is_rbs,
    int vap_hostap_run,int during_cac,u32 channel)
{
    struct om_rbsdfs_kernel_state *s=&ic->om_rbsdfs;
    struct om_rbsdfs_kernel_call c={ic,role,channel,0,0};
    struct om_rbsdfs_oem_hooks h=om_rbsdfs_kernel_hooks(&c);
    struct om_rbsdfs_state prior;unsigned long flags;int result;
    spin_lock_irqsave(&s->lock,flags);
    if(!s->initialized||s->stopping||s->quarantined||s->frame.active){result=-EBUSY;goto out;}
    if(!during_cac||!s->core.measurement_in_cac||(role==2&&ic->ic_extender_role!=2)||
        (role==1&&(wds_is_rbs||!vap_hostap_run))){result=0;goto out;}
    /* Required callback absent is an explicit missing-provider refusal BEFORE
     * the faithful core assigns allowance or requests temporary TX enable. */
    if(!om_rbsdfs_kernel_ready(s)){result=-EOPNOTSUPP;goto out;}
    if(s->window_active){result=-EBUSY;goto out;}
    result=om_qdrv_rbsdfs_claim_lifetime("rbsdfs-window");
    if(result)goto out;
    prior=s->core;c.deadline=(u32)jiffies+s->core.end_tx_ms*HZ/1000U;
    result=role==2?om_rbsdfs_oem_radar_window(&s->core,ic->ic_extender_role,1,channel,(u32)jiffies,HZ,&h):
        om_rbsdfs_oem_mbs_window(&s->core,wds_is_rbs,vap_hostap_run,1,&h);
    if(result!=1||c.result!=1){s->core=prior;result=c.result==0?-EOPNOTSUPP:-EBUSY;}
    else if(role==1){
        /* OEM MBS branch has no expiry timer. Adapter additionally bounds its
         * frame-only allowance to end_tx_ms; never presents this as OEMequal. */
        mod_timer(&s->timer,(unsigned long)c.deadline);
    }
out:spin_unlock_irqrestore(&s->lock,flags);return result;
}
int om_ieee80211_rbsdfs_radar(struct ieee80211com *ic,int cac,u32 channel)
{return om_rbsdfs_kernel_producer(ic,2,0,0,cac,channel);}
EXPORT_SYMBOL(om_ieee80211_rbsdfs_radar);
int om_ieee80211_rbsdfs_mbs(struct ieee80211vap *vap,int cac,u32 channel)
{return om_rbsdfs_kernel_producer(vap->iv_ic,1,ieee80211_scs_is_wds_rbs_node(vap->iv_ic),
    vap->iv_opmode==IEEE80211_M_HOSTAP&&vap->iv_state==IEEE80211_S_RUN,cac,channel);}
EXPORT_SYMBOL(om_ieee80211_rbsdfs_mbs);
int om_ieee80211_rbsdfs_send(struct ieee80211com *ic,struct ieee80211_node *ni,
    struct sk_buff *skb,int is_mgmt)
{
    struct om_rbsdfs_kernel_state *s=&ic->om_rbsdfs;
    struct om_rbsdfs_kernel_call c={ic,ic->ic_extender_role,0,0,0};
    struct om_rbsdfs_oem_hooks h=om_rbsdfs_kernel_hooks(&c);
    struct om_rbsdfs_state prior;unsigned long flags;int result;unsigned allowance;
    spin_lock_irqsave(&s->lock,flags);
    if(!s->initialized||s->stopping||s->quarantined||s->frame.active){result=-EBUSY;goto out;}
    if(!s->core.measurement_in_cac||(c.role!=1&&c.role!=2)){result=0;goto out;}
    prior=s->core;allowance=c.role==2?s->core.allow_rbs_report:s->core.allow_mbs_csa;
    result=om_rbsdfs_oem_send_permitted(&s->core,c.role,&h);
    if(result!=1){result=-EACCES;goto out;}
    if(!allowance){
        /* Retain ordinary available-channel queue. During CAC require the
         * missing exact-frame provider even if status scalar says available. */
        result=ic->ic_curchan&&ic->ic_curchan!=IEEE80211_CHAN_ANYC&&
            !(ic->ic_curchan->ic_flags&IEEE80211_CHAN_DFS_CAC_IN_PROGRESS)?0:-EACCES;
        goto out;
    }
    if(!is_mgmt||!skb||!ni||!s->window_active||!om_rbsdfs_kernel_ready(s)||
       time_after_eq(jiffies,(unsigned long)s->deadline)){
        s->core=prior;result=-EOPNOTSUPP;goto out;
    }
    /* skb pointer identity is retained by core on uncertain publication. Node
     * lifetime is held alongside it; caller must not release either in case2. */
    result=om_qdrv_rbsdfs_claim_lifetime("rbsdfs-frame");
    if(result){s->core=prior;goto out;}
    result=om_rbsdfs_exact_frame_window(&s->frame,skb,c.role,s->channel,s->deadline,&s->provider->frame);
    if(result==OMRBS_PUBLISHED_UNKNOWN){s->retained_node=ni;s->quarantined=1;result=2;}
    else if(result==OMRBS_COMPLETE)result=1;
    else{result=-EOPNOTSUPP;}
out:spin_unlock_irqrestore(&s->lock,flags);
    /* Provider consumes skb only. Confirmed completion releases this caller
     * node reference once outside the IRQ lock; unknown keeps it pinned. */
    if(result==1)ieee80211_free_node(ni);
    return result;
}
EXPORT_SYMBOL(om_ieee80211_rbsdfs_send);


struct qdrv_vap {struct ieee80211vap iv;struct net_device*ndev;};
#define QTN_SKB_CB_NI(s) ((s)->cb.ni)
#define QTN_SKB_ENCAP(s) ((s)->cb.encap)
#define QTN_SKB_ENCAP_80211_MGMT 1
#define QTN_SKB_ENCAP_80211_DATA 2
#define M_NO_AMSDU 0x400
#define false 0
#define M_FLAG_SET(s,f) ((s)->cb.flags|=(f))
static int ieee80211_is_chan_radar_detected(struct ieee80211_channel*c){return c&&c->ic_flags&0x1000;}
static int ieee80211_is_chan_cac_required(struct ieee80211_channel*c){return c&&c->ic_flags&0x800000;}
static void dev_kfree_skb_any(struct sk_buff*s){assert(!locks&&s->live);s->live=0;skb_frees++;}
static int dev_queue_xmit(struct sk_buff*s){assert(!locks&&s->live);s->live=0;skb_frees++;queued++;return 0;}

static int qdrv_wlan_80211_send(struct ieee80211com *ic, struct ieee80211_node *ni,
				struct sk_buff *skb, uint32_t priority, uint8_t is_mgmt)
{
	struct net_device *vdev;
	struct qdrv_vap *qv;
	int om_rbs_result;

	if (ic->sta_dfs_info.sta_dfs_strict_mode) {
		if (ic->sta_dfs_info.allow_measurement_report) {
			ic->sta_dfs_info.allow_measurement_report = false;
		} else if ((ieee80211_is_chan_radar_detected(ic->ic_curchan)) ||
				(ieee80211_is_chan_cac_required(ic->ic_curchan))) {
			ieee80211_free_node(ni);
			return 0;
		}
	}

	qv = container_of(ni->ni_vap, struct qdrv_vap, iv);
	vdev = qv->ndev;

	skb->dev = vdev;
	skb->dest_port = ni->ni_node_idx;
	skb->priority = priority;

	QTN_SKB_CB_NI(skb) = ni;

	if (is_mgmt)
		QTN_SKB_ENCAP(skb) = QTN_SKB_ENCAP_80211_MGMT;
	else
		QTN_SKB_ENCAP(skb) = QTN_SKB_ENCAP_80211_DATA;

	M_FLAG_SET(skb, M_NO_AMSDU);

	/* Missing exact-frame provider never enables radio-wide TX. */
	om_rbs_result=om_ieee80211_rbsdfs_send(ic,ni,skb,is_mgmt);
	if(om_rbs_result>0)return 0; /* bridge released node on completion, retained it on unknown */
	if(om_rbs_result<0){dev_kfree_skb_any(skb);ieee80211_free_node(ni);return 0;}
	dev_queue_xmit(skb);

	ieee80211_free_node(ni);

	return 0;
}

int main(void){struct ieee80211com ic;struct qdrv_vap qv;struct net_device dev;struct ieee80211_node ni;struct sk_buff skb;unsigned cases=0;
for(unsigned mode=0;mode<6;mode++){
setup(&ic);memset(&qv,0,sizeof(qv));memset(&ni,0,sizeof(ni));memset(&skb,0,sizeof(skb));qv.iv.iv_ic=&ic;qv.iv.iv_opmode=6;qv.iv.iv_state=4;qv.ndev=&dev;ni.refs=1;ni.ni_vap=&qv.iv;ni.ni_node_idx=7;skb.live=1;jiffies=100;
if(mode){ic.om_rbsdfs.core.measurement_in_cac=1;if(mode!=1){ic.om_rbsdfs.provider=&provider;assert(om_ieee80211_rbsdfs_radar(&ic,1,52)==1);} }
if(mode==3)fake.grant=2;
if(mode==4)fake.revoke=2;
if(mode==5)fake.grant=0;
assert(qdrv_wlan_80211_send(&ic,&ni,&skb,3,1)==0);assert(skb.cb.ni==&ni&&skb.cb.encap==1&&skb.cb.flags==0x400&&skb.dest_port==7&&skb.priority==3);
if(mode==0)assert(queued==1&&node_frees==1&&skb_frees==1&&ni.refs==0&&!skb.live&&!claims);
if(mode==1)assert(!queued&&node_frees==1&&skb_frees==1&&ni.refs==0&&!skb.live&&!claims&&!fake.granted);
if(mode==2)assert(!queued&&node_frees==1&&skb_frees==1&&ni.refs==0&&!skb.live&&claims==2&&fake.sent==1&&fake.revoked==1);
if(mode==3)assert(!queued&&!node_frees&&!skb_frees&&ni.refs==1&&skb.live&&claims==2&&fake.granted==1&&!fake.sent&&ic.om_rbsdfs.retained_node==&ni&&ic.om_rbsdfs.frame.frame==&skb);
if(mode==4)assert(!queued&&!node_frees&&skb_frees==1&&ni.refs==1&&!skb.live&&claims==2&&fake.sent==1&&ic.om_rbsdfs.retained_node==&ni);
if(mode==5)assert(!queued&&node_frees==1&&skb_frees==1&&ni.refs==0&&!skb.live&&claims==2&&!fake.sent);
if(mode==3||mode==4)assert(om_ieee80211_rbsdfs_stop(&ic)==-16&&om_ieee80211_rbsdfs_resume(&ic)==-16);
cases++;}
printf("{\"cases\":%u,\"actual_qdrv_send_body\":true,\"success_node_free_once\":true,\"provider_consumes_skb_only\":true,\"unknown_frame_node_retained\":true,\"claim_before_each_publication\":true}\n",cases);return 0;}
