
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>
typedef unsigned spinlock_t;static unsigned locks;
#define READ_ONCE(x) (x)
#define WRITE_ONCE(x,v) ((x)=(v))
#define spin_lock_irqsave(l,f) do{(f)=0;(*(l))++;locks++;}while(0)
#define spin_unlock_irqrestore(l,f) do{(void)(f);assert(*(l));(*(l))--;locks--;}while(0)
#define ISCAN_CANCEL 4
#define ISCAN_START 8
#define IEEE80211_F_SCAN 128
#define IEEE80211_PARAM_BEACON_ALLOW 0
struct timer_list{volatile LONG pending;};struct tasklet_struct{unsigned unused;};
struct ieee80211com;struct ieee80211_scan_state;
struct ieee80211_scanner{void(*scan_detach)(struct ieee80211_scan_state*);};
struct ieee80211vap{struct ieee80211com*iv_ic;void*iv_bss;};
struct ieee80211_scan_state{struct ieee80211vap*ss_vap;const struct ieee80211_scanner*ss_ops;void*ss_priv;};
struct scan_state{struct ieee80211_scan_state base;bool om_chanset_stopping;unsigned char om_scan_detaching;spinlock_t om_chanset_lock;struct ieee80211vap*om_chanset_owner;unsigned ss_iflags,ss_scanend,ss_duration;struct timer_list ss_scan_timer,ss_probe_timer;struct tasklet_struct ss_pwrsav;};
struct ieee80211com{struct ieee80211_scan_state*ic_scan;spinlock_t ic_comlock;unsigned ic_flags;void(*ic_setparam)(void*,int,int,void*,int);void(*ic_scan_start)(struct ieee80211com*);};
#define SCAN_PRIVATE(s) ((struct scan_state*)(s))
static HANDLE entered,resume,thread;static unsigned joinedscan,joinedprobe,taskletjoined;static unsigned long jiffies;
static void ieee80211_sta_pwrsave(struct ieee80211vap*v,int e){(void)v;(void)e;SetEvent(entered);assert(WaitForSingleObject(resume,5000)==WAIT_OBJECT_0);}
static int msecs_to_jiffies(int v){return v;}
static void mod_timer(struct timer_list*t,unsigned long when){(void)when;InterlockedExchange(&t->pending,1);}
static int del_timer_sync(struct timer_list*t){assert(!locks);InterlockedExchange(&t->pending,0);joinedscan++;return 0;}
static void tasklet_kill(struct tasklet_struct*t){(void)t;assert(!locks);SetEvent(resume);assert(WaitForSingleObject(thread,5000)==WAIT_OBJECT_0);taskletjoined++;}
static void om_chanset_kernel_cancel(struct ieee80211com*ic){SCAN_PRIVATE(ic->ic_scan)->om_chanset_owner=NULL;}
static void setparam(void*n,int p,int v,void*d,int s){(void)n;(void)p;(void)v;(void)d;(void)s;}
static void scanstart(struct ieee80211com*ic){(void)ic;}
static void scan_restart_pwrsav(uintptr_t arg)
{
	struct scan_state *ss = (struct scan_state *) arg;
	struct ieee80211vap *vap = READ_ONCE(ss->base.ss_vap);
	struct ieee80211com *ic;
	int delay;
	if(READ_ONCE(ss->om_chanset_stopping)||!vap)return;
	ic=vap->iv_ic;

	ieee80211_sta_pwrsave(vap, 1);
	/*
	 * Use an initial 1ms delay to ensure the null
	 * data frame has a chance to go out.
	 * XXX 1ms is a lot, better to trigger scan
	 * on tx complete.
	 */
	delay = msecs_to_jiffies(1);
	if (delay < 1)
		delay = 1;

	ic->ic_setparam(vap->iv_bss, IEEE80211_PARAM_BEACON_ALLOW,
			1, NULL, 0);
	ic->ic_scan_start(ic);			/* notify driver */
	ss->ss_scanend = jiffies + delay + ss->ss_duration;
	ss->ss_iflags |= ISCAN_START;
	mod_timer(&ss->ss_scan_timer, jiffies + delay);
	/*
	 * FIXME: Note, we are not delaying probes at the start here so there
	 * may be issues with probe requests not being on the correct
	 * channel for the first channel scanned.
	 */
}void ieee80211_scan_vdetach(struct ieee80211vap *vap)
{
    struct ieee80211com *ic=vap->iv_ic;
    struct ieee80211_scan_state *ss=ic->ic_scan;
    struct scan_state *scan;
    const struct ieee80211_scanner *policy;
    unsigned long flags,ic_irq_flags;
    if(!ss)return;
    scan=SCAN_PRIVATE(ss);
    spin_lock_irqsave(&ic->ic_comlock,ic_irq_flags);
    if(ss->ss_vap!=vap){spin_unlock_irqrestore(&ic->ic_comlock,ic_irq_flags);return;}
    WRITE_ONCE(scan->om_scan_detaching,1);
    WRITE_ONCE(scan->om_chanset_stopping,true);
    scan->ss_iflags|=ISCAN_CANCEL;
    ic->ic_flags&=~IEEE80211_F_SCAN;
#ifdef QTN_BG_SCAN
    ic->ic_flags_qtn&=~IEEE80211_QTN_BGSCAN;
#endif
    spin_unlock_irqrestore(&ic->ic_comlock,ic_irq_flags);
    /* Synchronous joins run outside the IC spinlock: callbacks need that lock.
     * Existing VAP teardown is serialized by its owning process/lifetime gate. */
    tasklet_kill(&scan->ss_pwrsav);
    del_timer_sync(&scan->ss_scan_timer);
    del_timer_sync(&scan->ss_probe_timer);
    om_chanset_kernel_cancel(ic);
    policy=ss->ss_ops;
    if(policy)policy->scan_detach(ss);
    spin_lock_irqsave(&ic->ic_comlock,ic_irq_flags);
    ss->ss_ops=NULL;ss->ss_priv=NULL;ss->ss_vap=NULL;
    spin_lock_irqsave(&scan->om_chanset_lock,flags);
    scan->om_chanset_owner=NULL;
    WRITE_ONCE(scan->om_scan_detaching,0);
    WRITE_ONCE(scan->om_chanset_stopping,false);
    spin_unlock_irqrestore(&scan->om_chanset_lock,flags);
    spin_unlock_irqrestore(&ic->ic_comlock,ic_irq_flags);
}
static struct scan_state scan;static DWORD WINAPI run(LPVOID p){scan_restart_pwrsav((uintptr_t)p);return 0;}
int main(void){struct ieee80211com ic={0};struct ieee80211vap vap={0};ic.ic_scan=&scan.base;ic.ic_setparam=setparam;ic.ic_scan_start=scanstart;vap.iv_ic=&ic;scan.base.ss_vap=&vap;scan.om_chanset_owner=&vap;scan.ss_duration=10;entered=CreateEvent(NULL,TRUE,FALSE,NULL);resume=CreateEvent(NULL,TRUE,FALSE,NULL);thread=CreateThread(NULL,0,run,&scan,0,NULL);assert(WaitForSingleObject(entered,5000)==WAIT_OBJECT_0);
ieee80211_scan_vdetach(&vap);assert(taskletjoined==1&&joinedscan==2&&scan.ss_scan_timer.pending==0&&scan.base.ss_vap==NULL&&!scan.om_chanset_stopping&&!locks);
printf("{\"cases\":1,\"timer_pending_after_actual_vdetach\":false,\"tasklet_joined\":true}\n");CloseHandle(thread);CloseHandle(entered);CloseHandle(resume);return 0;}
