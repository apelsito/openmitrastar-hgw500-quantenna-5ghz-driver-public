#include <stdint.h>
#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
typedef uint32_t u32;
#define READ_ONCE(x) (x)
#define WRITE_ONCE(x,v) ((x)=(v))
#define BOARD_PM_LEVEL_IDLE 5
#define PM_QOS_DEFAULT_VALUE (-1)
#define PM_QOS_POWER_SAVE 4
#define BOARD_PM_GOVERNOR_WLAN "wlan"
#define IEEE80211_M_STA 1
#define IEEE80211_S_RUN 4
#define IEEE80211_CHAN_DFS 0x00100000U
#define IEEE80211_CHAN_DFS_CAC_IN_PROGRESS 0x01000000U
#define IEEE80211_CHAN_ANYC ((struct ieee80211_channel *)(uintptr_t)0xffff)
#define QBMPS_ENABLE 1
#define IEEE80211_QTN_BMPS 0x20
#define BOARD_PM_WLAN_STA_IDLE_TIMEOUT 15
#define BOARD_PM_WLAN_IDLE_TIMEOUT 120
#define BOARD_PM_WLAN_DEFAULT_TIMEOUT 0
#define EXPORT_SYMBOL(x)
struct net_device {int unused;};
struct ieee80211_channel {u32 ic_flags;int ic_ieee;};
struct ieee80211vap {int iv_opmode,iv_state;struct ieee80211vap *iv_next;struct net_device*iv_dev;};
struct vaphead {struct ieee80211vap*first;};
struct ieee80211com {struct vaphead ic_vaps;struct ieee80211_channel*ic_bsschan;u32 om_coc_move_ndfs,om_coc_last_switch,om_coc_pm_trigger,ic_csa_reason;struct {unsigned ocac_running;}ic_ocac;unsigned ic_sta_assoc,ic_flags_qtn,ic_opmode;bool(*ic_dfs_is_eu_region)(void);int(*ic_check_channel)(struct ieee80211com*,struct ieee80211_channel*,int,int);int pm_work;unsigned om_coc_queue_lock;bool om_coc_stopping,om_coc_completion_pending,om_coc_pm_pending;int om_coc_completed_level;};
#define TAILQ_FIRST(h) ((h)->first)
#define TAILQ_FOREACH(v,h,next) for((v)=(h)->first;(v);(v)=(v)->next)
static __declspec(thread) unsigned irqdepth;static unsigned irqentered,irqrestored;static CRITICAL_SECTION qlock;
#define local_irq_save(flags) do{(flags)=irqdepth;irqdepth++;irqentered++;}while(0)
#define local_irq_restore(flags) do{assert(irqdepth==(flags)+1);irqdepth=(flags);irqrestored++;}while(0)
#define spin_lock_irqsave(lock,flags) do{EnterCriticalSection(&qlock);assert(*(lock)==0);*(lock)=1;local_irq_save(flags);}while(0)
#define spin_unlock_irqrestore(lock,flags) do{assert(*(lock)==1);*(lock)=0;local_irq_restore(flags);LeaveCriticalSection(&qlock);}while(0)
static bool eu;static int idle,lookup_kind,check_ok,csa_rc;static unsigned qos_count,pick_count,check_count,csa_count,finish_count,event_count,print_count,queue_count,flush_count;static unsigned long delay_seen;static int qos_level,flags_seen;static unsigned trace;
static void (*last_finish)(uintptr_t);static struct ieee80211_channel picked={0,44};static struct net_device dev;
static bool eu_cb(void){return eu;}
static int ieee80211_is_idle_state(struct ieee80211com*ic){return idle;}
static HANDLE worker_handle,qos_entered,qos_release,flush_entered;static int hold_qos;static LONG queued_runs;
static int pm_qos_update_requirement(int id,const char*name,int level){assert(!irqdepth&&id==4&&!strcmp(name,"wlan"));qos_count++;qos_level=level;trace=trace*10+1;if(hold_qos){SetEvent(qos_entered);assert(WaitForSingleObject(qos_release,5000)==WAIT_OBJECT_0);}return 0;}
static int ieee80211_scs_pick_channel(struct ieee80211com*ic,int flags,int unused){assert(!unused);pick_count++;flags_seen=flags;trace=trace*10+2;return 44;}
static struct ieee80211_channel*ieee80211_find_channel_by_ieee(struct ieee80211com*ic,int ieee){assert(ieee==44);return lookup_kind==0?NULL:(lookup_kind==1?IEEE80211_CHAN_ANYC:&picked);}
static int check_cb(struct ieee80211com*ic,struct ieee80211_channel*c,int a,int b){assert(c==&picked&&!a&&!b);check_count++;trace=trace*10+3;return check_ok;}
static int ieee80211_enter_csa(struct ieee80211com*ic,struct ieee80211_channel*c,void(*finish)(uintptr_t),u32 reason,int count,int mode,u32 flags){assert(c==&picked&&finish&&reason==9&&count==10&&mode==1&&flags==3);csa_count++;trace=trace*10+4;last_finish=finish;ic->ic_csa_reason=9;return csa_rc;}
static void ieee80211_finish_csa(uintptr_t arg){finish_count++;trace=trace*10+5;}
static int printk(const char*fmt,...){print_count++;return 0;}
static int ieee80211_eventf(struct net_device*d,const char*fmt,...){assert(d==NULL||d==&dev);event_count++;return 0;}
static void pm_queue_work(int*p,unsigned long delay){queue_count++;delay_seen=delay;InterlockedIncrement(&queued_runs);}
static void pm_flush_work(int*p){assert(!irqdepth);flush_count++;SetEvent(flush_entered);assert(WaitForSingleObject(worker_handle,5000)==WAIT_OBJECT_0);InterlockedExchange(&queued_runs,0);}
static void om_coc_finish_csa(uintptr_t arg)
{
    struct ieee80211com *ic=(struct ieee80211com *)arg;
    unsigned long flags;
    int level;
    if(READ_ONCE(ic->om_coc_stopping))return;
    level=ieee80211_is_idle_state(ic)?BOARD_PM_LEVEL_IDLE:PM_QOS_DEFAULT_VALUE;
    if(ic->ic_csa_reason!=9U){
        WRITE_ONCE(ic->om_coc_last_switch,0U);
        return;
    }
    ieee80211_finish_csa(arg);
    /* Timer context cannot call the mutex/blocking-notifier QoS provider.
     * Preserve the sampled pre-completion PM level until process work runs. */
    spin_lock_irqsave(&ic->om_coc_queue_lock,flags);
    if(!ic->om_coc_stopping){
        ic->om_coc_completed_level=level;
        ic->om_coc_completion_pending=true;
        pm_queue_work(&ic->pm_work,0);
    }
    spin_unlock_irqrestore(&ic->om_coc_queue_lock,flags);
}

