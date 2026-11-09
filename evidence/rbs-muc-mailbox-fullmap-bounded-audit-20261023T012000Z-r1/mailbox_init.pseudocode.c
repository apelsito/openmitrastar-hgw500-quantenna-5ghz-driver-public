
undefined4 FUN_ram_000f0bdc(int param_1)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  
  if ((*(ushort *)(param_1 + 0xe) & 0x40) != 0) {
    return 0;
  }
  iVar2 = FUN_ram_0018bb84();
  if (2 < iVar2) {
    iVar2 = FUN_ram_000ee73c(0x208,1);
    *(int *)(param_1 + 0x2c) = iVar2;
    if (iVar2 != 0) {
      iVar2 = FUN_ram_000ee734(0x1040,0);
      *(int *)(param_1 + 0x44) = iVar2;
      if (iVar2 != 0) {
        iVar2 = *(int *)(param_1 + 0x2c);
        iVar3 = 0;
        do {
          *(undefined4 *)(iVar2 + iVar3 * 4) = 0;
          iVar3 = iVar3 + 1;
        } while (iVar3 < 0x82);
        iVar2 = 0;
        do {
          uVar1 = FUN_ram_000f1c6c();
          FUN_ram_000f0ba4(param_1 + 0x2c,iVar2,uVar1,6);
          iVar2 = iVar2 + 1;
        } while (iVar2 < 1);
        iVar2 = 0;
        uVar1 = FUN_ram_000f1c6c();
        FUN_ram_000f0ba4(param_1 + 0x2c,1,uVar1,1);
        uVar1 = FUN_ram_000f1c6c();
        iVar3 = 0x80;
        do {
          FUN_ram_000f0ba4(param_1 + 0x2c,iVar2 + 2,uVar1,1);
          iVar3 = iVar3 + -1;
          iVar2 = iVar2 + 1;
        } while (iVar3 != 0);
        return 0x82;
      }
      FUN_ram_000ee834(*(undefined4 *)(param_1 + 0x2c));
      *(undefined4 *)(param_1 + 0x2c) = 0;
    }
  }
  return 0xfffffff4;
}

