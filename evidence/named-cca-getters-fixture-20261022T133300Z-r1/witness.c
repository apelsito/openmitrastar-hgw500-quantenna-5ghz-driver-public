/**
  Copyright (c) 2008 - 2013 Quantenna Communications Inc
  All Rights Reserved

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**/
#ifndef AUTOCONF_INCLUDED
#include <linux/config.h>
#endif
#include <linux/version.h>

#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/netdevice.h>
#include <linux/igmp.h>
#include <net/iw_handler.h> /* wireless_send_event(..) */
#include <net/sch_generic.h>
#include <asm/hardware.h>
#include <asm/gpio.h>
#include <qtn/qdrv_sch.h>
#include "qdrv_features.h"
#include "qdrv_f355_lifetime.h"
#include "qdrv_debug.h"
#include "qdrv_mac.h"
#include "qdrv_soc.h"
#include "qdrv_comm.h"
#include "qdrv_bld.h"
#include "qdrv_wlan.h"
#include "qdrv_hal.h"
#include "qdrv_vap.h"	/* For vnet_send_ioctl() etc ... */
#include "qdrv_control.h"
#include "qdrv_txbf.h"
#include "qdrv_radar.h"
#include "qdrv_pktlogger.h"
#include "qdrv_config.h"
#include "qdrv_pcap.h"
#include "qdrv_auc.h"
#include "qdrv_mac_reserve.h"
#include "qdrv_dma_wire.h"
#include <qtn_platform_support.h>

#include "qdrv_netdebug_checksum.h"
#include <qtn/qtn_buffers.h>
#include <qtn/qtn_global.h>
#include <qtn/registers.h> /* To get to mac->reg-> .... */
#include <qtn/muc_phy_stats.h> /* To get to qtn_stats_log .... */
#include <qtn/shared_params.h>
#include <qtn/hardware_revision.h>
#include <qtn/bootcfg.h>
#include <qtn/qtn_trace.h>
#include "qdrv_muc_stats.h"
#include "qdrv_sch_const.h"
#include "qdrv_sch_wmm.h"
#include "net80211/ieee80211_beacon_desc.h"
#include "../wlan/openmitrastar_beacon_preflight.h"
#ifdef CONFIG_QVSP
#include "qtn/qvsp.h"
#endif

#include <radar/radar.h>
#include <qtn/muc_phy_stats.h>

#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/ctype.h>
#include <linux/pm_qos_params.h>
#include <common/ruby_pm.h>

#include <asm/board/board_config.h>
#include <asm/board/troubleshoot.h>
#include <asm/cacheflush.h>
#include <qtn/topaz_hbm.h>
#include <qtn/topaz_fwt_sw.h>
#include <qtn/topaz_congest_queue.h>
#include "soc.h"
#include "qtn_logging.h"
#include <net/arp.h>
#ifdef CONFIG_IPV6
#include <net/ip6_checksum.h>
#endif


const unsigned om_cca_layout[]={offsetof(struct ieee80211com,cca_fix_disable),offsetof(struct ieee80211com,auto_cca_enable),offsetof(struct ieee80211com,ic_getparam),sizeof(struct ieee80211com)};
