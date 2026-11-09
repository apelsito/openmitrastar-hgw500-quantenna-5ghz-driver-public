#ifndef OPENMITRASTAR_NAC_GETTER_H
#define OPENMITRASTAR_NAC_GETTER_H
#include <linux/types.h>
#include <linux/build_bug.h>
/* Recovered shared prefix only. Entries and opaque tail are deliberately unused. */
struct qtn_nac_mon_info_v89 {u16 config_value0;u16 config_value1;u16 enabled;u16 reserved;};
static_assert(sizeof(struct qtn_nac_mon_info_v89)==8);
#endif
