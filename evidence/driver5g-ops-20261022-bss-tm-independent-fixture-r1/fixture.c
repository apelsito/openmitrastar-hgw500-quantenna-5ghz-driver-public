#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
typedef uint8_t u8;typedef uint32_t u32;
#define EINVAL 22
#define OM_OEM_PARAM_BSS_TM 493
struct ieee80211com{u32 om_bss_tm_flags;};
static unsigned calls;
static void ieee80211_beacon_update_all(struct ieee80211com*ic){calls++;}
/* Derived OEM493 ExtCapIE127 payloadbyte2 bit3 update. Guard shortTLVs
 * and use the length actually copied; no management action handler is added. */
static inline int om_bss_tm_update_ie(u8 *ies,size_t length,u32 enabled)
{
    size_t pos=0;
    while(pos<length){
        size_t n;
        if(length-pos<2)return -EINVAL;
        n=ies[pos+1];
        if(n>length-pos-2)return -EINVAL;
        if(ies[pos]==127){
            if(n<3)return -EINVAL;
            if(enabled & 1U)ies[pos+4]|=8U;
            else ies[pos+4]&=(u8)~8U;
            return 1;
        }
        pos+=n+2;
    }
    return 0;
}

static void actual_set(struct ieee80211com*ic,int param,int value){switch(param){
	case OM_OEM_PARAM_BSS_TM:
		if(value == 1)ic->om_bss_tm_flags |= 1U;
		else ic->om_bss_tm_flags &= ~1U;
		ieee80211_beacon_update_all(ic);
		break;

 default:break;}}
static void actual_get(struct ieee80211com*ic,int*param){switch(param[0]&0xffff){
	case OM_OEM_PARAM_BSS_TM:
		param[0] = ic->om_bss_tm_flags & 1U;
		break;

 default:break;}}
int main(void){unsigned i,n,enabled;struct ieee80211com ic;int result;u8 bytes[258],before[258];
for(i=0;i<65536;i++){ic.om_bss_tm_flags=i;calls=0;actual_set(&ic,493,1);assert(ic.om_bss_tm_flags==(i|1U)&&calls==1);result=(i<<16)|493;actual_get(&ic,&result);assert(result==1);actual_set(&ic,493,2);assert(ic.om_bss_tm_flags==(i&~1U)&&calls==2);}
for(n=0;n<256;n++)for(enabled=0;enabled<2;enabled++){memset(bytes,0xa5,sizeof(bytes));bytes[0]=127;bytes[1]=n;memcpy(before,bytes,sizeof(bytes));result=om_bss_tm_update_ie(bytes,n+2,enabled);if(n<3){assert(result==-22&&!memcmp(bytes,before,sizeof(bytes)));}else{assert(result==1);before[4]=enabled?(before[4]|8):(before[4]&~8);assert(!memcmp(bytes,before,sizeof(bytes)));}}
bytes[0]=0;bytes[1]=1;bytes[2]=4;bytes[3]=127;bytes[4]=3;bytes[5]=0;bytes[6]=0;bytes[7]=0;assert(om_bss_tm_update_ie(bytes,8,1)==1&&bytes[7]==8);memcpy(before,bytes,sizeof(bytes));assert(om_bss_tm_update_ie(bytes,7,0)==-22&&!memcmp(bytes,before,sizeof(bytes)));return 0;}
