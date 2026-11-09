#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u_int8_t;typedef uint8_t uint8_t;
#define READ_ONCE(x) (x)
#define IEEE80211_M_STA 1
#define IEEE80211_ELEMID_20_40_BSS_COEX 72
#define IEEE80211_ELEMID_20_40_IT_CH_REP 73
#define WLAN_20_40_BSS_COEX_INFO_REQ 1
#define WLAN_20_40_BSS_COEX_40MHZ_INTOL 2
#define WLAN_20_40_BSS_COEX_20MHZ_WIDTH_REQ 4
#define WLAN_20_40_BSS_COEX_OBSS_EXEMPT_REQ 8
#define WLAN_20_40_BSS_COEX_OBSS_EXEMPT_GRNT 16
#define IEEE80211_ACTION_CAT_PUBLIC 4
#define IEEE80211_ACTION_PUB_20_40_COEX 0
#define IEEE80211_FC0_SUBTYPE_ACTION 0xd0
#define WLAN_COEX_STATS_BW_ACTION 0
#define BW_HT20 20
#define IEEE80211_MSG_INPUT 0
#define IEEE80211_DISCARD(...) do{}while(0)
struct ieee80211_channel {uint16_t ic_freq;uint32_t ic_flags;uint8_t ic_ieee;};
struct ieee80211com {struct ieee80211_channel*ic_bsschan,*ic_curchan;int ic_opmode,ic_phymode;uint8_t om_20_40_bss_coex;int ic_obss_timer;void(*ic_coex_stats_update)(struct ieee80211com*,uint32_t);};
struct ieee80211vap {struct ieee80211com*iv_ic;uint8_t iv_coex;struct {unsigned is_rx_elem_toosmall,is_rx_elem_unknown;}iv_stats;};
struct ieee80211_node {struct ieee80211vap*ni_vap;uint8_t ni_coex;};
struct sk_buff {uint8_t*data;unsigned len;};
struct ieee80211_frame {int unused;};
struct ieee80211_action {uint8_t ia_category,ia_action;};
struct ieee80211_20_40_coex_param {uint8_t param_id,param_len,coex_param;};
struct ieee80211_20_40_in_ch_rep {uint8_t param_id,param_len,reg;uint8_t chan[0];};
struct ieee80211_action_data {int cat,action;void*params;};
#define IEEE80211_IS_11NG_40(ic) ((ic)->ic_phymode==9)
static unsigned sends,cancels,bw,stats,report_calls,grant_calls;static int report_bad;
#define IEEE80211_SEND_MGMT(ni,type,arg) do{assert((type)==0xd0);sends++;}while(0)
static int del_timer_sync(int*p){cancels++;return 0;}

static void om_ieee80211_change_bw(struct ieee80211vap*v,int value,int delay){assert(value==20&&delay==0&&stats==0);bw++;}
static void stats_cb(struct ieee80211com*ic,uint32_t reason){assert(bw==1&&reason==0);stats++;}
static int is_ieee80211_obss_grant(struct ieee80211com*ic,struct ieee80211_node*ni){grant_calls++;return 16;}
#include <stdbool.h>
typedef unsigned u_int;
#define IEEE80211_CHAN_2GHZ 0x80U
#define IEEE80211_CHAN_5GHZ 0x100U
#define IEEE80211_CHAN_HT40U 0x20000U
#define IEEE80211_CHAN_HT40D 0x40000U
#define IEEE80211_SEC_CHAN_OFFSET 4
#define IEEE80211_CHAN_SPACE 5
#define IEEE80211_BW_RANGE 25
#define IEEE80211_CHAN_ANYC ((struct ieee80211_channel*)(uintptr_t)0xffff)
u_int
ieee80211_ieee2mhz(u_int chan, u_int flags)
{
	if (flags & IEEE80211_CHAN_2GHZ) {	/* 2GHz band */
		if (chan == 14)
			return 2484;
		if (chan < 14)
			return 2407 + chan * 5;
		else
			return 2512 + ((chan - 15) * 20);
	} else if (flags & IEEE80211_CHAN_5GHZ) {	/* 5Ghz band */
		return 5000 + (chan * 5);
	} else {					/* either, guess */
		if (chan == 14)
			return 2484;
		if (chan < 14)			/* 0-13 */
			return 2407 + chan * 5;
		if (chan < 27)			/* 15-26 */
			return 2512 + ((chan - 15) * 20);
		return 5000 + (chan * 5);
	}
}

