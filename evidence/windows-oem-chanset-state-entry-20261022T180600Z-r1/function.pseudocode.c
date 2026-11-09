
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_ram_0001718c(undefined4 *param_1,uint param_2,int param_3)

{
  undefined1 uVar1;
  undefined2 uVar2;
  ushort uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined1 *puVar6;
  int iVar7;
  undefined4 *puVar8;
  char *pcVar9;
  ushort *puVar10;
  int *piVar11;
  undefined4 uVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  uint uVar17;
  int iVar18;
  int unaff_r25;
  uint uVar19;
  code *pcVar20;
  undefined8 uVar21;
  undefined1 auStack_40 [4];
  undefined4 *local_3c;
  undefined1 auStack_38 [4];
  undefined2 local_34;
  
  iVar15 = param_1[0x145];
  iVar14 = param_1[0x41];
  uVar17 = param_1[0x113];
  local_3c = param_1;
  if ((param_1[0x42] & 0x80000) != 0) {
    ieee80211_note(param_1,"%s: %s -> %s\n","__ieee80211_newstate",(&ieee80211_state_name)[uVar17],
                   (&ieee80211_state_name)[param_2]);
  }
  param_1[0x113] = param_2;
  if (param_2 == 0 && uVar17 == 0) {
    return 0;
  }
  if (local_3c[0x112] == 2) {
    if (iVar15 == 0) {
      iVar15 = *(int *)(*(int *)(iVar14 + 8) + 0x514);
      goto LAB_ram_00017226;
    }
  }
  else {
LAB_ram_00017226:
    if (iVar15 == 0) {
      (*(code *)&printk)("no bss node");
      (*(code *)&show_stacktrace)(0,0);
      (*(code *)&printk)("Kernel BUG in file %s: function %s: line %d!\n",
                         "../drivers/wlan/ieee80211_proto.c","__ieee80211_newstate",0x713);
    }
  }
  uVar19 = Auxregs0028;
  if ((uVar19 & 6) != 0) {
    Auxregs0028 = uVar19 & 0xfffffff9;
  }
  *(int *)(iVar15 + 0x24) = *(int *)(iVar15 + 0x24) + 1;
  Auxregs0028 = uVar19;
  local_3c[0x11e] = 0;
  local_3c[0x11b] = 0;
  (*(code *)&del_timer)(local_3c + 0x114);
  if (((local_3c[0x112] != 6) && (local_3c[0x112] != 2)) && (uVar17 != 1)) {
    ieee80211_cancel_scan(local_3c);
  }
  switch(param_2) {
  case 0:
    switch(uVar17) {
    case 1:
      ieee80211_cancel_scan(local_3c);
      break;
    case 2:
      break;
    case 3:
      if (local_3c[0x112] == 1) {
        (**(code **)(*(int *)(iVar15 + 4) + 0x5374))(iVar15,0xc0,3);
      }
      else if (local_3c[0x112] == 6) {
        puVar6 = &LAB_ram_00015a8c;
LAB_ram_0001749e:
        ieee80211_iterate_nodes(iVar14 + 0x4e5c,puVar6,local_3c,1);
      }
      break;
    case 4:
      iVar7 = local_3c[0x112];
      if (iVar7 == 1) {
        uVar12 = *local_3c;
        uVar5 = ether_sprintf(iVar15 + 0xa2c);
        (*(code *)&printk)("<4>%s: disassociated from AP %s\n",uVar12,uVar5);
        staAssociated = 0;
        (*(code *)&memset)(auStack_38,0,0x10);
        local_34 = (*(code *)&strlen)("DISASSOCIATED_FROM_AP");
        (*(code *)&wireless_send_event)(*local_3c,0x8c02,auStack_38,"DISASSOCIATED_FROM_AP");
        local_3c[0x10f] = local_3c[0x10f] & 0xf5ffffff;
        ieee80211_scan_remove(local_3c);
        if (param_3 == 0xb) {
          pcVar20 = *(code **)(*(int *)(iVar15 + 4) + 0x5374);
          uVar5 = 0xb;
        }
        else {
          pcVar20 = *(code **)(*(int *)(iVar15 + 4) + 0x5374);
          uVar5 = 8;
        }
        (*pcVar20)(iVar15,0xa0,uVar5);
        iVar14 = _jiffies;
        if (&stack0x77ffffc0 < (undefined1 *)0x80000) {
          uVar17 = *(uint *)(unaff_r25 + 4);
        }
        else {
          uVar17 = (uint)auStack_40 & 0xffffe000;
        }
        if ((*(uint *)(uVar17 + 4) & 0x7ffff00) == 0) {
          (*(code *)&msleep)(0x32);
        }
        else {
          do {
            iVar7 = (*(code *)&msecs_to_jiffies)(0x32);
          } while (-1 < (iVar7 + iVar14) - _jiffies);
        }
        FUN_ram_00017118(local_3c,iVar15);
      }
      else if ((iVar7 == 6) || (iVar7 == 2)) {
        puVar6 = &LAB_ram_00015be4;
        goto LAB_ram_0001749e;
      }
      break;
    default:
      goto switchD_ram_000172f6_default;
    }
    ieee80211_reset_bss(local_3c);
switchD_ram_000172f6_default:
    if (*(code **)(local_3c[0x212] + 8) != (code *)0x0) {
      (**(code **)(local_3c[0x212] + 8))(local_3c);
    }
    goto switchD_ram_000172de_default;
  case 1:
    if (4 < uVar17) goto switchD_ram_000172de_default;
    piVar11 = local_3c + 0x112;
    switch(uVar17) {
    case 0:
      goto switchD_ram_00017506_caseD_0;
    default:
      if (*piVar11 != 6) {
        if (param_3 != 0) {
          ieee80211_scan_assoc_fail(iVar14,iVar15 + 0xa2c,param_3);
        }
        iVar14 = *(int *)(iVar14 + 0x4e50);
LAB_ram_000177d0:
        if (iVar14 != 1) break;
        if ((((*(uint *)(local_3c[0x41] + 0x9c) & 0x40000) == 0) || (*piVar11 != 1)) ||
           (*(int *)(local_3c[0x41] + 0x4e0c) == 0xffff)) {
          uVar17 = 6;
        }
        else {
          uVar17 = 0x86;
        }
        uVar5 = local_3c[0x123];
        goto LAB_ram_00017818;
      }
      if (((uVar17 != 1) || (*(int *)(iVar14 + 0x4e1c) == 0xffff)) ||
         (((*(uint *)(iVar14 + 0x9c) & 0x40000) != 0 ||
          ((*(uint *)(*(int *)(iVar14 + 0x4e1c) + 4) & 0x300000) == 0x100000)))) {
        uVar17 = 0x10004;
        uVar5 = local_3c[0x123];
        goto LAB_ram_00017818;
      }
      goto LAB_ram_000176a6;
    case 4:
      iVar7 = *piVar11;
      if (iVar7 == 1) {
        uVar12 = *local_3c;
        uVar5 = ether_sprintf(iVar15 + 0xa2c);
        (*(code *)&printk)("<4>%s: disassociated from AP %s\n",uVar12,uVar5);
        staAssociated = 0;
        (*(code *)&memset)(auStack_38,0,0x10);
        local_34 = (*(code *)&strlen)("DISASSOCIATED_FROM_AP");
        (*(code *)&wireless_send_event)(*local_3c,0x8c02,auStack_38,"DISASSOCIATED_FROM_AP");
        ieee80211_scan_remove(local_3c);
        (**(code **)(*(int *)(iVar15 + 4) + 0x5374))(iVar15,0xa0,8);
        FUN_ram_00017118(local_3c,iVar15);
        local_3c[0x10e] = local_3c[0x10e] & 0xfffffdff;
        iVar14 = *(int *)(iVar14 + 0x4e50);
        goto LAB_ram_000177d0;
      }
      if ((iVar7 != 6) && (iVar7 != 2)) break;
      if (-1 < *(int *)(iVar14 + 0x98)) {
        ieee80211_iterate_nodes(iVar14 + 0x4e5c,&LAB_ram_00015be4,local_3c,1);
      }
switchD_ram_00017506_caseD_0:
      if ((((*(uint *)(local_3c[0x41] + 0x9c) & 0x40000) == 0) || (local_3c[0x112] != 1)) ||
         (*(int *)(local_3c[0x41] + 0x4e0c) == 0xffff)) {
        uVar17 = 0x10006;
      }
      else {
        uVar17 = 0x10086;
      }
      iVar7 = local_3c[0x112];
      if ((((iVar7 == 0) || (iVar7 == 2)) || (iVar7 == 3)) && (*(int *)(iVar14 + 0x4e1c) != 0xffff))
      {
LAB_ram_000176a6:
        ieee80211_create_bss(local_3c);
      }
      else {
        if (iVar7 == 6) {
          puVar8 = (undefined4 *)(iVar14 + 0x54a4);
          iVar7 = (*(code *)*puVar8)(iVar14);
          if (iVar7 < 0) {
            if (*(int *)(iVar14 + 0x4e1c) != 0xffff) {
              *(uint *)(iVar14 + 0x4e24) = (uint)*(byte *)(*(int *)(iVar14 + 0x4e1c) + 8);
            }
          }
          else {
            iVar7 = (*(code *)*puVar8)(iVar14);
            if (iVar7 == 0) {
              if (*(int *)(iVar14 + 0x4e1c) == 0xffff) {
                uVar17 = uVar17 | 0x20;
              }
              else {
                *(uint *)(iVar14 + 0x4e24) = (uint)*(byte *)(*(int *)(iVar14 + 0x4e1c) + 8);
                *(undefined4 *)(iVar14 + 20000) = 0;
              }
            }
            else {
              iVar7 = (*(code *)*puVar8)(iVar14);
              if (0 < iVar7) {
                *(undefined4 *)(iVar14 + 0x4e24) = 0;
                if (*(int *)(iVar14 + 0x4e1c) == 0xffff) {
                  uVar19 = 0;
                }
                else {
                  uVar19 = (uint)*(byte *)(*(int *)(iVar14 + 0x4e1c) + 8);
                }
                *(uint *)(iVar14 + 20000) = uVar19;
              }
            }
          }
        }
        iVar14 = ieee80211_chan_selection_allowed(iVar14);
        if (iVar14 == 0) {
          uVar5 = local_3c[0x123];
LAB_ram_00017818:
          ieee80211_check_scan(local_3c,uVar17,0x7fffffff,uVar5,local_3c + 0x124,0);
        }
        else {
          ieee80211_start_chanset_selection(local_3c,uVar17);
        }
      }
    }
    goto switchD_ram_000172de_default;
  case 2:
    if (1 < (uint)local_3c[0x112]) {
      (*(code *)&printk)("switch to %s state when operating in mode %u",
                         PTR_DAT_ram_00094e20_ram_00091ca8);
      (*(code *)&show_stacktrace)(0,0);
      (*(code *)&printk)("Kernel BUG in file %s: function %s: line %d!\n",
                         "../drivers/wlan/ieee80211_proto.c","__ieee80211_newstate",0x7dc);
    }
    switch(uVar17) {
    case 2:
    case 3:
      if (param_3 != 0xb0) {
        if (param_3 != 0xc0) goto switchD_ram_000172de_default;
        goto switchD_ram_000178d2_caseD_0;
      }
      uVar5 = 0xb0;
      pcVar20 = *(code **)(*(int *)(iVar15 + 4) + 0x5374);
      uVar12 = 2;
      break;
    case 4:
      uVar12 = *local_3c;
      uVar5 = ether_sprintf(iVar15 + 0xa2c);
      (*(code *)&printk)("<4>%s: deauthenticated from AP %s\n",uVar12,uVar5);
      staAssociated = 0;
      (*(code *)&memset)(auStack_38,0,0x10);
      local_34 = (*(code *)&strlen)("DISASSOCIATED_FROM_AP");
      (*(code *)&wireless_send_event)(*local_3c,0x8c02,auStack_38,"DISASSOCIATED_FROM_AP");
      if (param_3 == 0xb0) {
        (**(code **)(*(int *)(iVar15 + 4) + 0x5374))(iVar15,0xb0,2);
        local_3c[0x113] = 4;
        goto switchD_ram_000172de_default;
      }
      if ((param_3 != 0xc0) || (FUN_ram_00017118(local_3c,iVar15), *(int *)(iVar14 + 0x4e50) != 1))
      goto switchD_ram_000172de_default;
    case 0:
    case 1:
switchD_ram_000178d2_caseD_0:
      uVar5 = 0xb0;
      pcVar20 = *(code **)(*(int *)(iVar15 + 4) + 0x5374);
      uVar12 = 1;
      break;
    default:
      goto switchD_ram_000172de_default;
    }
    goto LAB_ram_00017a94;
  case 3:
    if (local_3c[0x112] != 1) {
      (*(code *)&printk)("switch to %s state when operating in mode %u",
                         PTR_s_ASSOC_ram_00094e28_ram_00091cac);
      (*(code *)&show_stacktrace)(0,0);
      (*(code *)&printk)("Kernel BUG in file %s: function %s: line %d!\n",
                         "../drivers/wlan/ieee80211_proto.c","__ieee80211_newstate",0x80e);
    }
    break;
  case 4:
    switch(uVar17) {
    case 0:
      iVar7 = local_3c[0x112];
      if (((iVar7 != 8) && (iVar7 != 2)) && (iVar7 != 6)) goto switchD_ram_00017b70_caseD_2;
      if ((*(int *)(iVar14 + 0x4e1c) == 0) || (*(int *)(iVar14 + 0x4e1c) == 0xffff)) {
        (*(code *)&printk)("Error: create BSS on an invalid desired channel");
        (*(code *)&show_stacktrace)(0,0);
        (*(code *)&printk)("Kernel BUG in file %s: function %s: line %d!\n",
                           "../drivers/wlan/ieee80211_proto.c","__ieee80211_newstate",0x83d);
      }
      ieee80211_create_bss(local_3c,*(undefined4 *)(iVar14 + 0x4e1c));
      if (local_3c[0x112] == 2) {
        iVar13 = local_3c[0x41];
        iVar16 = (int)local_3c + 0x91a;
        iVar18 = *(int *)(*(int *)(iVar13 + 8) + 0x514);
        iVar7 = (*(code *)&memcmp)(iVar16,&DAT_ram_0008619c,6);
        if (iVar7 != 0) {
          iVar7 = ieee80211_alloc_node(iVar13 + 0x4e5c,local_3c,iVar16,"wds peer");
          if (iVar7 == 0) {
            uVar12 = *local_3c;
            uVar5 = ether_sprintf(iVar16);
            (*(code *)&printk)("<4>%s: couldn\'t create WDS node for %s\n",uVar12,uVar5);
          }
          else {
            iVar4 = ieee80211_aid_acquire(iVar13,iVar7);
            if ((iVar4 == 0) &&
               (iVar4 = ieee80211_add_wds_addr(iVar13 + 0x4e5c,iVar7,iVar16,1), iVar4 == 0)) {
              ieee80211_node_authorize(iVar7);
              ieee80211_node_set_chan(iVar13,iVar7);
              uVar2 = *(undefined2 *)(iVar18 + 0x3e);
              *(undefined2 *)(iVar7 + 0xa5c) = *(undefined2 *)(iVar18 + 0xa5c);
              *(undefined2 *)(iVar7 + 0x3e) = uVar2;
              uVar1 = *(undefined1 *)(local_3c + 0x111);
              *(undefined4 *)(iVar7 + 0xe94) = 3;
              *(undefined1 *)(iVar7 + 0x74) = 1;
              *(undefined1 *)(iVar7 + 0x32) = uVar1;
              *(ushort *)(iVar7 + 0x2e) = *(ushort *)(iVar7 + 0x2e) & 0x9ff5 | 0x200a;
              uVar21 = (*(code *)&get_jiffies_64)();
              *(undefined8 *)(iVar7 + 0xc6c) = uVar21;
              if (*(code **)(iVar13 + 0x5390) != (code *)0x0) {
                (**(code **)(iVar13 + 0x5390))(iVar7,1);
              }
              pcVar9 = (char *)(local_3c + 0x248);
              if (*pcVar9 != '\0') {
                (*(code *)&memcpy)(iVar7 + 0xd0,pcVar9,0xc0);
                (*(code *)local_3c[0x210])(local_3c);
                (*(code *)local_3c[0x20f])(local_3c,pcVar9,iVar16);
                (*(code *)local_3c[0x211])(local_3c);
              }
              uVar19 = Auxregs0028;
              if ((uVar19 & 6) != 0) {
                Auxregs0028 = uVar19 & 0xfffffff9;
              }
              ieee80211_sta_assocs_inc(local_3c,"ieee80211_create_wds_node");
              *(short *)(iVar13 + 0x506c) = *(short *)(iVar13 + 0x506c) + 1;
              Auxregs0028 = uVar19;
              if ((*(char *)(iVar13 + 0xbec) == '\x01') &&
                 (1 < (int)((uint)*(ushort *)(iVar13 + 0x506a) - (uint)*(ushort *)(iVar13 + 0x5072))
                 )) {
                *(undefined1 *)(iVar13 + 0xbee) = 1;
                ieee80211_beacon_update_all(iVar13);
              }
            }
            ieee80211_free_node(iVar7);
          }
        }
        if (*(char *)(iVar14 + 0x5c28) == '\x02') {
          pcVar20 = ieee80211_beacon_update_all;
          iVar7 = iVar14;
          goto LAB_ram_00018142;
        }
      }
      break;
    case 1:
    case 3:
      iVar7 = *(int *)(iVar15 + 0xb40);
      if ((int)(uint)*(byte *)(iVar15 + 0xa80) <= iVar7) {
        (*(code *)&printk)("%s: bogus xmit rate %u setup\n","__ieee80211_newstate",iVar7);
        (*(code *)&show_stacktrace)(0,0);
        (*(code *)&printk)("Kernel BUG in file %s: function %s: line %d!\n",
                           "../drivers/wlan/ieee80211_proto.c","__ieee80211_newstate",0x855);
      }
      if ((local_3c[0x42] & 0x40000000) != 0) {
        pcVar9 = "associated";
        if (local_3c[0x112] != 1) {
          pcVar9 = "synchronized ";
        }
        uVar5 = ether_sprintf(iVar15 + 0xa32);
        ieee80211_note(local_3c,"%s with %s ssid ",pcVar9,uVar5);
        ieee80211_print_essid(iVar15 + 0xa5f,*(undefined1 *)(iVar15 + 0xa5e));
        uVar5 = ieee80211_chan2ieee(iVar14,*(undefined4 *)(iVar14 + 0x4e08));
        (*(code *)&printk)(" channel %d start %uMb\n",uVar5,
                           (*(byte *)(iVar15 + *(int *)(iVar15 + 0xb40) + 0xa81) & 0x7f) >> 1);
      }
      iVar7 = local_3c[0x112];
      if (iVar7 == 1) {
        uVar12 = *local_3c;
        uVar5 = ether_sprintf(iVar15 + 0xa2c);
        (*(code *)&printk)("<4>%s: associated with AP %s\n",uVar12,uVar5);
        staAssociated = iVar7;
        ieee80211_scan_assoc_success(iVar14,iVar15 + 0xa2c);
        ieee80211_notify_node_join(iVar15,param_3 == 0x10 || param_3 == 0x30);
        if (((*(byte *)(local_3c + 0x234) & 2) != 0) &&
           ((*(byte *)((int)local_3c + 0x8cf) & 1) != 0)) {
          (*(code *)&printk)("<4>%s: 4-address mode is supported by the associated AP but is disabled\n"
                             ,*local_3c);
        }
        if ('\0' < *(char *)(iVar14 + 0x4f5)) {
          ieee80211_param_to_qdrv(local_3c,0xb0,1,0,0);
        }
        if ((((local_3c[0x10f] & 0x9000000) == 0) && (local_3c[0xa5] != 0)) && (local_3c[0xa8] == 0)
           ) {
          ieee80211_tdls_trigger_rate_detection(local_3c);
        }
        ieee80211_sta_assocs_inc(local_3c,"__ieee80211_newstate");
        ieee80211_nonqtn_sta_join(local_3c,iVar15,"__ieee80211_newstate");
        if (*(char *)(iVar14 + 0x5d84) != '\0') {
          iVar7 = (**(code **)(iVar14 + 0x548c))(iVar14,*(undefined4 *)(iVar15 + 0xaf0));
          if ((iVar7 == 1) && (*(code **)(iVar14 + 0x5484) != (code *)0x0)) {
            (**(code **)(iVar14 + 0x5484))(iVar14,*(undefined4 *)(iVar15 + 0xaf0),2);
          }
        }
        if (0 < *(short *)(iVar14 + 0x5c2)) {
          pcVar9 = "Enabled";
          if ((*(uint *)(iVar14 + 0x9c) & 0x80) == 0) {
            pcVar9 = "Disabled";
          }
          (*(code *)&printk)("SCS: send qtn DFS report (DFS %s)\n",pcVar9);
        }
        pcVar20 = ieee80211_send_action_dfs_report;
        iVar7 = iVar15;
LAB_ram_00018142:
        (*pcVar20)(iVar7);
      }
      break;
    case 2:
switchD_ram_00017b70_caseD_2:
      if (local_3c[0x42] != 0) {
        ieee80211_note(local_3c,"%s: invalid transition\n","__ieee80211_newstate");
      }
    }
    if (((uVar17 != 4) && (local_3c[0x112] == 1)) && ((local_3c[0x10f] & 0x400) != 0)) {
      piVar11 = local_3c + 0x232;
      if (*piVar11 == 0) {
        *piVar11 = 100;
      }
      *(undefined2 *)((int)local_3c + 0x8c6) = 10;
      local_3c[0x226] = ieee80211_swbmiss;
      local_3c[0x227] = local_3c;
      puVar10 = (ushort *)(local_3c + 0x229);
      uVar3 = (*(code *)&__divsi3)(*piVar11 * 0x400 * (uint)*(ushort *)(iVar15 + 0xa42),5000);
      *puVar10 = uVar3;
      *(undefined1 *)((int)local_3c + 0x8cd) = 0;
      uVar3 = (*(code *)&__divsi3)(uVar3,0xb);
      *puVar10 = uVar3;
      (*(code *)&mod_timer)(local_3c + 0x222,(uint)uVar3 + _jiffies);
    }
    if ((local_3c[0x112] == 1) && ((*(uint *)(iVar14 + 0xa0) & 0x20) != 0)) {
      *(undefined4 *)(iVar14 + 0x5e0c) = 0x11;
      ieee80211_pm_queue_work(iVar14);
    }
    if ((local_3c[0x112] == 6) && (*(code **)(local_3c[0x212] + 4) != (code *)0x0)) {
      (**(code **)(local_3c[0x212] + 4))(local_3c);
    }
    else if (*(code **)(local_3c[0x212] + 8) != (code *)0x0) {
      (**(code **)(local_3c[0x212] + 8))(local_3c);
    }
    if ((*(char *)(iVar15 + 0x2c) != '\x03') && ((local_3c[0x10e] & 0x1800000) == 0)) {
      ieee80211_node_authorize(iVar15);
    }
  default:
    goto switchD_ram_000172de_default;
  }
  switch(uVar17) {
  case 0:
  case 1:
    if (local_3c[0x42] != 0) {
      ieee80211_note(local_3c,"%s: invalid transition\n","__ieee80211_newstate");
    }
    goto switchD_ram_000172de_default;
  case 2:
  case 3:
    uVar5 = 0;
    pcVar20 = *(code **)(*(int *)(iVar15 + 4) + 0x5374);
    break;
  case 4:
    uVar12 = *local_3c;
    uVar5 = ether_sprintf(iVar15 + 0xa2c);
    (*(code *)&printk)("<4>%s: disassociated from AP %s\n",uVar12,uVar5);
    staAssociated = 0;
    (*(code *)&memset)(auStack_38,0,0x10);
    local_34 = (*(code *)&strlen)("DISASSOCIATED_FROM_AP");
    (*(code *)&wireless_send_event)(*local_3c,0x8c02,auStack_38,"DISASSOCIATED_FROM_AP");
    FUN_ram_00017118(local_3c,iVar15);
    if (*(int *)(iVar14 + 0x4e50) != 1) goto switchD_ram_000172de_default;
    uVar5 = 0x20;
    pcVar20 = *(code **)(*(int *)(iVar15 + 4) + 0x5374);
    if (param_3 == 0) {
      uVar5 = 0;
    }
    break;
  default:
    goto switchD_ram_000172de_default;
  }
  uVar12 = 0;
LAB_ram_00017a94:
  (*pcVar20)(iVar15,uVar5,uVar12);
switchD_ram_000172de_default:
  ieee80211_free_node(iVar15);
  return 0;
}

