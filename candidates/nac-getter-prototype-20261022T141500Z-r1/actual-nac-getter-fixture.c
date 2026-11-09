#include <stdint.h>
#include <assert.h>
#include <stddef.h>
typedef uint8_t u8;typedef uint16_t u16;typedef uint32_t u32;
#define EAGAIN 11
#define READ_ONCE(x) (x)
struct qtn_nac_mon_info_v89 {u16 config_value0;u16 config_value1;u16 enabled;u16 reserved;};
/* Exact OEM457 getter prefix; no per-client NAC entries are exposed.
 * MuC can update the three fields asynchronously, so this is a sampled tuple.
 * Guard the zero-initialized/not-ready denominator; OEM itself had no guard. */
static int om_oem_get_nac_mode_sample(struct qtn_nac_mon_info_v89 *nac,
                                    int *value)
{
    u16 value0, value1;
    u8 enabled;
    u32 percent;

    if (!nac)
        return -EAGAIN;
    value0 = READ_ONCE(nac->config_value0);
    value1 = READ_ONCE(nac->config_value1);
    enabled = (u8)READ_ONCE(nac->enabled);
    if (!value1)
        return -EAGAIN;
    percent = ((u32)value0 * 100U) / value1;
    *value = ((u32)value1 << 16) | ((percent & 0xffU) << 8) | enabled;
    return 0;
}

int main(void){struct qtn_nac_mon_info_v89 n;unsigned a,b,e;int value;uint32_t expected;
value=123;assert(om_oem_get_nac_mode_sample(NULL,&value)==-11&&value==123);
n.config_value1=0;value=123;assert(om_oem_get_nac_mode_sample(&n,&value)==-11&&value==123);
for(a=0;a<65536;a++)for(b=1;b<65536;b+=257){n.config_value0=a;n.config_value1=b;n.enabled=(a^b);n.reserved=0xbeef;
expected=(b<<16)|(((a*100U/b)&255U)<<8)|((a^b)&255U);value=-1;assert(om_oem_get_nac_mode_sample(&n,&value)==0);assert((uint32_t)value==expected);assert(n.config_value0==a&&n.config_value1==b&&n.reserved==0xbeef);}
return 0;}
