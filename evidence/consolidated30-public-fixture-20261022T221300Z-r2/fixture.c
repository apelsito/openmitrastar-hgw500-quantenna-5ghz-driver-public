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
struct ieee80211com {int ic_opmode,ic_phymode;uint8_t om_20_40_bss_coex;int ic_obss_timer;void(*ic_coex_stats_update)(struct ieee80211com*,uint32_t);};
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
static int ieee80211_is_40_allowed(struct ieee80211vap*v,int channel){report_calls++;return channel!=report_bad;}
static void om_ieee80211_change_bw(struct ieee80211vap*v,int value,int delay){assert(value==20&&delay==0&&stats==0);bw++;}
static void stats_cb(struct ieee80211com*ic,uint32_t reason){assert(bw==1&&reason==0);stats++;}
static int is_ieee80211_obss_grant(struct ieee80211com*ic,struct ieee80211_node*ni){grant_calls++;return 16;}
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


int main(void){struct ieee80211com ic;struct ieee80211vap v;struct ieee80211_node ni;struct ieee80211_frame wh;uint8_t bytes[300];unsigned len,declared,flags,role,policy,mode,reportkind;unsigned long cases=0;
for(len=0;len<18;len++)for(declared=0;declared<256;declared++)for(flags=0;flags<32;flags++)for(role=0;role<2;role++)for(policy=0;policy<2;policy++)for(mode=0;mode<2;mode++)for(reportkind=0;reportkind<3;reportkind++){
memset(&ic,0,sizeof(ic));memset(&v,0,sizeof(v));memset(bytes,0,sizeof(bytes));ic.ic_opmode=role?1:6;ic.ic_phymode=mode?9:8;ic.om_20_40_bss_coex=policy;ic.ic_coex_stats_update=stats_cb;v.iv_ic=&ic;v.iv_coex=7;ni.ni_vap=&v;ni.ni_coex=0xa5;struct ieee80211_action*ia=(void*)bytes;uint8_t*p=bytes+2;p[0]=72;p[1]=1;p[2]=flags;p[3]=reportkind?73:99;p[4]=declared;p[5]=81;for(unsigned i=6;i<sizeof(bytes)-2;i++)p[i]=(uint8_t)i;report_bad=reportkind==2?6:-1;struct sk_buff skb={bytes,len+2};sends=cancels=bw=stats=report_calls=grant_calls=0;
bool_valid:;
int valid=len>=3;
if(valid&&len>=5&&reportkind){if(declared<1||declared>(len-5))valid=0;}
unsigned expected_reports=0;int report_change=0;if(valid&&!role&&len>=6&&reportkind){unsigned limit=declared-1;for(unsigned i=0;i<limit;i++){expected_reports++;if((uint8_t)(6+i)==report_bad){report_change=1;break;}}}
ieee80211_recv_action_public_coex(&ni,&skb,&wh,ia,0);
if(!valid){assert(ni.ni_coex==0xa5&&v.iv_stats.is_rx_elem_toosmall==1&&!bw&&!stats&&!sends&&!cancels&&!report_calls&&!grant_calls);}
else{assert(ni.ni_coex==flags&&!v.iv_stats.is_rx_elem_toosmall&&!v.iv_stats.is_rx_elem_unknown);if(role){assert(sends==!!(flags&1)&&cancels==!!(flags&16)&&!bw&&!stats&&!report_calls&&!grant_calls);}else{unsigned change=(report_change||!!(flags&6))&&policy&&mode;assert(report_calls==expected_reports&&bw==change&&stats==change&&sends==!!(flags&8)&&grant_calls==!!(flags&8)&&!cancels);}}
cases++;}
assert(cases==3538944UL);
for(unsigned n=0;n<256;n++){memset(&ic,0,sizeof(ic));memset(&v,0,sizeof(v));v.iv_ic=&ic;ni.ni_vap=&v;ni.ni_coex=123;memset(bytes,0,sizeof(bytes));bytes[2]=72;bytes[3]=n;bytes[4]=6;struct sk_buff skb={bytes,5};sends=cancels=bw=stats=0;ieee80211_recv_action_public_coex(&ni,&skb,&wh,(void*)bytes,0);if(n!=1)assert(ni.ni_coex==123&&v.iv_stats.is_rx_elem_unknown==1&&!bw&&!sends);}
return 0;}