int
ieee80211_20_40_operation_permitted(struct ieee80211vap *vap,
			uint8_t se_pri_chan, uint8_t se_sec_chan)
{
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_channel *bss = READ_ONCE(ic->ic_bsschan);
	uint16_t affected_start;
	uint16_t affected_end;
	uint8_t pri_chan;
	uint8_t sec_chan;
	uint16_t pri_freq;
	uint16_t sec_freq;
	uint16_t se_pri_freq;
	uint16_t se_sec_freq;

	/* OEM449 uses BSS geometry even while a scan has another current channel. */
	if (!bss || bss == IEEE80211_CHAN_ANYC)
		return 0;
	pri_chan = bss->ic_ieee;
	pri_freq = bss->ic_freq;
	if (bss->ic_flags & IEEE80211_CHAN_HT40U) {
		sec_chan = pri_chan + IEEE80211_SEC_CHAN_OFFSET;
		sec_freq = pri_freq + IEEE80211_SEC_CHAN_OFFSET * IEEE80211_CHAN_SPACE;
	} else if (bss->ic_flags & IEEE80211_CHAN_HT40D) {
		sec_chan = pri_chan - IEEE80211_SEC_CHAN_OFFSET;
		sec_freq = pri_freq - IEEE80211_SEC_CHAN_OFFSET * IEEE80211_CHAN_SPACE;
	} else {
		return 0;
	}

	/* Finding the frquency range */
	affected_start = ((pri_freq + sec_freq) >> 1) - IEEE80211_BW_RANGE;
	affected_end = ((pri_freq + sec_freq) >> 1) + IEEE80211_BW_RANGE;

	se_pri_freq = ieee80211_ieee2mhz((int8_t)se_pri_chan, 0);
	se_sec_freq = (se_sec_chan != 0) ? ieee80211_ieee2mhz((int8_t)se_sec_chan, 0) : 0;
	if (((se_pri_freq > affected_start) && (se_pri_freq < affected_end)) ||
			((se_sec_freq > affected_start) && (se_sec_freq < affected_end))) {
		if (((int)pri_chan == (int8_t)se_pri_chan) && (se_sec_chan == 0))
		      /*
		       * The scanned 20M bandwidth AP shares same channel
		       * with the primary channel of current AP
		       */
		      return 1;
		else if (((int)pri_chan == (int8_t)se_pri_chan) && ((int8_t)sec_chan == (int8_t)se_sec_chan))
		      /*
		       * The scanned 40M bandwidth AP shares same primary channel
		       * and secondary channel with current AP
		       */
		      return 1;
		else
		      return 0;
	}

	return 1;
}

static int
ieee80211_is_40_allowed(struct ieee80211vap *vap, int channel)
{
	struct ieee80211com *ic = vap->iv_ic;
	struct ieee80211_channel *bss = READ_ONCE(ic->ic_bsschan);
	uint16_t ch_freq;
	uint16_t affected_start;
	uint16_t affected_end;
	uint16_t pri_freq;
	uint16_t sec_freq;

	if (!bss || bss == IEEE80211_CHAN_ANYC)
		return 0;
	ch_freq = ieee80211_ieee2mhz(channel, 0);
	pri_freq = bss->ic_freq;

	if (bss->ic_flags & IEEE80211_CHAN_HT40U)
		sec_freq = pri_freq + 20;
	else if (bss->ic_flags & IEEE80211_CHAN_HT40D)
		sec_freq = pri_freq - 20;
	else {
		return 0;
	}

	/* Finding the frquency range */
	affected_start = ((pri_freq + sec_freq) >> 1) - IEEE80211_BW_RANGE;
	affected_end = ((pri_freq + sec_freq) >> 1) + IEEE80211_BW_RANGE;

	if ((ch_freq < affected_start || ch_freq > affected_end))
		return 1;

	return 0;
}


