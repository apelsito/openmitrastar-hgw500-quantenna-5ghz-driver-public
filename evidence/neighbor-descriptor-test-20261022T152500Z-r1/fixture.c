#include <assert.h>
#include <stdint.h>
#include <stddef.h>
typedef uint32_t u32;
struct beacon_shared_ie_t{u32 words[5];};
struct parameter{u32 curr;uint8_t *buf;u32 size;};
static int actual(struct parameter*param){switch(70){
	case 70:
		/* Exactone20Bdescriptor admission before writingRRM7B payload.
		 * NevergrowtheMuCfixedarena orpublishuntrackedpayload bytes. */
		if (param->curr < (u32)(unsigned long)param->buf ||
		    param->curr - (u32)(unsigned long)param->buf > param->size ||
		    param->size - (param->curr - (u32)(unsigned long)param->buf) <
		        sizeof(struct beacon_shared_ie_t))
			return 0;
		return 1;
		break;

 default:break;}return 2;}
int main(void){struct parameter p;unsigned size,used;for(size=0;size<=1000;size++)for(used=0;used<=1020;used++){
p.buf=(uint8_t*)(uintptr_t)0x10000;p.curr=0x10000+used;p.size=size;assert(actual(&p)==((used<=size&&size-used>=20)?1:0));assert(p.curr==0x10000+used&&p.size==size);}
p.curr=0xffff;assert(actual(&p)==0);p.curr=UINT32_MAX;assert(actual(&p)==0);return 0;}
