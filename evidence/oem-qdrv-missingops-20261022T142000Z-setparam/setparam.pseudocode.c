
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_00048224(undefined4 *param_1,int param_2,uint param_3,int param_4,uint param_5)

{
  ushort uVar1;
  undefined4 uVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  ushort *puVar6;
  uint uVar7;
  undefined1 *puVar8;
  byte *pbVar9;
  int iVar10;
  uint *puVar11;
  undefined4 *puVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  int iVar16;
  undefined4 *puVar17;
  byte bVar18;
  undefined2 *puVar20;
  undefined1 auStack_74 [16];
  uint local_64;
  byte local_54 [2];
  undefined2 local_52;
  undefined1 local_50;
  undefined4 local_4c;
  undefined2 local_48;
  undefined2 local_46;
  undefined1 local_44;
  undefined1 local_43;
  byte *local_40;
  undefined4 local_3c;
  undefined2 uVar19;
  
  puVar17 = (undefined4 *)*param_1;
  piVar4 = puVar17 + 0x32b;
  iVar16 = *piVar4;
  uVar2 = qdrv_soc_get_addr_dev();
  bVar18 = (byte)param_3;
  uVar19 = (undefined2)param_3;
  switch(param_2) {
  case 2:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_11AC_MODE (%d)\n","qdrv_wlan_80211_setparam",2);
    }
    iVar3 = *(int *)(iVar16 + 0x14);
    if (param_3 == 1) {
      if (3 < iVar3 - 0xbU) {
        if (iVar3 == 7) {
          *(undefined4 *)(iVar16 + 0x14) = 0xb;
        }
        else if (iVar3 == 10) {
          *(undefined4 *)(iVar16 + 0x14) = 0xc;
        }
        else {
          *(undefined4 *)(iVar16 + 0x14) = 0xd;
        }
      }
      iVar10 = (*(code *)&ieee80211_swfeat_is_supported)(4,1);
      if (iVar10 == 0) {
        *(int *)(iVar16 + 0x14) = iVar3;
      }
    }
    else if (iVar3 == 0xc || iVar3 == 0xd) {
      *(undefined4 *)(iVar16 + 0x14) = 10;
    }
    else if (iVar3 == 0xb) {
      *(undefined4 *)(iVar16 + 0x14) = 7;
    }
    if ((((_g_dbg_log_module & 1) != 0) && (9 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: 802.11ac mode = %d\n","qdrv_wlan_80211_set_11ac_mode",
                         *(undefined4 *)(iVar16 + 0x14));
    }
  case 0x112:
switchD_ram_000483e0_caseD_112:
    if (puVar17[0x112] == 6) {
LAB_ram_00048cf0:
      FUN_ram_000388ec(puVar17);
    }
    break;
  default:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: <0x%08x> (%d)\n","qdrv_wlan_80211_setparam",param_2,param_2);
    }
    break;
  case 0x3f:
    *(uint *)(iVar16 + 0x8000) = param_3;
    break;
  case 0x41:
    *(byte *)(param_1[1] + 0x5342) = bVar18;
    break;
  case 0x45:
  case 0x47:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_BW_SEL_MUC(%d)\n","qdrv_wlan_80211_setparam",param_2);
    }
    iVar3 = param_1[1];
    puVar12 = (undefined4 *)*param_1;
    if ((int)param_3 < 0x29) {
      iVar10 = *(int *)(iVar3 + 0x14);
    }
    else {
      iVar10 = (*(code *)&ieee80211_swfeat_is_supported)(4,1);
      if (iVar10 == 0) goto switchD_ram_000483e0_caseD_112;
      iVar10 = *(int *)(iVar3 + 0x14);
    }
    switch(iVar10) {
    case 1:
    case 2:
    case 3:
    case 7:
    case 8:
    case 0xb:
      uVar7 = 0x14;
      break;
    default:
      uVar7 = 0;
      break;
    case 9:
    case 10:
    case 0xc:
      uVar7 = 0x28;
      break;
    case 0xd:
      uVar7 = 0x50;
      break;
    case 0xe:
      uVar7 = 0xa0;
    }
    if (uVar7 != param_3) {
      if (param_3 == 0x14) {
        if (iVar10 - 0xbU < 4) {
          *(undefined4 *)(iVar3 + 0x14) = 0xb;
        }
        else {
          uVar7 = *(uint *)(*(int *)(iVar3 + 0x4e08) + 4);
          if ((uVar7 & 0x100) == 0) {
            if (((uVar7 & 0x80) != 0) && (iVar10 == 9)) {
              *(undefined4 *)(iVar3 + 0x14) = 8;
            }
          }
          else {
            *(undefined4 *)(iVar3 + 0x14) = 7;
          }
        }
      }
      else {
        if (param_3 != 0x28) {
          if (param_3 == 0x50) {
            if (iVar10 - 0xbU < 4) {
              *(undefined4 *)(iVar3 + 0x14) = 0xd;
              goto LAB_ram_0004a110;
            }
            if ((((_g_dbg_log_module & 1) != 0) && (5 < _g_dbg_log_level)) &&
               ((_g_dbg_log_func & 0x200) != 0)) {
              (*(code *)&printk)("%s: BW %d cannot be configured in current phymode\n",
                                 "qdrv_wlan_80211_set_cap_bw",0x50);
            }
          }
          else if ((((_g_dbg_log_module & 1) != 0) && (5 < _g_dbg_log_level)) &&
                  ((_g_dbg_log_func & 0x200) != 0)) {
            (*(code *)&printk)("%s: BW %d is not valid\n","qdrv_wlan_80211_set_cap_bw",param_3);
          }
          goto switchD_ram_000483e0_caseD_112;
        }
        if (iVar10 - 0xbU < 4) {
          *(undefined4 *)(iVar3 + 0x14) = 0xc;
        }
        else if ((*(uint *)(*(int *)(iVar3 + 0x4e08) + 4) & 0x100) == 0) {
          *(undefined4 *)(iVar3 + 0x14) = 9;
        }
        else {
          *(undefined4 *)(iVar3 + 0x14) = 10;
        }
      }
LAB_ram_0004a110:
      (*(code *)&ieee80211_update_bw_capa)(puVar12,param_3);
      (*(code *)&memset)(auStack_74,0,0x20);
      if (puVar12[0x20] != 0) {
        local_64 = (*(uint *)(iVar3 + 0x14) & 0xf) << 0x10 |
                   *(uint *)(puVar12[0x20] + 8) & 0xfff0ffff;
        iVar10 = (*(code *)&ifmedia_ioctl)(*puVar12,auStack_74,puVar12 + 0x1e,0xc0206937);
        if (iVar10 == -0x66) {
          *(undefined2 *)(iVar3 + 0x4e30) = *(undefined2 *)(iVar3 + 0x14);
          (*(code *)&ieee80211_setmode)(iVar3);
        }
        if ((((_g_dbg_log_module & 1) != 0) && (5 < _g_dbg_log_level)) &&
           ((_g_dbg_log_func & 0x200) != 0)) {
          (*(code *)&printk)("%s: QDRV: PHY mode %d\n","qdrv_wlan_80211_set_cap_bw",
                             *(undefined4 *)(iVar3 + 0x14));
        }
      }
    }
    goto switchD_ram_000483e0_caseD_112;
  case 0x4d:
    if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
       ((_g_dbg_log_func & 0x200) == 0)) {
      iVar16 = *piVar4;
    }
    else {
      (*(code *)&printk)("%s: IEEE80211_PARAM_TXBF_CTRL (%d)\n","qdrv_wlan_80211_setparam",0x4d);
      iVar16 = *piVar4;
    }
    qdrv_txbf_config_set(iVar16,param_3);
    return;
  case 0x4e:
    if (param_3 == 0) {
      puVar11 = (uint *)(iVar16 + 0x568);
      *(undefined1 *)(iVar16 + 0x567) = *(undefined1 *)(iVar16 + 0x567);
      *puVar11 = *puVar11 & 0xff000000 | *puVar11 & 0xffffe7;
      *(uint *)(iVar16 + 0x53c) = *(uint *)(iVar16 + 0x53c) & 0xffffe7ff;
    }
    else {
      puVar11 = (uint *)(iVar16 + 0x568);
      *(undefined1 *)(iVar16 + 0x567) = *(undefined1 *)(iVar16 + 0x567);
      *puVar11 = *puVar11 & 0xff000000 | (*puVar11 << 8 | 0x1800) >> 8;
      *(uint *)(iVar16 + 0x53c) = *(uint *)(iVar16 + 0x53c) | 0x1800;
    }
    *(uint *)(iVar16 + 0x538) = param_3;
    if (puVar17[0x112] != 6) break;
    goto LAB_ram_00048cf0;
  case 0x50:
    uVar7 = (param_3 & 0xff0000) >> 0x10;
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_HTBA_SEQ_CTRL (%d), %d, %d\n",
                         "qdrv_wlan_80211_setparam",0x50,uVar7,param_3 & 0xffff);
    }
    *(undefined2 *)(param_1 + uVar7 * 0xc + 0x1a8) = uVar19;
    return;
  case 0x51:
    uVar7 = (param_3 & 0xff0000) >> 0x10;
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_HTBA_SIZE_CTRL (%d), %d, %d\n",
                         "qdrv_wlan_80211_setparam",0x51,uVar7,param_3 & 0xffff);
    }
    *(undefined2 *)((int)param_1 + uVar7 * 0x30 + 0x69e) = uVar19;
    return;
  case 0x52:
    uVar7 = (param_3 & 0xff0000) >> 0x10;
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_HTBA_TIME_CTRL (%d), %d, %d\n",
                         "qdrv_wlan_80211_setparam",0x52,uVar7,param_3 & 0xffff);
    }
    *(undefined2 *)(param_1 + uVar7 * 0xc + 0x1a7) = uVar19;
    return;
  case 0x53:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_HT_ADDBA (%d)\n","qdrv_wlan_80211_setparam",0x53);
    }
    iVar16 = param_1[1];
    (*(code *)&memset)(&local_44,0,8);
    (*(code *)&memset)(local_54,0,0x10);
    local_44 = 3;
    local_4c = 1;
    local_50 = 0;
    local_43 = 0;
    local_52 = *(undefined2 *)(param_1 + param_3 * 0xc + 0x1a8);
    local_48 = *(undefined2 *)((int)param_1 + param_3 * 0x30 + 0x69e);
    local_46 = *(undefined2 *)(param_1 + param_3 * 0xc + 0x1a7);
    local_54[0] = bVar18;
    local_40 = local_54;
    (**(code **)(iVar16 + 0x5374))(param_1,0xd0,&local_44);
    return;
  case 0x54:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_HT_DELBA (%d)\n","qdrv_wlan_80211_setparam",0x54);
    }
    (*(code *)&ieee80211_send_delba)(param_1,param_3,0,1);
    (*(code *)&ieee80211_node_ba_del)(param_1,param_3 & 0xff,1,1);
    (*(code *)&ieee80211_send_delba)(param_1,param_3,1,1);
    (*(code *)&ieee80211_node_ba_del)(param_1,param_3 & 0xff,0,1);
    return;
  case 0x5d:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_SHORT_GI(%d)\n","qdrv_wlan_80211_setparam",0x5d);
    }
    iVar3 = param_1[1];
    if (param_3 == 0) {
      puVar11 = (uint *)(iVar3 + 0x568);
      *(byte *)(iVar3 + 0x567) = *(byte *)(iVar3 + 0x567) & 0xdf;
      puVar6 = (ushort *)(iVar3 + 0x500);
      *puVar11 = *puVar11 & 0xff000000 | *puVar11 & 0xffffff;
      uVar1 = *puVar6;
      *(uint *)(iVar3 + 0x53c) = *(uint *)(iVar3 + 0x53c) & 0xffffffdf;
      if ((uVar1 & 2) == 0) {
        *puVar6 = uVar1 & 0xffdf;
      }
      else {
        *puVar6 = uVar1 & 0xff9f;
      }
    }
    else {
      puVar11 = (uint *)(iVar3 + 0x568);
      *(byte *)(iVar3 + 0x567) = *(byte *)(iVar3 + 0x567) | 0x20;
      puVar6 = (ushort *)(iVar3 + 0x500);
      *puVar11 = *puVar11 & 0xff000000 | *puVar11 & 0xffffff;
      uVar1 = *puVar6;
      *(uint *)(iVar3 + 0x53c) = *(uint *)(iVar3 + 0x53c) | 0x20;
      if ((uVar1 & 2) == 0) {
        *puVar6 = uVar1 | 0x20;
      }
      else {
        *puVar6 = uVar1 | 0x60;
      }
    }
    break;
  case 0x6e:
    if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
       ((_g_dbg_log_func & 0x200) == 0)) {
      iVar3 = param_1[1];
    }
    else {
      (*(code *)&printk)("%s: IEEE80211_PARAM_LDPC(%d)\n","qdrv_wlan_80211_setparam",0x6e);
      iVar3 = param_1[1];
    }
    *(uint *)(iVar3 + 0x520) = param_3 & 1;
    break;
  case 0x70:
    if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
       ((_g_dbg_log_func & 0x200) == 0)) {
      iVar3 = param_1[1];
    }
    else {
      (*(code *)&printk)("%s: IEEE80211_PARAM_11N_40_ONLY_MODE (%d)\n","qdrv_wlan_80211_setparam",
                         0x70);
      iVar3 = param_1[1];
    }
    *(uint *)(iVar3 + 0xbd8) = param_3 & 1;
    break;
  case 0x7c:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_MAX_MGMT_FRAMES (%d)\n","qdrv_wlan_80211_setparam",
                         0x7c);
    }
    *(undefined2 *)(iVar16 + 0x5ee6) = uVar19;
    break;
  case 0x7d:
    *(uint *)(iVar16 + 0x7ec0) = param_3;
    break;
  case 0x81:
    *(uint *)(iVar16 + 0x7ec4) = param_3;
    break;
  case 0x8a:
    *(byte *)(param_1[1] + 0x5341) = bVar18;
    break;
  case 0x9f:
    if (param_3 == 0) {
      *(uint *)(iVar16 + 0x5e34) = *(uint *)(iVar16 + 0x5e34) & 0xfffffffb;
    }
    else {
      *(uint *)(iVar16 + 0x5e34) = *(uint *)(iVar16 + 0x5e34) | 4;
    }
    break;
  case 0xa0:
    if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
       ((_g_dbg_log_func & 0x200) == 0)) {
      iVar3 = param_1[1];
    }
    else {
      (*(code *)&printk)("%s: IEEE80211_PARAM_STBC(%d)\n","qdrv_wlan_80211_setparam",0xa0);
      iVar3 = param_1[1];
    }
    *(uint *)(iVar3 + 0x524) = param_3 & 1;
    break;
  case 0xa1:
    if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
       ((_g_dbg_log_func & 0x200) == 0)) {
      iVar3 = param_1[1];
    }
    else {
      (*(code *)&printk)("%s: IEEE80211_PARAM_RTS_CTS(%d)\n","qdrv_wlan_80211_setparam",0xa1);
      iVar3 = param_1[1];
    }
    *(uint *)(iVar3 + 0x4fc) = param_3 & 1;
    break;
  case 0xa4:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_GET_CH_INUSE(%d)\n","qdrv_wlan_80211_setparam",0xa4);
    }
    if (param_3 == 0) {
      *(uint *)(iVar16 + 0xa0) = *(uint *)(iVar16 + 0xa0) & 0xfffffffb;
    }
    else {
      *(uint *)(iVar16 + 0xa0) = *(uint *)(iVar16 + 0xa0) | 4;
    }
    break;
  case 0xa5:
    *(uint *)(iVar16 + 0x5344) = param_3;
    break;
  case 0xa6:
  case 0xa8:
    goto switchD_ram_000483e0_caseD_a6;
  case 0xa7:
    g_triggers_on = param_3;
    return;
  case 0xab:
  case 0xac:
    puVar8 = (undefined1 *)(iVar16 + 0x50a);
    (*(code *)&memset)(puVar8,0,10);
    iVar3 = *(int *)(iVar16 + 0xac);
    if (iVar3 == 1) {
      *puVar8 = 0xff;
    }
    else if (iVar3 == 2) {
      *puVar8 = 0xff;
      *(undefined1 *)(iVar16 + 0x50b) = 0xff;
      if (*(int *)(iVar16 + 0xa4) < 0) {
        *(undefined1 *)(iVar16 + 0x50e) = 0x7e;
      }
    }
    else if (iVar3 == 3) {
      *puVar8 = 0xff;
      *(undefined1 *)(iVar16 + 0x50b) = 0xff;
      *(undefined1 *)(iVar16 + 0x50c) = 0xff;
      if (*(int *)(iVar16 + 0xa4) < 0) {
        *(undefined1 *)(iVar16 + 0x50e) = 0xfe;
        *(undefined1 *)(iVar16 + 0x50f) = 0xff;
        *(undefined1 *)(iVar16 + 0x510) = 0x1f;
      }
    }
    else if (iVar3 == 4) {
      *puVar8 = 0xff;
      *(undefined1 *)(iVar16 + 0x50b) = 0xff;
      *(undefined1 *)(iVar16 + 0x50c) = 0xff;
      *(undefined1 *)(iVar16 + 0x50d) = 0xff;
      if (*(int *)(iVar16 + 0xa4) < 0) {
        *(undefined1 *)(iVar16 + 0x50e) = 0xfe;
        *(undefined1 *)(iVar16 + 0x50f) = 0xff;
        *(undefined1 *)(iVar16 + 0x510) = 0xff;
        *(undefined1 *)(iVar16 + 0x511) = 0xff;
        *(undefined1 *)(iVar16 + 0x512) = 0xff;
        *(undefined1 *)(iVar16 + 0x513) = 0x1f;
      }
    }
    pbVar9 = (byte *)(iVar16 + 0x518);
    *(undefined1 *)(iVar16 + 0x502) = 0;
    *pbVar9 = 1;
    if ((iVar3 == 4) && (iVar3 = (*(code *)&ieee80211_swfeat_is_supported)(6,0), iVar3 != 0)) {
      bVar18 = *pbVar9;
      *(undefined1 *)(iVar16 + 0x502) = 1;
      *pbVar9 = bVar18 | 2;
      if (*(int *)(iVar16 + 0xa4) < 0) {
        *pbVar9 = bVar18 | 0x12;
      }
    }
    break;
  case 0xb1:
    if (param_3 == 0) {
      *(uint *)(iVar16 + 0x5e34) = *(uint *)(iVar16 + 0x5e34) & 0xfffffff7;
    }
    else {
      *(uint *)(iVar16 + 0x5e34) = *(uint *)(iVar16 + 0x5e34) | 8;
    }
    break;
  case 0xb2:
    if (param_3 == 0) {
      *(uint *)(iVar16 + 0x5e34) = *(uint *)(iVar16 + 0x5e34) & 0xffffffef;
    }
    else {
      *(uint *)(iVar16 + 0x5e34) = *(uint *)(iVar16 + 0x5e34) | 0x10;
    }
    break;
  case 0xb6:
    *(uint *)(iVar16 + 0x7ec8) = param_3;
    break;
  case 0xb7:
    *(uint *)(iVar16 + 0x7ecc) = param_3;
    break;
  case 0xba:
    *(byte *)(iVar16 + 0x7ed4) = bVar18;
    break;
  case 0xbb:
    *(byte *)(iVar16 + 0x7ed5) = bVar18;
    break;
  case 0xc4:
    uVar7 = param_3 & 0xff;
    piVar4 = (int *)(iVar16 + 0x57d4);
    iVar3 = *piVar4;
    if (uVar7 < 0x16) {
      puVar11 = *(uint **)(iVar16 + 0x6e34);
      *(int *)(iVar16 + uVar7 * 4 + 0x57d4) = (int)param_3 >> 8;
      if (puVar11 != (uint *)0x0) {
        if (*piVar4 < 5) {
          uVar15 = Auxregs0028;
          if ((uVar15 & 6) != 0) {
            Auxregs0028 = uVar15 & 0xfffffff9;
          }
          uVar5 = *puVar11;
          *puVar11 = uVar5 & 0xfffffffb;
        }
        else {
          uVar15 = Auxregs0028;
          if ((uVar15 & 6) != 0) {
            Auxregs0028 = uVar15 & 0xfffffff9;
          }
          uVar5 = *puVar11;
          *puVar11 = uVar5 | 4;
        }
        Auxregs0028 = uVar15;
        if (((uVar5 == 0) != (*puVar11 == 0)) && ((code *)puVar11[1] != (code *)0x0)) {
          (*(code *)puVar11[1])();
        }
      }
      if ((iVar3 != *piVar4) && (*piVar4 != 0)) {
        qdrv_eventf(**(undefined4 **)(iVar16 + 8),"%s%s from %u to %u",&DAT_ram_000a02c8,
                    "PM-LEVEL-CHANGE",iVar3);
      }
    }
    if ((uVar7 == 7) && (iVar3 = (*(code *)&pm_qos_requirement)(4), 4 < iVar3)) {
      iVar3 = *(int *)(iVar16 + 0x57f0) * 1000;
      if (iVar3 < 0) {
        iVar3 = iVar3 + 0x3ff;
      }
      if ((uint)*(ushort *)(iVar16 + 0x4e0) != iVar3 >> 10) {
        (*(code *)&ieee80211_beacon_interval_set)(iVar16);
      }
    }
    break;
  case 0xc9:
    param_3 = (*(code *)&msecs_to_jiffies)(param_3);
    if (param_3 != puVar17[0x2be]) {
      puVar17[0x2be] = param_3;
      if (param_3 == 0) {
        (*(code *)&del_timer)(puVar17 + 0x2b7);
      }
      else {
        (*(code *)&mod_timer)(puVar17 + 0x2b7,param_3 + _jiffies);
      }
    }
    break;
  case 0xcc:
    qdrv_eventf(*puVar17,"QCAT state=%d",param_3);
    (*(code *)&printk)("QCAT state=%d\n",param_3);
    *(uint *)(iVar16 + 0x6068) = param_3;
    break;
  case 0xdd:
  case 0x1f0:
  case 0x1f1:
    break;
  case 0xde:
    uVar7 = param_3 & 0x3fffffff;
    uVar15 = param_3 >> 0x1e;
    if ((((_g_dbg_log_module & 1) != 0) && (7 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x1000000) != 0)) {
      (*(code *)&printk)("%s: manual ba throt: subcmd=%u, value=0x%x\n","qdrv_wlan_manual_ba_throt",
                         uVar15,uVar7);
    }
    switch(uVar15) {
    case 0:
      DAT_ram_00092118 = (param_3 & 0xff00) >> 8;
      DAT_ram_0009211c = uVar7 >> 0x10;
      DAT_ram_00092114 = param_3 & 0xff;
      if ((_g_dbg_log_module & 1) == 0) {
        return;
      }
      if (_g_dbg_log_level < 6) {
        return;
      }
      if ((_g_dbg_log_func & 0x1000000) == 0) {
        return;
      }
      (*(code *)&printk)("%s: set manual ba throt intv=%u dur=%u win_size=%u\n",
                         "qdrv_wlan_manual_ba_throt",DAT_ram_0009211c);
      return;
    case 1:
      uVar15 = (param_3 & 0xff00) >> 8;
      iVar16 = (*(code *)&ieee80211_find_node_by_node_idx)(puVar17,uVar15);
      if (iVar16 != 0) {
        if (uVar7 >> 0x10 == 0) {
          qdrv_wlan_vsp_ba_throt(iVar16,param_3 & 0xff,0,0,0);
        }
        else {
          qdrv_wlan_vsp_ba_throt
                    (iVar16,param_3 & 0xff,DAT_ram_0009211c,DAT_ram_00092118,DAT_ram_00092114);
        }
        (*(code *)&ieee80211_free_node)(iVar16);
        return;
      }
      break;
    case 2:
      uVar15 = param_3 & 0xff;
      iVar16 = (*(code *)&ieee80211_find_node_by_node_idx)(puVar17,uVar15);
      if (iVar16 != 0) {
        iVar3 = 0;
        (*(code *)&printk)("Node %u %pM BA table:\n",*(ushort *)(iVar16 + 0x3a) & 0x3fff,
                           iVar16 + 0xa2c);
        puVar20 = (undefined2 *)(iVar16 + 0x3cc);
        (*(code *)&printk)("tx tid state type win timeout\n");
        do {
          if (iVar3 == 0) {
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,0,*(undefined4 *)(iVar16 + 0x38c),
                               *(undefined4 *)(iVar16 + 0x398),*(undefined2 *)(iVar16 + 0x39e),
                               *(undefined2 *)(iVar16 + 0x39c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,1,*(undefined4 *)(iVar16 + 0x3bc),
                               *(undefined4 *)(iVar16 + 0x3c8),*(undefined2 *)(iVar16 + 0x3ce),
                               *puVar20);
            iVar3 = 1;
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,2,*(undefined4 *)(iVar16 + 0x3ec),
                               *(undefined4 *)(iVar16 + 0x3f8),*(undefined2 *)(iVar16 + 0x3fe),
                               *(undefined2 *)(iVar16 + 0x3fc));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,3,*(undefined4 *)(iVar16 + 0x41c),
                               *(undefined4 *)(iVar16 + 0x428),*(undefined2 *)(iVar16 + 0x42e),
                               *(undefined2 *)(iVar16 + 0x42c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,4,*(undefined4 *)(iVar16 + 0x44c),
                               *(undefined4 *)(iVar16 + 0x458),*(undefined2 *)(iVar16 + 0x45e),
                               *(undefined2 *)(iVar16 + 0x45c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,5,*(undefined4 *)(iVar16 + 0x47c),
                               *(undefined4 *)(iVar16 + 0x488),*(undefined2 *)(iVar16 + 0x48e),
                               *(undefined2 *)(iVar16 + 0x48c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,6,*(undefined4 *)(iVar16 + 0x4ac),
                               *(undefined4 *)(iVar16 + 0x4b8),*(undefined2 *)(iVar16 + 0x4be),
                               *(undefined2 *)(iVar16 + 0x4bc));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,7,*(undefined4 *)(iVar16 + 0x4dc),
                               *(undefined4 *)(iVar16 + 0x4e8),*(undefined2 *)(iVar16 + 0x4ee),
                               *(undefined2 *)(iVar16 + 0x4ec));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,8,*(undefined4 *)(iVar16 + 0x50c),
                               *(undefined4 *)(iVar16 + 0x518),*(undefined2 *)(iVar16 + 0x51e),
                               *(undefined2 *)(iVar16 + 0x51c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,9,*(undefined4 *)(iVar16 + 0x53c),
                               *(undefined4 *)(iVar16 + 0x548),*(undefined2 *)(iVar16 + 0x54e),
                               *(undefined2 *)(iVar16 + 0x54c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,10,*(undefined4 *)(iVar16 + 0x56c),
                               *(undefined4 *)(iVar16 + 0x578),*(undefined2 *)(iVar16 + 0x57e),
                               *(undefined2 *)(iVar16 + 0x57c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,0xb,*(undefined4 *)(iVar16 + 0x59c),
                               *(undefined4 *)(iVar16 + 0x5a8),*(undefined2 *)(iVar16 + 0x5ae),
                               *(undefined2 *)(iVar16 + 0x5ac));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,0xc,*(undefined4 *)(iVar16 + 0x5cc),
                               *(undefined4 *)(iVar16 + 0x5d8),*(undefined2 *)(iVar16 + 0x5de),
                               *(undefined2 *)(iVar16 + 0x5dc));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,0xd,*(undefined4 *)(iVar16 + 0x5fc),
                               *(undefined4 *)(iVar16 + 0x608),*(undefined2 *)(iVar16 + 0x60e),
                               *(undefined2 *)(iVar16 + 0x60c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,0xe,*(undefined4 *)(iVar16 + 0x62c),
                               *(undefined4 *)(iVar16 + 0x638),*(undefined2 *)(iVar16 + 0x63e),
                               *(undefined2 *)(iVar16 + 0x63c));
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",0,0xf,*(undefined4 *)(iVar16 + 0x65c),
                               *(undefined4 *)(iVar16 + 0x668),*(undefined2 *)(iVar16 + 0x66e),
                               *(undefined2 *)(iVar16 + 0x66c));
          }
          (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",iVar3,0,*(undefined4 *)(iVar16 + 0x68c),
                             *(undefined4 *)(iVar16 + 0x698),*(undefined2 *)(iVar16 + 0x69e),
                             *(undefined2 *)(iVar16 + 0x69c));
          iVar10 = 3;
          iVar13 = 1;
          do {
            iVar10 = iVar10 * 0x10 + iVar16;
            iVar14 = iVar13 + 1;
            (*(code *)&printk)("%2d %3d %5d %4d %3d %7d\n",iVar3,iVar13,
                               *(undefined4 *)(iVar10 + 0x68c),*(undefined4 *)(iVar10 + 0x698),
                               *(undefined2 *)(iVar10 + 0x69e),*(undefined2 *)(iVar10 + 0x69c));
            iVar10 = iVar14 * 3;
            iVar13 = iVar14;
          } while (iVar14 != 0x10);
          iVar3 = iVar3 + 1;
        } while (iVar3 != 2);
        (*(code *)&ieee80211_free_node)(iVar16);
        return;
      }
      break;
    case 3:
      uVar15 = uVar7 >> 0x18;
      iVar16 = uVar15 * 3;
      if (uVar15 < 8) {
        (&DAT_ram_0008974f)[iVar16] = (char)((param_3 & 0xf000) >> 0xc);
        (&DAT_ram_0008974d)[iVar16] = (char)((param_3 & 0xf00000) >> 0x14);
        (&DAT_ram_0008974e)[iVar16] = (byte)(uVar7 >> 0x10) & 0xf;
      }
      else if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
              ((_g_dbg_log_func & 0x1000000) != 0)) {
        (*(code *)&printk)("%s: invalid table index %u\n","qdrv_wlan_vsp_3rdpt_set_method",uVar15);
      }
      (*(code *)&printk)("VSP 3rd party method table:\n");
      (*(code *)&printk)("idx vendor throt_dur throt_winsize\n");
      (*(code *)&printk)("%3u 0x%04x %9u %13u\n",0,DAT_ram_0008974d,DAT_ram_0008974e,
                         DAT_ram_0008974f);
      (*(code *)&printk)("%3u 0x%04x %9u %13u\n",1,DAT_ram_00089750,DAT_ram_00089751,
                         DAT_ram_00089752);
      (*(code *)&printk)("%3u 0x%04x %9u %13u\n",2,DAT_ram_00089753,DAT_ram_00089754,
                         DAT_ram_00089755);
      (*(code *)&printk)("%3u 0x%04x %9u %13u\n",3,DAT_ram_00089756,DAT_ram_00089757,
                         DAT_ram_00089758);
      (*(code *)&printk)("%3u 0x%04x %9u %13u\n",4,DAT_ram_00089759,DAT_ram_0008975a,
                         DAT_ram_0008975b);
      (*(code *)&printk)("%3u 0x%04x %9u %13u\n",5,DAT_ram_0008975c,DAT_ram_0008975d,
                         DAT_ram_0008975e);
      (*(code *)&printk)("%3u 0x%04x %9u %13u\n",6,DAT_ram_0008975f,DAT_ram_00089760,
                         DAT_ram_00089761);
      (*(code *)&printk)("%3u 0x%04x %9u %13u\n",7,DAT_ram_00089762,DAT_ram_00089763,
                         DAT_ram_00089764);
      return;
    default:
      if ((_g_dbg_log_module & 1) == 0) {
        return;
      }
      if (_g_dbg_log_level < 6) {
        return;
      }
      if ((_g_dbg_log_func & 0x1000000) == 0) {
        return;
      }
      (*(code *)&printk)("%s: unknown subcmd %u\n","qdrv_wlan_manual_ba_throt",uVar15);
      return;
    }
    if ((_g_dbg_log_module & 1) == 0) {
      return;
    }
    if (_g_dbg_log_level < 4) {
      return;
    }
    if ((_g_dbg_log_func & 0x1000000) == 0) {
      return;
    }
    (*(code *)&printk)("%s: node %u not found\n","qdrv_wlan_manual_ba_throt",uVar15);
    return;
  case 0xdf:
    *(byte *)(*(int *)(iVar16 + 0x7fb4) + 0x18) = bVar18;
    break;
  case 0xe2:
    uVar7 = param_3 & 0x3fffffff;
    uVar15 = param_3 >> 0x1e;
    if ((((_g_dbg_log_module & 1) != 0) && (7 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x1000000) != 0)) {
      (*(code *)&printk)("%s: manual wme throt: subcmd=%u, value=0x%x\n",
                         "qdrv_wlan_manual_wme_throt",uVar15,uVar7);
    }
    if (uVar15 == 1) {
      if (uVar7 >> 0x18 != 0) {
        qdrv_wlan_vsp_wme_throt();
        return;
      }
      qdrv_wlan_vsp_wme_throt(iVar16,(param_3 & 0xf00000) >> 0x14,0,0,0,0,0,0);
      return;
    }
    if (uVar15 == 0) {
      DAT_ram_00092128 = (param_3 & 0xf00000) >> 0x14;
      DAT_ram_00092124 = (param_3 & 0xf0000) >> 0x10;
      DAT_ram_0009212c = uVar7 >> 0x18;
      DAT_ram_00092120 = param_3 & 0xffff;
      if ((_g_dbg_log_module & 1) == 0) {
        return;
      }
      if (_g_dbg_log_level < 6) {
        return;
      }
      if ((_g_dbg_log_func & 0x1000000) == 0) {
        return;
      }
      (*(code *)&printk)("%s: set manual wme throt aifsn=%u ecwmin=%u ecwmax=%u txoplimit=%u\n",
                         "qdrv_wlan_manual_wme_throt",DAT_ram_0009212c,DAT_ram_00092128);
      return;
    }
    if (uVar15 == 2) {
      puVar11 = (uint *)(iVar16 + 0x502c);
      (*(code *)&printk)("VSP wme throt state: throt_bm=0x%x, add_qwme_ie=%u\n",*puVar11,
                         *(undefined4 *)(iVar16 + 0x5054));
      (*(code *)&printk)("ac enable aifsn ecwmin ecwmax txoplimit\n");
      (*(code *)&printk)("%2u %6u %5u %6u %6u %9u\n",0,*puVar11 & 1,*(undefined1 *)(iVar16 + 0x5033)
                         ,*(undefined1 *)(iVar16 + 0x5034),*(undefined1 *)(iVar16 + 0x5035),
                         *(undefined2 *)(iVar16 + 0x5036));
      (*(code *)&printk)("%2u %6u %5u %6u %6u %9u\n",1,*puVar11 >> 1 & 1,
                         *(undefined1 *)(iVar16 + 0x503b),*(undefined1 *)(iVar16 + 0x503c),
                         *(undefined1 *)(iVar16 + 0x503d),*(undefined2 *)(iVar16 + 0x503e));
      (*(code *)&printk)("%2u %6u %5u %6u %6u %9u\n",2,*puVar11 >> 2 & 1,
                         *(undefined1 *)(iVar16 + 0x5043),*(undefined1 *)(iVar16 + 0x5044),
                         *(undefined1 *)(iVar16 + 0x5045),*(undefined2 *)(iVar16 + 0x5046));
      (*(code *)&printk)("%2u %6u %5u %6u %6u %9u\n",3,*puVar11 >> 3 & 1,
                         *(undefined1 *)(iVar16 + 0x504b),*(undefined1 *)(iVar16 + 0x504c),
                         *(undefined1 *)(iVar16 + 0x504d),*(undefined2 *)(iVar16 + 0x504e));
      return;
    }
    if ((_g_dbg_log_module & 1) == 0) {
      return;
    }
    if (_g_dbg_log_level < 6) {
      return;
    }
    if ((_g_dbg_log_func & 0x1000000) == 0) {
      return;
    }
    (*(code *)&printk)("%s: unknown subcmd %u\n","qdrv_wlan_manual_wme_throt",uVar15);
    return;
  case 0xe5:
    (*(code *)&printk)("%s: set QTN_GENPCAP to 1 and recompile\n","qdrv_genpcap_set");
    break;
  case 0xef:
    g_carrier_id = param_3;
    break;
  case 0xf0:
    *(byte *)(iVar16 + 0x7ed6) = bVar18;
    break;
  case 0xf2:
    *(uint *)(iVar16 + 0x7ed0) = param_3;
    break;
  case 0x10d:
  case 0x19b:
    FUN_ram_00044258(iVar16 + 0x567,*(undefined4 *)(iVar16 + 0xb0),*(undefined4 *)(iVar16 + 0xa8));
    FUN_ram_00044258(iVar16 + 0x53c,*(undefined4 *)(iVar16 + 0xb4),*(undefined4 *)(iVar16 + 0xa8));
    break;
  case 0x113:
    *(bool *)(iVar16 + 0x5361) = param_3 != 0;
    break;
  case 0x119:
    if (((int)param_3 >> 0x10 == 1) && ((param_3 & 0xffff) == 1)) {
      (*(code *)&gpio_config)(10,1);
      (*(code *)&gpio_wowlan_output)(10,0);
    }
    break;
  case 0x124:
    if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 4)) ||
       ((_g_dbg_log_func & 0x200) == 0)) {
      iVar3 = param_1[1];
    }
    else {
      (*(code *)&printk)("%s: IEEE80211_PARAM_TX_QOS_SCHED(%d)\n","qdrv_wlan_80211_setparam",0x124);
      iVar3 = param_1[1];
    }
    *(byte *)(iVar3 + 0xbea) = bVar18 & 0xf;
    break;
  case 0x129:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_PEER_RTS_MODE(%d)\n","qdrv_wlan_80211_setparam",0x129)
      ;
    }
    iVar3 = param_1[1];
    if ((int)param_3 < 3) {
      *(byte *)(iVar3 + 0xbec) = bVar18;
      if (param_3 == 2) goto LAB_ram_000493d8;
      if (param_3 == 1) {
        if ((int)((uint)*(ushort *)(iVar3 + 0x506a) - (uint)*(ushort *)(iVar3 + 0x5072)) < 2) {
          if (*(char *)(iVar3 + 0xbee) != '\0') {
            *(char *)(iVar3 + 0xbee) = '\0';
            goto LAB_ram_00048954;
          }
        }
        else if (*(char *)(iVar3 + 0xbee) != '\x01') {
          *(char *)(iVar3 + 0xbee) = '\x01';
          iVar10 = *(int *)(iVar3 + 8);
          goto LAB_ram_00048958;
        }
        break;
      }
      if (param_3 != 0) break;
      *(undefined1 *)(iVar3 + 0xbee) = 0;
LAB_ram_00048954:
      iVar10 = *(int *)(iVar3 + 8);
    }
    else {
      *(undefined1 *)(iVar3 + 0xbec) = 2;
LAB_ram_000493d8:
      if (*(char *)(iVar3 + 0xbee) == *(char *)(iVar3 + 0xbed)) break;
      *(char *)(iVar3 + 0xbee) = *(char *)(iVar3 + 0xbed);
      iVar10 = *(int *)(iVar3 + 8);
    }