static void
ieee80211_recv_action_public_coex(struct ieee80211_node *ni, struct sk_buff *skb,
	struct ieee80211_frame *wh, struct ieee80211_action *ia, int rssi)
{
	struct ieee80211vap *vap = ni->ni_vap;
	struct ieee80211com *ic = vap->iv_ic;
	uint8_t *frm = (u_int8_t *)(ia + 1);
	uint8_t *efrm = skb->data + skb->len;
	struct ieee80211_20_40_coex_param *coex_ie;
	struct ieee80211_20_40_in_ch_rep *ch_rep_ie;
	int change_bw = 0;
	int i;

	if (efrm - frm < sizeof(struct ieee80211_20_40_coex_param)) {
		IEEE80211_DISCARD(vap, IEEE80211_MSG_INPUT,
			wh, "mgt", "%s", "20/40 coexistence action frame header too small");
		vap->iv_stats.is_rx_elem_toosmall++;
		return;
	}
	coex_ie = (struct ieee80211_20_40_coex_param *)frm;
	if (coex_ie->param_id != IEEE80211_ELEMID_20_40_BSS_COEX ||
	    coex_ie->param_len != 1) {
		vap->iv_stats.is_rx_elem_unknown++;
		return;
	}
	/* A present channel report owns its declared bytes before node mutation. */
	if (efrm - (u_int8_t *)(coex_ie + 1) >= 2 &&
	    *((u_int8_t *)(coex_ie + 1)) == IEEE80211_ELEMID_20_40_IT_CH_REP) {
		ch_rep_ie = (struct ieee80211_20_40_in_ch_rep *)(coex_ie + 1);
		if (ch_rep_ie->param_len < 1 ||
		    ch_rep_ie->param_len > efrm - (u_int8_t *)ch_rep_ie - 2) {
			vap->iv_stats.is_rx_elem_toosmall++;
			return;
		}
	}
	ni->ni_coex =  coex_ie->coex_param;
	frm = (u_int8_t *)(coex_ie + 1);


	if (ic->ic_opmode == IEEE80211_M_STA) {
		if (coex_ie->coex_param & WLAN_20_40_BSS_COEX_INFO_REQ) {
			u_int8_t coex = vap->iv_coex;
			struct ieee80211_action_data action_data;
			action_data.cat = IEEE80211_ACTION_CAT_PUBLIC;
			action_data.action = IEEE80211_ACTION_PUB_20_40_COEX;
			action_data.params = &coex;

			IEEE80211_SEND_MGMT(ni, IEEE80211_FC0_SUBTYPE_ACTION, (int)&action_data);
		}
		if (coex_ie->coex_param & WLAN_20_40_BSS_COEX_OBSS_EXEMPT_GRNT)
			del_timer_sync(&ic->ic_obss_timer);
		return;
	}

	/* For element id and length */
	if (efrm - frm >= 3) {
		if (*frm == IEEE80211_ELEMID_20_40_IT_CH_REP) {
			ch_rep_ie = (struct ieee80211_20_40_in_ch_rep *)frm;
			for (i = 0;i < ch_rep_ie->param_len - 1;i++) {
				if (!ieee80211_is_40_allowed(vap, ch_rep_ie->chan[i])) {
					change_bw = 1;
					break;
				}
			}
		}
	}

