#include <assert.h>
#include <stdint.h>
#include <stddef.h>
typedef uint16_t u16;typedef uint8_t u8;typedef uint32_t u32;
#define EAGAIN 11
#define READ_ONCE(x) (x)
#define OM_OEM_PARAM_NAC_MODE 457
#define IEEE80211_M_HOSTAP 6
struct ieee80211vap{int iv_opmode;};
static int observedvalue;static unsigned sends;
static void ieee80211_param_to_qdrv(struct ieee80211vap*v,int id,int value,void*d,int n){sends++;observedvalue=value;assert(id==457&&d==NULL&&n==0);}
struct om_nac_prefix {u16 value0;u16 value1;u16 enabled;u16 reserved;};
static int om_nac_prefix_sample(struct om_nac_prefix *nac,int *value)
{
    u16 first,second;
    u8 enabled;
    u32 percent;
    if(!nac)return -EAGAIN;
    first=READ_ONCE(nac->value0);
    second=READ_ONCE(nac->value1);
    enabled=(u8)READ_ONCE(nac->enabled);
    if(!second)return -EAGAIN;
    percent=((u32)first*100U)/second;
    *value=((u32)second<<16)|((percent&255U)<<8)|enabled;
    return 0;
}
static int actual_set(struct ieee80211vap*vap,int param,int value){switch(param){
	case OM_OEM_PARAM_NAC_MODE:
		/* OEMnon-HOSTAPcase is a successfulno-op; no mode activation here. */
		if(vap->iv_opmode == IEEE80211_M_HOSTAP)
			ieee80211_param_to_qdrv(vap,param,value,NULL,0);
		break;

 default:break;}return 0;}
int main(void){struct om_nac_prefix n;struct ieee80211vap v;unsigned a,b,e;int result;uint32_t expected;
result=123;assert(om_nac_prefix_sample(NULL,&result)==-11&&result==123);n.value1=0;result=123;assert(om_nac_prefix_sample(&n,&result)==-11&&result==123);
for(a=0;a<65536;a++)for(b=1;b<65536;b+=257){n.value0=a;n.value1=b;n.enabled=(a^b);n.reserved=0xbeef;result=-1;expected=(b<<16)|(((a*100U/b)&255U)<<8)|((a^b)&255U);assert(om_nac_prefix_sample(&n,&result)==0&&(uint32_t)result==expected);assert(n.value0==a&&n.value1==b&&n.reserved==0xbeef);}
for(a=0;a<16;a++)for(b=0;b<65536;b++){v.iv_opmode=a;sends=0;assert(actual_set(&v,457,b)==0&&sends==(a==6));if(sends)assert(observedvalue==(int)b);}
return 0;}
