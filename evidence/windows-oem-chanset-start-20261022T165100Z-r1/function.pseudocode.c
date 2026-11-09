
/* WARNING: Type propagation algorithm not settling */

int ieee80211_start_chanset_selection(int param_1,undefined4 param_2)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  uint uVar7;
  undefined *puVar8;
  byte bVar9;
  uint uVar10;
  char *pcVar11;
  uint uVar12;
  int iVar13;
  undefined4 uVar14;
  char *pcVar15;
  uint *puVar16;
  bool bVar17;
  int local_3c;
  
  iVar13 = *(int *)(param_1 + 0x104);
  iVar1 = ieee80211_get_bw(iVar13);
  *(undefined4 *)(iVar13 + 0x5e04) = param_2;
  (*(code *)&memset)(iVar13 + 0x5de4,0,0x1c);
  pcVar2 = (char *)ieee80211_get_bw(iVar13);
  local_3c = 0;
  uVar12 = 0;
LAB_ram_0001378c:
  uVar7 = uVar12;
  if ((int)uVar12 < 0) {
    uVar7 = uVar12 + 7;
  }
  uVar10 = uVar12 & 0x80000007;
  bVar17 = (int)uVar10 < 0;
  if (bVar17) {
    uVar10 = uVar10 - 1;
  }
  bVar9 = (byte)uVar10;
  if (bVar17) {
    bVar9 = (bVar9 | 0xf8) + 1;
  }
  if (((int)(uint)*(byte *)(iVar13 + ((int)uVar7 >> 3) + 0x4b90) >> (bVar9 & 0x1f) & 1U) != 0) {
    puVar16 = &DAT_ram_0008f380;
    do {
      if (puVar16 == (uint *)&DAT_ram_0008f7e0) {
        puVar16 = &DAT_ram_0008fd80;
        goto LAB_ram_000137f6;
      }
      uVar7 = *puVar16;
      puVar16 = puVar16 + 0x14;
    } while (uVar7 != uVar12);
  }
  goto LAB_ram_00013814;
  while (uVar7 = *puVar16, puVar16 = puVar16 + 0x14, uVar7 != uVar12) {
LAB_ram_000137f6:
    if (puVar16 == (uint *)0x906e0) {
      if (-1 < *(int *)(iVar13 + 0x5e08)) {
        (*(code *)&printk)("%s: fail to find channel %d in chanset table, please update chanset table\n"
                           ,"ieee80211_check_chanset_table",uVar12);
      }
      break;
    }
  }
LAB_ram_00013814:
  uVar12 = uVar12 + 1;
  if (uVar12 == 0xff) goto code_r0x0001381e;
  goto LAB_ram_0001378c;
code_r0x0001381e:
  if (*(int *)(iVar13 + 0x5de0) != 0) {
    iVar3 = *(int *)(iVar13 + 0x5ddc);
    iVar4 = iVar3;
    if (*(int *)(iVar3 + -4) == -0x21524111) {
      iVar4 = iVar3 + -8;
      g_wlan_tot_free = g_wlan_tot_free + *(int *)(iVar3 + -8);
      g_wlan_tot_free_cnt = g_wlan_tot_free_cnt + 1;
      g_wlan_balance = g_wlan_balance - *(int *)(iVar3 + -8);
    }
    (*(code *)&kfree)(iVar4);
    *(undefined4 *)(iVar13 + 0x5de0) = 0;
    *(undefined4 *)(iVar13 + 0x5ddc) = 0;
  }
  if ((*(int *)(iVar13 + 0x14) - 2U < 2) || (*(int *)(iVar13 + 0x14) - 8U < 2)) {
    iVar4 = 0x20;
    uVar14 = 0;
    if ((int)pcVar2 < 0x28) {
      iVar4 = 0xe;
    }
  }
  else {
    if ((int)pcVar2 < 0x28) {
      iVar4 = 0x1e;
    }
    else {
      if (0x4f < (int)pcVar2) {
        iVar4 = 0x62;
        uVar14 = 1;
        if ((int)pcVar2 < 0xa0) {
          iVar4 = 0x52;
        }
        goto LAB_ram_000138f6;
      }
      iVar4 = 0x3a;
    }
    uVar14 = 1;
  }