	if (coex_ie->coex_param &
		(WLAN_20_40_BSS_COEX_40MHZ_INTOL | WLAN_20_40_BSS_COEX_20MHZ_WIDTH_REQ)) {
		change_bw = 1;
	}

	if (change_bw && IEEE80211_IS_11NG_40(ic) &&
	    READ_ONCE(ic->om_20_40_bss_coex)) {
		om_ieee80211_change_bw(vap,BW_HT20,0);
		ic->ic_coex_stats_update(ic, WLAN_COEX_STATS_BW_ACTION);
	}

	if (coex_ie->coex_param & WLAN_20_40_BSS_COEX_OBSS_EXEMPT_REQ) {
		struct ieee80211_action_data action_data;
		uint8_t coex_value = 0;
		action_data.cat = IEEE80211_ACTION_CAT_PUBLIC;
		action_data.action = IEEE80211_ACTION_PUB_20_40_COEX;

		coex_value |= is_ieee80211_obss_grant(ic, ni);
		action_data.params = &coex_value;

		IEEE80211_SEND_MGMT(ni, IEEE80211_FC0_SUBTYPE_ACTION, (int)&action_data);
	}
}


static uint16_t ref_freq(unsigned c){if(c==14)return 2484;if(c<14)return(uint16_t)(2407U+c*5U);if(c<27)return(uint16_t)(2512U+(c-15U)*20U);return(uint16_t)(5000U+c*5U);}
static bool report_conflict(struct ieee80211_channel*b,unsigned c){unsigned sec;if(b->ic_flags&0x20000)sec=(b->ic_freq+20U)&65535;else if(b->ic_flags&0x40000)sec=(b->ic_freq-20U)&65535;else return true;unsigned center=(b->ic_freq+sec)/2,lo=(center-25U)&65535,hi=(center+25U)&65535,f=ref_freq(c);return f>=lo&&f<=hi;}
int main(void){struct ieee80211com ic;struct ieee80211vap v;struct ieee80211_node ni;struct ieee80211_frame wh;struct ieee80211_channel bss,current={9999,0,99};uint8_t bytes[16];unsigned frequencies[]={0,19,20,2407,2412,2422,2437,2462,2484,65510,65535};unsigned long tested=0;
for(unsigned fi=0;fi<11;fi++)for(unsigned bwflags=0;bwflags<4;bwflags++)for(unsigned channel=0;channel<256;channel++)for(unsigned coex=0;coex<32;coex++)for(unsigned policy=0;policy<2;policy++)for(unsigned mode=0;mode<2;mode++){
memset(&ic,0,sizeof(ic));memset(&v,0,sizeof(v));memset(bytes,0,sizeof(bytes));bss.ic_freq=frequencies[fi];bss.ic_flags=(bwflags&1?0x20000:0)|(bwflags&2?0x40000:0);bss.ic_ieee=6;ic.ic_bsschan=&bss;ic.ic_curchan=&current;ic.ic_opmode=6;ic.ic_phymode=mode?9:8;ic.om_20_40_bss_coex=policy;ic.ic_coex_stats_update=stats_cb;v.iv_ic=&ic;ni.ni_vap=&v;ni.ni_coex=0xa5;bytes[2]=72;bytes[3]=1;bytes[4]=coex;bytes[5]=73;bytes[6]=2;bytes[7]=81;bytes[8]=channel;struct sk_buff skb={bytes,9};sends=cancels=bw=stats=report_calls=grant_calls=0;
ieee80211_recv_action_public_coex(&ni,&skb,&wh,(void*)bytes,0);bool downgrade=policy&&mode&&((coex&6)||report_conflict(&bss,channel));assert(ni.ni_coex==coex&&!v.iv_stats.is_rx_elem_toosmall&&!v.iv_stats.is_rx_elem_unknown&&bw==downgrade&&stats==downgrade&&sends==!!(coex&8)&&grant_calls==!!(coex&8));tested++;}
assert(tested==1441792UL);return 0;}
