
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_ram_00155ec8(int param_1,byte *param_2,int param_3,uint param_4,undefined4 param_5,
                undefined4 param_6,uint param_7,int param_8,int param_9)

{
  bool bVar1;
  char cVar2;
  undefined1 uVar3;
  undefined4 uVar4;
  uint uVar5;
  byte bVar6;
  int *piVar7;
  ushort uVar8;
  ushort uVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  int *piVar13;
  int iVar14;
  undefined4 uVar15;
  uint local_44;
  undefined4 local_40;
  byte abStack_3c [4];
  byte local_4;
  
  local_4 = (byte)param_4;
  if ((param_2 < (byte *)0x6ec400) || ((byte *)0x26ec3ff < param_2)) {
    if ((param_2 < (byte *)0x26fc400) || ((byte *)0x48fc3ff < param_2)) {
      uVar11 = 0xffffffff;
    }
    else {
      uVar11 = 1;
    }
  }
  else {
    uVar11 = 0;
  }
  uVar10 = (uint)(param_2 + param_3 + 3) & 0xfffffffc;
  uVar9 = 0;
  bVar6 = *param_2 & 0xf0;
  if ((0x7f < param_4) || (piVar7 = *(int **)(param_1 + 0x3b54 + param_4 * 4), piVar7 == (int *)0x0)
     ) {
    if ((param_7 == 0) && ((-1 < (int)uVar11 && ((int)uVar11 < 4)))) {
      uVar11 = uVar11 & 0xff;
      uVar10 = 0;
      if (uVar11 == 1) {
        uVar10 = (uint)(param_2 + -0x26fc400) % 0x4400;
      }
      else if (uVar11 == 0) {
        uVar10 = (uint)(param_2 + -0x6ec400) & 0xfff;
      }
      uVar5 = Auxregs0028;
      if ((uVar5 & 2) != 0) {
        Auxregs0028 = uVar5 & 0xfffffffd;
      }
      if ((_DAT_ram_e1020114 & 0x80000000) == 0) {
        iVar12 = 0;
        bVar1 = false;
        do {
          iVar14 = 0;
          do {
            sync(0);
            iVar14 = iVar14 + 1;
          } while (iVar14 < 10);
          if ((!bVar1) && (iVar12 = iVar12 + 1, iVar12 == 0x3e9)) {
            FUN_ram_000ee260("MuC: %s\n","__topaz_hbm_wait timeout");
            bVar1 = true;
          }
        } while ((_DAT_ram_e1020114 & 0x80000000) == 0);
      }
      sync(0);
      _DAT_ram_e1020114 = uVar11 << 2 | 1;
      sync(0);
      Auxregs0028 = uVar5;
      _DAT_ram_e1020144 = param_2 + (0x40 - uVar10);
    }
    return 0xffffffff;
  }
  if (((*param_2 & 0xc) == 0) &&
     (((bVar6 == 0x50 ||
       ((bVar6 == 0xc0 || bVar6 == 0xa0 &&
        ((*(ushort *)(piVar7 + 0x104) & 0x40) == 0 && (*(ushort *)(piVar7 + 0x104) & 0x80) == 0))))
      && (piVar13 = *(int **)(*piVar7 + 0x38), piVar13 != (int *)0x0)))) {
    local_4 = *(byte *)(piVar13 + 0x19f);
    piVar7 = piVar13;
  }
  cVar2 = FUN_ram_0012d3c8(param_1,piVar7,uVar10,param_2,param_3,param_5,param_6,abStack_3c);
  uVar15 = 1;
  if (param_9 != 0) {
    uVar3 = FUN_ram_0013d894(piVar7,1);
    uVar4 = FUN_ram_000e44c0(uVar10,*(undefined4 *)(param_1 + 0x3b20),
                             *(undefined1 *)((int)piVar7 + 0x683),*(undefined1 *)(param_1 + 0x474),
                             uVar15,uVar3,(char)piVar7[0x14b],(int)piVar7 + 0x665,
                             (int)piVar7 + 0x666,(char)piVar7[0x14c],
                             *(undefined1 *)((int)piVar7 + 0x52d),*(ushort *)(piVar7 + 3) >> 0xe & 1
                            );
    bVar6 = *(byte *)(piVar7 + 0x1a2);
    *(undefined4 *)(uVar10 + 0x2c) = uVar4;
    *(uint *)(uVar10 + 0x18) = *(uint *)(uVar10 + 0x18) & 0x1fffffff | 0xe0000000;
    *(uint *)(uVar10 + 0x30) = *(uint *)(uVar10 + 0x30) & 0xfffff8ff | (bVar6 & 7) << 8;
  }
  FUN_ram_000ed0a0(param_2,param_3);
  FUN_ram_000ed0a0(uVar10,0x6c);
  if (cVar2 == '\0') {
    if (param_8 == 0) {
      if (param_9 == 0) goto LAB_ram_00156130;
      uVar9 = 0;
      goto LAB_ram_00156128;
    }
    uVar9 = 0;
  }
  else {
    uVar9 = 1;
    if (param_8 == 0) {
      if (param_9 == 0) goto LAB_ram_00156130;
LAB_ram_00156128:
      uVar9 = uVar9 | 8;
      goto LAB_ram_00156130;
    }
  }
  uVar9 = uVar9 | 2;
LAB_ram_00156130:
  if (((((*param_2 & 0xc) == 0 && (*param_2 & 0xf0) == 0xd0) && (param_2[0x18] == 0)) &&
      (param_2[0x19] == 4)) && ((param_2[4] & 1) != 0)) {
    uVar9 = uVar9 | 0x20;
  }
  uVar10 = 1 - param_7;
  uVar8 = 0;
  if (1 < param_7) {
    uVar10 = 0;
  }
  if (&param_9 == (undefined4 *)0x4) {
    uVar5 = 0;
  }
  else {
    uVar5 = (uint)local_4;
  }
  if ((uVar11 & 0xff) == 1) {
    uVar8 = (ushort)((uint)(param_2 + -0x26fc400) % 0x4400);
  }
  else if ((uVar11 & 0xff) == 0) {
    uVar8 = (short)param_2 + 0x3c00U & 0xfff;
  }
  local_44 = CONCAT22((ushort)(((uVar10 & 1) << 0x1b) >> 0x10) |
                      (ushort)(((uVar11 & 3) << 0x1c) >> 0x10),0x40 - uVar8) | 0x40000000;
  local_40 = CONCAT22(uVar9,(short)param_3);
  FUN_ram_80025dec("qtn_auc_tqe_mgmt_xmit",0x314);
  sync(0);
  if ((DAT_ram_0018c02c != 0) && ((_DAT_ram_e0000170 & 0xf) == 2)) {
    _DAT_ram_e0000170 = 0x20000000;
  }
  _DAT_ram_e1035038 = uVar15;
  _DAT_ram_e1035034 = local_40;
  _DAT_ram_e1035030 = local_44;
  _DAT_ram_e103502c = 0;
  _DAT_ram_e1035028 = (uVar5 & 0x7f) << 4 | abStack_3c[0] & 0xf | 0x1000;
  _DAT_ram_e1035024 = param_2;
  _DAT_ram_e1035020 = 0;
  return 0;
}