static bool om_coc_publish_completion(struct ieee80211com *ic)
{
    struct ieee80211vap *vap;
    struct ieee80211_channel *chan;
    unsigned long flags;
    int level;
    bool pending;
    spin_lock_irqsave(&ic->om_coc_queue_lock,flags);
    pending=ic->om_coc_completion_pending;
    level=ic->om_coc_completed_level;
    ic->om_coc_completion_pending=false;
    spin_unlock_irqrestore(&ic->om_coc_queue_lock,flags);
    if(!pending)return false;
    pm_qos_update_requirement(PM_QOS_POWER_SAVE,BOARD_PM_GOVERNOR_WLAN,level);
    chan=READ_ONCE(ic->ic_bsschan);
    if(chan&&chan!=IEEE80211_CHAN_ANYC){
        printk("CoC: Channel changed to %d as PM level changed to %d\n",
               chan->ic_ieee,level);
        vap=TAILQ_FIRST(&ic->ic_vaps);
        ieee80211_eventf(vap?vap->iv_dev:NULL,
               "CoC: Channel changed to %d as PM level changed to %d\n",
               chan->ic_ieee,level);
    }
    return true;
}

void om_ieee80211_coc_quiesce(struct ieee80211com *ic)
{
    unsigned long flags;
    spin_lock_irqsave(&ic->om_coc_queue_lock,flags);
    ic->om_coc_stopping=true;
    spin_unlock_irqrestore(&ic->om_coc_queue_lock,flags);
    /* Existing provider is cancel_delayed_work_sync. Producers are gated
     * before cancellation; teardown then joins the preexisting CSA timer. */
    pm_flush_work(&ic->pm_work);
}
EXPORT_SYMBOL(om_ieee80211_coc_quiesce);

