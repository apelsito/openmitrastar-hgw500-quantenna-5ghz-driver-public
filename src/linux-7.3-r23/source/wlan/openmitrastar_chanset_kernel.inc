
/* Actual scanner-owned adapter for OEM channel-set selection. The existing
 * validated channel tables supply regulatory/DFS state; templates grant no TX.
 * Shared scan counters are sampled scalars, not a coherent cross-MuC snapshot.
 * No allocation or sleeping operation occurs in scan completion/timer context.
 */
int om_chanset_kernel_allowed(struct ieee80211vap *vap)
{
    struct ieee80211com *ic;
    unsigned mode;
    if(!vap||vap->iv_opmode!=IEEE80211_M_HOSTAP)return 0;
    ic=vap->iv_ic;mode=ic->ic_phymode;
    return mode==IEEE80211_MODE_11B||mode==IEEE80211_MODE_11G||
        mode==IEEE80211_MODE_11NG||mode==IEEE80211_MODE_11NG_HT40PM;
}
int om_chanset_kernel_policy(struct ieee80211com *ic,int *value,int write)
{
    struct scan_state *scan;
    unsigned long flags;
    if(!ic||!value||!ic->ic_scan)return -EAGAIN;
    scan=SCAN_PRIVATE(ic->ic_scan);
    spin_lock_irqsave(&scan->om_chanset_lock,flags);
    if(scan->om_chanset_stopping){spin_unlock_irqrestore(&scan->om_chanset_lock,flags);return -EBUSY;}
    if(write)scan->om_chanset_policy=(u8)*value;
    else *value=scan->om_chanset_policy;
    spin_unlock_irqrestore(&scan->om_chanset_lock,flags);
    return 0;
}
int om_chanset_kernel_start(struct ieee80211vap *vap,int flags,
    unsigned duration,unsigned nssid,const struct ieee80211_scan_ssid *ssids)
{
    struct ieee80211com *ic;
    struct scan_state *scan;
    unsigned long irq,ic_irq;
    unsigned generation;
    int result;
    if(!vap||!om_chanset_kernel_allowed(vap))return -EOPNOTSUPP;
    ic=vap->iv_ic;
    if(!ic->ic_scan)return -EAGAIN;
    scan=SCAN_PRIVATE(ic->ic_scan);
    spin_lock_irqsave(&ic->ic_comlock,ic_irq);
    spin_lock_irqsave(&scan->om_chanset_lock,irq);
    if(scan->om_chanset_stopping||scan->om_scan_detaching||scan->om_chanset.busy||
       (ic->ic_flags&IEEE80211_F_SCAN)){
        spin_unlock_irqrestore(&scan->om_chanset_lock,irq);
        spin_unlock_irqrestore(&ic->ic_comlock,ic_irq);return -EBUSY;
    }
    result=om_chanset_prepare(&scan->om_chanset,OM_CHANSET_BAND24,
        ieee80211_get_bw(ic),ic->ic_country_code!=CTRY_DEFAULT,scan->om_chanset_policy);
    generation=scan->om_chanset.generation;
    if(!result){
        scan->om_chanset_owner=vap;
        scan->om_chanset_last_error=0;
        scan->om_chanset.initial_flags=(unsigned)flags;
    }
    spin_unlock_irqrestore(&scan->om_chanset_lock,irq);
    spin_unlock_irqrestore(&ic->ic_comlock,ic_irq);
    if(result)return result;
    /* Existing VAP caller holds its lifetime serialization. If startup refuses,
     * clear only the exact generation we reserved, never a later transaction. */
    result=ieee80211_start_scan(vap,flags&~IEEE80211_SCAN_USECACHE,duration,nssid,ssids);
    if(!result){
        spin_lock_irqsave(&ic->ic_comlock,ic_irq);
        spin_lock_irqsave(&scan->om_chanset_lock,irq);
        if(scan->om_chanset.generation==generation&&scan->om_chanset_owner==vap){
            if(scan->om_chanset.busy)(void)om_chanset_end(&scan->om_chanset);
            scan->om_chanset_owner=NULL;
        }
        spin_unlock_irqrestore(&scan->om_chanset_lock,irq);
        spin_unlock_irqrestore(&ic->ic_comlock,ic_irq);
    }
    return result;
}

