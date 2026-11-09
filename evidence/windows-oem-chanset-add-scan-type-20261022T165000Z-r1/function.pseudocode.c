
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_ram_000131a8(int param_1,int param_2)

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  undefined4 uVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  
  iVar7 = 7;
  piVar3 = (int *)(param_1 + 0x5de4);
  iVar5 = 0;
  _DAT_auxregs_0008 = 0x131c0;
  _DAT_auxregs_000c = 0x131d0;
  do {
    iVar1 = *piVar3;
    piVar3 = piVar3 + 1;
    if (iVar1 == 0) goto LAB_ram_000131d2;
    iVar5 = iVar5 + 1;
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  iVar5 = 0;
LAB_ram_000131d2:
  *(int *)(param_1 + 0x5e00) = param_2;
  if (param_2 == 0x28) {
    if ((*(int *)(param_1 + 0x14) - 2U < 2) || (*(int *)(param_1 + 0x14) - 8U < 2)) {
      if (5 < iVar5) {
        if (-1 < *(int *)(param_1 + 0x5e08)) {
          (*(code *)&printk)("%s: incorrect scan type index %d\n","ieee80211_add_chanset_scan_type",
                             iVar5);
        }
        return 0xffffffff;
      }
      *(undefined4 *)(param_1 + iVar5 * 4 + 0x5de4) = 3;
      pcVar6 = "BW40_ABOVE BW40_BELOW";
      *(undefined4 *)(param_1 + (iVar5 + 1) * 4 + 0x5de4) = 4;
      goto LAB_ram_000132e0;
    }
    pcVar6 = "BW40";
    puVar2 = (undefined4 *)(param_1 + iVar5 * 4 + 0x5de4);
    uVar4 = 2;
  }
  else if (param_2 < 0x29) {
    if (param_2 != 0x14) {
LAB_ram_000131fc:
      pcVar6 = "INVALID";
      goto LAB_ram_000132e0;
    }
    pcVar6 = "BW20";
    puVar2 = (undefined4 *)(param_1 + iVar5 * 4 + 0x5de4);
    uVar4 = 1;
  }
  else {
    if (param_2 != 0x50) {
      if (param_2 == 0xa0) {
        *(undefined4 *)(param_1 + iVar5 * 4 + 0x5de4) = 6;
        pcVar6 = "BW160";
        goto LAB_ram_000132e0;
      }
      goto LAB_ram_000131fc;
    }
    pcVar6 = "BW80";
    puVar2 = (undefined4 *)(param_1 + iVar5 * 4 + 0x5de4);
    uVar4 = 5;
  }
  *puVar2 = uVar4;
LAB_ram_000132e0:
  if (1 < *(int *)(param_1 + 0x5e08)) {
    (*(code *)&printk)("%s: add chanset scan type %s\n","ieee80211_add_chanset_scan_type",pcVar6);
  }
  return 0;
}

