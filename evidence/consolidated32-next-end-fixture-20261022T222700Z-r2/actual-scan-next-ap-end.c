
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
struct tasklet_stub{unsigned scheduled;};struct ap_state{spinlock_t asl_lock;struct ap_scan_list as_scan_list[255];int(*as_action)(struct ieee80211vap*,const struct ieee80211_scan_entry*);struct ieee80211_scan_entry as_selbss;struct tasklet_stub as_actiontq;};
#define TAILQ_FOREACH(v,h,f) for((v)=*(h);(v);(v)=(v)->next)
struct ieee80211_scanner;struct ieee80211_node;struct timer_list{unsigned pending;unsigned long when;};struct ieee80211_scan_state{struct ieee80211vap*ss_vap;void*ss_priv;const struct ieee80211_scanner*ss_ops;unsigned ss_flags,ss_pick_flags,ss_next,ss_last,is_scan_valid;struct ieee80211_channel*ss_chans[255];};
struct scan_state{struct ieee80211_scan_state base;spinlock_t om_chanset_lock;bool om_chanset_stopping;u8 om_chanset_policy;struct ieee80211vap*om_chanset_owner;struct om_chanset_transaction om_chanset;struct om_chanset_observation om_observations[255];struct om_chanset_channel_state om_channels[98];u8 om_scan_detaching;int om_chanset_last_error;unsigned long ss_scanend,ss_chanmindwell;unsigned ss_iflags;struct timer_list ss_scan_timer,ss_probe_timer;};
#define SCAN_PRIVATE(ss) ((struct scan_state*)(ss))
struct ieee80211com{spinlock_t ic_comlock;int ic_chan_is_set;void(*ic_setparam)(struct ieee80211_node*,int,int,void*,int);int(*ic_get_init_cac_duration)(struct ieee80211com*);void(*ic_dfs_action_scan_done)(void);void(*ic_scan_end)(struct ieee80211com*);unsigned ic_des_chan_after_init_scan;struct ieee80211_channel*ic_des_chan,*ic_curchan,*ic_scanchan;unsigned ic_flags_qtn,ic_flags_ext,ic_lastscan,ic_scan_comp,ic_pwr_adjust_scancnt,ic_11n_40_only_mode,ic_opmode;unsigned ic_phymode,ic_flags,ic_country_code,ic_max_system_bw,bw;struct ieee80211_scan_state*ic_scan;struct ieee80211_channel*ic_bsschan;uint8_t ic_chan_disabled[32],ic_chan_pri_inactive[32],ic_chan_active_20[32],ic_chan_active_40[32];struct ieee80211_channel channels[255];int(*ic_check_channel)(struct ieee80211com*,struct ieee80211_channel*,int,int);};
struct net_device{char name[8];};struct ieee80211_node;struct ieee80211vap{struct net_device*iv_dev;struct ieee80211_node*iv_bss;struct{unsigned is_scan_active,is_scan_passive;}iv_stats;unsigned iv_state;unsigned iv_opmode;struct ieee80211com*iv_ic;};
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