LAB_ram_000138f6:
  iVar3 = iVar4 * 0x50;
  piVar5 = (int *)(*(code *)&__kmalloc)(iVar3 + 8,0xd0);
  if (piVar5 != (int *)0x0) {
    *piVar5 = iVar3;
    piVar5[1] = -0x21524111;
    g_wlan_tot_alloc = g_wlan_tot_alloc + iVar3;
    g_wlan_balance = g_wlan_balance + iVar3;
    g_wlan_tot_alloc_cnt = g_wlan_tot_alloc_cnt + 1;
    piVar5 = piVar5 + 2;
  }
  *(int *)(iVar13 + 0x5ddc) = (int)piVar5;
  if (piVar5 == (int *)0x0) {
    if (-1 < *(int *)(iVar13 + 0x5e08)) {
      (*(code *)&printk)("%s: fail to allocate channel table\n","ieee80211_init_chanset_table");
    }
LAB_ram_00013c60:
    if (-1 < *(int *)(iVar13 + 0x5e08)) {
      (*(code *)&printk)("%s: fail to initilize channel table\n","ieee80211_start_chanset_selection"
                        );
      return -1;
    }
    return -1;
  }
  iVar3 = 0;
  pcVar15 = "160";
  *(int *)(iVar13 + 0x5de0) = iVar4;
  for (; 0x13 < (int)pcVar2; pcVar2 = (char *)((int)pcVar2 >> 1)) {
    iVar6 = FUN_ram_00012f18(uVar14,pcVar2,&local_3c);
    if (iVar6 == 0) {
      if (-1 < *(int *)(iVar13 + 0x5e08)) {
        if ((*(int *)(iVar13 + 0x14) - 2U < 2) || (*(int *)(iVar13 + 0x14) - 8U < 2)) {
          puVar8 = &DAT_ram_00094218;
        }
        else {
          puVar8 = &UNK_ram_00094220;
        }
        pcVar15 = "%s: fail to find channel table for band %s and bandwidth %d\n";
LAB_ram_00013abc:
        (*(code *)&printk)(pcVar15,"ieee80211_init_chanset_table",puVar8,pcVar2);
      }
LAB_ram_00013be0:
      iVar4 = *(int *)(iVar13 + 0x5ddc);
      iVar1 = iVar4;
      if (*(int *)(iVar4 + -4) == -0x21524111) {
        iVar1 = iVar4 + -8;
        g_wlan_tot_free = g_wlan_tot_free + *(int *)(iVar4 + -8);
        g_wlan_tot_free_cnt = g_wlan_tot_free_cnt + 1;
        g_wlan_balance = g_wlan_balance - *(int *)(iVar4 + -8);
      }
      (*(code *)&kfree)(iVar1);
      *(undefined4 *)(iVar13 + 0x5de0) = 0;
      *(undefined4 *)(iVar13 + 0x5ddc) = 0;
      goto LAB_ram_00013c60;
    }
    if (iVar4 < iVar3 + local_3c) {
      if (1 < *(int *)(iVar13 + 0x5e08)) {
        if ((*(int *)(iVar13 + 0x14) - 2U < 2) || (*(int *)(iVar13 + 0x14) - 8U < 2)) {
          puVar8 = &DAT_ram_00094218;
        }
        else {
          puVar8 = &UNK_ram_00094220;
        }
        if (pcVar2 == (char *)0x28) {
          pcVar2 = "40";
        }
        else if ((int)pcVar2 < 0x29) {
          if (pcVar2 == (char *)0x14) {
            pcVar2 = "20";
          }
          else {
LAB_ram_00013aa0:
            pcVar2 = "invalid";
          }
        }
        else if (pcVar2 == (char *)0x50) {
          pcVar2 = "80";
        }
        else {
          if (pcVar2 != (char *)0xa0) goto LAB_ram_00013aa0;
          pcVar2 = "160";
        }
        pcVar15 = "%s: channel set table overflow for %s %s\n";
        goto LAB_ram_00013abc;
      }
      goto LAB_ram_00013be0;
    }
    (*(code *)&memcpy)(iVar3 * 0x50 + *(int *)(iVar13 + 0x5ddc),iVar6,local_3c * 0x50);
    if (1 < *(int *)(iVar13 + 0x5e08)) {
      puVar8 = &DAT_ram_00094218;
      if (1 < *(int *)(iVar13 + 0x14) - 2U) {
        if (*(int *)(iVar13 + 0x14) - 8U < 2) {
          puVar8 = &DAT_ram_00094218;
        }
        else {
          puVar8 = &UNK_ram_00094220;
        }
      }
      if (pcVar2 == (char *)0x28) {
        pcVar11 = "40";
      }
      else if ((int)pcVar2 < 0x29) {
        if (pcVar2 == (char *)0x14) {
          pcVar11 = "20";
        }
        else {
LAB_ram_00013b42:
          pcVar11 = "invalid";
        }
      }
      else if (pcVar2 == (char *)0x50) {
        pcVar11 = "80";
      }
      else {
        pcVar11 = pcVar15;
        if (pcVar2 != (char *)0xa0) goto LAB_ram_00013b42;
      }
      (*(code *)&printk)("%s: initialize chanset table for band %s bandwith %s\n",
                         "ieee80211_init_chanset_table",puVar8,pcVar11);
    }
    iVar3 = iVar3 + local_3c;
  }
  for (iVar4 = 0; iVar4 < *(int *)(iVar13 + 0x5de0); iVar4 = iVar4 + 1) {
    iVar3 = iVar4 * 0x50 + *(int *)(iVar13 + 0x5ddc);
    *(undefined4 *)(iVar3 + 0x10) = 0;
    *(undefined4 *)(iVar3 + 0x14) = 0;
    (*(code *)&memset)(iVar3 + 0x18,0,0x10);
    (*(code *)&memset)(iVar3 + 0x28,0,8);
    *(undefined4 *)(iVar3 + 0x4c) = 0;
    *(undefined4 *)(iVar3 + 0x30) = 0;
    *(undefined4 *)(iVar3 + 0x34) = 0;
    *(undefined4 *)(iVar3 + 0x38) = 0;
    *(undefined4 *)(iVar3 + 0x3c) = 0;
    *(undefined4 *)(iVar3 + 0x40) = 0;
    *(undefined4 *)(iVar3 + 0x44) = 0;
    *(undefined4 *)(iVar3 + 0x48) = 0;
  }
  iVar4 = iVar1;
  if ((*(int *)(iVar13 + 0x5db8) == 0) && (iVar4 = 0x14, *(int *)(iVar13 + 0x5dbc) != 0)) {
    iVar4 = iVar1;
  }
  iVar1 = FUN_ram_000131a8(iVar13,iVar4);
  if (iVar1 < 0) {
    iVar13 = *(int *)(iVar13 + 0x5e08);
  }
  else {
    if ((*(char *)(iVar13 + 0x4df0) == '\0') || (iVar4 < 0x15)) {
LAB_ram_00013d96:
      iVar1 = ieee80211_start_chanset_scan(param_1,param_2);
      return iVar1;
    }
    iVar4 = iVar4 / 2;
    iVar1 = FUN_ram_000131a8(iVar13,iVar4);
    if (-1 < iVar1) goto LAB_ram_00013d96;
    iVar13 = *(int *)(iVar13 + 0x5e08);
  }
  if (iVar13 < 0) {
    return iVar1;
  }
  if (iVar4 == 0x28) {
    pcVar2 = "40";
    goto LAB_ram_00013d78;
  }
  if (iVar4 < 0x29) {
    if (iVar4 == 0x14) {
      pcVar2 = "20";
      goto LAB_ram_00013d78;
    }
  }
  else {
    if (iVar4 == 0x50) {
      pcVar2 = "80";
      goto LAB_ram_00013d78;
    }
    if (iVar4 == 0xa0) {
      pcVar2 = "160";
      goto LAB_ram_00013d78;
    }
  }
  pcVar2 = "invalid";
LAB_ram_00013d78:
  (*(code *)&printk)("%s: fail to add scan bandwidth %s\n","ieee80211_start_chanset_selection",
                     pcVar2);
  return iVar1;
}

