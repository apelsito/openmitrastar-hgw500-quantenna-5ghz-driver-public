#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#define OM_OEM_PARAM_WEATHER_CAC_ENABLE 478
#define WRITE_ONCE(x,v) ((x)=(v))
#define READ_ONCE(x) (x)
struct ieee80211com {uint32_t prefix;unsigned char om_weather_cac_enabled;uint32_t suffix;};
static int setter(struct ieee80211com *ic,int param,int value){switch(param){	case OM_OEM_PARAM_WEATHER_CAC_ENABLE:
		WRITE_ONCE(ic->om_weather_cac_enabled,!!value);
		return 0;

default:return -1;}}
static int getter(struct ieee80211com *ic,int *param){switch(param[0]&65535){	case OM_OEM_PARAM_WEATHER_CAC_ENABLE:
		param[0]=READ_ONCE(ic->om_weather_cac_enabled);
		return 0;

default:return -1;}}
int main(void){struct ieee80211com ic={123,0,456};unsigned cases=0;
uint32_t values[]={0,1,2,255,256,65535,65536,0x7fffffff,0x80000000,0xffffffff};
for(unsigned i=0;i<10;i++)for(unsigned tag=0;tag<65536;tag++){
assert(!setter(&ic,478,(int)values[i]));assert(ic.om_weather_cac_enabled==!!values[i]);
int value=(int)(tag<<16|478);assert(!getter(&ic,&value));assert(value==!!values[i]);
assert(ic.prefix==123&&ic.suffix==456);cases++;}
printf("%u\n",cases);return 0;}
