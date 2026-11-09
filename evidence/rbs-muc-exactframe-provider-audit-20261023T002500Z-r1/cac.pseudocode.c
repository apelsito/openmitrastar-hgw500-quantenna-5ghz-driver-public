
undefined4 FUN_ram_00111330(int *param_1,undefined4 *param_2)

{
  ushort uVar1;
  ushort uVar2;
  int iVar3;
  undefined4 uVar4;
  undefined2 uVar5;
  ushort uVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  undefined2 uVar9;
  undefined2 uVar10;
  undefined2 uVar11;
  undefined1 uVar12;
  int iVar13;
  
  iVar13 = *(int *)(*param_1 + 0x1c);
  FUN_ram_000e6c98(0xd);
  if (param_2[1] == 0) {
    *(undefined1 *)(iVar13 + 0x40bc) = 0;
    if (1 < *(ushort *)(iVar13 + 0x40c0)) {
      FUN_ram_000ee260("DFS_s_radio: set cac pending\n");
      return 0;
    }
  }
  else {
    *(undefined4 *)(iVar13 + 0x40d4) = *param_2;
    uVar4 = param_2[1];
    uVar5 = *(undefined2 *)(param_2 + 5);
    *(undefined2 *)(iVar13 + 0x40c2) = uVar5;
    uVar6 = *(ushort *)((int)param_2 + 0x16);
    *(undefined4 *)(iVar13 + 0x40d0) = uVar4;
    if (0x17 < uVar6) {
      uVar6 = 0x17;
    }
    *(ushort *)(iVar13 + 0x40ce) = uVar6;
    uVar7 = *(undefined2 *)(param_2 + 6);
    *(undefined2 *)(iVar13 + 0x40c4) = uVar7;
    uVar8 = *(undefined2 *)((int)param_2 + 0x1a);
    *(undefined4 **)(iVar13 + 0x4168) = param_2;
    *(undefined2 *)(iVar13 + 0x40c6) = uVar8;
    uVar9 = *(undefined2 *)(param_2 + 7);
    *(undefined2 *)(iVar13 + 0x40c8) = uVar9;
    uVar11 = *(undefined2 *)(param_2 + 8);
    uVar12 = (undefined1)*(undefined2 *)((int)param_2 + 0x1e);
    *(undefined2 *)(iVar13 + 0x40ca) = uVar11;
    uVar10 = *(undefined2 *)((int)param_2 + 0x22);
    *(undefined1 *)(iVar13 + 0x40be) = uVar12;
    *(undefined2 *)(iVar13 + 0x40cc) = uVar10;
    iVar3 = param_2[3];
    *(undefined1 *)(iVar13 + 0x40bc) = 1;
    if (iVar3 == *(int *)(iVar13 + 0x40dc)) {
      uVar2 = *(ushort *)(iVar13 + 0x40c0);
    }
    else if (*(char *)(iVar13 + 0x40bd) == '\0') {
      uVar2 = *(ushort *)(param_2 + 4);
      *(int *)(iVar13 + 0x40e8) = iVar3;
      uVar1 = *(ushort *)((int)param_2 + 0x12);
      *(uint *)(iVar13 + 0x40ec) = (uint)uVar2;
      *(uint *)(iVar13 + 0x40f0) = (uint)uVar1;
      *(undefined1 *)(iVar13 + 0x40bd) = 1;
      if ((*(uint *)(iVar13 + 0x4060) & 4) == 0) {
        FUN_ram_001101c8(iVar13);
        if (*(ushort *)(iVar13 + 0x40c0) < 2) {
          return 0;
        }
        uVar4 = *(undefined4 *)(iVar13 + 0x40d0);
        uVar5 = *(undefined2 *)(iVar13 + 0x40c2);
        uVar6 = *(ushort *)(iVar13 + 0x40ce);
        uVar7 = *(undefined2 *)(iVar13 + 0x40c4);
        uVar8 = *(undefined2 *)(iVar13 + 0x40c6);
        uVar9 = *(undefined2 *)(iVar13 + 0x40c8);
        uVar12 = *(undefined1 *)(iVar13 + 0x40be);
        uVar11 = *(undefined2 *)(iVar13 + 0x40ca);
        uVar10 = *(undefined2 *)(iVar13 + 0x40cc);
        goto LAB_ram_0011148c;
      }
      uVar2 = *(ushort *)(iVar13 + 0x40c0);
    }
    else {
      uVar2 = *(ushort *)(iVar13 + 0x40c0);
    }
    if (1 < uVar2) {
LAB_ram_0011148c:
      FUN_ram_000ee260("DFS_s_radio: set cac running, qtnchan: %x, dwell: %u, secure: %u, thrshld(fat: %u, trfc: %u, fat_dec: %u), trfc_ctrl: %u, offset(txhalt: %u, offchan: %u)\n"
                       ,uVar4,uVar5,uVar6,uVar7,uVar8,uVar9,uVar12,uVar11,uVar10);
      return 0;
    }
  }
  return 0;
}

