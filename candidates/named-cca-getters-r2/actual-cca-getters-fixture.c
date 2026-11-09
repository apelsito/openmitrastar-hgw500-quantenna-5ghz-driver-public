#include <assert.h>
#include <stdint.h>
struct ieee80211com { uint8_t cca_fix_disable; uint8_t auto_cca_enable; };
static void actual_getter(struct ieee80211com *ic, int param, int *value)
{
 switch(param) {

	/* OEM read-only getters331/275 use these existing named fields.
	 * The port compiler ABI witness is independent of OEM numeric offsets. */
	case 331:
		*value = ic->cca_fix_disable;
		break;
	case 275:
		*value = ic->auto_cca_enable;
		break;

 default: break;
 }
}
int main(void) {
 struct ieee80211com ic;
 unsigned a,b; int result; unsigned checks=0;
 for(a=0;a<256;a++) for(b=0;b<256;b++) {
  ic.cca_fix_disable=a;ic.auto_cca_enable=b;
  result=-1;actual_getter(&ic,331,&result);assert(result==(int)a);checks++;
  result=-1;actual_getter(&ic,275,&result);assert(result==(int)b);checks++;
  assert(ic.cca_fix_disable==a && ic.auto_cca_enable==b);checks++;
 }
 result=123456;actual_getter(&ic,999,&result);assert(result==123456);checks++;
 assert(checks==196609);return 0;
}