#include <stddef.h>
#include <signal.h>
#include <stdlib.h>
typedef uint32_t u_int32_t;typedef uint8_t u_int8_t;
#define IEEE80211_M_STA 1
#define IEEE80211_S_RUN 4
#define IEEE80211_QTN_PRINT_CH_INUSE 1
#define IEEE80211_SCAN_OBSS 0x10000
#define IEEE80211_SCAN_QTN_SEARCH_MBS 0x200
#define IEEE80211_SCAN_DFS_ACTION 0x400
#define IEEE80211_SCAN_NOPICK 0x8
#define IEEE80211_SCAN_GOTPICK 0x10
#define IEEE80211_SCAN_ONCE 0x20
#define IEEE80211_SCAN_BGSCAN 0x40
#define IEEE80211_SCAN_PICK1ST 0x4
#define IEEE80211_SCAN_ACTIVE 0x1
#define IEEE80211_SCAN_PICK_NOT_AVAILABLE_DFS_ONLY 0x80
#define IEEE80211_FEXT_SCAN_20 0x1
#define IEEE80211_FEXT_SCAN_40 0x2
#define IEEE80211_FEXT_BGSCAN 0x4
#define IEEE80211_CHAN_PASSIVE 0x200
#define IEEE80211_CHAN_TURBO 0x10
#define ISCAN_MINDWELL 1
#define ISCAN_DISCARD 2
#define ISCAN_CANCEL 4
#define ISCAN_START 8
#define IEEE80211_PARAM_BEACON_ALLOW 0
#define IEEE80211_IS_CHAN_TURBO(c) (0)
#define IEEE80211_IS_CHAN_STURBO(c) (0)
#define IEEE80211_IS_11NG_40(ic) (0)
#define IEEE80211_AID(v) ((v)&0x3fff)
#define IEEE80211_LOCK_IRQ(ic) do{locks++;}while(0)
#define IEEE80211_UNLOCK_IRQ(ic) do{assert(locks);locks--;}while(0)
#define IEEE80211_DPRINTF(...)
#define KASSERT(...)
#define KERN_ERR ""
#define KERN_DEBUG ""
#define printk(...) ((void)0)
#define from_timer(v,t,m) ((struct scan_state*)((char*)(t)-offsetof(struct scan_state,m)))
#define time_before(a,b) ((long)((a)-(b))<0)
#define time_after(a,b) time_before(b,a)
#define IEEE80211_SCHEDULE_TQUEUE(t) do{(t)->scheduled=1;}while(0)
struct ieee80211_node{unsigned ni_associd;};
struct ieee80211_scanner{int(*scan_end)(struct ieee80211_scan_state*,struct ieee80211vap*,int(*)(struct ieee80211vap*,const struct ieee80211_scan_entry*),u_int32_t);int(*scan_default)(struct ieee80211vap*,const struct ieee80211_scan_entry*);};
static unsigned long jiffies=100;static unsigned txpow_rxgain_state,txpow_rxgain_count,ends,sets,probes;
static void get_max_min_dwell(struct ieee80211_scan_state*s,struct ieee80211_node*n,int p,int o,int*a,int*b){(void)s;(void)n;(void)p;(void)o;*a=1;*b=2;}
static void ieee80211_pwr_adjust(struct ieee80211vap*v,unsigned x){(void)v;(void)x;}
static void change_channel(struct ieee80211com*ic,struct ieee80211_channel*c){ic->ic_curchan=c;sets++;}
static void mod_timer(struct timer_list*t,unsigned long when){t->pending=1;t->when=when;}
static void ieee80211_check_type_of_neighborhood(struct ieee80211com*ic){(void)ic;}
static void wake_up_interruptible_all(void*p){(void)p;}
static void ieee80211_sta_pwrsave(struct ieee80211vap*v,int x){(void)v;(void)x;}
static void ieee80211_notify_scan_done(struct ieee80211vap*v){(void)v;}
static void ieee80211_send_20_40_bss_coex(struct ieee80211vap*v){(void)v;}
static void ieee80211_check_mode(struct ieee80211vap*v){(void)v;}
static void ap_list_asl_table(struct ieee80211_scan_state*s){(void)s;}
static struct ieee80211_channel*ieee80211_find_channel(struct ieee80211com*ic,unsigned f,unsigned flags){(void)flags;for(unsigned i=0;i<255;i++)if(ic->channels[i].ic_freq==f)return&ic->channels[i];return NULL;}
struct ieee80211_channel*om_chanset_kernel_pick(struct ieee80211com*);
static struct ieee80211_channel*ap_pick_channel(struct ieee80211com*ic,struct ieee80211_scan_state*ss,unsigned flags){(void)ss;(void)flags;return om_chanset_kernel_pick(ic);}
static int cac(struct ieee80211com*ic){(void)ic;return 0;}
static void done(struct ieee80211com*ic){(void)ic;ends++;}
static void dfsdone(void){}
static int scan_default(struct ieee80211vap*v,const struct ieee80211_scan_entry*e){(void)v;(void)e;return 0;}
static int ieee80211_start_scan(struct ieee80211vap*v,int f,unsigned d,unsigned n,const struct ieee80211_scan_ssid*s){(void)v;(void)f;(void)d;(void)n;(void)s;return 0;}
static void crash(int s){(void)s;puts("{\"empty_scan_list_null_dereference\":true}");exit(0);}

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
static int ap_end(struct ieee80211_scan_state *ss, struct ieee80211vap *vap,
       int (*action)(struct ieee80211vap *, const struct ieee80211_scan_entry *),
       u_int32_t flags)
{
	struct ieee80211_channel * bestchan = NULL;
	struct ap_state *as = ss->ss_priv;
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_scan_entry se;
	int ret;

	KASSERT(vap->iv_opmode == IEEE80211_M_HOSTAP,
		("wrong opmode %u", vap->iv_opmode));

	/* scan end, no action and return */
	if (ss->ss_flags & IEEE80211_SCAN_QTN_SEARCH_MBS)
		return 1;

	/* scan end, do DFS action and return */
	if (ss->ss_flags & IEEE80211_SCAN_DFS_ACTION) {
		ic->ic_dfs_action_scan_done();
		return 1;
	}

#ifdef QTN_BG_SCAN
	if (ss->ss_flags & IEEE80211_SCAN_QTN_BGSCAN) {
		ss->ss_pick_flags = IEEE80211_PICK_DEFAULT;	/* clean the flag */
		return 1;
	}
#endif

	memset(&se, 0, sizeof(se));

	if (ic->ic_get_init_cac_duration(ic) > 0) {
		printk(KERN_DEBUG "ap_pick_channel : pick dfs channels only: for eu ICAC\n");
		flags = IEEE80211_SCAN_PICK_NOT_AVAILABLE_DFS_ONLY;
	}

	bestchan = ap_pick_channel(ic, ss, flags);
	if (bestchan == NULL) {
		printk(KERN_ERR "%s: no suitable channel! Go back!\n", vap->iv_dev->name);

		/*
		 * When max_boot_cac is a very large value, all channels are cleared.
		 * Return to ICAC completion procedure
		 */
		if (ic->ic_get_init_cac_duration(ic) > 0) {
			return 0;
		}

		if (ic->ic_bsschan != IEEE80211_CHAN_ANYC) {
			se.se_chan = ic->ic_bsschan;
			ret = 0;
		} else {
			return om_chanset_kernel_allowed(vap)?om_chanset_kernel_finished(ic):0; /* recovered scan queue */ /* restart scan */
		}
	} else {
		struct ieee80211_channel *c;
		/* XXX notify all vap's? */
		/* if this is a dynamic turbo frequency , start with normal mode first */

		c = bestchan;
		if (IEEE80211_IS_CHAN_TURBO(c) && !IEEE80211_IS_CHAN_STURBO(c)) {
			if ((c = ieee80211_find_channel(ic, c->ic_freq,
					c->ic_flags & ~IEEE80211_CHAN_TURBO)) == NULL) {
				/* should never happen ?? */
				return 0;
			}
		}

		/*
		 * If bss channel is valid and if the
		 * scan is to not pick any channel then select the
		 * bss channel, otherwise choose the best channel.
		 */
		if ((ic->ic_bsschan != IEEE80211_CHAN_ANYC) &&
					(ss->ss_flags & IEEE80211_SCAN_NOPICK)) {
			se.se_chan = ic->ic_bsschan;
		} else {
			se.se_chan = c;
		}

		ret = 1;
	}

	/*
	 * ic->ic_des_chan_after_init_scan is valid only during initial bootup scan
	 * Any Scan after the initial bootup scan, shall choose the best channel
	 * referring to as_chanmetric;
	 */
	if (ic->ic_des_chan_after_init_scan) {
		struct ieee80211_channel *ch = ieee80211_find_channel_by_ieee(ic, ic->ic_des_chan_after_init_scan);
		if (ch) {
			se.se_chan = ch;
		}
		ic->ic_des_chan_after_init_scan = 0;
	}


	ic->ic_des_chan = se.se_chan;

	as->as_action = ss->ss_ops->scan_default;
	if (action)
		as->as_action = action;
	as->as_selbss = se;

	/*
	 * Must defer action to avoid possible recursive call through 80211
	 * state machine, which would result in recursive locking.
	 */
	IEEE80211_SCHEDULE_TQUEUE(&as->as_actiontq);

	return ret;
}static void scan_next(struct timer_list *timer)
{
#define	ISCAN_REP	(ISCAN_MINDWELL | ISCAN_START | ISCAN_DISCARD)
	struct scan_state *scan = from_timer(scan, timer, ss_scan_timer);
	struct ieee80211_scan_state *ss = &scan->base;
	struct ieee80211vap *vap = READ_ONCE(ss->ss_vap);
	struct ieee80211com *ic;
	struct ieee80211_node *ni;
	struct ieee80211_channel *chan;
	unsigned long maxdwell, scanend;
	int scanning, scandone;

	if (READ_ONCE(scan->om_chanset_stopping)||!vap)return;
	ic=vap->iv_ic;ni=vap->iv_bss;
	/* Make passive channels special */
	int is_passive;
	int is_obss = 0;
	int maxdwell_used;
	int mindwell_used;
#ifdef QTN_BG_SCAN
	int bgscan_dwell = 0;
#endif /* QTN_BG_SCAN */

	if (ss->ss_flags & IEEE80211_SCAN_OBSS) {
		if (ni && IEEE80211_AID(ni->ni_associd))
			is_obss = 1;
		else
			return;
	}

	IEEE80211_LOCK_IRQ(ic);
	scanning = ((ic->ic_flags & IEEE80211_F_SCAN)
#ifdef QTN_BG_SCAN
			|| (ic->ic_flags_qtn & IEEE80211_QTN_BGSCAN)
#endif /* QTN_BG_SCAN */
		);
	IEEE80211_UNLOCK_IRQ(ic);
	if (!scanning)			/* canceled */
		return;

again:
	scandone = (ss->ss_next >= ss->ss_last) ||
		(SCAN_PRIVATE(ss)->ss_iflags & ISCAN_CANCEL) != 0;
	scanend = SCAN_PRIVATE(ss)->ss_scanend;

	if ((vap->iv_opmode == IEEE80211_M_STA) && (ss->ss_next == 0)) {
		/*
		 * Periodically scan using low Rx gain and Tx power in case
		 * association is failing because the AP is too close.
		 * More suitable power settings will be determined after association.
		 */
		IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN,
				  "%s Starting a scan (Low power %s, count %d)\n",
				  __func__, txpow_rxgain_state ? "on" : "off", txpow_rxgain_count);

		if ((ic->ic_pwr_adjust_scancnt > 0) && (ss->is_scan_valid) &&
#ifdef QTN_BG_SCAN
		     !(ss->ss_flags & IEEE80211_SCAN_QTN_BGSCAN) &&
#endif /* QTN_BG_SCAN */
		    (txpow_rxgain_count) && ((txpow_rxgain_count % ic->ic_pwr_adjust_scancnt) == 0)) {
			ieee80211_pwr_adjust(vap, txpow_rxgain_state);
			txpow_rxgain_state = !txpow_rxgain_state;
		}
		txpow_rxgain_count++;
#ifdef QTN_BG_SCAN
		if ((ss->ss_flags & IEEE80211_SCAN_QTN_BGSCAN) &&
				(SCAN_PRIVATE(ss)->ss_iflags & ISCAN_START)) {
			ic->ic_bgscan_start(ic);
		}
#endif /* QTN_BG_SCAN */
	}

	/* Work out the previous channel (to use passive vs. active mindwell) */
	chan = (ss->ss_next) ? ss->ss_chans[ss->ss_next-1] : ss->ss_chans[0];
	is_passive = (!(ss->ss_flags & IEEE80211_SCAN_ACTIVE) ||
				(chan->ic_flags & IEEE80211_CHAN_PASSIVE));
	get_max_min_dwell(ss, ni, is_passive, is_obss, &mindwell_used, &maxdwell_used);

	if (!scandone &&
	    (ss->ss_flags & IEEE80211_SCAN_GOTPICK) == 0 &&
	    ((SCAN_PRIVATE(ss)->ss_iflags & ISCAN_START) ||
	     time_before(jiffies + mindwell_used, scanend))) {


		chan = ss->ss_chans[ss->ss_next++];
		is_passive = (!(ss->ss_flags & IEEE80211_SCAN_ACTIVE) ||
					(chan->ic_flags & IEEE80211_CHAN_PASSIVE));
		ic->ic_scanchan = chan;

#ifdef QTN_BG_SCAN
		if (ss->ss_flags & IEEE80211_SCAN_QTN_BGSCAN) {
			uint32_t scan_mode = ss->ss_pick_flags & IEEE80211_PICK_BG_MODE_MASK;

			if (!is_passive) {
				scan_mode = IEEE80211_PICK_BG_ACTIVE;
			}

			if (chan->ic_ieee == ic->ic_bsschan->ic_ieee) {
				if (vap->iv_opmode != IEEE80211_M_STA) {
					scan_mode = IEEE80211_PICK_BG_ACTIVE;
				} else if (scan_mode & IEEE80211_PICK_BG_ACTIVE) {
					scan_mode = 0;
				}
			}

			if (!scan_mode) {
				/*
				 * Auto passive mode selection:
				 * 1) if FAT is larger than the threshold for fast mode
				 *  which is 60% by default, will pick passive fast mode
				 * 2) else if FAT is larger than the threshold for normal mode
				 * which is 30% by default, will pick passive normal mode
				 * 3) else pick passive slow mode.
				 */
				if (ic->ic_scs.scs_cca_idle_smthed >=
						ic->ic_qtn_bgscan.thrshld_fat_passive_fast) {
					scan_mode = IEEE80211_PICK_BG_PASSIVE_FAST;
				} else if (ic->ic_scs.scs_cca_idle_smthed >=
						ic->ic_qtn_bgscan.thrshld_fat_passive_normal) {
					scan_mode = IEEE80211_PICK_BG_PASSIVE_NORMAL;
				} else {
					scan_mode = IEEE80211_PICK_BG_PASSIVE_SLOW;
				}
			}

			if (scan_mode & IEEE80211_PICK_BG_ACTIVE) {
				maxdwell_used = msecs_to_jiffies(ic->ic_qtn_bgscan.duration_msecs_active);
			} else if (scan_mode & IEEE80211_PICK_BG_PASSIVE_FAST) {
				maxdwell_used = msecs_to_jiffies(ic->ic_qtn_bgscan.duration_msecs_passive_fast);
			} else if (scan_mode & IEEE80211_PICK_BG_PASSIVE_NORMAL) {
				maxdwell_used = msecs_to_jiffies(ic->ic_qtn_bgscan.duration_msecs_passive_normal);
			} else {
				maxdwell_used = msecs_to_jiffies(ic->ic_qtn_bgscan.duration_msecs_passive_slow);
			}
			maxdwell = mindwell_used = maxdwell_used;

			if (scan_mode & IEEE80211_PICK_BG_ACTIVE) {
				bgscan_dwell = ic->ic_qtn_bgscan.dwell_msecs_passive;
			} else {
				bgscan_dwell = ic->ic_qtn_bgscan.dwell_msecs_active;
			}

			/*
			 * Workaround: in STA mode, don't send probe request frame
			 * directly because the probe response frame from other AP
			 * may mess up the txalert timer
			 */
			if (chan->ic_ieee == ic->ic_bsschan->ic_ieee &&
					vap->iv_opmode != IEEE80211_M_STA) {
				ieee80211_send_probereq(vap->iv_bss,
						vap->iv_myaddr, vap->iv_dev->broadcast,
						vap->iv_dev->broadcast,
						(u_int8_t *)"", 0,
						vap->iv_opt_ie, vap->iv_opt_ie_len);

			} else {
				ic->ic_bgscan_channel(vap, chan, scan_mode, bgscan_dwell);
			}
		} else {
#endif /* QTN_BG_SCAN */
			/* Reset mindwell and maxdwell as the new channel could be passive */
			get_max_min_dwell(ss, ni, is_passive, is_obss, &mindwell_used, &maxdwell_used);

			/*
			 * Watch for truncation due to the scan end time.
			 */
			if (time_after(jiffies + maxdwell_used, scanend))
				maxdwell = scanend - jiffies;
			else
				maxdwell = maxdwell_used;

			IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN,
				"%s: chan %3d%c -> %3d%c [%s, dwell min %lu max %lu]\n",
				__func__,
				ieee80211_chan2ieee(ic, ic->ic_curchan),
					channel_type(ic->ic_curchan),
				ieee80211_chan2ieee(ic, chan), channel_type(chan),
				(ss->ss_flags & IEEE80211_SCAN_ACTIVE) &&
					(chan->ic_flags & IEEE80211_CHAN_PASSIVE) == 0 ?
					"active" : "passive",
				mindwell_used, maxdwell);

			/*
			 * Potentially change channel and phy mode.
			 */
			/* Channel change done with 20MHz wide channels unless in 40MHz only mode */
			if (!((ss->ss_flags & IEEE80211_SCAN_BGSCAN) &&
					chan->ic_ieee == ic->ic_curchan->ic_ieee)) {
				unsigned om_type=om_chanset_kernel_scan_type(ic);
				if(om_type){
				    unsigned secondary=om_type==3?chan->ic_ieee+4:om_type==4?chan->ic_ieee-4:0;
				    if((om_type>=2&&om_type<=4)&&(!isset(ic->ic_chan_active_40,chan->ic_ieee)||
				        !secondary||secondary>=IEEE80211_CHAN_MAX||
				        !isset(ic->ic_chan_active_40,secondary))){
				        goto again; /* The channel fetch already advanced ss_next once. */
				    }
				}
				if(om_type>=2&&om_type<=4){
				    ic->ic_flags_ext &= ~IEEE80211_FEXT_SCAN_20;
				    ic->ic_flags_ext |= IEEE80211_FEXT_SCAN_40;
				    if(om_type==3||om_type==4)ieee80211_update_sec_chan_offset(chan,om_type==3?1:3);
				}else if (!ic->ic_11n_40_only_mode)
				      ic->ic_flags_ext |= IEEE80211_FEXT_SCAN_20;

				change_channel(ic, chan);
				if(om_type)ic->ic_flags_ext &= ~IEEE80211_FEXT_SCAN_40;

				/* Clear the flag */
				if (!ic->ic_11n_40_only_mode)
				      ic->ic_flags_ext &= ~IEEE80211_FEXT_SCAN_20;
			}
#ifdef QTN_BG_SCAN
		}
