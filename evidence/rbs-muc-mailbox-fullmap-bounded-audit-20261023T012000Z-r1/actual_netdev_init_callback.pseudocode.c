
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_ram_0010862c(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  undefined2 local_28 [2];
  undefined4 local_24;
  
  iVar2 = *(int *)(param_1 + 0x1c);
  FUN_ram_000f2ddc();
  if (DAT_ram_8002c8bc == '\x03') {
    iVar3 = *(int *)(param_1 + 0x1c);
    uVar4 = *(undefined4 *)(iVar3 + 0x3b20);
    if ((*(ushort *)(param_1 + 0xe) & 4) != 0) {
      FUN_ram_000e82dc(param_1,4);
      FUN_ram_000efbbc(param_1);
      FUN_ram_000f8be8(uVar4,0);
      FUN_ram_0012a4d0(iVar3);
      FUN_ram_000f990c(uVar4);
    }
  }
  iVar3 = FUN_ram_00108288(iVar2,*(undefined1 *)(iVar2 + 0x1e));
  if (iVar3 == 0) {
    _DAT_ram_0046e000 = 0;
    FUN_ram_0015cc94(iVar2,0);
    if (DAT_ram_8002c8bc == '\x03') {
      iVar3 = FUN_ram_001135e4(iVar2);
      if (iVar3 == 0) goto LAB_ram_00108864;
      goto joined_r0x00108940;
    }
    iVar3 = FUN_ram_00108354();
    if (iVar3 != 0) {
      FUN_ram_000ee260("Unable to allocate memory for the Rx buffers\n");
      FUN_ram_80030314();
    }
    uVar4 = *(undefined4 *)(iVar2 + 0x3b20);
    local_28[0] = **(undefined2 **)(iVar2 + 0x387c);
    FUN_ram_000f2ccc("channel set %x\n");
    iVar1 = FUN_ram_000f94b4(uVar4,local_28);
    if (iVar1 == 0) {
      *(undefined2 *)(iVar2 + 0x3b2c) = local_28[0];
      *(undefined4 *)(iVar2 + 0x3b30) = local_24;
    }
    FUN_ram_000fd2e4(*(undefined4 *)(iVar2 + 0x3b20));
    uVar4 = DAT_ram_0018c33c;
    iVar1 = *(int *)(iVar2 + 0x3b20);
    *(undefined4 *)(*(int *)(iVar1 + 0xc) + 0x2008) = DAT_ram_0018c338;
    *(undefined4 *)(*(int *)(iVar1 + 0xc) + 0x200c) = uVar4;
    *(undefined4 *)(*(int *)(iVar1 + 0xc) + 0x2004) = 9;
    uVar4 = DAT_ram_0018c344;
    *(undefined4 *)(*(int *)(iVar1 + 0xc) + 0x2008) = DAT_ram_0018c340;
    *(undefined4 *)(*(int *)(iVar1 + 0xc) + 0x200c) = uVar4;
    *(undefined4 *)(*(int *)(iVar1 + 0xc) + 0x2004) = 9;
    DAT_ram_0018c348 = 0;
    if (iVar3 == 0) {
LAB_ram_00108864:
      DAT_ram_8002c960 = iVar2;
      FUN_ram_00108464(iVar2);
      if (DAT_ram_8002c8bc == '\x03') {
        *(undefined4 *)(iVar2 + 0x3b3c) = 0xf0031f93;
        uVar4 = 0xf0031f93;
        if (DAT_ram_8002c8bc == '\x03') {
          uVar4 = 0xf00f1f93;
LAB_ram_001088b8:
          *(undefined4 *)(iVar2 + 0x3b3c) = uVar4;
        }
      }
      else {
        *(undefined4 *)(iVar2 + 0x3b3c) = 0xf0041f11;
        uVar4 = 0xf0041f11;
        if (DAT_ram_8002c8bc == '\x03') {
          uVar4 = 0xf00f1f11;
          goto LAB_ram_001088b8;
        }
      }
      FUN_ram_000f8be8(*(undefined4 *)(iVar2 + 0x3b20),uVar4);
      FUN_ram_000f94c4(*(undefined4 *)(iVar2 + 0x3b20));
      if (DAT_ram_8002c8bc == '\x03') {
        if (DAT_ram_0018c100 == 0) {
          DAT_ram_0018c100 = 3;
        }
        FUN_ram_000f1d2c(4);
        DAT_ram_800261d0 = 0x10;
        DAT_ram_800261d4 = 0x2c;
        DAT_ram_800261d8 = 1000;
        DAT_ram_800261dc = 10;
      }
      else {
        FUN_ram_000f94ec(*(undefined4 *)(iVar2 + 0x3b20),4);
      }
      FUN_ram_000e82a4(param_1,0x26);
      goto joined_r0x00108940;
    }
    if (DAT_ram_8002c8bc != '\x03') goto LAB_ram_001089a4;
    if (DAT_ram_0018c010 != 3) goto LAB_ram_00108968;
LAB_ram_001086f8:
    (*_DAT_ram_002017d8)(1,0x66);
  }
  else {
    FUN_ram_000ee260("%s: Unable to reset hw - error %d\n",param_1 + 4,iVar3);
joined_r0x00108940:
    if (DAT_ram_8002c8bc != '\x03') goto LAB_ram_001089a4;
    if (DAT_ram_0018c010 == 3) goto LAB_ram_001086f8;
LAB_ram_00108968:
    FUN_ram_0016ed08(1,0x66);
  }
  FUN_ram_0011be2c(iVar2,DAT_ram_8002c950);
  if (DAT_ram_0018c0ec != 0) {
    FUN_ram_0018a920();
    FUN_ram_001055d8(*(undefined4 *)(iVar2 + 0x3b20));
  }
LAB_ram_001089a4:
  FUN_ram_000f7c10(1000,iVar2,0,&LAB_ram_00112588,3);
  FUN_ram_0013db5c();
  FUN_ram_0014bf74(iVar2);
  FUN_ram_00163ea4();
  if (DAT_ram_8002c8bc == '\x03') {
    FUN_ram_00163f48(iVar2,DAT_ram_8002c948);
  }
  FUN_ram_000f2ccc("init complete\n");
  return iVar3;
}

