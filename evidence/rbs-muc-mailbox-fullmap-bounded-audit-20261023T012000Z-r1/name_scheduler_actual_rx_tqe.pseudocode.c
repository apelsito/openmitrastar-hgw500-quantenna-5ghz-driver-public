
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_ram_800239d8(int param_1,int param_2)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  undefined4 *puVar4;
  
  puVar4 = (undefined4 *)(&DAT_ram_8002c98c)[param_1 * 3];
  if (puVar4 == (undefined4 *)0x0) {
LAB_ram_80023a0c:
    uVar1 = 0;
  }
  else {
    if (param_2 == 0) {
      if (param_1 == 0) {
        _DAT_ram_ed000328 = 1;
        uVar2 = _DAT_ram_ed000330;
      }
      else {
        _DAT_ram_e8000328 = 1;
        uVar2 = _DAT_ram_e8000330;
      }
      uVar3 = uVar2 & 0x1ff;
      uVar2 = uVar2 >> 0xd & 0x1ff;
      if (uVar2 < uVar3) {
        uVar2 = (uVar3 - uVar2) - 1;
      }
      else {
        uVar2 = 0x200 - (uVar2 - uVar3);
      }
      if (uVar2 < 8) goto LAB_ram_80023a0c;
      puVar4 = (undefined4 *)(&DAT_ram_8002c98c)[param_1 * 3];
    }
    (&DAT_ram_8002c98c)[param_1 * 3] = *puVar4;
    uVar2 = _DAT_ram_e1035038 & 1;
    *puVar4 = 0;
    if (uVar2 != 0) {
      uVar2 = 0;
      do {
        uVar2 = uVar2 + 1;
        if (((uVar2 & 0xffff0000) != 0) && ((uVar2 & 0xffff) == 0)) {
          FUN_ram_000ee260("stuck in topaz_tqe_wait()\n");
        }
      } while ((_DAT_ram_e1035038 & 1) != 0);
    }
    FUN_ram_80025dec("qtn_soft_ring_push_to_tqe",0x158);
    _DAT_ram_e1035020 = *puVar4;
    _DAT_ram_e1035024 = puVar4[1];
    _DAT_ram_e1035028 = puVar4[2];
    _DAT_ram_e103502c = puVar4[3];
    _DAT_ram_e1035030 = puVar4[4];
    _DAT_ram_e1035034 = puVar4[5];
    sync(0);
    _DAT_ram_e1035038 = 1;
    FUN_ram_80025e74(_DAT_ram_e1035034);
    uVar1 = 1;
    *puVar4 = DAT_ram_8002c988;
    DAT_ram_8003728c = DAT_ram_8003728c + 1;
    DAT_ram_8002c988 = puVar4;
  }
  return uVar1;
}

