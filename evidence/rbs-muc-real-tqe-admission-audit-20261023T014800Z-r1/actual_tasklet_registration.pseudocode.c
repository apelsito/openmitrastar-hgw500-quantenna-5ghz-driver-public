
undefined4
FUN_ram_000e83ec(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
                undefined4 param_6)

{
  uint uVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x1c;
  if (param_2 == 0) {
    FUN_ram_000eea80(&DAT_ram_8002be4c + param_1 * 7,0,0x14);
  }
  else {
    (&DAT_ram_8002be4c)[param_1 * 7] = param_2;
    *(undefined4 *)(&DAT_ram_8002be50 + iVar2) = param_3;
    *(undefined4 *)(&DAT_ram_8002be54 + iVar2) = param_4;
    *(undefined4 *)(&DAT_ram_8002be5c + iVar2) = 3;
    *(int *)(&DAT_ram_8002be58 + iVar2) = param_1;
    (&DAT_ram_8002c1cc)[DAT_ram_8002c1ec] = (byte)param_1;
    DAT_ram_8002c1ec = DAT_ram_8002c1ec + 1;
  }
  *(undefined4 *)(&DAT_ram_8002be60 + iVar2) = param_5;
  *(undefined4 *)(&DAT_ram_8002be64 + iVar2) = param_6;
  uVar1 = Auxregs1030;
  Auxregs1030 = uVar1 | 1 << ((byte)param_1 & 0x1f);
  return 0;
}

