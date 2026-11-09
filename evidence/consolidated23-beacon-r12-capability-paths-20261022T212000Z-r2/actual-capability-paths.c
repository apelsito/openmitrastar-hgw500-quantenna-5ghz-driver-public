
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
typedef uint8_t u8;typedef uint16_t __le16;typedef uint32_t u32;
#define EINVAL 22
#define IEEE80211_CAPINFO_ESS 1
#define IEEE80211_CAPINFO_IBSS 2
#define IEEE80211_CAPINFO_PRIVACY 0x10
#define IEEE80211_CAPINFO_SHORT_PREAMBLE 0x20
#define IEEE80211_CAPINFO_SHORT_SLOTTIME 0x400
#define IEEE80211_CAPINFO_SPECTRUM_MGMT 0x100
#define IEEE80211_F_PRIVACY 0x10
#define IEEE80211_F_SHPREAMBLE 0x40000
#define IEEE80211_F_SHSLOT 0x400
#define IEEE80211_F_DOTH 0x40000000
#define IEEE80211_M_IBSS 0
#define IEEE80211_M_HOSTAP 6
#define IEEE80211_S_RUN 4
#define IEEE80211_APPIE_MAX 1024
#define IEEE80211_APPIE_FRAME_BEACON 0
#define IEEE80211_APPIE_FRAME_PROBE_RESP 2
#define IEEE80211_APPIE_FRAME_ASSOC_RESP 4
#define OM_OEM_PARAM_NEIGHBOR_REPORT 492
#define OM_OEM_PARAM_BSS_TM 493
#define EXPORT_SYMBOL(x)
#define htole16(x) (x)
struct channel{int two;};
struct app{u8*ie;unsigned length;};
struct ieee80211_beacon_param_t{u32 curr;u8*buf;unsigned size;};
struct ieee80211_beacon_offsets{__le16*bo_caps;u8*bo_om_rrm;};
struct ieee80211com;
struct ieee80211vap{struct ieee80211com*iv_ic;int iv_opmode,iv_state;u32 iv_flags;struct app app_ie[6];struct ieee80211_beacon_param_t*param;struct ieee80211vap*iv_next;struct ieee80211_beacon_offsets bo;u8 frame[1056];unsigned updates;};
struct ieee80211com{u32 om_neighbor_report_flags,om_bss_tm_flags,ic_flags;struct channel*ic_bsschan,*ic_curchan;struct{struct ieee80211vap*first;}ic_vaps;void(*ic_beacon_update)(struct ieee80211vap*);};
struct beacon_shared_ie_t{u32 values[5];};
#define TAILQ_FOREACH(v,h,n) for((v)=(h)->first;(v);(v)=(v)->n)
#define IEEE80211_IS_CHAN_2GHZ(c) ((c)->two)
static unsigned locked;
#define IEEE80211_LOCK_BH(ic) do{(void)(ic);assert(!locked);locked=1;}while(0)
#define IEEE80211_UNLOCK_BH(ic) do{(void)(ic);assert(locked);locked=0;}while(0)
static void ieee80211_beacon_update_all(struct ieee80211com*ic);
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

/* Exact OEM RRMcapabilityIE: id70,len5, bit1 whenNeighborReport selected. */
static inline u8 *om_neighbor_rrm_ie(u8 *frm,u32 selected)
{
    size_t n;
    for(n=0;n<7;n++)frm[n]=0;
    frm[0]=70;frm[1]=5;frm[2]=(selected&1U)?2U:0U;
    return frm+7;
}

