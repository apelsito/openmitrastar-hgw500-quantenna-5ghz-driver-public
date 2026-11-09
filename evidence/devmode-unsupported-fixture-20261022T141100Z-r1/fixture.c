#include <assert.h>
#include <stdint.h>
#define EOPNOTSUPP 95
#define IEEE80211_EXTENDER_ROLE_MBS 1
#define IEEE80211_EXTENDER_ROLE_RBS 2
#define OM_OEM_PARAM_DEVICE_MODE 451
#define OM_OEM_PARAM_IGNORE_ICAC_SELECTION 465
#define OM_OEM_PARAM_FT_OVER_DS 495
struct ieee80211com {uint8_t ic_extender_role;uint32_t flags;};
static int ieee80211_is_repeater(struct ieee80211com *ic){return !!(ic->flags&0x40000);}
static int actual_get(struct ieee80211com *ic,int *param){switch(param[0]&0xffff){
	case OM_OEM_PARAM_DEVICE_MODE:
		if (ic->ic_extender_role == IEEE80211_EXTENDER_ROLE_MBS)
			param[0] = 1;
		else if (ic->ic_extender_role == IEEE80211_EXTENDER_ROLE_RBS)
			param[0] = 2;
		else
			param[0] = ieee80211_is_repeater(ic) ? 3 : 0;
		break;
	case OM_OEM_PARAM_FT_OVER_DS:
		return -EOPNOTSUPP;

 default:break;}return 0;}
static int actual_set(int param){switch(param){
	case OM_OEM_PARAM_IGNORE_ICAC_SELECTION:
		return -EOPNOTSUPP;

 default:break;}return 0;}
int main(void){unsigned role,f,high;int result,expected;struct ieee80211com ic;
for(role=0;role<256;role++)for(f=0;f<2;f++){
ic.ic_extender_role=role;ic.flags=f?0x40000:0;expected=role==1?1:role==2?2:f?3:0;
for(high=0;high<65536;high+=257){result=(high<<16)|451;assert(actual_get(&ic,&result)==0);assert(result==expected);assert(ic.ic_extender_role==role);}}
for(high=0;high<65536;high++){result=(high<<16)|495;assert(actual_get(&ic,&result)==-95);assert(result==(int)((high<<16)|495));}
assert(actual_set(465)==-95);return 0;}
