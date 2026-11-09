#include <assert.h>
#include <stdint.h>
typedef uint8_t u8;
#define OM_OEM_PARAM_DUPLICATE_RTS 498
struct qdrv_wlan{u8 om_duplicate_rts_value;};
static int sent;static unsigned calls;
static void actual_set(struct qdrv_wlan*qw,int param,int value){switch(param){

	case OM_OEM_PARAM_DUPLICATE_RTS:
		qw->om_duplicate_rts_value = (u8)value;
		break;

 default:break;}sent=value;calls++;}
static void actual_get(struct qdrv_wlan*qw,int param,int*value){switch(param){

	case OM_OEM_PARAM_DUPLICATE_RTS:
		*value = qw->om_duplicate_rts_value;
		break;

 default:break;}}
int main(void){unsigned i,high;struct qdrv_wlan q;int result;for(i=0;i<65536;i++)for(high=0;high<256;high+=17){uint32_t v=i|(high<<24);calls=0;actual_set(&q,498,v);result=-1;actual_get(&q,498,&result);assert(calls==1&&(uint32_t)sent==v&&result==(u8)v);}return 0;}
