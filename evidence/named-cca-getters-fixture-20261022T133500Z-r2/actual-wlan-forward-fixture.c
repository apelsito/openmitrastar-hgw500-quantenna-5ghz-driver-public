#include <assert.h>
#include <stdint.h>
static int observed;
static void ieee80211_param_from_qdrv(void *vap,int id,int *out,void *data,int len) {observed=id;*out=77;}
static void actual_wlan_getter(int *param) {void *vap=0;switch(param[0]&0xffff) {
case 331:
	case 275:
		ieee80211_param_from_qdrv(vap, param[0] & 0xffff, &param[0], NULL, 0);
		break;

 default: break;}}
int main(void) {unsigned high;int result;for(high=0;high<65536;high++) {
result=(high<<16)|331;observed=-1;actual_wlan_getter(&result);assert(observed==331 && result==77);
result=(high<<16)|275;observed=-1;actual_wlan_getter(&result);assert(observed==275 && result==77);
}return 0;}
