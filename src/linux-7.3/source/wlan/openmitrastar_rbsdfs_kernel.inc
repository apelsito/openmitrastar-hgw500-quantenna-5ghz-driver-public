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
