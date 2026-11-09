
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * FUN_ram_00012f9c(int *param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  piVar1 = (int *)0x0;
  if (param_1 != (int *)0x0) {
    iVar2 = 0;
    iVar3 = 1;
    if (-1 < param_1[1]) {
      iVar3 = param_1[1] + 1;
    }
    _DAT_auxregs_000c = 0x12ff6;
    _DAT_auxregs_0008 = 0x12fd0;
    do {
      iVar3 = iVar3 + -1;
      if (iVar3 == 0) {
        _DAT_auxregs_0008 = 0x12fd0;
        _DAT_auxregs_000c = 0x12ff6;
        return (int *)0x0;
      }
      piVar1 = (int *)(iVar2 * 0x50 + *param_1);
      iVar2 = iVar2 + 1;
    } while (((param_2 != *piVar1) || (param_3 != piVar1[2])) || (param_4 != piVar1[1]));
  }
  return piVar1;
}