static int om_coc_channel_switch(struct ieee80211com *ic,int level,int pick_flags,u32 trigger)
{
    struct ieee80211vap *vap;
    struct ieee80211_channel *chan=READ_ONCE(ic->ic_bsschan);
    unsigned long irqflags;
    bool eu_ready=ic->ic_dfs_is_eu_region();
    bool dfs,move=false;
    if(READ_ONCE(ic->om_coc_stopping))return 0;
    if(!chan||chan==IEEE80211_CHAN_ANYC)return 0;
    if(eu_ready){
        local_irq_save(irqflags);
        TAILQ_FOREACH(vap,&ic->ic_vaps,iv_next){
            if(vap->iv_opmode==IEEE80211_M_STA&&vap->iv_state==IEEE80211_S_RUN){
                eu_ready=false;
                break;
            }
        }
        local_irq_restore(irqflags);
    }
    dfs=!!(chan->ic_flags&IEEE80211_CHAN_DFS);
    if(level==PM_QOS_DEFAULT_VALUE){
        if(READ_ONCE(ic->om_coc_move_ndfs)&&!ic->ic_ocac.ocac_running&&!dfs&&
           eu_ready&&READ_ONCE(ic->om_coc_last_switch)==9U&&
           trigger==13U)move=true;
    }else if(dfs&&eu_ready){
        move=READ_ONCE(ic->om_coc_move_ndfs)!=0U;
    }
    if(!move)return 0;
    chan=ieee80211_find_channel_by_ieee(ic,ieee80211_scs_pick_channel(ic,pick_flags,0));
    if(chan&&chan!=IEEE80211_CHAN_ANYC&&ic->ic_check_channel(ic,chan,0,0))
        return ieee80211_enter_csa(ic,chan,om_coc_finish_csa,9U,10,1,3)==0;
    return 0;
}

static void om_coc_update_pm(struct ieee80211com *ic)
{
    struct ieee80211_channel *chan;
    int level;
    int switched;
    unsigned long flags;
    bool pending;
    u32 trigger;
    if(READ_ONCE(ic->om_coc_stopping))return;
    /* Consume one actual producer request before publishing completion.
     * A producer arriving later owns a newly queued run and stays pending. */
    spin_lock_irqsave(&ic->om_coc_queue_lock,flags);
    pending=ic->om_coc_pm_pending;
    trigger=ic->om_coc_pm_trigger;
    ic->om_coc_pm_pending=false;
    spin_unlock_irqrestore(&ic->om_coc_queue_lock,flags);
    om_coc_publish_completion(ic);
    if(!pending)return;
    chan=READ_ONCE(ic->ic_bsschan);
    /* The OEM worker accesses BSS flags before an invalid-pointer check.
     * Avoid changing QoS, pick state or callback ownership when not ready. */
    if(!chan||chan==IEEE80211_CHAN_ANYC)return;
    if(chan->ic_flags&IEEE80211_CHAN_DFS_CAC_IN_PROGRESS)return;
    level=ieee80211_is_idle_state(ic)?BOARD_PM_LEVEL_IDLE:PM_QOS_DEFAULT_VALUE;
    if(ic->ic_sta_assoc>0&&ic->ic_dfs_is_eu_region()&&
       level!=PM_QOS_DEFAULT_VALUE&&(chan->ic_flags&IEEE80211_CHAN_DFS)){
        switched=om_coc_channel_switch(ic,level,0x12,trigger);
        if(switched)WRITE_ONCE(ic->om_coc_last_switch,9U);
    }else{
        pm_qos_update_requirement(PM_QOS_POWER_SAVE,BOARD_PM_GOVERNOR_WLAN,level);
        switched=om_coc_channel_switch(ic,level,0x14,trigger);
        if(switched)WRITE_ONCE(ic->om_coc_last_switch,0U);
    }
}

