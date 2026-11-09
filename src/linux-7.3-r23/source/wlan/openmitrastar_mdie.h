#ifndef OPENMITRASTAR_MDIE_H
#define OPENMITRASTAR_MDIE_H
#include <linux/types.h>
/* Exact OEM IE54/length3: MDID little-endian bytes and nonzero FTOverDS bit0.
 * The caller reserves five owned frame bytes before invoking this producer. */
static inline u8 *om_mdie_write(u8 *frm,u16 mdid,u8 ft_over_ds)
{
    frm[0]=54;frm[1]=3;
    frm[2]=(u8)mdid;frm[3]=(u8)(mdid>>8);
    frm[4]=ft_over_ds?1U:0U;
    return frm+5;
}
#endif