void ieee80211_beacon_update_all(struct ieee80211com *ic)
{
	struct ieee80211vap *vap;

	TAILQ_FOREACH(vap, &ic->ic_vaps, iv_next) {
		if (vap->iv_opmode != IEEE80211_M_HOSTAP)
			continue;
		if (vap->iv_state != IEEE80211_S_RUN)
			continue;

		ic->ic_beacon_update(vap);
	}
}static void actual_beacon_caps(struct ieee80211vap*vap,struct ieee80211com*ic,struct ieee80211_beacon_offsets*bo){unsigned capinfo;
	/* XXX faster to recalculate entirely or just changes? */
	if (vap->iv_opmode == IEEE80211_M_IBSS)
		capinfo = IEEE80211_CAPINFO_IBSS;
	else
		capinfo = IEEE80211_CAPINFO_ESS;
	if (vap->iv_flags & IEEE80211_F_PRIVACY)
		capinfo |= IEEE80211_CAPINFO_PRIVACY;
	if ((ic->ic_flags & IEEE80211_F_SHPREAMBLE) &&
	    IEEE80211_IS_CHAN_2GHZ(ic->ic_bsschan))
		capinfo |= IEEE80211_CAPINFO_SHORT_PREAMBLE;
	if (ic->ic_flags & IEEE80211_F_SHSLOT)
		capinfo |= IEEE80211_CAPINFO_SHORT_SLOTTIME;
	if (ic->ic_flags & IEEE80211_F_DOTH)
		capinfo |= IEEE80211_CAPINFO_SPECTRUM_MGMT;
	if(ic->om_neighbor_report_flags & 1U)
		capinfo |= 0x1000U;

	*bo->bo_caps = htole16(capinfo);
	if(bo->bo_om_rrm)
		(void)om_neighbor_rrm_ie(bo->bo_om_rrm,ic->om_neighbor_report_flags);

}
static u8*actual_beacon_app(struct ieee80211vap*vap,struct ieee80211com*ic,u8*frm){		memcpy(frm, vap->app_ie[IEEE80211_APPIE_FRAME_BEACON].ie,
			vap->app_ie[IEEE80211_APPIE_FRAME_BEACON].length);
		(void)om_bss_tm_update_ie(frm,
		    vap->app_ie[IEEE80211_APPIE_FRAME_BEACON].length,
		    ic->om_bss_tm_flags);
		frm += vap->app_ie[IEEE80211_APPIE_FRAME_BEACON].length;
return frm;}
static unsigned int om_app_ie_copy(struct ieee80211vap *vap,
        unsigned int frame_type, uint8_t *destination)
{
    struct ieee80211com *ic = vap->iv_ic;
    unsigned int length = 0;
    IEEE80211_LOCK_BH(ic);
    if (vap->app_ie[frame_type].ie &&
        vap->app_ie[frame_type].length <= IEEE80211_APPIE_MAX) {
        length = vap->app_ie[frame_type].length;
        memcpy(destination, vap->app_ie[frame_type].ie, length);
    }
    IEEE80211_UNLOCK_BH(ic);
    return length;
}static u8*actual_probe(struct ieee80211vap*vap,struct ieee80211com*ic,u8*frm){		if(ic->om_neighbor_report_flags & 1U)
			frm = om_neighbor_rrm_ie(frm,ic->om_neighbor_report_flags);

		{
			unsigned int app_len = om_app_ie_copy(vap, IEEE80211_APPIE_FRAME_PROBE_RESP, frm);
			(void)om_bss_tm_update_ie(frm, app_len, ic->om_bss_tm_flags);
			frm += app_len;
		}
return frm;}
static u8*actual_assoc(struct ieee80211vap*vap,struct ieee80211com*ic,u8*frm){		if(ic->om_neighbor_report_flags & 1U)
			frm = om_neighbor_rrm_ie(frm,ic->om_neighbor_report_flags);

		{
			unsigned int app_len = om_app_ie_copy(vap, IEEE80211_APPIE_FRAME_ASSOC_RESP, frm);
			(void)om_bss_tm_update_ie(frm, app_len, ic->om_bss_tm_flags);
			frm += app_len;
		}
return frm;}
static unsigned actual_probe_cap(struct ieee80211vap*vap,struct ieee80211com*ic){unsigned capinfo;		if (vap->iv_opmode == IEEE80211_M_IBSS)
			capinfo = IEEE80211_CAPINFO_IBSS;
		else
			capinfo = IEEE80211_CAPINFO_ESS;
		if (vap->iv_flags & IEEE80211_F_PRIVACY)
			capinfo |= IEEE80211_CAPINFO_PRIVACY;
		if ((ic->ic_flags & IEEE80211_F_SHPREAMBLE) &&
		    IEEE80211_IS_CHAN_2GHZ(ic->ic_curchan))
			capinfo |= IEEE80211_CAPINFO_SHORT_PREAMBLE;
		if (ic->ic_flags & IEEE80211_F_SHSLOT)
			capinfo |= IEEE80211_CAPINFO_SHORT_SLOTTIME;
		if (ic->ic_flags & IEEE80211_F_DOTH)
			capinfo |= IEEE80211_CAPINFO_SPECTRUM_MGMT;
		if(ic->om_neighbor_report_flags & 1U)
			capinfo |= 0x1000U;
return capinfo;}
static unsigned actual_assoc_cap(struct ieee80211vap*vap,struct ieee80211com*ic){unsigned capinfo;		capinfo = IEEE80211_CAPINFO_ESS;
		if (vap->iv_flags & IEEE80211_F_PRIVACY)
			capinfo |= IEEE80211_CAPINFO_PRIVACY;
		if ((ic->ic_flags & IEEE80211_F_SHPREAMBLE) &&
		    IEEE80211_IS_CHAN_2GHZ(ic->ic_curchan))
			capinfo |= IEEE80211_CAPINFO_SHORT_PREAMBLE;
		if (ic->ic_flags & IEEE80211_F_SHSLOT)
			capinfo |= IEEE80211_CAPINFO_SHORT_SLOTTIME;
		if (ic->ic_flags & IEEE80211_F_DOTH)
			capinfo |= IEEE80211_CAPINFO_SPECTRUM_MGMT;
		if(ic->om_neighbor_report_flags & 1U)
			capinfo |= 0x1000U;
return capinfo;}
static u8*actual_descriptor70(struct ieee80211_beacon_param_t*param,struct ieee80211com*ic,u8*frm){u8*pre_frm=frm;		/* Exactone20Bdescriptor admission before writingRRM7B payload.
		 * NevergrowtheMuCfixedarena orpublishuntrackedpayload bytes. */
		if (param->curr < (u32)(unsigned long)param->buf ||
		    param->curr - (u32)(unsigned long)param->buf > param->size ||
		    param->size - (param->curr - (u32)(unsigned long)param->buf) <
		        sizeof(struct beacon_shared_ie_t))
			return pre_frm;
		frm = om_neighbor_rrm_ie(frm,ic->om_neighbor_report_flags);
return frm;}

