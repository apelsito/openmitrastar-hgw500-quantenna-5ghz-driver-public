#include <assert.h>
#include <string.h>
struct ieee80211com {unsigned char prefix[22605];unsigned char om_weather_cac_enabled;unsigned char suffix[22680-22606];};
struct qdrv_wlan {struct ieee80211com ic;unsigned char suffix[30592-22680];};
static void actual_outer_zero(struct qdrv_wlan*qw){memset(qw, 0, sizeof(struct qdrv_wlan));}
static void actual_inner_default(struct ieee80211com*ic){ic->om_weather_cac_enabled = 0;}
int main(void){struct qdrv_wlan q;for(unsigned x=0;x<256;x++){memset(&q,x,sizeof(q));actual_outer_zero(&q);assert(q.ic.om_weather_cac_enabled==0);for(unsigned n=0;n<sizeof(q);n++)assert(((unsigned char*)&q)[n]==0);q.ic.om_weather_cac_enabled=(unsigned char)x;actual_inner_default(&q.ic);assert(q.ic.om_weather_cac_enabled==0);}return 0;}
