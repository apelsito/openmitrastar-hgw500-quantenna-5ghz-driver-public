#include <assert.h>
#include <stdint.h>
#define OM_OEM_PARAM_BBDF_WAR 483
struct qdrv_wlan {int om_bbdf_war_enabled;};
static void actual_set(struct qdrv_wlan*qw,int param,int value){switch(param){
	case OM_OEM_PARAM_BBDF_WAR:
		qw->om_bbdf_war_enabled = !!value;
		break;

 default:break;}}
static void actual_get(struct qdrv_wlan*qw,int param,int*value){switch(param){
	case OM_OEM_PARAM_BBDF_WAR:
		*value = !!qw->om_bbdf_war_enabled;
		break;

	/* OEM read-only getters331/275 use these existing named fields.
	 * The port compiler ABI witness is independent of OEM numeric offsets. */
 default:break;}}
int main(void){struct qdrv_wlan q;unsigned i;int value;for(i=0;i<65536;i++){actual_set(&q,483,(int)i);value=-1;actual_get(&q,483,&value);assert(value==!!i);actual_set(&q,483,-(int)i);value=-1;actual_get(&q,483,&value);assert(value==!!i);}return 0;}