static unsigned setter_cases,frame_cases,tlv_cases;
static void actual_beacon_callback(struct ieee80211vap*v){struct ieee80211com*ic=v->iv_ic;actual_beacon_caps(v,ic,&v->bo);actual_beacon_app(v,ic,v->frame+16);v->updates++;}
static int set(struct ieee80211com*ic,int param,int value){switch(param){	case OM_OEM_PARAM_NEIGHBOR_REPORT:
		if(value == 1)ic->om_neighbor_report_flags |= 1U;
		else ic->om_neighbor_report_flags &= ~1U;
		ieee80211_beacon_update_all(ic);
		break;

	case OM_OEM_PARAM_BSS_TM:
		if(value == 1)ic->om_bss_tm_flags |= 1U;
		else ic->om_bss_tm_flags &= ~1U;
		ieee80211_beacon_update_all(ic);
		break;

 default:return-22;}return 0;}
static int get(struct ieee80211com*ic,int param){switch(param&0xffff){	case OM_OEM_PARAM_NEIGHBOR_REPORT:
		param = ic->om_neighbor_report_flags & 1U;
		break;

	case OM_OEM_PARAM_BSS_TM:
		param = ic->om_bss_tm_flags & 1U;
		break;

 default:return-22;}return param;}
int main(void){struct channel channel={1};struct ieee80211com ic={.ic_bsschan=&channel,.ic_curchan=&channel,.ic_beacon_update=actual_beacon_callback};struct ieee80211vap v[4]={0};u8 source[1024],saved[1024],response[1056],expected[1056];__le16 caps[4];u8 rrm[4][7];int inputs[]={INT_MIN,-32769,-1,0,1,2,32768,INT_MAX};
for(unsigned i=0;i<4;i++){v[i].iv_ic=&ic;v[i].iv_opmode=i==2?1:6;v[i].iv_state=i==3?0:4;v[i].iv_next=i<3?&v[i+1]:NULL;v[i].bo.bo_caps=&caps[i];v[i].bo.bo_om_rrm=rrm[i];v[i].app_ie[0].ie=source;v[i].app_ie[0].length=5;}
ic.ic_vaps.first=v;source[0]=127;source[1]=3;source[2]=0xaa;source[3]=0xbb;source[4]=0x55;memcpy(saved,source,1024);
for(unsigned state=0;state<65536;state++)for(unsigned p=492;p<=493;p++)for(unsigned value=0;value<8;value++){
 ic.om_neighbor_report_flags=ic.om_bss_tm_flags=state;for(unsigned n=0;n<4;n++)v[n].updates=0;assert(set(&ic,p,inputs[value])==0);u32 wanted=inputs[value]==1?(state|1U):(state&~1U);assert((p==492?ic.om_neighbor_report_flags:ic.om_bss_tm_flags)==wanted);assert((p==492?ic.om_bss_tm_flags:ic.om_neighbor_report_flags)==state);assert(get(&ic,(state<<16)|p)==(wanted&1));assert(v[0].updates==1&&v[1].updates==1&&!v[2].updates&&!v[3].updates);assert(caps[0]==(1|((ic.om_neighbor_report_flags&1)?0x1000:0)));assert(rrm[0][0]==70&&rrm[0][1]==5&&rrm[0][2]==((ic.om_neighbor_report_flags&1)?2:0));assert(v[0].frame[20]==((ic.om_bss_tm_flags&1)?(source[4]|8):(source[4]&~8)));assert(!memcmp(saved,source,1024));setter_cases++;
}
u32 random=0x81452390;for(unsigned n=0;n<100000;n++){random=random*1664525U+1013904223U;ic.om_bss_tm_flags=random;assert(set(&ic,493,(int)random)==0);assert(ic.om_bss_tm_flags==((random==1)?random|1U:random&~1U));setter_cases++;}
for(unsigned flags=0;flags<32;flags++)for(unsigned selected=0;selected<2;selected++)for(unsigned bss=0;bss<2;bss++)for(unsigned frame=0;frame<2;frame++){
 ic.ic_flags=((flags&1)?0x40000:0)|((flags&2)?0x400:0)|((flags&4)?0x40000000:0);v[0].iv_flags=(flags&8)?0x10:0;v[0].iv_opmode=(flags&16)?0:6;channel.two=flags&1;ic.om_neighbor_report_flags=selected;ic.om_bss_tm_flags=bss;memset(response,0x5c,sizeof(response));memcpy(expected,response,sizeof(response));v[0].app_ie[frame?4:2].ie=source;v[0].app_ie[frame?4:2].length=5;u8*end=frame?actual_assoc(&v[0],&ic,response+16):actual_probe(&v[0],&ic,response+16);assert(end==response+16+selected*7+5);assert(!memcmp(source,saved,1024));assert(response[0]==0x5c&&response[1055]==0x5c);if(selected){assert(response[16]==70&&response[17]==5&&response[18]==2);for(unsigned i=19;i<23;i++)assert(!response[i]);}unsigned offset=16+selected*7;assert(response[offset+4]==(bss?(source[4]|8):(source[4]&~8)));
 unsigned want=(frame?1:((flags&16)?2:1))|((flags&8)?0x10:0)|((flags&1)?0x20:0)|((flags&2)?0x400:0)|((flags&4)?0x100:0)|(selected?0x1000:0);assert((frame?actual_assoc_cap(&v[0],&ic):actual_probe_cap(&v[0],&ic))==want);frame_cases++;
}
for(unsigned id=0;id<256;id++)for(unsigned len=0;len<256;len++)for(unsigned enabled=0;enabled<2;enabled++){
 u8 data[260],before[260];memset(data,0x55,sizeof(data));data[1]=id;data[2]=len;memcpy(before,data,sizeof(data));int r=om_bss_tm_update_ie(data+1,len+2,enabled);if(id==127&&len>=3){assert(r==1);before[5]=enabled?(before[5]|8):(before[5]&~8);}else if(id==127)assert(r==-22);else assert(!r);assert(!memcmp(data,before,sizeof(data)));tlv_cases++;
}
for(unsigned length=0;length<257;length++){u8 data[260],before[260];memset(data,0x55,sizeof(data));data[1]=127;data[2]=255;memcpy(before,data,sizeof(data));int r=om_bss_tm_update_ie(data+1,length,1);assert(r==-22||!r);assert(!memcmp(data,before,sizeof(data)));tlv_cases++;}
v[0].app_ie[2].length=1024;v[0].app_ie[2].ie=source;memset(source,0,1024);memset(response,0x5c,sizeof(response));ic.om_neighbor_report_flags=1;assert(actual_probe(&v[0],&ic,response+16)==response+1047);assert(response[1047]==0x5c);frame_cases++;
u8 arena[1000];struct ieee80211_beacon_param_t param={.buf=arena,.size=1000};for(unsigned used=0;used<=1020;used+=20){param.curr=(u32)(uintptr_t)arena+used;memset(response,0x5c,sizeof(response));u8*end=actual_descriptor70(&param,&ic,response+16);assert(end==response+16+(used<=980?7:0));if(used>980)for(unsigned i=0;i<1056;i++)assert(response[i]==0x5c);frame_cases++;}
printf("{\"setter_getter_transactions\":%u,\"actual_frame_block_cases\":%u,\"TLV_cases\":%u}\n",setter_cases,frame_cases,tlv_cases);return 0;}
