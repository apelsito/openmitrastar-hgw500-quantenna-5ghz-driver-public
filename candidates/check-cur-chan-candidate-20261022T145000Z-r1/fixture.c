#include <assert.h>
#include <stdint.h>
#define OM_OEM_PARAM_CHECK_CURRENT_CHANNEL 464
struct ieee80211com {int ic_chan_is_set;};
static void actual(struct ieee80211com*ic,int param,int value){switch(param){

	case OM_OEM_PARAM_CHECK_CURRENT_CHANNEL:
		ic->ic_chan_is_set = value;
		break;

 default:break;}}
int main(void){struct ieee80211com ic;unsigned i;for(i=0;i<65536;i++){actual(&ic,464,i);assert(ic.ic_chan_is_set==(int)i);actual(&ic,464,-(int)i);assert(ic.ic_chan_is_set==-(int)i);}actual(&ic,464,INT32_MIN);assert(ic.ic_chan_is_set==INT32_MIN);actual(&ic,464,INT32_MAX);assert(ic.ic_chan_is_set==INT32_MAX);return 0;}
