
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 om_oem_qdrv_getparam(int *param_1,int param_2,uint *param_3,undefined4 param_4)

{
  int iVar1;
  uint uVar2;
  uint *puVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int *piVar8;
  int *piVar9;
  undefined4 local_30;
  uint local_2c;
  
  iVar7 = *(int *)(*param_1 + 0xcac);
  piVar9 = (int *)(iVar7 + 0x5e3c);
  piVar8 = (int *)(iVar7 + 0x5364);
  if (*piVar8 == 0) {
    iVar1 = *(int *)(*(int *)(*piVar9 + 0x1f8) + 0x730);
    iVar5 = nocache_to_muc(iVar1);
    if (iVar5 == 0) {
      iVar5 = iVar1;
    }
    uVar4 = bus_to_virt(iVar5);
    iVar5 = (*(code *)&ioremap_nocache)(uVar4,0x1ec);
    *piVar8 = iVar5;
  }
  if ((((_g_dbg_log_module & 1) != 0) && (9 < _g_dbg_log_level)) && ((_g_dbg_log_func & 1) != 0)) {
    (*(code *)&printk)("%s: -->Enter\n","qdrv_wlan_80211_getparam");
  }
  if (param_2 == 0xdc) {
    *param_3 = *(uint *)(iVar7 + 0x6df0) >> 4 & 1;
  }
  else if (param_2 < 0xdd) {
    if (param_2 == 0x7c) {
      *param_3 = (uint)*(ushort *)(iVar7 + 0x5ee6);
    }
    else if (param_2 < 0x7d) {
      if (param_2 == 0x4e) {
        *param_3 = *(uint *)(iVar7 + 0x538);
      }
      else if (param_2 < 0x4f) {
        if (param_2 == 0x41) {
          *param_3 = (uint)*(byte *)(param_1[1] + 0x5342);
        }
        else if (param_2 < 0x42) {
          if (param_2 == 2) {
            if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 10)) ||
               ((_g_dbg_log_func & 0x200) == 0)) {
              iVar7 = *(int *)(iVar7 + 0x14);
            }
            else {
              (*(code *)&printk)("%s: 802.11ac mode = %d\n","qdrv_wlan_80211_get_11ac_mode",
                                 *(undefined4 *)(iVar7 + 0x14));
              iVar7 = *(int *)(iVar7 + 0x14);
            }
            *param_3 = (uint)(iVar7 - 0xbU < 4);
          }
          else {
            if (param_2 != 0x3f) goto LAB_ram_00040904;
            *param_3 = *(uint *)(iVar7 + 0x8000);
          }
        }
        else {
          if (param_2 == 0x47) {
            if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
               ((_g_dbg_log_func & 0x200) == 0)) {
              iVar7 = param_1[1];
            }
            else {
              (*(code *)&printk)("%s: IEEE80211_PARAM_BW_SEL_MUC (%d)\n","qdrv_wlan_80211_getparam",
                                 0x47);
              iVar7 = param_1[1];
            }
            switch(*(undefined4 *)(iVar7 + 0x14)) {
            case 1:
            case 2:
            case 3:
            case 7:
            case 8:
            case 0xb:
              uVar6 = 0x14;
              break;
            default:
              uVar6 = 0;
              break;
            case 9:
            case 10:
            case 0xc:
              uVar6 = 0x28;
              break;
            case 0xd:
              uVar6 = 0x50;
              break;
            case 0xe:
              uVar6 = 0xa0;
            }
            goto LAB_ram_00040c80;
          }
          if (param_2 != 0x4d) goto LAB_ram_00040904;
          if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
             ((_g_dbg_log_func & 0x200) != 0)) {
            (*(code *)&printk)("%s: IEEE80211_PARAM_TXBF_CTRL (%d)\n","qdrv_wlan_80211_getparam",
                               0x4d);
          }
          qdrv_txbf_config_get(iVar7,&local_2c);
          *param_3 = local_2c;
        }
      }
      else if (param_2 == 0x6a) {
        uVar4 = qdrv_soc_get_addr_dev();
        uVar6 = qdrv_command_read_rf_reg(uVar4,0xa6);
        uVar2 = qdrv_command_read_rf_reg(uVar4,0xa8);
        uVar6 = (uint)((uVar6 & 0x1c000) == 0x1c000 && (uVar2 & 0xc) == 0);
LAB_ram_00040c80:
        *param_3 = uVar6;
      }
      else if (param_2 < 0x6b) {
        if (param_2 == 0x5a) {
          if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
             ((_g_dbg_log_func & 0x200) != 0)) {
            (*(code *)&printk)("%s: IEEE80211_PARAM_GET_RFCHIP_ID (%d)\n","qdrv_wlan_80211_getparam"
                               ,0x5a);
          }
          *param_3 = (uint)*(byte *)(iVar7 + 0x5e3a);
        }
        else {
          if (param_2 != 0x5b) goto LAB_ram_00040904;
          if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
             ((_g_dbg_log_func & 0x200) != 0)) {
            (*(code *)&printk)("%s: IEEE80211_PARAM_GET_RFCHIP_VERID (%d)\n",
                               "qdrv_wlan_80211_getparam",0x5b);
          }
          *param_3 = (uint)*(byte *)(iVar7 + 0x5e3b);
        }
      }
      else if (param_2 == 0x6e) {
        if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
           ((_g_dbg_log_func & 0x200) == 0)) {
          iVar7 = param_1[1];
        }
        else {
          (*(code *)&printk)("%s: IEEE80211_PARAM_LDPC (%d)\n","qdrv_wlan_80211_getparam",0x6e);
          iVar7 = param_1[1];
        }
        *param_3 = *(uint *)(iVar7 + 0x520);
      }
      else {
        if (param_2 != 0x70) goto LAB_ram_00040904;
        if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
           ((_g_dbg_log_func & 0x200) == 0)) {
          iVar7 = param_1[1];
        }
        else {
          (*(code *)&printk)("%s: IEEE80211_PARAM_11N_40_ONLY_MODE (%d)\n",
                             "qdrv_wlan_80211_getparam",0x70);
          iVar7 = param_1[1];
        }
        *param_3 = *(uint *)(iVar7 + 0xbd8);
      }
    }
    else if (param_2 == 0xa5) {
      if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
         ((_g_dbg_log_func & 0x200) == 0)) {
        iVar7 = param_1[1];
      }
      else {
        (*(code *)&printk)("%s: IEEE80211_PARAM_RX_AGG_TIMEOUT (%d)\n","qdrv_wlan_80211_getparam",
                           0xa5);
        iVar7 = param_1[1];
      }
      *param_3 = *(uint *)(iVar7 + 0x5344);
    }
    else if (param_2 < 0xa6) {
      if (param_2 == 0x8a) {
        *param_3 = (uint)*(byte *)(param_1[1] + 0x5341);
      }
      else if (param_2 < 0x8b) {
        if (param_2 == 0x7d) {
          *param_3 = *(uint *)(iVar7 + 0x7ec0);
        }
        else {
          if (param_2 != 0x81) goto LAB_ram_00040904;
          *param_3 = *(uint *)(iVar7 + 0x7ec4);
        }
      }
      else if (param_2 == 0xa0) {
        if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
           ((_g_dbg_log_func & 0x200) == 0)) {
          iVar7 = param_1[1];
        }
        else {
          (*(code *)&printk)("%s: IEEE80211_PARAM_STBC (%d)\n","qdrv_wlan_80211_getparam",0xa0);
          iVar7 = param_1[1];
        }
        *param_3 = *(uint *)(iVar7 + 0x524);
      }
      else {
        if (param_2 != 0xa1) goto LAB_ram_00040904;
        if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
           ((_g_dbg_log_func & 0x200) == 0)) {
          iVar7 = param_1[1];
        }
        else {
          (*(code *)&printk)("%s: IEEE80211_PARAM_RTS_CTS (%d)\n","qdrv_wlan_80211_getparam",0xa1);
          iVar7 = param_1[1];
        }
        *param_3 = *(uint *)(iVar7 + 0x4fc);
      }
    }
    else if (param_2 == 0xba) {
      *param_3 = (uint)*(byte *)(iVar7 + 0x7ed4);
    }
    else if (param_2 < 0xbb) {
      if (param_2 == 0xb6) {
        *param_3 = *(uint *)(iVar7 + 0x7ec8);
      }
      else {
        if (param_2 != 0xb7) goto LAB_ram_00040904;
        *param_3 = *(uint *)(iVar7 + 0x7ecc);
      }
    }
    else if (param_2 == 0xbb) {
      *param_3 = (uint)*(byte *)(iVar7 + 0x7ed5);
    }
    else {
      if (param_2 != 0xd5) goto LAB_ram_00040904;
      qdrv_get_br_ipaddr(iVar7,param_3);
    }
  }
  else if (param_2 == 0x14b) {
    *param_3 = (uint)*(byte *)(iVar7 + 0x5360);
  }
  else if (param_2 < 0x14c) {
    if (param_2 == 0x108) {
      iVar5 = *(int *)(iVar7 + 0x65a8);
      if (iVar5 == 0) {
        if (*(int *)(*piVar9 + 0x1f8) != 0) {
          iVar1 = *(int *)(*(int *)(*piVar9 + 0x1f8) + 0x724);
          iVar5 = nocache_to_muc(iVar1);
          if (iVar5 == 0) {
            iVar5 = iVar1;
          }
          uVar4 = bus_to_virt(iVar5);
          iVar5 = (*(code *)&ioremap_nocache)(uVar4,0x3ec);
          *(int *)(iVar7 + 0x65a8) = iVar5;
          if (iVar5 != 0) goto LAB_ram_00040e94;
        }
        uVar6 = 0;
      }
      else {
LAB_ram_00040e94:
        uVar6 = *(uint *)(iVar5 + 0x124);
      }
      *param_3 = uVar6;
    }
    else if (param_2 < 0x109) {
      if (param_2 == 0xf0) {
        *param_3 = (uint)*(byte *)(iVar7 + 0x7ed6);
      }
      else if (param_2 < 0xf1) {
        if (param_2 == 0xdf) {
          *param_3 = (uint)*(byte *)(*(int *)(iVar7 + 0x7fb4) + 0x18);
        }
        else {
          if (param_2 != 0xef) goto LAB_ram_00040904;
          *param_3 = g_carrier_id;
        }
      }
      else if (param_2 == 0xf2) {
        *param_3 = *(uint *)(iVar7 + 0x7ed0);
      }
      else {
        if (param_2 != 0x103) goto LAB_ram_00040904;
        iVar5 = *(int *)(iVar7 + 0x65b4);
        if (iVar5 == 0) {
          if (*(int *)(*piVar9 + 0x1f8) != 0) {
            iVar1 = *(int *)(*(int *)(*piVar9 + 0x1f8) + 0x730);
            iVar5 = nocache_to_muc(iVar1);
            if (iVar5 == 0) {
              iVar5 = iVar1;
            }
            uVar4 = bus_to_virt(iVar5);
            iVar5 = (*(code *)&ioremap_nocache)(uVar4,0x1ec);
            *(int *)(iVar7 + 0x65b4) = iVar5;
            if (iVar5 != 0) goto LAB_ram_000411cc;
          }
LAB_ram_000411e8:
          uVar6 = 0xfffffff2;
        }
        else {
LAB_ram_000411cc:
          uVar6 = (*(int *)(iVar5 + 0xe8) + 5U) / 10;
          if (10 < uVar6) goto LAB_ram_000411e8;
        }
        *param_3 = uVar6;
      }
    }
    else if (param_2 == 0x129) {
      if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
         ((_g_dbg_log_func & 0x200) == 0)) {
        iVar7 = param_1[1];
      }
      else {
        (*(code *)&printk)("%s: IEEE80211_PARAM_PEER_RTS_MODE (%d)\n","qdrv_wlan_80211_getparam",
                           0x129);
        iVar7 = param_1[1];
      }
      *param_3 = (uint)*(byte *)(iVar7 + 0xbec);
    }
    else if (param_2 < 0x12a) {
      if (param_2 == 0x113) {
        *param_3 = (uint)*(byte *)(iVar7 + 0x5361);
      }
      else {
        if (param_2 != 0x124) goto LAB_ram_00040904;
        if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
           ((_g_dbg_log_func & 0x200) == 0)) {
          iVar7 = param_1[1];
        }
        else {
          (*(code *)&printk)("%s: IEEE80211_PARAM_TX_QOS_SCHED (%d)\n","qdrv_wlan_80211_getparam",
                             0x124);
          iVar7 = param_1[1];
        }
        *param_3 = (uint)*(byte *)(iVar7 + 0xbea);
      }
    }
    else if (param_2 == 0x12a) {
      if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
         ((_g_dbg_log_func & 0x200) != 0)) {
        (*(code *)&printk)("%s: IEEE80211_PARAM_DYN_WMM (%d)\n","qdrv_wlan_80211_getparam",0x12a);
      }
      *param_3 = (uint)*(byte *)(iVar7 + 0xbef);
    }
    else {
      if (param_2 != 0x132) goto LAB_ram_00040904;
      *param_3 = *(uint *)(iVar7 + 0x5354);
    }
  }
  else if (param_2 == 0x1ac) {
    *param_3 = DAT_ram_00092110;
    DAT_ram_00092110 = DAT_ram_00092110 + 1;
  }
  else if (param_2 < 0x1ad) {
    if (param_2 == 0x170) {
      iVar5 = *param_1;
      uVar6 = *param_3;
      iVar7 = vnet_alloc_ioctl(iVar5);
      if ((iVar7 == 0) ||
         (puVar3 = (uint *)qdrv_hostlink_alloc_coherent(0,4,&local_30,0x21), puVar3 == (uint *)0x0))
      {
        if (1 < _g_dbg_log_level) {
          (*(code *)&printk)("%s: ERROR - Failed to allocate DISASSOC message\n",
                             "qdrv_get_mu_grp_qmat");
        }
        vnet_free_ioctl(iVar7);
        *param_3 = 0xffffffff;
        goto LAB_ram_00040b04;
      }
      if ((((_g_dbg_log_module & 1) != 0) && (5 < _g_dbg_log_level)) &&
         ((_g_dbg_log_func & 0x200) != 0)) {
        (*(code *)&printk)("%s: (1)ioctl %p dma ptr %p\n","qdrv_get_mu_grp_qmat",iVar7,puVar3);
      }
      *(undefined4 *)(iVar7 + 4) = 0x3a;
      *(undefined4 *)(iVar7 + 8) = *(undefined4 *)(iVar5 + 0xcb4);
      *(uint *)(iVar7 + 0xc) = (int)uVar6 >> 0x10 & 0xff;
      *(undefined4 *)(iVar7 + 0x10) = local_30;
      iVar7 = vnet_send_ioctl(iVar5,iVar7);
      if (iVar7 == 0) {
LAB_ram_00040d3c:
        uVar6 = 0xffffffff;
      }
      else {
LAB_ram_00041018:
        uVar6 = *puVar3;
      }
      qdrv_hostlink_free_coherent(0,4,puVar3,local_30);
      *param_3 = uVar6;
    }
    else if (param_2 < 0x171) {
      if (param_2 == 0x15d) {
        iVar5 = *param_1;
        iVar7 = vnet_alloc_ioctl(iVar5);
        if ((iVar7 != 0) &&
           (puVar3 = (uint *)qdrv_hostlink_alloc_coherent(0,4,&local_30,0x21), puVar3 != (uint *)0x0
           )) {
          if ((_g_dbg_log_module & 1) == 0) {
LAB_ram_00040d00:
            uVar4 = 0x39;
          }
          else {
            uVar4 = 0x39;
            if (5 < _g_dbg_log_level) {
              if ((_g_dbg_log_func & 0x200) == 0) goto LAB_ram_00040d00;
              (*(code *)&printk)("%s: (1)ioctl %p dma ptr %p\n","qdrv_get_mu_enable",iVar7,puVar3);
              uVar4 = 0x39;
            }
          }
LAB_ram_00040d04:
          *(undefined4 *)(iVar7 + 4) = uVar4;
          *(undefined4 *)(iVar7 + 8) = *(undefined4 *)(iVar5 + 0xcb4);
          *(undefined4 *)(iVar7 + 0x10) = local_30;
          iVar7 = vnet_send_ioctl(iVar5,iVar7);
          if (iVar7 != 0) goto LAB_ram_00041018;
          goto LAB_ram_00040d3c;
        }
        if (1 < _g_dbg_log_level) {
          (*(code *)&printk)("%s: ERROR - Failed to allocate DISASSOC message\n",
                             "qdrv_get_mu_enable");
        }
LAB_ram_00041574:
        vnet_free_ioctl(iVar7);
        *param_3 = 0xffffffff;
      }
      else {
        if (param_2 != 0x160) goto LAB_ram_00040904;
        iVar5 = *param_1;
        iVar7 = vnet_alloc_ioctl(iVar5);
        if ((iVar7 == 0) ||
           (iVar1 = qdrv_hostlink_alloc_coherent(0,0xc00,&local_30,0x21), iVar1 == 0)) {
          if (1 < _g_dbg_log_level) {
            (*(code *)&printk)("%s: ERROR - Failed to allocate DISASSOC message\n","qdrv_get_mu_grp"
                              );
          }
          vnet_free_ioctl(iVar7);
        }
        else {
          if ((((_g_dbg_log_module & 1) != 0) && (5 < _g_dbg_log_level)) &&
             ((_g_dbg_log_func & 0x200) != 0)) {
            (*(code *)&printk)("%s: (1)ioctl %p dma ptr %p\n","qdrv_get_mu_grp",iVar7,iVar1);
          }
          *(undefined4 *)(iVar7 + 4) = 0x37;
          *(undefined4 *)(iVar7 + 8) = *(undefined4 *)(iVar5 + 0xcb4);
          *(undefined4 *)(iVar7 + 0x10) = local_30;
          iVar7 = vnet_send_ioctl(iVar5,iVar7);
          if (iVar7 != 0) {
            (*(code *)&memcpy)(param_4,iVar1,0xc00);
          }
          qdrv_hostlink_free_coherent(0,0xc00,iVar1,local_30);
        }
      }
    }
    else {
      if (param_2 == 0x171) {
        iVar5 = *param_1;
        iVar7 = vnet_alloc_ioctl(iVar5);
        if ((iVar7 == 0) ||
           (puVar3 = (uint *)qdrv_hostlink_alloc_coherent(0,4,&local_30,0x21), puVar3 == (uint *)0x0
           )) {
          if (1 < _g_dbg_log_level) {
            (*(code *)&printk)("%s: ERROR - Failed to allocate DISASSOC message\n",
                               "qdrv_get_mu_use_eq");
          }
          goto LAB_ram_00041574;
        }
        if ((_g_dbg_log_module & 1) == 0) {
LAB_ram_000414c4:
          uVar4 = 0x3b;
        }
        else {
          uVar4 = 0x3b;
          if (5 < _g_dbg_log_level) {
            if ((_g_dbg_log_func & 0x200) == 0) goto LAB_ram_000414c4;
            (*(code *)&printk)("%s: (1)ioctl %p dma ptr %p\n","qdrv_get_mu_use_eq",iVar7,puVar3);
            uVar4 = 0x3b;
          }
        }
        goto LAB_ram_00040d04;
      }
      if (param_2 != 0x1a5) goto LAB_ram_00040904;
      *param_3 = (uint)*(byte *)(iVar7 + 0x7fbe);
    }
  }
  else {
    if (param_2 == 0x1e3) {
      uVar6 = (uint)(*(char *)(iVar7 + 0x5d3d) != '\0');
      goto LAB_ram_00040c80;
    }
    if (param_2 < 0x1e4) {
      if (param_2 == 0x1ca) {
        *param_3 = *(uint *)(*piVar8 + 0xe0);
      }
      else {
        if (param_2 != 0x1df) goto LAB_ram_00040904;
        *param_3 = *(uint *)(iVar7 + 0x4f8);
      }
    }
    else if (param_2 < 0x1f0) {
LAB_ram_00040904:
      if ((_g_dbg_log_module & 1) == 0) {
        return 0;
      }
      if ((_g_dbg_log_level < 4) || ((_g_dbg_log_func & 0x200) == 0)) goto LAB_ram_00040940;
      (*(code *)&printk)("%s: <0x%08x> (%d)\n","qdrv_wlan_80211_getparam",param_2,param_2);
    }
    else if (param_2 < 0x1f2) {
      *param_3 = 8;
    }
    else {
      if (param_2 != 0x89f3) goto LAB_ram_00040904;
      if (*(char *)(*(int *)(*param_1 + 0x514) + 0xc4) == '\x03') {
        *param_3 = *(uint *)(iVar7 + 0x6038);
      }
      else {
        *param_3 = 0;
      }
    }
  }
LAB_ram_00040b04:
  if ((_g_dbg_log_module & 1) == 0) {
    return 0;
  }
LAB_ram_00040940:
  if ((9 < _g_dbg_log_level) && ((_g_dbg_log_func & 1) != 0)) {
    (*(code *)&printk)("%s: <--Exit\n","qdrv_wlan_80211_getparam");
  }
  return 0;
}