#endif /* QTN_BG_SCAN */
		/*
		 * If doing an active scan and the channel is not
		 * marked passive-only then send a probe request.
		 * Otherwise just listen for traffic on the channel.
		 */
		if ((ss->ss_flags & IEEE80211_SCAN_ACTIVE) &&
#ifdef QTN_BG_SCAN
		    /* qtn bgscan sends probe request in MuC */
		    (ss->ss_flags & IEEE80211_SCAN_QTN_BGSCAN) == 0 &&
#endif /* QTN_BG_SCAN */
		    (chan->ic_flags & IEEE80211_CHAN_PASSIVE) == 0) {
			/*
			 * Delay sending the probe requests so we are on
			 * the new channel. Current delay is half of maxdwell
			 * to make sure it is well within the dwell time,
			 * this can be fine tuned later if necessary.
			 */
			mod_timer(&SCAN_PRIVATE(ss)->ss_probe_timer,
				  jiffies + (maxdwell / 2));
		}
		SCAN_PRIVATE(ss)->ss_chanmindwell = jiffies + mindwell_used;
		mod_timer(&SCAN_PRIVATE(ss)->ss_scan_timer, jiffies + maxdwell);
		/* clear mindwell lock and initial channel change flush */
		SCAN_PRIVATE(ss)->ss_iflags &= ~ISCAN_REP;
	} else {
		ic->ic_scan_end(ic);		/* notify driver */
		/*
		 * Record scan complete time.  Note that we also do
		 * this when canceled so any background scan will
		 * not be restarted for a while.
		 */
		if (scandone)
			ic->ic_lastscan = jiffies;

		/* return to the bss channel */
		if (ic->ic_bsschan != IEEE80211_CHAN_ANYC) {
#ifdef QTN_BG_SCAN
			if (ss->ss_flags & IEEE80211_SCAN_QTN_BGSCAN) {
				struct ieee80211vap *tmp_vap;
				/*
				 * Need to update beacon because beacon may have been
				 * updated when AP is on off channel, and in this case,
				 * the channel offset in beacon's retry table setting
				 * may not be correct.
				 */
				TAILQ_FOREACH(tmp_vap, &ic->ic_vaps, iv_next) {
					if (tmp_vap->iv_opmode != IEEE80211_M_HOSTAP)
						continue;
					if (tmp_vap->iv_state != IEEE80211_S_RUN)
						continue;
					ic->ic_beacon_update(tmp_vap);
				}
			}
			else
#endif /* QTN_BG_SCAN */
				change_channel(ic, ic->ic_bsschan);
		}

		/* clear internal flags and any indication of a pick */
		SCAN_PRIVATE(ss)->ss_iflags &= ~ISCAN_REP;
		ss->ss_flags &= ~IEEE80211_SCAN_GOTPICK;

		if ((SCAN_PRIVATE(ss)->ss_iflags & ISCAN_CANCEL) == 0) {
			ieee80211_check_type_of_neighborhood(ic);
#ifdef QSCS_ENABLED
			ieee80211_scs_update_ranking_table_by_scan(ic);
			ieee80211_scs_adjust_cca_threshold(ic);
#endif
		}

		/*
		 * If not canceled and scan completed, do post-processing.
		 * If the callback function returns 0, then it wants to
		 * continue/restart scanning.  Unfortunately we needed to
		 * notify the driver to end the scan above to avoid having
		 * rx frames alter the scan candidate list.
		 */
		if ((SCAN_PRIVATE(ss)->ss_iflags & ISCAN_CANCEL) == 0 &&
		    !ss->ss_ops->scan_end(ss, vap, NULL, 0) &&
		    (ss->ss_flags & IEEE80211_SCAN_ONCE) == 0 &&
		    time_before(jiffies + mindwell_used, scanend)) {
			IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN,
				"%s: done, restart "
				"[jiffies %lu, dwell min %lu scanend %lu]\n",
				__func__,
				jiffies, mindwell_used, scanend);
			ss->ss_next = 0;	/* reset to beginning */
			if (ss->ss_flags & IEEE80211_SCAN_ACTIVE)
				vap->iv_stats.is_scan_active++;
			else
				vap->iv_stats.is_scan_passive++;

//			ic->ic_scan_start(ic);	/* notify driver */
			goto again;
		} else {
			/* past here, scandone is ``true'' if not in bg mode */
			if ((ss->ss_flags & IEEE80211_SCAN_BGSCAN) == 0)
				scandone = 1;

			IEEE80211_DPRINTF(vap, IEEE80211_MSG_SCAN,
				"%s: %s, "
				"[jiffies %lu, dwell min %lu scanend %lu]\n",
				__func__, scandone ? "done" : "stopped",
				jiffies, mindwell_used, scanend);

			/* don't care about bgscan case */
			if ((ic->ic_flags & IEEE80211_F_SCAN)
#ifdef QTN_BG_SCAN
				|| (ic->ic_flags_qtn & IEEE80211_QTN_BGSCAN)
#endif /* QTN_BG_SCAN */
				) {
				wake_up_interruptible_all(&ic->ic_scan_comp);
			}
			/*
			 * Clear the SCAN bit first in case frames are
			 * pending on the station power save queue.  If
			 * we defer this then the dispatch of the frames
			 * may generate a request to cancel scanning.
			 */
			ic->ic_flags &= ~IEEE80211_F_SCAN;
#ifdef QTN_BG_SCAN
			ic->ic_flags_qtn &= ~IEEE80211_QTN_BGSCAN;
#endif /* QTN_BG_SCAN */

#if defined(QBMPS_ENABLE)
			if ((ic->ic_flags_qtn & IEEE80211_QTN_BMPS) &&
			    (vap->iv_opmode == IEEE80211_M_STA)) {
				/* re-enter power-saving if possible */
				ieee80211_pm_queue_work(ic);
			}
#endif
			/*
			 * Drop out of power save mode when a scan has
			 * completed.  If this scan was prematurely terminated
			 * because it is a background scan then don't notify
			 * the ap; we'll either return to scanning after we
			 * receive the beacon frame or we'll drop out of power
			 * save mode because the beacon indicates we have frames
			 * waiting for us.
			 */
			if (scandone) {
				ieee80211_sta_pwrsave(vap, 0);
				if ((vap->iv_state == IEEE80211_S_RUN) &&
				    (vap->iv_opmode == IEEE80211_M_STA)) {
					ic->ic_setparam(vap->iv_bss, IEEE80211_PARAM_BEACON_ALLOW,
							0, NULL, 0);
				}

				if (ss->ss_next >= ss->ss_last) {
					ieee80211_notify_scan_done(vap);
					ic->ic_flags_ext &= ~IEEE80211_FEXT_BGSCAN;
					if (IEEE80211_IS_11NG_40(ic)) {
						if (ic->ic_opmode == IEEE80211_M_STA)
							ieee80211_send_20_40_bss_coex(vap);
						else
							ieee80211_check_mode(vap);
					}
				}
			}

			if ((ic->ic_flags_qtn & IEEE80211_QTN_PRINT_CH_INUSE) &&
			    (ic->ic_opmode == IEEE80211_M_HOSTAP))
				ap_list_asl_table(ss);

			SCAN_PRIVATE(ss)->ss_iflags &= ~ISCAN_CANCEL;
			ss->ss_flags &=
			    ~(IEEE80211_SCAN_ONCE | IEEE80211_SCAN_PICK1ST);
		}
	}
