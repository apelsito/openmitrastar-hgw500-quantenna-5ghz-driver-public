#include "openmitrastar_rbsdfs_recovered.h"
void om_rbsdfs_init(struct om_rbsdfs_state *s)
{ if(s){s->measurement_in_cac=0;s->allow_rbs_report=0;s->allow_mbs_csa=0;s->end_tx_ms=500;} }
int om_rbsdfs_set466(struct om_rbsdfs_state *s,omrbs_u32 v)
{ if(!s)return -22;s->measurement_in_cac=v!=0;return 0; }
int om_rbsdfs_get466(const struct om_rbsdfs_state *s,omrbs_u32 *v)
{ if(!s||!v)return -22;*v=s->measurement_in_cac;return 0; }
int om_rbsdfs_set468(struct om_rbsdfs_state *s,omrbs_u32 v)
{ if(!s||(omrbs_u32)(v-1)>999U)return -22;s->end_tx_ms=v;return 0; }
int om_rbsdfs_get468(const struct om_rbsdfs_state *s,omrbs_u32 *v)
{ if(!s||!v)return -22;*v=s->end_tx_ms;return 0; }
int om_rbsdfs_oem_radar_window(struct om_rbsdfs_state *s,unsigned role,
    int during_cac,omrbs_u32 channel,omrbs_u32 now,omrbs_u32 hz,
    const struct om_rbsdfs_oem_hooks *h)
{
    omrbs_u32 ticks;
    if(!s||!h)return -22;
    if(role!=2||!during_cac||!s->measurement_in_cac)return 0;
    if(!h->enable_tx||!h->arm_timer||!hz||hz>0xffffffffU/1000U||
        (omrbs_u32)(s->end_tx_ms-1)>999U)return -22;
    /* Actual OEM assigns allowance then enables TX then arms its close timer.
     * floor(ms*HZ/1000), unsigned wrapping deadline, HZ200 observed OEM build. */
    s->allow_rbs_report=1;h->enable_tx(h->context);
    ticks=s->end_tx_ms*hz/1000U;
    h->arm_timer(h->context,channel,now+ticks);return 1;
}
int om_rbsdfs_oem_mbs_window(struct om_rbsdfs_state *s,int wds_is_rbs,
    int vap_hostap_run,int during_cac,const struct om_rbsdfs_oem_hooks *h)
{
    if(!s||!h)return -22;
    if(wds_is_rbs||!vap_hostap_run||!during_cac||!s->measurement_in_cac)return 0;
    if(!h->enable_tx)return -22;
    s->allow_mbs_csa=1;h->enable_tx(h->context);return 1;
}
int om_rbsdfs_oem_send_permitted(struct om_rbsdfs_state *s,unsigned role,
    const struct om_rbsdfs_oem_hooks *h)
{
    omrbs_u8 *allow;
    unsigned status;
    if(!s||!h)return -22;
    if(!s->measurement_in_cac||(role!=1&&role!=2))return 1;
    allow=role==2?&s->allow_rbs_report:&s->allow_mbs_csa;
    if(*allow){*allow=0;return 1;}
    if(!h->channel_status)return -22;
    /* OEM calls status twice along the short-circuit branch. */
    status=h->channel_status(h->context);if(status==4)return 0;
    status=h->channel_status(h->context);return status!=8;
}
int om_rbsdfs_oem_expire(omrbs_u32 channel,const struct om_rbsdfs_oem_hooks *h)
{
    void *found;
    if(!h||!h->find_channel||!h->equal_current||!h->disable_tx)return -22;
    found=h->find_channel(h->context,channel);
    if(!found)return -22; /* Adapter input guard, not an OEM null-proof claim. */
    if(h->equal_current(h->context,found)){h->disable_tx(h->context);return 1;}
    return 0;
}
enum om_rbsdfs_frame_result om_rbsdfs_exact_frame_window(struct om_rbsdfs_frame_state *s,
    const void *frame,unsigned role,omrbs_u32 channel,omrbs_u32 deadline,
    const struct om_rbsdfs_frame_hooks *h)
{
    int result;
    if(!s||!frame||!h||!h->validate_measurement||!h->grant_frame||
        !h->send_frame||!h->revoke_frame||s->active||s->quarantined||
        s->generation==0xffffffffU||(role!=1&&role!=2))return OMRBS_REFUSED;
    if(h->validate_measurement(h->context,frame,role,channel)!=1)return OMRBS_REFUSED;
    /* Caller serializes and pins state/frame/provider before this admission.
     * No module integration exists until provider lifetime/pins are proven. */
    s->generation++;s->frame=frame;s->channel=channel;s->deadline=deadline;s->active=1;
    result=h->grant_frame(h->context,frame);
    if(result==0){s->active=0;s->frame=0;return OMRBS_REFUSED;}
    if(result!=1){s->quarantined=1;return OMRBS_PUBLISHED_UNKNOWN;}
    result=h->send_frame(h->context,frame);
    if(result!=0&&result!=1){
        (void)h->revoke_frame(h->context,frame);s->quarantined=1;
        return OMRBS_PUBLISHED_UNKNOWN;
    }
    if(h->revoke_frame(h->context,frame)!=1){s->quarantined=1;return OMRBS_PUBLISHED_UNKNOWN;}
    s->active=0;s->frame=0;
    return result==1?OMRBS_COMPLETE:OMRBS_REFUSED;
}
