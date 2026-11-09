#include <assert.h>
#include <stdint.h>
typedef uint8_t u_int8_t;
#define OM_OEM_PARAM_MAX_BANDWIDTH 460
struct ieee80211com{uint8_t ic_max_system_bw;};
static void actual_set(struct ieee80211com*ic,int param,int value){switch(param){
	case OM_OEM_PARAM_MAX_BANDWIDTH:
		ic->ic_max_system_bw = (u_int8_t)value;
		break;

 default:break;}}
static void actual_get(struct ieee80211com*ic,int*param){switch(param[0]&0xffff){
	case OM_OEM_PARAM_MAX_BANDWIDTH:
		param[0] = ic->ic_max_system_bw;
		break;

 default:break;}}
int main(void){struct ieee80211com ic;unsigned i;int result;for(i=0;i<65536;i++){
actual_set(&ic,460,i);assert(ic.ic_max_system_bw==(uint8_t)i);result=(i<<16)|460;actual_get(&ic,&result);assert(result==(uint8_t)i);
actual_set(&ic,460,-(int)i);assert(ic.ic_max_system_bw==(uint8_t)(-(int)i));}return 0;}
