#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
typedef uint8_t u8;typedef uint16_t u16;
#define IEEE80211_M_HOSTAP 6
#define OM_OEM_PARAM_MDID 494
#define OM_OEM_PARAM_FT_OVER_DS 495
#define EINVAL 22
#define EOPNOTSUPP 95
#define READ_ONCE(x) (x)
#define WRITE_ONCE(x,v) ((x)=(v))
struct ieee80211vap;struct ieee80211com{void(*ic_beacon_update)(struct ieee80211vap*);};struct ieee80211vap{struct ieee80211com*iv_ic;unsigned iv_opmode;u16 om_mdid;u8 om_ft_over_ds;uint32_t untouched;};
struct ieee80211_node{struct ieee80211vap*ni_vap;};static unsigned updates,descs;static u8 frame[16];
static inline u8 *om_mdie_write(u8 *frm,u16 mdid,u8 ft_over_ds)
{
    frm[0]=54;frm[1]=3;
    frm[2]=(u8)mdid;frm[3]=(u8)(mdid>>8);
    frm[4]=ft_over_ds?1U:0U;
    return frm+5;
}

static u8*descriptor(struct ieee80211_node*ni,unsigned id,u8*frm){struct ieee80211vap*vap=ni->ni_vap;switch(id){	case 54:
		frm=om_mdie_write(frm,READ_ONCE(vap->om_mdid),READ_ONCE(vap->om_ft_over_ds));
		break;default:return frm;}descs++;return frm;}
#define ieee80211_add_beacon_desc_ie descriptor
static u8*beacon_build(struct ieee80211vap*vap,u8*frm){struct ieee80211_node node={vap};struct ieee80211_node*ni=&node;	if(READ_ONCE(vap->om_mdid))
		frm=ieee80211_add_beacon_desc_ie(ni,54,frm);return frm;}
static u8*probe_build(struct ieee80211vap*vap,u8*frm){		if(READ_ONCE(vap->om_mdid))
			frm=om_mdie_write(frm,READ_ONCE(vap->om_mdid),READ_ONCE(vap->om_ft_over_ds));return frm;}
static void update(struct ieee80211vap*v){updates++;memset(frame,0xa5,sizeof(frame));beacon_build(v,frame+1);}
static int set(struct ieee80211vap*vap,int param,int value){struct ieee80211com*ic=vap->iv_ic;switch(param){	case OM_OEM_PARAM_MDID:
		if(vap->iv_opmode!=IEEE80211_M_HOSTAP)return -EINVAL;
		WRITE_ONCE(vap->om_mdid,(u16)value);
		ic->ic_beacon_update(vap);
		return 0;
	case OM_OEM_PARAM_FT_OVER_DS:
		if(vap->iv_opmode!=IEEE80211_M_HOSTAP)return -EINVAL;
		WRITE_ONCE(vap->om_ft_over_ds,(u8)value);
		return 0;
default:return-22;}}
static int get(struct ieee80211vap*vap,int*param){switch(*param&0xffff){	case OM_OEM_PARAM_MDID:
		*param=(u16)READ_ONCE(vap->om_mdid);
		break;
	case OM_OEM_PARAM_FT_OVER_DS:
		return -EOPNOTSUPP;

	/* Output handling lives in qualified QDRV and returns existing fields. */default:return-22;}return 0;}
int main(void){struct ieee80211com ic={update};struct ieee80211vap vap={&ic,6,0,0,0x1234abcd};unsigned transactions=0,iecases=0;u8 bytes[8],expected[8];int value;
for(unsigned md=0;md<65536;md++)for(unsigned ft=0;ft<256;ft++){vap.om_mdid=md;vap.om_ft_over_ds=ft;memset(bytes,0xa5,sizeof(bytes));memset(expected,0xa5,sizeof(expected));expected[1]=54;expected[2]=3;expected[3]=(u8)md;expected[4]=(u8)(md>>8);expected[5]=ft?1:0;assert(om_mdie_write(bytes+1,md,ft)==bytes+6&&!memcmp(bytes,expected,sizeof(bytes)));iecases++;}
int inputs[]={INT_MIN,-65537,-1,0,1,65535,65536,INT_MAX};for(unsigned role=0;role<8;role++)for(unsigned input=0;input<8;input++)for(unsigned prior=0;prior<2;prior++){vap.iv_opmode=role;vap.om_mdid=prior?0xabcd:0;vap.om_ft_over_ds=0x77;updates=0;u16 before=vap.om_mdid;int rc=set(&vap,494,inputs[input]);assert(rc==(role==6?0:-22));assert(vap.om_mdid==(role==6?(u16)inputs[input]:before)&&vap.om_ft_over_ds==0x77&&updates==(role==6)&&vap.untouched==0x1234abcd);value=494|0x80000000U;assert(!get(&vap,&value)&&value==vap.om_mdid);u16 md=vap.om_mdid;updates=0;u8 old=vap.om_ft_over_ds;rc=set(&vap,495,inputs[input]);assert(rc==(role==6?0:-22)&&vap.om_ft_over_ds==(role==6?(u8)inputs[input]:old)&&vap.om_mdid==md&&!updates);value=495;assert(get(&vap,&value)==-95&&value==495);transactions++;}
for(unsigned md=0;md<65536;md++){vap.iv_opmode=6;vap.om_mdid=md;vap.om_ft_over_ds=md&255;memset(bytes,0xa5,sizeof(bytes));descs=0;assert(beacon_build(&vap,bytes+1)==bytes+1+(md?5:0)&&descs==(md!=0));memset(bytes,0xa5,sizeof(bytes));assert(probe_build(&vap,bytes+1)==bytes+1+(md?5:0));transactions++;}
assert(iecases==16777216);printf("{\"MDIE_cases\":%u,\"actual_control_and_caller_cases\":%u,\"495_getter_faithful_unsupported\":true}\n",iecases,transactions);return 0;}
