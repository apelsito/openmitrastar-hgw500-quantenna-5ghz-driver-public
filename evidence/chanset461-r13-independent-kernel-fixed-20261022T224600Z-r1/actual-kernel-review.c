
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "openmitrastar_chanset_recovered.h"
typedef uint8_t u8;typedef uint32_t u32;
#define IEEE80211_M_HOSTAP 6
#define IEEE80211_MODE_11B 2
#define IEEE80211_MODE_11G 3
#define IEEE80211_MODE_11NG 8
#define IEEE80211_MODE_11NG_HT40PM 9
#define IEEE80211_F_SCAN 0x80
#define IEEE80211_SCAN_USECACHE 0x40000
#define IEEE80211_CHAN_MAX 255
#define CTRY_DEFAULT 0
#define IEEE80211_CHAN_2GHZ 0x80
#define IEEE80211_CHAN_RADAR 0x1000
#define IEEE80211_CHAN_DFS 0x100000
#define IEEE80211_CHAN_HT40U 0x20000
#define IEEE80211_CHAN_HT40D 0x40000
#define IEEE80211_CHAN_ANYC ((struct ieee80211_channel*)1)
#define PWR_IDX_BF_OFF 0
#define PWR_IDX_1SS 0
#define PWR_IDX_40M 1
#define PWR_IDX_20M 0
#define READ_ONCE(v) (v)
#define EXPORT_SYMBOL(v)
typedef unsigned spinlock_t;
static unsigned locks;
#define spin_lock_irqsave(l,f) do{(f)=0;(*(l))++;locks++;}while(0)
#define spin_unlock_irqrestore(l,f) do{(void)(f);assert(*(l));(*(l))--;locks--;}while(0)
#define spin_lock(l) do{(*(l))++;locks++;}while(0)
#define spin_unlock(l) do{assert(*(l));(*(l))--;locks--;}while(0)
#define isset(a,n) (((a)[(n)/8]>>((n)%8))&1U)
static void bitset(uint8_t*a,unsigned n){a[n/8]|=1U<<(n%8);}
struct ieee80211vap;struct ieee80211com;
struct ieee80211_channel{unsigned ic_ieee,ic_freq,ic_flags;u8 ic_center_f_40MHz;int ic_maxregpower;int ic_maxpower_table[1][1][2];};
struct ieee80211_scan_ssid{unsigned len;};
struct ieee80211_scan_entry{struct ieee80211_channel*se_chan;int se_rssi;unsigned bw,primary,secondary;};
struct ap_scan_entry{struct ieee80211_scan_entry base;struct ap_scan_entry*next;};
struct ap_scan_list{struct ap_scan_entry*asl_head;};
struct ap_state{spinlock_t asl_lock;struct ap_scan_list as_scan_list[255];};
#define TAILQ_FOREACH(v,h,f) for((v)=*(h);(v);(v)=(v)->next)
struct ieee80211_scan_state{struct ieee80211vap*ss_vap;void*ss_priv;};
struct scan_state{struct ieee80211_scan_state base;spinlock_t om_chanset_lock;bool om_chanset_stopping;u8 om_chanset_policy;struct ieee80211vap*om_chanset_owner;struct om_chanset_transaction om_chanset;struct om_chanset_observation om_observations[255];struct om_chanset_channel_state om_channels[98];u8 om_scan_detaching;int om_chanset_last_error;};
#define SCAN_PRIVATE(ss) ((struct scan_state*)(ss))
struct ieee80211com{spinlock_t ic_comlock;int ic_chan_is_set;unsigned ic_phymode,ic_flags,ic_country_code,ic_max_system_bw,bw;struct ieee80211_scan_state*ic_scan;struct ieee80211_channel*ic_bsschan;uint8_t ic_chan_disabled[32],ic_chan_pri_inactive[32],ic_chan_active_20[32],ic_chan_active_40[32];struct ieee80211_channel channels[255];int(*ic_check_channel)(struct ieee80211com*,struct ieee80211_channel*,int,int);};
struct ieee80211vap{unsigned iv_opmode;struct ieee80211com*iv_ic;};
struct qtn_scs_scan_info{u32 bw_sel,cca_try,cca_idle,cca_busy,cca_tx,cca_intf,cca_pri,cca_sec,cca_sec40,bcn_rcvd,crc_err,lpre_err,spre_err;};
struct qtn_scs_info_set{struct qtn_scs_scan_info scan_info[255];};
struct shared_params{struct qtn_scs_info_set*scs_info_lhost;};
static struct shared_params shares;static struct qtn_scs_info_set stats;static unsigned shared_calls,lookup_calls,check_calls,changes,start_calls,nested_result,start_behavior,check_result=1,neighbor_visits;static struct ieee80211vap*other;
static unsigned ieee80211_get_bw(struct ieee80211com*ic){return ic->bw;}
static struct shared_params*qtn_mproc_sync_shared_params_get(void){shared_calls++;return &shares;}
static struct ieee80211_channel*ieee80211_find_channel_by_ieee(struct ieee80211com*ic,unsigned c){lookup_calls++;return c<255&&ic->channels[c].ic_ieee?&ic->channels[c]:NULL;}
static unsigned ieee80211_find_sec_chan(struct ieee80211_channel*c){return(c->ic_flags&IEEE80211_CHAN_HT40U)?c->ic_ieee+4:(c->ic_flags&IEEE80211_CHAN_HT40D)?c->ic_ieee-4:0;}
static int ieee80211_ieee2mhz(unsigned c,int flags){(void)flags;return 2407+5*c;}
static unsigned ieee80211_get_max_ap_bw(struct ieee80211_scan_entry*e){neighbor_visits++;return e->bw;}
static void ieee80211_find_ht_pri_sec_chan(struct ieee80211vap*v,struct ieee80211_scan_entry*e,uint8_t*p,uint8_t*s){(void)v;*p=e->primary;*s=e->secondary;}
static int ieee80211_get_type_of_neighborhood(struct ieee80211com*ic){(void)ic;return 0;}
static int ieee80211_dual_sec_chan_supported(struct ieee80211vap*v,unsigned c){(void)v;(void)c;return 1;}
static void ieee80211_update_sec_chan_offset(struct ieee80211_channel*c,unsigned o){c->ic_flags&=~(IEEE80211_CHAN_HT40U|IEEE80211_CHAN_HT40D);c->ic_flags|=o==1?IEEE80211_CHAN_HT40U:o==3?IEEE80211_CHAN_HT40D:0;}
static void om_ieee80211_change_bw(struct ieee80211vap*v,unsigned w,int reason){(void)reason;v->iv_ic->bw=w;changes++;}
static int check(struct ieee80211com*ic,struct ieee80211_channel*c,int x,int y){(void)ic;(void)c;(void)x;(void)y;check_calls++;return check_result;}
int om_chanset_kernel_start(struct ieee80211vap*,int,unsigned,unsigned,const struct ieee80211_scan_ssid*);
void om_chanset_kernel_cancel(struct ieee80211com*);
static int ieee80211_start_scan(struct ieee80211vap*v,int f,unsigned d,unsigned n,const struct ieee80211_scan_ssid*s){(void)f;(void)d;(void)n;(void)s;start_calls++;struct scan_state*scan=SCAN_PRIVATE(v->iv_ic->ic_scan);if(start_behavior==1){start_behavior=0;nested_result=om_chanset_kernel_start(other,0,10,0,NULL);return 1;}if(start_behavior==2){scan->om_chanset_stopping=true;return 0;}return 1;}

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
static int review_ap_null_result(struct ieee80211vap*vap,struct ieee80211com*ic){return om_chanset_kernel_allowed(vap)?om_chanset_kernel_finished(ic):0;}
static int review_scan_restarts(int apreturn){return !apreturn;}