#undef ISCAN_REP
}
static struct ieee80211com ic;static struct scan_state scan;static struct ap_state ap;static struct ieee80211vap vap;static struct ieee80211_node node;static struct net_device dev;static const struct ieee80211_scanner scanner={ap_end,scan_default};
static void clear(void){memset(&ic,0,sizeof(ic));memset(&scan,0,sizeof(scan));memset(&ap,0,sizeof(ap));memset(&stats,0,sizeof(stats));ic.ic_scan=&scan.base;ic.ic_phymode=8;ic.bw=20;ic.ic_max_system_bw=20;ic.ic_bsschan=IEEE80211_CHAN_ANYC;ic.ic_check_channel=check;ic.ic_get_init_cac_duration=cac;ic.ic_scan_end=done;ic.ic_dfs_action_scan_done=dfsdone;vap.iv_ic=&ic;vap.iv_opmode=6;vap.iv_bss=&node;vap.iv_dev=&dev;scan.base.ss_vap=&vap;scan.base.ss_priv=&ap;scan.base.ss_ops=&scanner;shares.scs_info_lhost=&stats;shared_calls=lookup_calls=check_calls=changes=start_calls=start_behavior=nested_result=locks=neighbor_visits=ends=sets=0;check_result=1;for(unsigned c=1;c<=13;c++){ic.channels[c].ic_ieee=c;ic.channels[c].ic_freq=2407+5*c;ic.channels[c].ic_flags=IEEE80211_CHAN_2GHZ;bitset(ic.ic_chan_active_20,c);bitset(ic.ic_chan_active_40,c);}scan.base.ss_chans[0]=&ic.channels[1];scan.base.ss_last=1;scan.base.ss_next=1;scan.ss_scanend=jiffies+1000;ic.ic_flags=IEEE80211_F_SCAN;}
int main(int argc,char**argv){unsigned cases=0;clear();if(argc>1){signal(SIGSEGV,crash);scan.base.ss_next=scan.base.ss_last=0;scan.base.ss_chans[0]=NULL;scan_next(&scan.ss_scan_timer);puts("{\"empty_scan_list_null_dereference\":false}");return 0;}
assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;scan_next(&scan.ss_scan_timer);assert(!scan.om_chanset.busy&&!ic.ic_flags&&ap.as_actiontq.scheduled==1&&ends==1&&!locks);cases++;
clear();assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;shares.scs_info_lhost=NULL;scan_next(&scan.ss_scan_timer);assert(!scan.om_chanset.busy&&!ic.ic_flags&&scan.om_chanset_last_error==-EAGAIN&&ends==1&&!locks);cases++;
clear();assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;memset(ic.ic_chan_active_20,0,32);scan_next(&scan.ss_scan_timer);assert(!scan.om_chanset.busy&&!ic.ic_flags&&scan.om_chanset_last_error==-ENOENT&&ends==1&&!locks);cases++;
clear();assert(!om_chanset_prepare(&scan.om_chanset,0,20,0,0));scan.om_chanset_owner=&vap;om_chanset_kernel_cancel(&ic);scan.ss_iflags|=ISCAN_CANCEL;scan_next(&scan.ss_scan_timer);assert(!scan.om_chanset.busy&&!ic.ic_flags&&ends==1&&!locks);cases++;

clear();assert(!om_chanset_prepare(&scan.om_chanset,0,40,0,0));scan.om_chanset_owner=&vap;scan.om_chanset.queue.cells[0]=3;scan.om_chanset.queue.cells[1]=1;scan.om_chanset.queue.current_bandwidth=40;scan.base.ss_next=0;scan.base.ss_last=2;scan.base.ss_chans[0]=&ic.channels[1];scan.base.ss_chans[1]=&ic.channels[6];scan.ss_iflags=ISCAN_START;ic.ic_chan_active_40[0]&=~(1U<<5);scan_next(&scan.ss_scan_timer);assert(sets==1&&ic.ic_curchan==&ic.channels[6]&&scan.base.ss_next==2&&scan.ss_scan_timer.pending&&!locks);cases++;
/* Rejected lastchannel must complete pass then restart queue20 atfirstchannel. */
clear();assert(!om_chanset_prepare(&scan.om_chanset,0,40,0,0));scan.om_chanset_owner=&vap;scan.om_chanset.queue.cells[0]=3;scan.om_chanset.queue.cells[1]=1;scan.om_chanset.queue.current_bandwidth=40;scan.base.ss_next=1;scan.base.ss_last=2;scan.base.ss_chans[0]=&ic.channels[6];scan.base.ss_chans[1]=&ic.channels[1];scan.ss_iflags=ISCAN_START;ic.ic_chan_active_40[0]&=~(1U<<5);scan_next(&scan.ss_scan_timer);assert(sets==1&&ic.ic_curchan==&ic.channels[6]&&scan.base.ss_next==1&&ends==1&&scan.om_chanset.queue.cells[0]==1&&scan.om_chanset.busy&&scan.ss_scan_timer.pending&&!locks);cases++;
/* BELOW queue4: rejectprimary5 withsecondary1 then visitprimary10/secondary6. */
clear();assert(!om_chanset_prepare(&scan.om_chanset,0,40,0,0));scan.om_chanset_owner=&vap;scan.om_chanset.queue.cells[0]=4;scan.om_chanset.queue.cells[1]=1;scan.om_chanset.queue.current_bandwidth=40;scan.base.ss_next=0;scan.base.ss_last=2;scan.base.ss_chans[0]=&ic.channels[5];scan.base.ss_chans[1]=&ic.channels[10];scan.ss_iflags=ISCAN_START;ic.ic_chan_active_40[0]&=~(1U<<1);scan_next(&scan.ss_scan_timer);assert(sets==1&&ic.ic_curchan==&ic.channels[10]&&scan.base.ss_next==2&&scan.ss_scan_timer.pending&&!locks);cases++;
printf("{\"cases\":%u,\"full_scan_next_ap_end_selected_missing_empty_cancel\":true}\n",cases);return 0;}
