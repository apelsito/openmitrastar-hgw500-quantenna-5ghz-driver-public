#include <stddef.h>
#include <stdint.h>

/* Exact OEM RRM EnabledCapabilitiesIE70/length5 format. The first capability
 * byte's bit1 advertises NeighborReport support; all other bytes are zero. */
static int om_rrm_enabled_ie(uint8_t *dst,size_t capacity,int neighbor_report)
{
    size_t i;
    if(capacity<7)return -1;
    for(i=0;i<7;i++)dst[i]=0;
    dst[0]=70;dst[1]=5;dst[2]=neighbor_report?2:0;
    return 7;
}

/* Recover OEM update_bss_tm ExtCapIE127 payloadbyte2 bit3. A short or malformed
 * element is refused without an out-of-bounds store; OEM lacked that check.
 * Return1 when changed,0 noExtCap, -1 malformed/short. This is a source
 * prototype, not a port wire API or complete802.11v BSStransition feature. */
static int om_update_bss_tm_bit(uint8_t *ies,size_t len,int enabled)
{
    size_t cursor=0;
    while(cursor<len){
        size_t element_len;
        if(len-cursor<2)return -1;
        element_len=ies[cursor+1];
        if(element_len>len-cursor-2)return -1;
        if(ies[cursor]==127){
            if(element_len<3)return -1;
            if(enabled)ies[cursor+4]|=8;
            else ies[cursor+4]&=(uint8_t)~8U;
            return 1;
        }
        cursor+=element_len+2;
    }
    return 0;
}