static struct ieee80211com ic;static struct scan_state scan;static struct ap_state ap;static struct ieee80211vap vap,vap2;
static void clear(void){memset(&ic,0,sizeof(ic));memset(&scan,0,sizeof(scan));memset(&ap,0,sizeof(ap));memset(&stats,0,sizeof(stats));ic.ic_scan=&scan.base;ic.ic_phymode=8;ic.bw=40;ic.ic_max_system_bw=40;ic.ic_bsschan=IEEE80211_CHAN_ANYC;ic.ic_check_channel=check;vap.iv_ic=vap2.iv_ic=&ic;vap.iv_opmode=vap2.iv_opmode=6;other=&vap2;scan.base.ss_vap=&vap;scan.base.ss_priv=&ap;shares.scs_info_lhost=&stats;shared_calls=lookup_calls=check_calls=changes=start_calls=start_behavior=nested_result=locks=neighbor_visits=0;check_result=1;for(unsigned c=1;c<=13;c++){ic.channels[c].ic_ieee=c;ic.channels[c].ic_freq=2407+5*c;ic.channels[c].ic_flags=IEEE80211_CHAN_2GHZ;bitset(ic.ic_chan_active_20,c);bitset(ic.ic_chan_active_40,c);}}
int main(void){unsigned cases=0;struct ap_scan_entry bad={0};
clear();assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;shares.scs_info_lhost=NULL;assert(!om_chanset_kernel_pick(&ic)&&!scan.om_chanset.busy&&!scan.om_chanset_owner&&scan.om_chanset_last_error==-EAGAIN&&review_ap_null_result(&vap,&ic)==1);cases++;
clear();assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;memset(ic.ic_chan_active_20,0,32);assert(!om_chanset_kernel_pick(&ic)&&!scan.om_chanset.busy&&!scan.om_chanset_owner&&scan.om_chanset_last_error==-ENOENT&&review_ap_null_result(&vap,&ic)==1);cases++;
/* Adapter accepts unsupported40geometry then whole pure rank refuses. */
clear();assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;bad.base.se_chan=&ic.channels[1];bad.base.bw=40;bad.base.primary=1;bad.base.secondary=2;bad.base.se_rssi=200;ap.as_scan_list[1].asl_head=&bad;assert(!om_chanset_kernel_pick(&ic)&&!scan.om_chanset.busy&&!scan.om_chanset_owner&&scan.om_chanset_last_error==-EINVAL&&om_chanset_kernel_finished(&ic));cases++;
/* A 20MHz selected candidate changes channelflags before finalreject. */
clear();ic.bw=20;assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;ic.channels[1].ic_flags|=IEEE80211_CHAN_HT40U;check_result=0;assert(!om_chanset_kernel_pick(&ic)&&check_calls==1&&!scan.om_chanset.busy&&(ic.channels[1].ic_flags&IEEE80211_CHAN_HT40U)&&scan.om_chanset_last_error==-EINVAL);cases++;
/* Traversal bound uses accepted n only: rejected80MHznodes all visited. */
static struct ap_scan_entry rejected[2048];clear();assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;for(unsigned i=0;i<2048;i++){rejected[i].base.se_chan=&ic.channels[1];rejected[i].base.bw=80;rejected[i].next=i+1<2048?&rejected[i+1]:NULL;}ap.as_scan_list[1].asl_head=rejected;assert(om_chanset_kernel_pick(&ic)&&neighbor_visits==255&&!locks);cases++;
printf("{\"cases\":%u,\"missing_shared_fixed\":true,\"empty_legal_set_fixed\":true,\"rank_failure_leaves_busy\":false,\"rejected_choice_changes_flags\":false,\"rejected_list_visits\":%u}\n",cases,neighbor_visits);return 0;}
