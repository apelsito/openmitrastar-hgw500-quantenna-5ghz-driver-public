
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int ieee80211_chanset_pick_channel(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  char *pcVar7;
  uint uVar8;
  uint *extraout_r1;
  uint *extraout_r1_00;
  uint *puVar9;
  byte bVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  byte *pbVar17;
  undefined4 *puVar18;
  undefined4 *puVar19;
  undefined4 *puVar20;
  undefined4 *puVar21;
  undefined4 *puVar22;
  int iVar23;
  int iVar24;
  undefined4 *puVar25;
  undefined4 *puVar26;
  int iVar27;
  int iVar28;
  int iVar29;
  int iVar30;
  int *piVar31;
  int *piVar32;
  int iVar33;
  uint *puVar34;
  uint *puVar35;
  char *pcVar36;
  uint *puVar37;
  int in_lp_count;
  bool bVar38;
  char in_is_in_loop;
  undefined8 uVar39;
  int local_ac;
  undefined1 auStack_a0 [52];
  int local_6c [4];
  undefined4 local_5c;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  
  iVar27 = *(int *)(param_1 + 0x104);
  uVar39 = ieee80211_get_bw(iVar27);
  iVar5 = (int)uVar39;
  if ((*(int *)(iVar27 + 0x24) == 6) &&
     ((*(int *)(iVar27 + 0x14) - 2U < 2 || (*(int *)(iVar27 + 0x14) - 8U < 2)))) {
    iVar23 = 0;
    do {
      uVar39 = findchannel(iVar27,iVar23,*(undefined2 *)(iVar27 + 0x4e30));
      iVar33 = (int)uVar39;
      if (((iVar33 != 0) && (iVar33 != 0xffff)) &&
         (uVar39 = ieee80211_scs_get_scaled_scan_info(iVar27,iVar23,local_6c), (int)uVar39 == 0)) {
        if (local_6c[0] == 0x14) {
LAB_ram_00013efe:
          uVar3 = 0;
        }
        else if ((*(uint *)(iVar33 + 4) & 0x20000) == 0) {
          if ((*(uint *)(iVar33 + 4) & 0x40000) == 0) goto LAB_ram_00013efe;
          uVar3 = 3;
        }
        else {
          uVar3 = 1;
        }
        uVar39 = FUN_ram_00012f9c(iVar27 + 0x5ddc,iVar23,local_6c[0],uVar3);
        iVar33 = (int)uVar39;
        if (iVar33 == 0) {
          if (0 < *(int *)(iVar27 + 0x5e08)) {
            uVar39 = (*(code *)&printk)("%s: don\'t find chanset for channel %d bw %d and sec_chan_offset %d\n"
                                        ,"ieee80211_update_scan_cca_info",iVar23,local_6c[0],uVar3);
          }
        }
        else {
          *(undefined4 *)(iVar33 + 0x30) = local_5c;
          *(undefined4 *)(iVar33 + 0x18) = local_54;
          *(undefined4 *)(iVar33 + 0x1c) = local_50;
          *(undefined4 *)(iVar33 + 0x28) = 0;
          *(undefined4 *)(iVar33 + 0x20) = local_4c;
          *(undefined4 *)(iVar33 + 0x2c) = local_54;
        }
      }
      iVar23 = iVar23 + 1;
    } while (iVar23 != 0xff);
  }
  *(int *)(iVar27 + 0x5de4) = *(int *)(iVar27 + 0x5de8);
  *(int *)(iVar27 + 0x5de8) = *(int *)(iVar27 + 0x5dec);
  *(int *)(iVar27 + 0x5dec) = *(int *)(iVar27 + 0x5df0);
  *(int *)(iVar27 + 0x5df0) = *(int *)(iVar27 + 0x5df4);
  *(int *)(iVar27 + 0x5df4) = *(int *)(iVar27 + 0x5df8);
  *(int *)(iVar27 + 0x5df8) = *(int *)(iVar27 + 0x5dfc);
  *(int *)(iVar27 + 0x5dfc) = 0;
  if (*(int *)(iVar27 + 0x4e0c) != 0xffff) {
    if (0 < *(int *)(iVar27 + 0x5e08)) {
      (*(code *)&printk)("%s: BSS channel is already configured and bypass channel selection\n",
                         "ieee80211_chanset_pick_channel");
    }
    return *(int *)(iVar27 + 0x4e0c);
  }
  iVar23 = *(int *)(iVar27 + 0x14);
  if (((iVar23 != 1) && (iVar23 != 7)) &&
     ((iVar23 != 10 && ((3 < iVar23 - 0xbU && (*(int *)(iVar27 + 0x5de4) != 0)))))) {
    if (*(int *)(iVar27 + 0x5e08) < 2) {
      return 0;
    }
    (*(code *)&printk)("%s: Channel selection not finished, start next scan\n",
                       "ieee80211_chanset_pick_channel");
    return 0;
  }
  puVar34 = *(uint **)(iVar27 + 0x5ddc);
  iVar33 = *(int *)(iVar27 + 0x5de0);
  if ((*(int *)(iVar27 + 0x24) == 6) && ((iVar23 - 2U < 2 || (iVar23 - 8U < 2)))) {
    iVar30 = iVar27 + 0x4bb0;
    iVar23 = iVar30;
    puVar9 = puVar34;
    for (iVar28 = 0; iVar28 < iVar33; iVar28 = iVar28 + 1) {
      uVar13 = puVar34[2];
      iVar14 = iVar27 + 0x4db0;
      if (uVar13 != 0x28) {
        if ((int)uVar13 < 0x29) {
          iVar14 = iVar30;
          if (uVar13 == 0x14) {
            iVar14 = iVar27 + 0x4d90;
          }
        }
        else {
          iVar14 = iVar27 + 0x4dd0;
          if ((uVar13 != 0x50) && (iVar14 = iVar23, uVar13 != 0xa0)) {
            iVar14 = iVar30;
          }
        }
      }
      uVar8 = *puVar34;
      uVar11 = uVar8 & 0x80000007;
      uVar12 = uVar8;
      if ((int)uVar8 < 0) {
        uVar12 = uVar8 + 7;
      }
      if ((int)uVar11 < 0) {
        uVar11 = (uVar11 - 1 | 0xfffffff8) + 1;
      }
      if (((((int)(uint)*(byte *)(iVar14 + ((int)uVar12 >> 3)) >> ((byte)uVar11 & 0x1f) & 1U) == 0)
          || (((int)(uint)*(byte *)(((int)uVar12 >> 3) + iVar27 + 0x4cd0) >> ((byte)uVar11 & 0x1f) &
              1U) != 0)) ||
         (iVar23 = ieee80211_is_channel_disabled(iVar27,uVar8,uVar13), iVar23 != 0)) {
LAB_ram_0001427a:
        puVar34[4] = 1;
      }
      else if (0x14 < (int)puVar34[2]) {
        if (puVar34[1] == 1) {
          uVar13 = *puVar34 + 4;
        }
        else {
          uVar13 = *puVar34;
          if (puVar34[1] == 3) {
            uVar13 = uVar13 - 4;
          }
        }
        uVar12 = uVar13 & 0x80000007;
        if ((int)uVar13 < 0) {
          uVar13 = uVar13 + 7;
        }
        bVar38 = (int)uVar12 < 0;
        if (bVar38) {
          uVar12 = uVar12 - 1;
        }
        bVar10 = (byte)uVar12;
        if (bVar38) {
          bVar10 = (bVar10 | 0xf8) + 1;
        }
        if (((int)(uint)*(byte *)(iVar14 + ((int)uVar13 >> 3)) >> (bVar10 & 0x1f) & 1U) == 0)
        goto LAB_ram_0001427a;
        if (0x28 < (int)puVar34[2]) {
          iVar23 = findchannel_any(iVar27,*puVar34,*(undefined2 *)(iVar27 + 0x4e30));
          if ((iVar23 != 0) && (iVar23 != 0xffff)) {
            uVar8 = ieee80211_find_sec40u_chan();
            uVar13 = ieee80211_find_sec40l_chan(iVar23);
            uVar12 = uVar8;
            if ((int)uVar8 < 0) {
              uVar12 = uVar8 + 7;
            }
            bVar10 = (byte)(uVar8 & 0x80000007);
            if ((int)(uVar8 & 0x80000007) < 0) {
              bVar10 = (bVar10 - 1 | 0xf8) + 1;
            }
            if (((int)(uint)*(byte *)(iVar14 + ((int)uVar12 >> 3)) >> (bVar10 & 0x1f) & 1U) != 0) {
              uVar12 = uVar13 & 0x80000007;
              if ((int)uVar13 < 0) {
                uVar13 = uVar13 + 7;
              }
              bVar38 = (int)uVar12 < 0;
              if (bVar38) {
                uVar12 = uVar12 - 1;
              }
              bVar10 = (byte)uVar12;
              if (bVar38) {
                bVar10 = (bVar10 | 0xf8) + 1;
              }
              if (((int)(uint)*(byte *)(iVar14 + ((int)uVar13 >> 3)) >> (bVar10 & 0x1f) & 1U) != 0)
              goto LAB_ram_00014280;
            }
            goto LAB_ram_0001427a;
          }
          if (-1 < *(int *)(iVar27 + 0x5e08)) {
            (*(code *)&printk)("%s: fail to find channel %d\n",
                               "ieee80211_udpate_chan_table_invalid_flag",*puVar34);
          }
        }
      }
LAB_ram_00014280:
      puVar34 = puVar34 + 0x14;
      iVar23 = iVar14;
    }
    iVar23 = 1;
    if (-1 < iVar33) {
      iVar23 = iVar33 + 1;
    }
    _DAT_auxregs_000c = 0x142c0;
    _DAT_auxregs_0008 = (code *)&LAB_ram_000142b4;
    puVar34 = puVar9;
    while( true ) {
      iVar23 = iVar23 + -1;
      if (iVar23 == 0) break;
      puVar34[0xd] = puVar34[0xc];
      puVar34 = puVar34 + 0x14;
    }
    puVar34 = puVar9;
    for (iVar23 = 0; iVar23 < iVar33; iVar23 = iVar23 + 1) {
      iVar28 = FUN_ram_00013598(iVar27,puVar9);
      iVar30 = FUN_ram_00013514(iVar27,puVar9);
      puVar35 = puVar34;
      for (iVar14 = 0; iVar14 < iVar33; iVar14 = iVar14 + 1) {
        iVar1 = FUN_ram_00013598(iVar27,puVar34);
        iVar24 = FUN_ram_00013514(iVar27,puVar34);
        piVar32 = &DAT_ram_00085f78;
        puVar37 = puVar34;
        do {
          iVar29 = *piVar32;
          piVar32 = piVar32 + 3;
          if ((iVar30 - iVar29 < iVar1) && (iVar1 <= iVar30)) {
            puVar9[0xe] = puVar9[0xe] + puVar37[10];
          }
          if ((iVar24 < iVar28 + iVar29) && (iVar28 <= iVar24)) {
            puVar9[0xe] = puVar9[0xe] + puVar37[10];
          }
          puVar37 = puVar37 + 1;
        } while (piVar32 != (int *)0x85f90);
        puVar34 = puVar34 + 0x14;
      }
      puVar9 = puVar9 + 0x14;
      puVar34 = puVar35;
    }
    if (*(int *)(iVar27 + 0x24) == 6) {
      piVar32 = (int *)(*(int *)(*(int *)(iVar27 + 0x4e4c) + 8) + 0x21c8);
      local_ac = 1;
      iVar23 = 0;
      do {
        iVar28 = *piVar32;
        while (iVar28 != 0) {
          iVar14 = *(int *)(iVar28 + 0xd0);
          uVar3 = ieee80211_get_max_ap_bw(iVar28,puVar9);
          uVar4 = ieee80211_get_ap_sec_chan_offset(iVar28);
          iVar30 = FUN_ram_00012ffc(iVar23,uVar3,uVar4);
          if (iVar30 == 0) {
            (*(code *)&memset)(local_6c,0,0x21);
            bVar10 = 0x20;
            if (*(byte *)(iVar28 + 0xd) < 0x21) {
              bVar10 = *(byte *)(iVar28 + 0xd);
            }
            (*(code *)&memcpy)(local_6c,iVar28 + 0xe,bVar10);
            puVar9 = extraout_r1;
            iVar28 = iVar14;
            if (-1 < *(int *)(iVar27 + 0x5e08)) {
              (*(code *)&printk)("%s: fail to find chanset for beacon %s (channel %d bw %d sec20_offset %d)\n"
                                 ,"ieee80211_udpate_chan_table_cci_aci_longterm",local_6c,iVar23,
                                 uVar3,uVar4);
              puVar9 = extraout_r1_00;
            }
          }
          else {
            iVar1 = FUN_ram_00013598(iVar27,iVar30);
            iVar30 = FUN_ram_00013514(iVar27,iVar30);
            iVar24 = *(char *)(iVar28 + 0x8a) + -0x5a;
            if (iVar24 < -0x1e) {
              iVar28 = 1;
              if (iVar24 < -0x3e) {
                iVar28 = local_ac;
              }
            }
            else {
              local_ac = 0;
              iVar28 = local_ac;
            }
            local_ac = iVar28;
            puVar9 = puVar34;
            for (iVar29 = 0; iVar28 = iVar14, iVar29 < iVar33; iVar29 = iVar29 + 1) {
              iVar28 = FUN_ram_00013598(iVar27);
              iVar2 = FUN_ram_00013514(iVar27,puVar9);
              if (((*(int *)(iVar27 + 0x5dd0) < iVar24) && (iVar2 < iVar1)) && (iVar30 < iVar28)) {
                puVar9[0xf] = puVar9[0xf] + 1;
              }
              if (((iVar2 - (&DAT_ram_00085f78)[local_ac * 3] < iVar1) && (iVar1 <= iVar2)) ||
                 ((iVar30 < iVar28 + (&DAT_ram_00085f78)[local_ac * 3] && (iVar28 <= iVar30)))) {
                puVar9[0x10] = puVar9[0x10] + *(int *)(&DAT_ram_00085f7c + local_ac * 0xc);
              }
              puVar9 = puVar9 + 0x14;
            }
          }
        }
        iVar23 = iVar23 + 1;
        piVar32 = piVar32 + 2;
      } while (iVar23 != 0xff);
    }
    puVar9 = puVar34;
    for (iVar23 = 0; iVar23 < iVar33; iVar23 = iVar23 + 1) {
      iVar28 = findchannel_any(iVar27,*puVar34,*(undefined2 *)(iVar27 + 0x4e30));
      if ((iVar28 == 0) || (iVar28 == 0xffff)) {
        if (0 < *(int *)(iVar27 + 0x5e08)) {
          (*(code *)&printk)("%s: fail to find channel %d\n",
                             "ieee80211_update_chan_table_range_cost",*puVar34);
        }
      }
      else {
        puVar34[0x11] = (int)*(char *)(iVar28 + 9) - (int)*(char *)(iVar28 + puVar34[2] + 0xe);
      }
      puVar34 = puVar34 + 0x14;
    }
    puVar34 = puVar9;
    for (iVar23 = 0; iVar23 < iVar33; iVar23 = iVar23 + 1) {
      iVar28 = findchannel_any(iVar27,*puVar9,*(undefined2 *)(iVar27 + 0x4e30));
      if ((iVar28 == 0) || (iVar28 == 0xffff)) {
        if (0 < *(int *)(iVar27 + 0x5e08)) {
          (*(code *)&printk)("%s: fail to find channel %d\n","ieee80211_update_chan_table_dfs_flag",
                             *puVar9);
        }
      }
      else {
        puVar9[0x12] = (uint)((*(uint *)(iVar28 + 4) & 0x100000) != 0);
      }
      puVar9 = puVar9 + 0x14;
    }
    if (*(int *)(iVar27 + 0x24) == 6) {
      piVar31 = (int *)(*(int *)(*(int *)(iVar27 + 0x4e4c) + 8) + 0x21c8);
      iVar23 = 0;
      piVar32 = (int *)(iVar27 + 0x5e08);
      do {
        iVar28 = *piVar31;
        puVar9 = puVar34;
        while (puVar34 = puVar9, iVar28 != 0) {
          iVar1 = *(int *)(iVar28 + 0xd0);
          uVar3 = ieee80211_get_max_ap_bw(iVar28);
          iVar30 = ieee80211_get_ap_sec_chan_offset(iVar28);
          iVar14 = FUN_ram_00012ffc(iVar23,uVar3,iVar30);
          if (iVar14 == 0) {
            (*(code *)&memset)(local_6c,0,0x21);
            bVar10 = 0x20;
            if (*(byte *)(iVar28 + 0xd) < 0x21) {
              bVar10 = *(byte *)(iVar28 + 0xd);
            }
            (*(code *)&memcpy)(local_6c,iVar28 + 0xe,bVar10);
            iVar28 = iVar1;
            puVar9 = puVar34;
            if (-1 < *piVar32) {
              (*(code *)&printk)("%s: fail to find chanset for beacon %s (channel %d bw %d sec20_offset %d)\n"
                                 ,"ieee80211_udpate_chan_table_inactive_flag",local_6c,iVar23,uVar3,
                                 iVar30);
              puVar9 = puVar34;
            }
          }
          else {
            iVar28 = findchannel_any(iVar27,iVar23,*(undefined2 *)(iVar27 + 0x4e30));
            puVar9 = puVar34;
            if ((iVar28 == 0) || (iVar28 == 0xffff)) {
              iVar28 = iVar1;
              if (-1 < *piVar32) {
                (*(code *)&printk)("%s: fail to find channel %d for beacon %s\n",
                                   "ieee80211_udpate_chan_table_inactive_flag",iVar23,local_6c);
                puVar9 = puVar34;
              }
            }
            else {
              uVar13 = iVar23 + 4;
              if ((iVar30 != 1) && (uVar13 = 0, iVar30 == 3)) {
                uVar13 = iVar23 - 4;
              }
              for (iVar30 = 0; iVar28 = iVar1, iVar30 < iVar33; iVar30 = iVar30 + 1) {
                if ((puVar34[5] != 1) && (puVar34[2] != 0x14)) {
                  iVar28 = findchannel_any(iVar27,*puVar34,*(undefined2 *)(iVar27 + 0x4e30));
                  if ((iVar28 == 0) || (iVar28 == 0xffff)) {
                    if (-1 < *piVar32) {
                      (*(code *)&printk)("%s: fail to find channel %d\n",
                                         "ieee80211_udpate_chan_table_inactive_flag",*puVar34);
                    }
                  }
                  else {
                    (*(code *)&memcpy)(auStack_a0,iVar28,0x34);
                    iVar28 = ieee80211_dual_sec_chan_supported(iVar27,*puVar34);
                    if (iVar28 != 0) {
                      ieee80211_update_sec_chan_offset(auStack_a0,puVar34[1]);
                    }
                    iVar28 = ieee80211_20_40_operation_permitted
                                       (iVar27,auStack_a0,iVar23,uVar13 & 0xff);
                    puVar34[5] = (uint)(iVar28 == 0);
                  }
                }
                puVar34 = puVar34 + 0x14;
              }
            }
          }
        }
        iVar23 = iVar23 + 1;
        piVar31 = piVar31 + 2;
      } while (iVar23 != 0xff);
    }
    in_lp_count = 1;
    if (-1 < iVar33) {
      in_lp_count = iVar33 + 1;
    }
    uVar39 = CONCAT44(iVar27 + 0x5dc4,iVar27 + 24000);
    _DAT_auxregs_000c = 0x148ea;
    in_is_in_loop = '\x01';
    _DAT_auxregs_0008 = (code *)&LAB_ram_0001488e;
  }
                    /* WARNING: Could not recover jumptable at 0x000148ea. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  if ((in_is_in_loop != '\0') && (in_lp_count != 1)) {
    iVar5 = (*_DAT_auxregs_0008)((int)uVar39);
    return iVar5;
  }
  piVar32 = (int *)(iVar27 + 0x5e08);
  iVar33 = *(int *)(*(int *)(iVar27 + 0x4e4c) + 8);
  ieee80211_get_type_of_neighborhood(iVar27,(int)((ulonglong)uVar39 >> 0x20));
  iVar23 = *piVar32;
  if (1 < iVar23) {
    uVar4 = *(undefined4 *)(iVar27 + 0x5d18);
    uVar3 = ieee80211_neighborhood_type2str();
    (*(code *)&printk)("%d BSSes are found and the environment is %s\n",uVar4,uVar3);
    iVar23 = *piVar32;
  }
  if (1 < iVar23) {
    (*(code *)&printk)("%-32s %-7s %-9s %-9s %-9s %-9s %-4s %-8s\n","Beacon","Channel","Bandwidth",
                       "Sec20_off","CCI_edge-","CCI_edge+","RSSI","ACI_span");
  }
  piVar31 = (int *)(iVar33 + 0x21c8);
  iVar23 = 1;
  iVar33 = 0;
  do {
    iVar28 = *piVar31;
    while (iVar28 != 0) {
      iVar30 = *(int *)(iVar28 + 0xd0);
      (*(code *)&memset)(local_6c,0,0x21);
      bVar10 = *(byte *)(iVar28 + 0xd);
      if (0x20 < bVar10) {
        bVar10 = 0x20;
      }
      (*(code *)&memcpy)(local_6c,iVar28 + 0xe,bVar10);
      iVar14 = ieee80211_get_max_ap_bw(iVar28);
      uVar3 = ieee80211_get_ap_sec_chan_offset(iVar28);
      iVar1 = FUN_ram_00012ffc(iVar33,iVar14,uVar3);
      if (iVar1 == 0) {
        iVar28 = iVar30;
        if (-1 < *piVar32) {
          (*(code *)&printk)("%s: fail to find chanset for beacon %s (channel %d bw %d sec20_offset %d)\n"
                             ,"ieee80211_dump_neighbor_beacon_info",local_6c,iVar33,iVar14,uVar3);
        }
      }
      else {
        uVar4 = FUN_ram_00013598(iVar27,iVar1);
        uVar6 = FUN_ram_00013514(iVar27,iVar1);
        iVar1 = *(char *)(iVar28 + 0x8a) + -0x5a;
        if (iVar1 < -0x1e) {
          iVar24 = 1;
          if (iVar1 < -0x3e) {
            iVar24 = iVar23;
          }
        }
        else {
          iVar24 = 0;
        }
        iVar28 = iVar30;
        iVar23 = iVar24;
        if (1 < *piVar32) {
          if (iVar14 == 0x28) {
            pcVar36 = "40";
          }
          else if (iVar14 < 0x29) {
            if (iVar14 == 0x14) {
              pcVar36 = "20";
            }
            else {
LAB_ram_00014ad8:
              pcVar36 = "invalid";
            }
          }
          else if (iVar14 == 0x50) {
            pcVar36 = "80";
          }
          else {
            if (iVar14 != 0xa0) goto LAB_ram_00014ad8;
            pcVar36 = "160";
          }
          iVar30 = (&DAT_ram_00085f78)[iVar24 * 3];
          if (iVar30 == 0x28) {
            pcVar7 = "40";
          }
          else if (iVar30 < 0x29) {
            if (iVar30 == 0x14) {
              pcVar7 = "20";
            }
            else {
LAB_ram_00014b2c:
              pcVar7 = "invalid";
            }
          }
          else if (iVar30 == 0x50) {
            pcVar7 = "80";
          }
          else {
            if (iVar30 != 0xa0) goto LAB_ram_00014b2c;
            pcVar7 = "160";
          }
          (*(code *)&printk)("%-32s %-7d %-9s %-9d %-9d %-9d %-4d %-8s\n",local_6c,iVar33,pcVar36,
                             uVar3,uVar6,uVar4,iVar1,pcVar7);
        }
      }
    }
    iVar33 = iVar33 + 1;
    piVar31 = piVar31 + 2;
  } while (iVar33 != 0xff);
  piVar32 = (int *)(iVar27 + 0x5e08);
  iVar23 = *piVar32;
  puVar22 = *(undefined4 **)(iVar27 + 0x5ddc);
  iVar33 = *(int *)(iVar27 + 0x5de0);
  if (1 < iVar23) {
    (*(code *)&printk)("Dump Chanset table info:\n");
    iVar23 = *piVar32;
  }
  if (1 < iVar23) {
    (*(code *)&printk)("%-7s %-8s %-9s %-11s %-9s %-9s %-7s %-8s %-19s %-13s %-10s %-10s %-12s %-12s %-10s %-6s %-5s\n"
                       ,"Chanset","Pri_chan","Bandwidth","Center_chan","CCI_edge-","CCI_edge+",
                       "Invalid","Inactive","CCA_Array[0~3]","CCA_Pri[0~1]","CCI_instnt",
                       "ACI_instnt","CCI_longterm","ACI_longterm","Range_cost","Is_dfs",
                       &DAT_ram_0009463c);
  }
  pcVar36 = "invalid";
  for (iVar23 = 0; iVar23 < iVar33; iVar23 = iVar23 + 1) {
    uVar3 = FUN_ram_00013598(iVar27,puVar22);
    uVar4 = FUN_ram_00013514(iVar27,puVar22);
    if (1 < *piVar32) {
      iVar28 = puVar22[2];
      if (iVar28 == 0x28) {
        pcVar7 = "40";
      }
      else {
        pcVar7 = pcVar36;
        if (iVar28 < 0x29) {
          if (iVar28 == 0x14) {
            pcVar7 = "20";
          }
        }
        else if (iVar28 == 0x50) {
          pcVar7 = "80";
        }
        else if (iVar28 == 0xa0) {
          pcVar7 = "160";
        }
      }
      (*(code *)&printk)("%-7d %-8d %-9s %-11d %-9d %-9d %-7d %-8d ",iVar23,*puVar22,pcVar7,
                         puVar22[3],uVar4,uVar3,puVar22[4],puVar22[5]);
    }
    iVar28 = 0;
    puVar16 = puVar22;
    do {
      iVar28 = iVar28 + 1;
      if (1 < *piVar32) {
        (*(code *)&printk)("%-4d ",puVar16[6]);
      }
      puVar16 = puVar16 + 1;
    } while (iVar28 != 4);
    iVar28 = 0;
    puVar16 = puVar22;
    do {
      iVar28 = iVar28 + 1;
      if (1 < *piVar32) {
        (*(code *)&printk)("%-6d ",puVar16[10]);
      }
      puVar16 = puVar16 + 1;
    } while (iVar28 != 2);
    if (1 < *piVar32) {
      (*(code *)&printk)("%-10d %-10d %-12d %-12d %-10d %-6d %-5d\n",puVar22[0xd],puVar22[0xe],
                         puVar22[0xf],puVar22[0x10],puVar22[0x11],puVar22[0x12],puVar22[0x13]);
    }
    puVar22 = puVar22 + 0x14;
  }
  puVar22 = *(undefined4 **)(iVar27 + 0x5ddc);
  iVar23 = 1;
  if (-1 < *(int *)(iVar27 + 0x5de0)) {
    iVar23 = *(int *)(iVar27 + 0x5de0) + 1;
  }
  pbVar17 = (byte *)(iVar27 + 0x4df1);
  piVar32 = (int *)(iVar27 + 0x5e00);
  _DAT_auxregs_000c = 0x14ee8;
  _DAT_auxregs_0008 = (code *)&LAB_ram_00014dd2;
  puVar16 = (undefined4 *)0x0;
  puVar19 = (undefined4 *)0x0;
  puVar21 = (undefined4 *)0x0;
  puVar26 = (undefined4 *)0x0;
  while (iVar23 = iVar23 + -1, iVar23 != 0) {
    iVar33 = puVar22[2];
    puVar15 = puVar16;
    puVar18 = puVar19;
    puVar20 = puVar21;
    puVar25 = puVar26;
    if (iVar33 == 0xa0) {
      if ((((0x9f < *pbVar17) && (*piVar32 < 0xa1)) && (puVar22[4] == 0)) &&
         (((puVar22[5] == 0 && (puVar25 = puVar22, puVar26 != (undefined4 *)0x0)) &&
          ((int)puVar26[0x13] <= (int)puVar22[0x13])))) {
        puVar25 = puVar26;
      }
    }
    else if (iVar33 == 0x50) {
      if (((0x4f < *pbVar17) && (*piVar32 < 0x51)) &&
         (((puVar22[4] == 0 &&
           ((puVar22[5] == 0 && (puVar15 = puVar22, puVar16 != (undefined4 *)0x0)))) &&
          ((int)puVar16[0x13] <= (int)puVar22[0x13])))) {
        puVar15 = puVar16;
      }
    }
    else if (iVar33 == 0x28) {
      if (((0x27 < *pbVar17) && (*piVar32 < 0x29)) &&
         ((puVar22[4] == 0 &&
          (((puVar22[5] == 0 && (puVar18 = puVar22, puVar19 != (undefined4 *)0x0)) &&
           ((int)puVar19[0x13] <= (int)puVar22[0x13])))))) {
        puVar18 = puVar19;
      }
    }
    else if ((((((iVar33 == 0x14) && (0x13 < *pbVar17)) && (*piVar32 < 0x15)) &&
              ((puVar22[4] == 0 && (puVar22[5] == 0)))) &&
             (puVar20 = puVar22, puVar21 != (undefined4 *)0x0)) &&
            ((int)puVar21[0x13] <= (int)puVar22[0x13])) {
      puVar20 = puVar21;
    }
    puVar22 = puVar22 + 0x14;
    puVar16 = puVar15;
    puVar19 = puVar18;
    puVar21 = puVar20;
    puVar26 = puVar25;
  }
  puVar22 = puVar26;
  if (((puVar26 == (undefined4 *)0x0) && (puVar22 = puVar16, puVar16 == (undefined4 *)0x0)) &&
     (puVar22 = puVar21, puVar19 != (undefined4 *)0x0)) {
    puVar22 = puVar19;
  }
  puVar15 = puVar22;
  if (*(char *)(iVar27 + 0x4df0) != '\0') {
    if ((*(int *)(iVar27 + 0x14) - 2U < 2) || (*(int *)(iVar27 + 0x14) - 8U < 2)) {
      if ((puVar19 != (undefined4 *)0x0) &&
         ((puVar21 != (undefined4 *)0x0 &&
          (puVar21[0x13] * *(int *)(iVar27 + 0x5dd4) < (int)puVar19[0x13])))) {
        puVar15 = puVar21;
      }
    }
    else if (((puVar26 != (undefined4 *)0x0) && (puVar16 != (undefined4 *)0x0)) &&
            (puVar15 = puVar16, (int)puVar26[0x13] <= puVar16[0x13] * *(int *)(iVar27 + 0x5dd4))) {
      puVar15 = puVar22;
    }
  }
  if (puVar15 != (undefined4 *)0x0) {
    if (1 < *(int *)(iVar27 + 0x5e08)) {
      iVar23 = puVar15[2];
      if (iVar23 == 0x28) {
        pcVar36 = "40";
      }
      else if (iVar23 < 0x29) {
        if (iVar23 == 0x14) {
          pcVar36 = "20";
        }
        else {
LAB_ram_0001506c:
          pcVar36 = "invalid";
        }
      }
      else if (iVar23 == 0x50) {
        pcVar36 = "80";
      }
      else {
        if (iVar23 != 0xa0) goto LAB_ram_0001506c;
        pcVar36 = "160";
      }
      (*(code *)&printk)("%s: candidate channel %d bandwidth %s sec20_offsest %d\n",
                         "ieee80211_chanset_pick_channel",*puVar15,pcVar36,puVar15[1]);
    }
    iVar23 = findchannel_any(iVar27,*puVar15,*(undefined2 *)(iVar27 + 0x4e30));
    if ((iVar23 == 0) || (iVar23 == 0xffff)) {
      if (*(int *)(iVar27 + 0x5e08) < 0) {
        return 0;
      }
      pcVar36 = (char *)*puVar15;
      pcVar7 = "%s: fail to find candidate channel %d\n";
      goto LAB_ram_000150dc;
    }
    iVar33 = ieee80211_dual_sec_chan_supported(iVar27,*puVar15);
    if (iVar33 != 0) {
      ieee80211_update_sec_chan_offset(iVar23,puVar15[1]);
    }
    iVar33 = puVar15[2];
    if (iVar5 == iVar33) {
      return iVar23;
    }
    if (*(int *)(iVar27 + 0x5e08) < 2) goto LAB_ram_00015180;
    if (iVar33 == 0x28) {
      pcVar36 = "40";
    }
    else if (iVar33 < 0x29) {
      if (iVar33 == 0x14) {
        pcVar36 = "20";
      }
      else {
LAB_ram_00015158:
        pcVar36 = "invalid";
      }
    }
    else if (iVar33 == 0x50) {
      pcVar36 = "80";
    }
    else {
      if (iVar33 != 0xa0) goto LAB_ram_00015158;
      pcVar36 = "160";
    }
    (*(code *)&printk)("%s: change bandwidth to %s\n","ieee80211_chanset_pick_channel",pcVar36);
LAB_ram_00015180:
    ieee80211_change_bw(param_1,puVar15[2],0);
    return iVar23;
  }
  if (*(int *)(iVar27 + 0x5e00) < 0x15) {
    _DAT_auxregs_000c = 0x14ee8;
    return 0;
  }
  if (1 < *(int *)(iVar27 + 0x5e08)) {
    (*(code *)&printk)("%s: all candidate channels are inactive,try to halve bandwidth and rescan\n"
                       ,"ieee80211_chanset_pick_channel");
  }
  iVar23 = *(int *)(iVar27 + 0x5e00) >> 1;
  iVar5 = FUN_ram_000131a8(iVar27,iVar23);
  if (-1 < iVar5) {
    return 0;
  }
  if (*(int *)(iVar27 + 0x5e08) < 2) {
    return 0;
  }
  if (iVar23 == 0x28) {
    pcVar36 = "40";
  }
  else if (iVar23 < 0x29) {
    if (iVar23 == 0x14) {
      pcVar36 = "20";
    }
    else {
LAB_ram_00015008:
      pcVar36 = "invalid";
    }
  }
  else if (iVar23 == 0x50) {
    pcVar36 = "80";
  }
  else {
    if (iVar23 != 0xa0) goto LAB_ram_00015008;
    pcVar36 = "160";
  }
  pcVar7 = "%s: failed to add new scan tyep for bandwidth %s\n";
LAB_ram_000150dc:
  (*(code *)&printk)(pcVar7,"ieee80211_chanset_pick_channel",pcVar36);
  return 0;
}

