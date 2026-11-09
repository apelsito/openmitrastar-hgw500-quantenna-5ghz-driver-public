
void ieee80211_start_chanset_scan(int param_1,uint param_2)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined *puVar4;
  
  iVar3 = *(int *)(param_1 + 0x104);
  iVar2 = *(int *)(iVar3 + 0x5de4);
  if (iVar2 - 2U < 3) {
    param_2 = param_2 | 0x100000;
    puVar4 = &DAT_ram_0009ce1c;
  }
  else {
    if (iVar2 == 5) {
      param_2 = param_2 | 0x200000;
      puVar4 = &DAT_ram_0009ce34;
      goto LAB_ram_0001368e;
    }
    puVar4 = &DAT_ram_0009aaa0;
  }
  if (iVar2 == 3) {
    uVar1 = 1;
  }
  else {
    uVar1 = 3;
    if (iVar2 != 4) goto LAB_ram_0001368e;
  }
  FUN_ram_00013418(iVar3,uVar1);
LAB_ram_0001368e:
  if (1 < *(int *)(iVar3 + 0x5e08)) {
    (*(code *)&printk)("%s: Start scan with bandwidth %s\n","ieee80211_start_chanset_scan",puVar4);
  }
  ieee80211_check_scan
            (param_1,param_2,0x7fffffff,*(undefined4 *)(param_1 + 0x48c),param_1 + 0x490,0);
  return;
}

