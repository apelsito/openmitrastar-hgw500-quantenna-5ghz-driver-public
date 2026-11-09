
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
