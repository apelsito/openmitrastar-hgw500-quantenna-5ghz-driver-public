
undefined4
FUN_ram_0012d3c8(int param_1,int *param_2,uint *param_3,byte *param_4,uint param_5,int param_6,
                int param_7,byte *param_8)

{
  byte bVar1;
  char cVar2;
  undefined1 uVar3;
  byte bVar4;
  byte bVar5;
  bool bVar6;
  undefined4 uVar7;
  byte bVar8;
  byte bVar9;
  byte *pbVar10;
  uint uVar11;
  byte bVar12;
  bool bVar13;
  byte *pbVar14;
  int iVar15;
  uint uVar16;
  uint uVar17;
  int iVar18;
  int iVar19;
  undefined4 uVar20;
  undefined4 local_3c;
  
  iVar19 = *(int *)(param_1 + 0x3b20);
  bVar8 = *param_4;
  pbVar14 = param_4 + 4;
  uVar3 = *(undefined1 *)(param_1 + 0x474);
  bVar12 = bVar8 & 0xf0;
  uVar20 = 1;
  bVar6 = false;
  bVar4 = *(byte *)(param_2 + 0x116);
  local_3c = 0;
  uVar11 = *(uint *)(param_1 + 0x28);
  iVar18 = 0x18;
  if ((bVar8 & 0xc) == 0 && bVar12 == 0xc0) {
    if (((uint)pbVar14 & 1) != 0 || ((uint)(param_2 + 0x27) & 1) != 0) {
      uVar16 = FUN_ram_000eeb74(pbVar14,param_2 + 0x27,6);
      iVar15 = 1 - uVar16;
      if (1 < uVar16) {
        iVar15 = 0;
      }
      if (iVar15 != 0) goto LAB_ram_0012d4dc;
      goto LAB_ram_0012d4d4;
    }
    if ((((short)param_2[0x28] != *(short *)(param_4 + 8)) ||
        (*(short *)((int)param_2 + 0x9e) != *(short *)(param_4 + 6))) ||
       ((short)param_2[0x27] != *(short *)(param_4 + 4))) goto LAB_ram_0012d4d4;
LAB_ram_0012d4dc:
    if (*(char *)(param_1 + 0x1e) != '\x06') goto LAB_ram_0012d4d4;
    if (((uint)pbVar14 & 1) == 0) {
      param_4[4] = 0xff;
      param_4[5] = 0xff;
      param_4[6] = 0xff;
      param_4[7] = 0xff;
      param_4[8] = 0xff;
      param_4[9] = 0xff;
    }
    else {
      FUN_ram_0039fe00(pbVar14,&DAT_ram_001c42c4,6);
    }
    uVar16 = 0x200;
  }
  else {
LAB_ram_0012d4d4:
    uVar16 = 0;
  }
  FUN_ram_000eea80(param_3,0,0x6c);
  uVar17 = uVar16 | 0x81100;
  *param_3 = uVar17;
  *param_8 = 7;
  if ((bVar8 & 0xc) == 0) {
    iVar15 = 1;
    if (bVar12 == 0x50) {
      uVar7 = 0x10;
      iVar15 = 6;
LAB_ram_0012d56c:
      FUN_ram_00151ca0(param_1,uVar7);
      uVar17 = *param_3;
    }
    else {
      if ((bVar12 == 0x10) || (bVar12 == 0x30)) {
        uVar7 = 0x11;
        iVar15 = 1;
        goto LAB_ram_0012d56c;
      }
      if (bVar12 == 0xb0) {
        FUN_ram_00151ca0(param_1,0x12);
        uVar17 = *param_3;
      }
    }
    bVar8 = *param_4;
    bVar9 = bVar8 & 0xf0;
    if (((*(ushort *)(param_2 + 0x104) & 0x40) == 0) && ((*(ushort *)(param_2 + 0x104) & 0x80) == 0)
       ) {
switchD_ram_0012d5f8_caseD_4:
      bVar6 = false;
    }
    else {
      if (bVar9 != 0xa0 && bVar9 != 0xc0) {
        if (bVar9 != 0xd0) goto switchD_ram_0012d5f8_caseD_4;
        if (param_4[0x18] != 0x7e) {
          switch(param_4[0x18]) {
          case 0:
          case 1:
          case 2:
          case 3:
          case 5:
          case 6:
          case 8:
          case 9:
          case 10:
          case 0xd:
          case 0xe:
            break;
          default:
            goto switchD_ram_0012d5f8_caseD_4;
          }
        }
      }
      bVar6 = true;
    }
    bVar12 = *pbVar14;
  }
  else {
    if (((bVar8 & 0xc) == 8) && (((bVar8 & 0xf0) == 0 || ((bVar8 & 0x80) != 0)))) {
      if ((bVar8 & 0x80) == 0) {
        if (bVar12 == 0xc0) {
          *param_8 = 4;
          cVar2 = *(char *)((int)param_2 + 0x67e);
        }
        else {
          cVar2 = *(char *)((int)param_2 + 0x67e);
        }
        if (cVar2 != '\x06') {
          iVar18 = 0x18;
          goto LAB_ram_0012d6f0;
        }
        goto LAB_ram_0012d6a4;
      }
      bVar8 = param_4[0x18];
      *param_8 = bVar8 & 0xf;
      iVar18 = 0x1a;
      param_3[0xc] = (bVar8 & 3) << 0x11 | (bVar8 & 0xc) << 0x17 | param_3[0xc];
      if (bVar12 == 0xc0) {
        *param_8 = 4;
        if (*(char *)((int)param_2 + 0x67e) != '\x06') goto LAB_ram_0012d6f0;
        goto LAB_ram_0012d6a4;
      }
      if (*(char *)((int)param_2 + 0x67e) == '\x06') goto LAB_ram_0012d6a4;
LAB_ram_0012d6f0:
      if (bVar12 == 0xc0) goto LAB_ram_0012d6a4;
      uVar17 = uVar16 | 0xa3100;
      param_4[1] = param_4[1] | 0x40;
      bVar12 = *pbVar14;
      iVar15 = *param_2;
      uVar16 = param_3[6];
      *param_3 = uVar17;
      if ((bVar12 & 1) == 0) {
        param_3[6] = (*(byte *)(param_2 + 0x19f) & 0x7f) << 0xc | uVar16;
      }
      else if (*(char *)(param_1 + 0x1e) == '\x01') {
        param_3[6] = uVar16 | 0x3f000;
      }
      else {
        param_3[6] = (*(byte *)(iVar15 + 0x7c) & 3) << 0x14 |
                     uVar16 | (*(byte *)(param_2 + 0x19f) & 0x7f) << 0xc;
      }
      bVar8 = *param_4;
      iVar15 = 2;
      param_2[300] = param_2[300] + 1;
    }
    else {
LAB_ram_0012d6a4:
      bVar8 = *param_4;
      bVar12 = *pbVar14;
      iVar15 = 1;
    }
    bVar9 = bVar8 & 0xf0;
  }
  pbVar10 = param_4 + 0x18;
  *param_3 = iVar15 << 4 | uVar17 | iVar18 << 0x18;
  if (param_6 == 0) {
    *param_3 = iVar15 << 4 | uVar17 & 0xffffefff | iVar18 << 0x18;
  }
  bVar1 = param_4[5];
  param_3[2] = 0x1f0000;
  param_3[1] = param_5;
  bVar5 = param_4[9];
  param_3[9] = CONCAT13(param_4[7],CONCAT12(param_4[6],CONCAT11(bVar1,bVar12)));
  param_3[10] = *(int *)(iVar19 + 0x16c) << 0x10 | (uint)CONCAT11(bVar5,param_4[8]);
  if ((bVar8 & 0xc) == 0) {
    if (bVar9 != 0xe0) {
      if ((bVar9 == 0xd0) && (*pbVar10 == 0x15)) {
        bVar1 = param_4[0x19];
        goto joined_r0x0012d854;
      }
      goto LAB_ram_0012d858;
    }
    if (*pbVar10 != 0x15) goto LAB_ram_0012d858;
    bVar1 = param_4[0x19];
joined_r0x0012d854:
    if (bVar1 != 0) goto LAB_ram_0012d858;
LAB_ram_0012d884:
    bVar13 = true;
  }
  else {
LAB_ram_0012d858:
    if ((((bVar8 & 0xc) == 0 && bVar9 == 0xd0) && (*pbVar10 == 7)) &&
       (param_4[0x19] == 5 || param_4[0x19] == 6)) goto LAB_ram_0012d884;
    bVar13 = false;
  }
  if (bVar13) {
    uVar20 = 0;
    cVar2 = *(char *)((int)param_2 + 0x67e);
    param_3[6] = param_3[6] | (uint)*(byte *)((int)param_2 + 0x4ea) << 0x1d;
    if ((cVar2 != '\x03') || (!bVar6)) goto LAB_ram_0012d9b0;
  }
  else {
    bVar13 = param_7 != 0 || (bVar12 & 1) == 1;
    uVar11 = FUN_ram_000e433c(param_3,iVar19,bVar13,1,0,uVar3,(bVar4 & 8) != 0,bVar8,&local_3c,
                              *(undefined1 *)((int)param_2 + 0x67f),uVar11 >> 0x12 & 1);
    cVar2 = *(char *)((int)param_2 + 0x67e);
    param_3[0xb] = uVar11;
    if (((cVar2 != '\x03') || (!bVar6)) || (bVar13)) goto LAB_ram_0012d9b0;
    uVar20 = 1;
  }
  uVar16 = *param_3;
  iVar18 = *param_2;
  param_4[1] = param_4[1] | 0x40;
  bVar4 = *pbVar14;
  uVar11 = param_3[6];
  *param_3 = uVar16 | 0x22000;
  if ((bVar4 & 1) == 0) {
    param_3[6] = (*(byte *)(param_2 + 0x19f) & 0x7f) << 0xc | uVar11;
  }
  else if (*(char *)(param_1 + 0x1e) == '\x01') {
    param_3[6] = uVar11 | 0x3f000;
  }
  else {
    param_3[6] = (*(byte *)(iVar18 + 0x7c) & 3) << 0x14 |
                 (*(byte *)(param_2 + 0x19f) & 0x7f) << 0xc | uVar11;
  }
LAB_ram_0012d9b0:
  param_3[0xd] = (uint)*(ushort *)(iVar19 + 0x1c4) | *(int *)(iVar19 + 0x1fc) << 0x10;
  return uVar20;
}

