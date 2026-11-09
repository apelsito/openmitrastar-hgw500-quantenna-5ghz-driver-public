#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#define OM_OEM_PARAM_DYNAMIC_SIFS 463
static int observedid,observedvalue;static unsigned count;
static void ieee80211_param_to_qdrv(void*v,int id,int value,void*d,int n){observedid=id;observedvalue=value;count++;assert(d==NULL&&n==0);}
static void actual(int param,int value){void*vap=0;switch(param){

	/* ExactOEM463 genericwire forwarding; MuC validatesselectortriplet. */
	case OM_OEM_PARAM_DYNAMIC_SIFS:
		ieee80211_param_to_qdrv(vap, param, value, NULL, 0);
		break;

 default:break;}}
int main(void){unsigned a,b,c;for(a=0;a<256;a++)for(b=0;b<256;b++)for(c=0;c<3;c++){uint32_t v=(a<<24)|(b<<16)|c;count=0;actual(463,v);assert(count==1&&observedid==463&&(uint32_t)observedvalue==v);}return 0;}
