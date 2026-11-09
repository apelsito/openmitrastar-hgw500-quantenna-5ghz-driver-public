/* OEM derived reconstruction. Callback errors are ignored where OEM ignores
 * them; adapter/provider availability remains an independent acceptance gate. */
#include "openmitrastar_vopt481_recovered.h"
typedef char om_vopt_u32_is_four[(sizeof(om_vopt_u32)==4)?1:-1];
void om_vopt_initialize(struct om_vopt_state *state,unsigned pppc,unsigned airfair)
{
    struct om_vopt_state zero={0};
    *state=zero;
    state->saved_probe=1;
    state->saved_pppc=(om_vopt_u8)pppc;
    state->saved_airfair=(om_vopt_u8)airfair;
    state->live_pppc=(om_vopt_u8)pppc;
    state->live_airfair=(om_vopt_u8)airfair;
}
int om_vopt_set(struct om_vopt_state *state,unsigned opmode,om_vopt_u32 value)
{
    /* Signed OEM bounds0..2; uint comparison rejects every negative encoding. */
    if(!state||opmode!=6||value>2)return -95;
    state->policy=(om_vopt_u8)value;return 0;
}
int om_vopt_get(const struct om_vopt_state *state,unsigned opmode,int *output)
{
    if(!state||!output)return -22;
    *output=opmode==6?((state->policy&15U)<<4)|state->effective:0;
    return 0;
}
static int om_vopt_byte_value(unsigned value)
{
    /* OEM ldb.x sign extends its saved byte before param_to_qdrv. */
    return value<128? (int)value:(int)value-256;
}
int om_vopt_apply(struct om_vopt_state *state,unsigned effective,
    const struct om_vopt_environment *environment,const struct om_vopt_provider *provider)
{
    om_vopt_u32 value;
    if(!state||!environment||!provider||!provider->parameter||
       !provider->scs||!provider->ocac||!provider->reassociate)return -95;
    /* Provider availability checked before mutation. Original consumer assumes
     * these callbacks exist. This refusal is a declared source adapter guard. */
    state->effective=(om_vopt_u8)effective;
    if(!state->effective){
        if(state->saved_txbf){
            provider->parameter(provider->context,78,om_vopt_byte_value(state->saved_txbf));
            provider->parameter(provider->context,68,17);
        }
        if(state->saved_pppc){
            provider->parameter(provider->context,158,om_vopt_byte_value(state->saved_pppc));
            state->live_pppc=state->saved_pppc;
        }
        if(state->saved_scs){
            value=state->saved_scs|0x10000U;
            if(!provider->scs(provider->context,value))
                provider->parameter(provider->context,147,(int)value);
        }
        if(state->saved_ocac){
            value=environment->ocac_channel&65535U;value|=0x10000U;
            provider->ocac(provider->context,value);
        }
        if(state->saved_bgscan&&provider->background_set)
            provider->background_set(provider->context,0,1);
        if(state->saved_probe)
            provider->parameter(provider->context,195,om_vopt_byte_value(state->saved_probe));
        if(state->saved_airfair){
            state->live_airfair=state->saved_airfair;
            provider->parameter(provider->context,288,om_vopt_byte_value(state->saved_airfair));
        }
        provider->parameter(provider->context,482,om_vopt_byte_value((unsigned)environment->mode_value&255U));
        state->live_duplicate_mu=0;
        provider->parameter(provider->context,414,0);
    }else{
        provider->parameter(provider->context,68,16);
        provider->parameter(provider->context,78,0);
        if(!(environment->flags&0x40000000U)||!(environment->flags_ext&0x800000U))
            provider->parameter(provider->context,158,0);
        state->live_pppc=0;
        provider->parameter(provider->context,195,0);
        state->live_airfair=0;
        provider->parameter(provider->context,288,0);
        value=0x10000U;
        if(!provider->scs(provider->context,value))
            provider->parameter(provider->context,147,(int)value);
        value=0x20000U;
        provider->ocac(provider->context,value);
        if(provider->background_get&&provider->background_set&&
           !provider->background_get(provider->context,0,&value)){
            state->saved_bgscan=value;
            provider->background_set(provider->context,0,0);
        }
        provider->parameter(provider->context,482,0);
        state->live_duplicate_mu=3;
        provider->parameter(provider->context,414,3);
    }
    provider->reassociate(provider->context,0,0);
    return 0;
}
void om_vopt_count_peer(const struct om_vopt_peer *peer,om_vopt_u32 *total,om_vopt_u32 *qualified)
{
    if(!peer||!total||!qualified||peer->self_bss||peer->self_address)return;
    ++*total;if(peer->flags&0x800U)++*qualified;
}
int om_vopt_tick(struct om_vopt_state *state,om_vopt_u32 *global_backoff,
    om_vopt_u32 total,om_vopt_u32 qualified,const struct om_vopt_environment *environment,
    const struct om_vopt_provider *provider)
{
    int result;
    if(!state||!global_backoff)return -22;
    if(state->policy==state->effective)return 0;
    if(state->policy==0||state->policy==1){
        result=om_vopt_apply(state,state->policy,environment,provider);
        if(!result)*global_backoff=0;return result;
    }
    if(state->policy==2){
        if(*global_backoff){--*global_backoff;return 0;}
        if(total==qualified&&state->effective!=1)
            return om_vopt_apply(state,1,environment,provider);
        if(total!=qualified&&state->effective!=0){
            result=om_vopt_apply(state,0,environment,provider);
            if(!result)*global_backoff=10;return result;
        }
    }
    return 0;
}