static void
om_coc_pm_queue(struct ieee80211com *ic,u32 trigger)
{
	unsigned long delay;
	unsigned long flags;
	int idle;
	if(READ_ONCE(ic->om_coc_stopping))return;
	idle = ieee80211_is_idle_state(ic);

	if (idle) {
#if defined(QBMPS_ENABLE)
		if ((ic->ic_flags_qtn & IEEE80211_QTN_BMPS) &&
		    (ic->ic_opmode & IEEE80211_M_STA))
			delay = BOARD_PM_WLAN_STA_IDLE_TIMEOUT;
		else
#endif
			delay = BOARD_PM_WLAN_IDLE_TIMEOUT;
	} else
		delay = BOARD_PM_WLAN_DEFAULT_TIMEOUT;

	spin_lock_irqsave(&ic->om_coc_queue_lock,flags);
	if(!ic->om_coc_stopping){
		WRITE_ONCE(ic->om_coc_pm_trigger,trigger);
		ic->om_coc_pm_pending=true;
		pm_queue_work(&ic->pm_work,ic->om_coc_completion_pending?0:delay);
	}
	spin_unlock_irqrestore(&ic->om_coc_queue_lock,flags);
}
void ieee80211_pm_queue_work(struct ieee80211com *ic)
{
    om_coc_pm_queue(ic,0U);
}
EXPORT_SYMBOL(ieee80211_pm_queue_work);

void om_ieee80211_pm_queue_node_join(struct ieee80211com *ic)
{
    om_coc_pm_queue(ic,13U);
}
EXPORT_SYMBOL(om_ieee80211_pm_queue_node_join);
static void reset(void){irqdepth=irqentered=irqrestored=qos_count=pick_count=check_count=csa_count=finish_count=event_count=print_count=queue_count=flush_count=trace=0;flags_seen=qos_level=-999;last_finish=NULL;}
static unsigned long tested;

static struct ieee80211com ic;static struct ieee80211_channel bss={0,52};static struct ieee80211vap vap;
static DWORD WINAPI worker(void*p){(void)p;om_coc_update_pm(&ic);return 0;}
static DWORD WINAPI stopper(void*p){(void)p;om_ieee80211_coc_quiesce(&ic);return 0;}
int main(void){InitializeCriticalSection(&qlock);qos_entered=CreateEvent(NULL,TRUE,FALSE,NULL);qos_release=CreateEvent(NULL,TRUE,FALSE,NULL);flush_entered=CreateEvent(NULL,TRUE,FALSE,NULL);ic.ic_bsschan=&bss;ic.ic_vaps.first=&vap;vap.iv_dev=&dev;ic.ic_dfs_is_eu_region=eu_cb;ic.ic_check_channel=check_cb;ic.om_coc_pm_pending=true;ic.om_coc_pm_trigger=13;ic.om_coc_move_ndfs=1;ic.om_coc_last_switch=9;eu=true;idle=0;lookup_kind=2;check_ok=1;hold_qos=1;
worker_handle=CreateThread(NULL,0,worker,NULL,0,NULL);assert(WaitForSingleObject(qos_entered,5000)==WAIT_OBJECT_0);HANDLE stop=CreateThread(NULL,0,stopper,NULL,0,NULL);assert(WaitForSingleObject(flush_entered,5000)==WAIT_OBJECT_0);assert(ic.om_coc_stopping);assert(WaitForSingleObject(stop,100)==WAIT_TIMEOUT);
unsigned before=queue_count;ieee80211_pm_queue_work(&ic);om_ieee80211_pm_queue_node_join(&ic);om_coc_finish_csa((uintptr_t)&ic);assert(queue_count==before&&!finish_count);SetEvent(qos_release);assert(WaitForSingleObject(worker_handle,5000)==WAIT_OBJECT_0);assert(WaitForSingleObject(stop,5000)==WAIT_OBJECT_0);assert(!queued_runs&&!pick_count&&!csa_count&&qos_count==1);om_ieee80211_coc_quiesce(&ic);assert(flush_count==2);puts("{\"running_worker_joined_before_stop_return\":true,\"stopped_producers_refused\":true,\"running_worker_cannot_arm_CSA_after_stop\":true}");return 0;}
