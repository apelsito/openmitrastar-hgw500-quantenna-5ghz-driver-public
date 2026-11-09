
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_80023900(void)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  do {
    DAT_ram_8002c1f0 = 0;
    uVar2 = (uint)DAT_ram_8002c1ec;
    if (uVar2 != 0) {
      iVar4 = 0;
      do {
        uVar1 = (uint)(byte)(&DAT_ram_8002c1cc)[iVar4];
        iVar3 = uVar1 * 0x1c;
        if ((*(uint *)(&DAT_ram_8002be5c + iVar3) & 4) != 0) {
          uVar2 = Auxregs0028;
          if ((uVar2 & 6) != 0) {
            Auxregs0028 = uVar2 & 0xfffffff9;
          }
          _DAT_ram_001cb28c = uVar1;
          *(uint *)(&DAT_ram_8002be5c + iVar3) = *(uint *)(&DAT_ram_8002be5c + iVar3) & 0xfffffffb;
          Auxregs0028 = uVar2;
          (*(code *)(&DAT_ram_8002be4c)[uVar1 * 7])
                    (*(undefined4 *)(&DAT_ram_8002be50 + iVar3),
                     *(undefined4 *)(&DAT_ram_8002be54 + iVar3));
          uVar2 = (uint)DAT_ram_8002c1ec;
        }
        iVar4 = iVar4 + 1;
      } while (iVar4 < (int)uVar2);
    }
    FUN_ram_0018bae8();
  } while( true );
}