LAB_ram_00048958:
    if (iVar10 != 0) {
      do {
        if ((*(int *)(iVar10 + 0x448) == 6) && (*(int *)(iVar10 + 0x44c) == 4)) {
          (**(code **)(iVar3 + 0x5444))(iVar10);
          iVar10 = *(int *)(iVar10 + 0xf8);
        }
        else {
          iVar10 = *(int *)(iVar10 + 0xf8);
        }
      } while (iVar10 != 0);
    }
    break;
  case 0x12a:
    if ((((_g_dbg_log_module & 1) != 0) && (3 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: IEEE80211_PARAM_DYN_WMM(%d)\n","qdrv_wlan_80211_setparam",0x12a);
    }
    *(byte *)(iVar16 + 0xbef) = bVar18;
    break;
  case 0x132:
    *(uint *)(iVar16 + 0x5354) = param_3;
    break;
  case 0x14b:
    *(bool *)(iVar16 + 0x5360) = param_3 != 0;
    break;
  case 0x1a5:
    *(bool *)(iVar16 + 0x7fbe) = param_3 != 0;
    break;
  case 0x1af:
    qdrv_hostlink_suspend_off_chan(iVar16,param_3 != 0);
    break;
  case 0x1df:
    *(uint *)(iVar16 + 0x4f8) = param_3;
    break;
  case 0x1e2:
    qdrv_calcmd_set_tx_power(uVar2,param_3 & 0xff);
    break;
  case 0x1e3:
    *(bool *)(iVar16 + 0x5d3d) = param_3 != 0;
  }
  if ((param_4 == 0) || (param_5 < 0x41)) {
    iVar3 = vnet_alloc_ioctl(puVar17);
    if ((iVar3 == 0) ||
       (piVar4 = (int *)qdrv_hostlink_alloc_coherent(0,0x4c,&local_3c,0x21), piVar4 == (int *)0x0))
    {
      if (1 < _g_dbg_log_level) {
        (*(code *)&printk)("%s: ERROR - Failed to allocate SETPARAM message\n",
                           "qdrv_wlan_80211_setparam");
      }
      vnet_free_ioctl(iVar3);
    }
    else {
      *piVar4 = param_2;
      if (param_2 == 2) {
        piVar4[1] = *(int *)(iVar16 + 0x14);
      }
      else {
        piVar4[1] = param_3;
      }
      piVar4[2] = 0;
      if ((param_4 != 0) && (0 < (int)param_5)) {
        (*(code *)&memcpy)(piVar4 + 3,param_4,param_5);
        piVar4[2] = param_5;
        if (param_2 == 0x15a) {
          (*(code *)&memcpy)((int)(piVar4 + 3) + param_5,param_1 + 0x28b,6);
          piVar4[2] = piVar4[2] + 6;
        }
      }
      *(undefined4 *)(iVar3 + 4) = 0xb;
      *(undefined4 *)(iVar3 + 8) = puVar17[0x32d];
      *(undefined4 *)(iVar3 + 0x10) = local_3c;
      vnet_send_ioctl(puVar17,iVar3);
      qdrv_hostlink_free_coherent(0,0x4c,piVar4,local_3c);
    }
  }
  else if (1 < _g_dbg_log_level) {
    (*(code *)&printk)("%s: ERROR - Unable to transport %d bytes of data (max is %ld)\n",
                       "qdrv_wlan_80211_setparam",param_5,0x40);
  }
switchD_ram_000483e0_caseD_a6:
  return;
}

