#include <stdint.h>
#include <assert.h>
#include <string.h>
typedef uint32_t u32;
#define READ_ONCE(x) (x)
#define WRITE_ONCE(x,v) ((x)=(v))
#define OM_OEM_PARAM_COC_MOVE_NDFS 491
struct ieee80211com {u32 om_coc_move_ndfs,unchanged;};
static int set(struct ieee80211com*ic,int param,int value){switch(param){
	case OM_OEM_PARAM_COC_MOVE_NDFS:
		WRITE_ONCE(ic->om_coc_move_ndfs,(u32)value);
		return 0;

default:return -1;}}
static int get(struct ieee80211com*ic,int*param){switch(param[0]&0xffff){
	case OM_OEM_PARAM_COC_MOVE_NDFS:
		param[0]=(int)READ_ONCE(ic->om_coc_move_ndfs);
		return 0;

default:return -1;}}
int main(void){struct ieee80211com ic;int v,p;unsigned lo;for(lo=0;lo<65536;lo++){uint32_t xs[]={lo,0xffff0000U|lo,0x7fff0000U|lo,0x80000000U|lo};for(unsigned i=0;i<4;i++){memset(&ic,0,sizeof(ic));ic.unchanged=0x12345678;p=491;v=(int)xs[i];assert(set(&ic,p,v)==0&&ic.om_coc_move_ndfs==xs[i]&&ic.unchanged==0x12345678);p=491|0x5a0000;assert(get(&ic,&p)==0&&(u32)p==xs[i]&&ic.unchanged==0x12345678);}}ic.om_coc_move_ndfs=123;p=490;assert(set(&ic,p,-5)==-1&&ic.om_coc_move_ndfs==123);assert(get(&ic,&p)==-1&&ic.om_coc_move_ndfs==123);return 0;}
