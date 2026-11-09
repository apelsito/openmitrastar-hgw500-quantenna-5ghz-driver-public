#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8;typedef uint32_t u32;
#define OM_OEM_PARAM_NEIGHBOR_REPORT 492
struct ieee80211com{u32 om_neighbor_report_flags;};
static unsigned updates;
static void ieee80211_beacon_update_all(struct ieee80211com*ic){updates++;}
static inline u8 *om_neighbor_rrm_ie(u8 *frm,u32 selected)
{
    size_t n;
    for(n=0;n<7;n++)frm[n]=0;
    frm[0]=70;frm[1]=5;frm[2]=(selected&1U)?2U:0U;
    return frm+7;
}

static void actual_set(struct ieee80211com*ic,int param,int value){switch(param){
	case OM_OEM_PARAM_NEIGHBOR_REPORT:
		if(value == 1)ic->om_neighbor_report_flags |= 1U;
		else ic->om_neighbor_report_flags &= ~1U;
		ieee80211_beacon_update_all(ic);
		break;

 default:break;}}
static void actual_get(struct ieee80211com*ic,int*param){switch(param[0]&0xffff){
	case OM_OEM_PARAM_NEIGHBOR_REPORT:
		param[0] = ic->om_neighbor_report_flags & 1U;
		break;

 default:break;}}
int main(void){struct ieee80211com ic;u8 bytes[9],before[9];unsigned i;int result;
for(i=0;i<65536;i++){ic.om_neighbor_report_flags=i;updates=0;actual_set(&ic,492,1);assert(ic.om_neighbor_report_flags==(i|1U)&&updates==1);result=(i<<16)|492;actual_get(&ic,&result);assert(result==1);memset(bytes,0xa5,sizeof(bytes));assert(om_neighbor_rrm_ie(bytes+1,ic.om_neighbor_report_flags)==bytes+8);assert(bytes[0]==0xa5&&bytes[8]==0xa5&&bytes[1]==70&&bytes[2]==5&&bytes[3]==2);actual_set(&ic,492,2);assert(ic.om_neighbor_report_flags==(i&~1U)&&updates==2);assert(om_neighbor_rrm_ie(bytes+1,ic.om_neighbor_report_flags)==bytes+8&&bytes[3]==0);for(result=4;result<8;result++)assert(bytes[result]==0);}
return 0;}
