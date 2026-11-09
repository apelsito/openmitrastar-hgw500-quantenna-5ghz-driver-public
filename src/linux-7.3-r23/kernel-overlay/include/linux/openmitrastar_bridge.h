/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _LINUX_OPENMITRASTAR_BRIDGE_H
#define _LINUX_OPENMITRASTAR_BRIDGE_H

#include <linux/if_ether.h>
#include <linux/types.h>

struct net_device;

#define QTN_FLAG_WIFI_DEVICE       0x80000000U
#define QTN_FLAG_INTRA_BSS_ISOLATE 0x40000000U
#define QTN_FLAG_BSS_ISOLATE       0x20000000U
#define QTN_FLAG_WIFI_INTRA_BSS \
	(QTN_FLAG_WIFI_DEVICE | QTN_FLAG_INTRA_BSS_ISOLATE)
#define QTN_FLAG_WIFI_BSS_ISOLATE \
	(QTN_FLAG_WIFI_DEVICE | QTN_FLAG_BSS_ISOLATE)
#define QTN_FLAG_IS_INTRA_BSS(flags) \
	(((flags) & QTN_FLAG_WIFI_INTRA_BSS) == QTN_FLAG_WIFI_INTRA_BSS)
#define QTN_FLAG_IS_BSS_ISOLATE(flags) \
	(((flags) & QTN_FLAG_WIFI_BSS_ISOLATE) == QTN_FLAG_WIFI_BSS_ISOLATE)

struct openmitrastar_fdb_entry {
	u8 mac_addr[ETH_ALEN];
	u8 is_local;
	u8 is_wlan;
	u32 ageing_timer_value;
};

int openmitrastar_br_fdb_fillbuf(struct net_device *dev,
		struct openmitrastar_fdb_entry *entries, unsigned long maxnum);
void openmitrastar_br_fdb_update_const(struct net_device *dev,
		const unsigned char *addr, u16 sub_port);
void br_set_ap_isolate(int enable);
int br_get_ap_isolate(void);

#endif /* _LINUX_OPENMITRASTAR_BRIDGE_H */