unsigned om_chanset_kernel_scan_type(struct ieee80211com *ic)
{
    struct scan_state *scan;
    if(!ic||!ic->ic_scan)return 0;
    scan=SCAN_PRIVATE(ic->ic_scan);
    if(READ_ONCE(scan->om_chanset_stopping)||!READ_ONCE(scan->om_chanset.active))return 0;
    return READ_ONCE(scan->om_chanset.queue.cells[0]);
}
void om_chanset_kernel_cancel(struct ieee80211com *ic)
{
    struct scan_state *scan;
    unsigned long flags;
    if(!ic||!ic->ic_scan)return;
    scan=SCAN_PRIVATE(ic->ic_scan);
    spin_lock_irqsave(&scan->om_chanset_lock,flags);
    if(scan->om_chanset.busy)(void)om_chanset_end(&scan->om_chanset);
    scan->om_chanset_owner=NULL;
    spin_unlock_irqrestore(&scan->om_chanset_lock,flags);
}
int om_chanset_kernel_active(struct ieee80211com *ic)
{
    return ic&&ic->ic_scan&&!READ_ONCE(SCAN_PRIVATE(ic->ic_scan)->om_chanset_stopping)&&
        READ_ONCE(SCAN_PRIVATE(ic->ic_scan)->om_chanset.active);
}
int om_chanset_kernel_finished(struct ieee80211com *ic)
{
    struct scan_state *scan;
    if(!ic||!ic->ic_scan)return 1;
    scan=SCAN_PRIVATE(ic->ic_scan);
    return !READ_ONCE(scan->om_chanset.active)||!READ_ONCE(scan->om_chanset.queue.cells[0]);
}
static int om_cs_kernel_disabled(struct ieee80211com *ic,unsigned channel,unsigned width)
{
    struct ieee80211_channel *chan;
    unsigned sec;
    if(channel>=IEEE80211_CHAN_MAX||isset(ic->ic_chan_disabled,channel))return 1;
    if(width<40)return 0;
    chan=ieee80211_find_channel_by_ieee(ic,channel);
    if(!chan)return 1;
    sec=ieee80211_find_sec_chan(chan);
    return !sec||sec>=IEEE80211_CHAN_MAX||isset(ic->ic_chan_disabled,sec);
}
static int om_cs_kernel_40_permitted(const struct ieee80211_channel *candidate,
    unsigned candidate_offset,unsigned other_primary,unsigned other_offset)
{
    int secondary_frequency,center,first,second=0;
    unsigned primary=candidate->ic_ieee,secondary;
    if(candidate_offset!=1&&candidate_offset!=3)return 0;
    if(primary<1||primary>13||other_primary<1||other_primary>13||other_offset>3||other_offset==2)return 0;
    if((candidate_offset==3&&primary<5)||(candidate_offset==1&&primary>9))return 0;
    if((other_offset==3&&other_primary<5)||(other_offset==1&&other_primary>9))return 0;
    secondary=candidate_offset==1?primary+4:primary-4;
    secondary_frequency=(int)candidate->ic_freq+(candidate_offset==1?20:-20);
    center=((int)candidate->ic_freq+secondary_frequency)/2;
    first=ieee80211_ieee2mhz(other_primary,0);
    if(other_offset)second=ieee80211_ieee2mhz(other_offset==1?other_primary+4:other_primary-4,0);
    if((first>center-25&&first<center+25)||(second>center-25&&second<center+25))
        return primary==other_primary&&(!other_offset||secondary==(other_offset==1?other_primary+4:other_primary-4));
    return 1;
}
struct ieee80211_channel *om_chanset_kernel_pick(struct ieee80211com *ic)
{
    struct scan_state *scan;
    struct ieee80211_scan_state *ss;
    struct shared_params *shared;
    struct qtn_scs_info_set *scs;
    struct ap_state *as;
    struct ap_scan_entry *se;
    struct ieee80211vap *vap;
    struct ieee80211_channel *chan,*chosen=NULL;
    struct om_chanset_transaction *transaction;
    struct om_chanset_view view;
    const struct om_chanset_entry *picked;
    struct om_chanset_scan_sample sample;
    unsigned long flags;
    unsigned i,j,n=0,current_bw,pass_type,observed=0;
    unsigned samples=0;
    int neighborhood,result,index;
    if(!ic||!(ss=ic->ic_scan)||!(vap=ss->ss_vap)||!ss->ss_priv)return NULL;
    scan=SCAN_PRIVATE(ss);transaction=&scan->om_chanset;
    spin_lock_irqsave(&scan->om_chanset_lock,flags);
    if(scan->om_chanset_stopping||!transaction->active||scan->om_chanset_owner!=vap)goto out;
    pass_type=transaction->queue.cells[0];
    shared=qtn_mproc_sync_shared_params_get();
    scs=shared?READ_ONCE(shared->scs_info_lhost):NULL;
    if(!scs){
        scan->om_chanset_last_error=-EAGAIN;
        (void)om_chanset_end(transaction);scan->om_chanset_owner=NULL;goto out;
    }
    if(scs){
        for(i=0;i<IEEE80211_CHAN_MAX;i++){
            const struct qtn_scs_scan_info *raw=&scs->scan_info[i];
            sample.bandwidth=READ_ONCE(raw->bw_sel);sample.attempts=READ_ONCE(raw->cca_try);
            sample.cca_idle=READ_ONCE(raw->cca_idle);sample.cca_busy=READ_ONCE(raw->cca_busy);
            sample.cca_tx=READ_ONCE(raw->cca_tx);sample.cca_interference=READ_ONCE(raw->cca_intf);
            sample.cca_primary=READ_ONCE(raw->cca_pri);sample.cca_secondary=READ_ONCE(raw->cca_sec);
            sample.cca_secondary40=READ_ONCE(raw->cca_sec40);sample.beacons=READ_ONCE(raw->bcn_rcvd);
            sample.crc_errors=READ_ONCE(raw->crc_err);sample.long_preamble_errors=READ_ONCE(raw->lpre_err);
            sample.short_preamble_errors=READ_ONCE(raw->spre_err);
            chan=ieee80211_find_channel_by_ieee(ic,i);
            if(chan&&sample.attempts&&((pass_type==1&&sample.bandwidth==20)||
                (pass_type>=2&&pass_type<=4&&sample.bandwidth==40))){
                unsigned direction=sample.bandwidth==20?0:pass_type==3?1:pass_type==4?3:
                    (chan->ic_flags&IEEE80211_CHAN_HT40U)?1:(chan->ic_flags&IEEE80211_CHAN_HT40D)?3:0;
                if(!om_chanset_scan_sample(transaction,i,direction,&sample))samples++;
            }
        }
    }
    (void)om_chanset_scan_advance(transaction);
    if(ic->ic_bsschan&&ic->ic_bsschan!=IEEE80211_CHAN_ANYC){
        chosen=ic->ic_bsschan;
        (void)om_chanset_end(transaction);scan->om_chanset_owner=NULL;goto out;
    }
    if(transaction->queue.cells[0])goto out;
    /* Lock the shared scan cache while copying only bounded channel metrics.
     * No SSID/MAC/client data is recorded in source evidence or this snapshot. */
    as=ss->ss_priv;spin_lock(&as->asl_lock);
    for(i=0;i<IEEE80211_CHAN_MAX;i++)TAILQ_FOREACH(se,&as->as_scan_list[i].asl_head,ase_list){
        uint8_t primary,secondary;
        unsigned width;
        struct om_chanset_view neighbor_view;
        const struct om_chanset_entry *neighbor_entry;
        if(observed++==255)goto snapshot_done;
        if(!se->base.se_chan)continue;
        width=ieee80211_get_max_ap_bw(&se->base);
        if(width!=20&&width!=40)continue;
        ieee80211_find_ht_pri_sec_chan(vap,&se->base,&primary,&secondary);
        if(!primary)primary=se->base.se_chan->ic_ieee;
        if(primary<1||primary>13)continue;
        if(width==20)secondary=0;
        if(width==40&&(!secondary||secondary<1||secondary>13))continue;
        if(om_chanset_template(OM_CHANSET_BAND24,width,&neighbor_view)||
            om_chanset_find(&neighbor_view,primary,width,secondary==0?0:secondary>primary?1:3,
                            &neighbor_entry)!=1)continue;
        scan->om_observations[n].channel=primary;scan->om_observations[n].bandwidth=width;
        scan->om_observations[n].secondary_offset=secondary==0?0:secondary>primary?1:3;
        scan->om_observations[n].rssi_dbm=se->base.se_rssi;n++;
    }
snapshot_done:
    spin_unlock(&as->asl_lock);
    neighborhood=ieee80211_get_type_of_neighborhood(ic);
    for(i=0;i<transaction->count;i++){
        struct om_chanset_entry *entry=&transaction->entries[i];
        struct om_chanset_channel_state *state=&scan->om_channels[i];
        const uint8_t *active=entry->bandwidth==40?ic->ic_chan_active_40:ic->ic_chan_active_20;
        chan=ieee80211_find_channel_by_ieee(ic,entry->primary);
        state->allowed=chan&&isset(active,entry->primary)&&
            !isset(ic->ic_chan_pri_inactive,entry->primary)&&
            !om_cs_kernel_disabled(ic,entry->primary,20)&&
            (chan->ic_flags&IEEE80211_CHAN_2GHZ)&&
            !(chan->ic_flags&(IEEE80211_CHAN_RADAR|IEEE80211_CHAN_DFS));
        state->inactive=0;state->dfs=chan?!!(chan->ic_flags&IEEE80211_CHAN_DFS):0;
        index=entry->bandwidth==40?PWR_IDX_40M:PWR_IDX_20M;
        state->power_range_cost=chan?(u32)((int)chan->ic_maxregpower-
            (int)chan->ic_maxpower_table[PWR_IDX_BF_OFF][PWR_IDX_1SS][index]):0;
        if(entry->bandwidth==40){
            unsigned secondary=entry->secondary_offset==1?entry->primary+4:
                entry->secondary_offset==3?entry->primary-4:IEEE80211_CHAN_MAX;
            struct ieee80211_channel *second=secondary<IEEE80211_CHAN_MAX?
                ieee80211_find_channel_by_ieee(ic,secondary):NULL;
            if(!second||!isset(ic->ic_chan_active_40,secondary)||
                isset(ic->ic_chan_disabled,secondary)||
                !(second->ic_flags&IEEE80211_CHAN_2GHZ)||
                (second->ic_flags&(IEEE80211_CHAN_RADAR|IEEE80211_CHAN_DFS)))state->allowed=0;
            for(j=0;j<n&&chan;j++)if(!om_cs_kernel_40_permitted(chan,
                entry->secondary_offset,scan->om_observations[j].channel,
                scan->om_observations[j].secondary_offset))state->inactive=1;
        }
    }
    result=om_chanset_rank(transaction,scan->om_channels,transaction->count,
                           scan->om_observations,n,neighborhood);
    if(result){
        scan->om_chanset_last_error=result;
        (void)om_chanset_end(transaction);scan->om_chanset_owner=NULL;goto out;
    }
    view.entries=transaction->entries;view.count=transaction->count;view.capacity=98;
    current_bw=ieee80211_get_bw(ic);
    if(om_chanset_select(&view,OM_CHANSET_BAND24,ic->ic_max_system_bw,
        transaction->queue.current_bandwidth,scan->om_chanset_policy,
        transaction->params.max_bandwidth_benefit,&picked)==1){
        chosen=ieee80211_find_channel_by_ieee(ic,picked->primary);
        if(chosen){
            u32 previous_flags=chosen->ic_flags;
            u8 previous_center=chosen->ic_center_f_40MHz;
            int previous_is_set=ic->ic_chan_is_set;
            if(ieee80211_dual_sec_chan_supported(vap,picked->primary))
                ieee80211_update_sec_chan_offset(chosen,picked->secondary_offset);
            if(!ic->ic_check_channel(ic,chosen,0,0)){
                chosen->ic_flags=previous_flags;
                chosen->ic_center_f_40MHz=previous_center;
                ic->ic_chan_is_set=previous_is_set;
                scan->om_chanset_last_error=-EINVAL;
                chosen=NULL;
            }
        }
        if(chosen&&current_bw!=picked->bandwidth)om_ieee80211_change_bw(vap,picked->bandwidth,0);
        (void)om_chanset_end(transaction);scan->om_chanset_owner=NULL;
    }else if(transaction->queue.current_bandwidth>20){
        if(om_chanset_add_scan_type(&transaction->queue,OM_CHANSET_BAND24,
                                   transaction->queue.current_bandwidth/2)){
            scan->om_chanset_last_error=-ENOSPC;
            (void)om_chanset_end(transaction);scan->om_chanset_owner=NULL;
        }
    }else{
        scan->om_chanset_last_error=-ENOENT;
        (void)om_chanset_end(transaction);scan->om_chanset_owner=NULL;
    }
out:
    spin_unlock_irqrestore(&scan->om_chanset_lock,flags);
    return chosen;
}
EXPORT_SYMBOL(om_chanset_kernel_allowed);
EXPORT_SYMBOL(om_chanset_kernel_start);
EXPORT_SYMBOL(om_chanset_kernel_policy);
EXPORT_SYMBOL(om_chanset_kernel_scan_type);
EXPORT_SYMBOL(om_chanset_kernel_pick);
EXPORT_SYMBOL(om_chanset_kernel_finished);
EXPORT_SYMBOL(om_chanset_kernel_cancel);

EXPORT_SYMBOL(om_chanset_kernel_active);
