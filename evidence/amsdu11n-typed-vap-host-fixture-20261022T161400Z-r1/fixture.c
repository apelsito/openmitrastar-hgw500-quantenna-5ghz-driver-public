#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#define OM_OEM_PARAM_TX_AMSDU_11N 484
#define IEEE80211_NODE_HT 0x8
#define IEEE80211_NODE_VHT 0x4000
#define IEEE80211_A_BA_AMSDU_SUPPORTED 1
struct ieee80211vap{uint8_t iv_tx_amsdu,iv_tx_amsdu_11n;};
struct ieee80211_node{uint32_t ni_flags;};
static int sent_id,sent_value;static unsigned sends,reassocs;
static void ieee80211_param_to_qdrv(struct ieee80211vap*v,int p,int val,void*d,int n){sent_id=p;sent_value=val;sends++;assert(d==NULL&&n==0);}
static void ieee80211_wireless_reassoc(struct ieee80211vap*v,int a,int b){reassocs++;assert(a==0&&b==1);}
static void actual_set(struct ieee80211vap*vap,int param,int value){switch(param){
	case OM_OEM_PARAM_TX_AMSDU_11N:
		/* ExactOEMchangedboolpath; fulloriginalvalue reachesMuCparam484. */
		if (vap->iv_tx_amsdu_11n != !!value) {
			vap->iv_tx_amsdu_11n = !!value;
			ieee80211_param_to_qdrv(vap, param, value, NULL, 0);
			ieee80211_wireless_reassoc(vap, 0, 1);
		}
		break;

 default:break;}}
static void actual_get(struct ieee80211vap*vap,int*param){switch(param[0]&0xffff){
	case OM_OEM_PARAM_TX_AMSDU_11N:
		param[0] = vap->iv_tx_amsdu_11n;
		break;

 default:break;}}
static uint16_t actual_addba(struct ieee80211vap*vap,struct ieee80211_node*ni,uint16_t baseline){uint16_t temp16=baseline;
				if (vap->iv_tx_amsdu &&
				    (((ni->ni_flags & (IEEE80211_NODE_HT | IEEE80211_NODE_VHT))
				       != IEEE80211_NODE_HT) || vap->iv_tx_amsdu_11n))
					temp16 |= IEEE80211_A_BA_AMSDU_SUPPORTED;
return temp16;}
int main(void){unsigned flags,base,n;int vals[]={0,1,2,255,-1,INT32_MIN,INT32_MAX};unsigned i;int result;struct ieee80211vap v;struct ieee80211_node ni;
v.iv_tx_amsdu=1;v.iv_tx_amsdu_11n=1;assert(v.iv_tx_amsdu==1&&v.iv_tx_amsdu_11n==1);
for(base=0;base<2;base++)for(n=0;n<2;n++)for(flags=0;flags<65536;flags++){
v.iv_tx_amsdu=base;v.iv_tx_amsdu_11n=n;ni.ni_flags=flags;uint16_t before=0xa5a0;uint16_t out=actual_addba(&v,&ni,before);int expected=base&&(((flags&0x4008)!=8)||n);assert(out==(uint16_t)(before|(expected?1:0)));assert(v.iv_tx_amsdu==base&&v.iv_tx_amsdu_11n==n);}
for(base=0;base<2;base++)for(n=0;n<2;n++)for(i=0;i<sizeof(vals)/sizeof(vals[0]);i++){
v.iv_tx_amsdu=base;v.iv_tx_amsdu_11n=n;sends=0;reassocs=0;actual_set(&v,484,vals[i]);assert(v.iv_tx_amsdu==base&&v.iv_tx_amsdu_11n==!!vals[i]);assert(sends==(n!=!!vals[i])&&reassocs==sends);if(sends)assert(sent_id==484&&sent_value==vals[i]);result=484;actual_get(&v,&result);assert(result==!!vals[i]);}
/* Explicitcounterexamples: disabling11n leavesVHT/legacybasepolicy untouched. */
v.iv_tx_amsdu=1;v.iv_tx_amsdu_11n=0;ni.ni_flags=8;assert(actual_addba(&v,&ni,0)==0);ni.ni_flags=0x4008;assert(actual_addba(&v,&ni,0)==1);v.iv_tx_amsdu=0;assert(actual_addba(&v,&ni,0)==0);return 0;}
