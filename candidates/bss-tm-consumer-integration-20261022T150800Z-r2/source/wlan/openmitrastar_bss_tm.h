#ifndef OPENMITRASTAR_BSS_TM_H
#define OPENMITRASTAR_BSS_TM_H
#include <linux/types.h>
#include <linux/stddef.h>
#include <linux/errno.h>
/* Derived OEM493 ExtCapIE127 payloadbyte2 bit3 update. Guard shortTLVs
 * and use the length actually copied; no management action handler is added. */
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

#endif
