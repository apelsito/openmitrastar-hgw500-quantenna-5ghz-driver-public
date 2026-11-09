
int FUN_ram_00013598(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_2[2] / 2;
  if (((*(int *)(param_1 + 0x14) - 2U < 2) || (*(int *)(param_1 + 0x14) - 8U < 2)) &&
     (iVar1 = ieee80211_get_type_of_neighborhood(), iVar1 == 2)) {
    iVar2 = *(int *)(param_1 + 0x5dd8);
  }
  if (*param_2 < 0x24) {
    iVar1 = 0x967;
  }
  else {
    iVar1 = 4000;
    if (*param_2 < 0xb7) {
      iVar1 = 5000;
    }
  }
  return iVar2 + param_2[3] * 5 + iVar1;
}

