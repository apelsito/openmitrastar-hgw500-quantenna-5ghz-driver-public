
/* WARNING: Instruction at (ram,0xe5005ca4) overlaps instruction at (ram,0xe5005ca0)
    */
/* WARNING: Switch with 1 destination removed at 0xe5005c98 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_e5005c50(uint param_1,uint param_2,uint param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined1 *puVar3;
  undefined4 *puVar4;
  undefined1 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint uVar9;
  bool bVar10;
  bool bVar11;
  
  if (param_3 == 0) {
    return;
  }
  if (((param_1 | param_2) & 3) != 0) {
    puVar3 = (undefined1 *)(param_2 - 1);
    puVar5 = (undefined1 *)(param_1 - 1);
    _DAT_auxregs_0008 = 0xe5005cf4;
    _DAT_auxregs_000c = 0xe5005cfc;
    do {
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
      *puVar5 = *puVar3;
      param_3 = param_3 - 1;
    } while (param_3 != 0);
    return;
  }
  uVar9 = param_3 >> 4;
  puVar1 = (undefined4 *)(param_2 - 4);
  puVar4 = (undefined4 *)(param_1 - 4);
  bVar10 = false;
  if (uVar9 == 0) goto LAB_ram_e5005c98;
  _DAT_auxregs_0008 = 0xe5005c78;
  _DAT_auxregs_000c = 0xe5005c98;
  bVar10 = true;
  do {
    puVar2 = puVar1 + 1;
    uVar6 = puVar1[2];
    uVar7 = puVar1[3];
    puVar1 = puVar1 + 4;
    uVar8 = *puVar1;
    puVar4[1] = *puVar2;
    puVar4[2] = uVar6;
    puVar4[3] = uVar7;
    puVar4 = puVar4 + 4;
    *puVar4 = uVar8;
LAB_ram_e5005c98:
  } while ((bVar10) && (uVar9 = uVar9 - 1, uVar9 != 0));
  bVar10 = (param_3 & 4) == 0;
  if ((param_3 >> 3 & 1) == 0) {
    bVar11 = (param_3 & 3) == 0;
    if (bVar10) goto LAB_ram_e5005ca8;
  }
  else {
    puVar2 = puVar1 + 1;
    puVar1 = puVar1 + 2;
    uVar6 = *puVar1;
    puVar4[1] = *puVar2;
    puVar4 = puVar4 + 2;
    *puVar4 = uVar6;
    if (bVar10) {
      bVar11 = (param_3 & 3) == 0;
      goto LAB_ram_e5005ca8;
    }
    bVar11 = (param_3 & 3) == 0;
  }
  puVar1 = puVar1 + 1;
  puVar4 = puVar4 + 1;
  *puVar4 = *puVar1;
LAB_ram_e5005ca8:
  param_3 = param_3 & 3;
  puVar3 = (undefined1 *)((int)puVar1 + 3);
  if (!bVar11) {
    puVar5 = (undefined1 *)((int)puVar4 + 3);
    _DAT_auxregs_0008 = 0xe5005cb8;
    _DAT_auxregs_000c = 0xe5005cc0;
    do {
      puVar3 = puVar3 + 1;
      puVar5 = puVar5 + 1;
      *puVar5 = *puVar3;
      param_3 = param_3 - 1;
    } while (param_3 != 0);
    return;
  }
  return;
}

