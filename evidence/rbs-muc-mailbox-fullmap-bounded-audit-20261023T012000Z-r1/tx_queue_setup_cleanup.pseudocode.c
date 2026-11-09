
void FUN_ram_0012a34c(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined1 *puVar4;
  int iVar5;
  uint uVar6;
  
  if (*(int *)(param_1 + 0x3f7c) != 0) {
    uVar6 = 0;
    do {
      uVar3 = 0;
      iVar5 = uVar6 * 0x38;
      puVar4 = (undefined1 *)(param_1 + 0x3dbc + iVar5);
      do {
        iVar2 = uVar3 + 0x1e;
        uVar3 = uVar3 + 1;
        puVar4[iVar2] = 0;
      } while (uVar3 < 2);
      puVar4[0x1d] = 0;
      *(undefined4 *)(puVar4 + 0x20) = 0;
      uVar1 = *(undefined4 *)(param_1 + 0x3b20);
      *(undefined4 *)(puVar4 + 0x30) = *(undefined4 *)(puVar4 + 0x28);
      FUN_ram_0010252c(uVar1,*puVar4);
      iVar2 = param_1 + 0x3d84 + iVar5;
      *(undefined1 *)(iVar2 + 0x38) = 0;
      *(undefined1 *)(iVar2 + 0x54) = 0;
      *(undefined1 *)(iVar2 + 0x5c) = 0;
      iVar2 = param_1 + 0x3fe0 + iVar5;
      *(undefined4 *)(iVar2 + -0x204) = 0;
      *(undefined4 *)(iVar2 + -0x1fc) = 0;
      puVar4 = (undefined1 *)(uVar6 * 0x38 + param_1 + 0x3e9c);
      puVar4[0x1e] = 0;
      puVar4[0x1f] = 0;
      puVar4[0x1d] = 0;
      *(undefined4 *)(puVar4 + 0x20) = 0;
      uVar1 = *(undefined4 *)(param_1 + 0x3b20);
      *(undefined4 *)(puVar4 + 0x30) = *(undefined4 *)(puVar4 + 0x28);
      FUN_ram_0010252c(uVar1,*puVar4);
      iVar5 = iVar5 + param_1 + 0x3f80;
      uVar1 = *(undefined4 *)(param_1 + 0x3b20);
      *(undefined1 *)(iVar5 + -0xe4) = 0;
      *(undefined1 *)(iVar5 + -200) = 0;
      *(undefined1 *)(iVar5 + -0xc0) = 0;
      *(undefined4 *)(iVar2 + -0x124) = 0;
      *(undefined4 *)(iVar2 + -0x11c) = 0;
      FUN_ram_00102688(uVar1,uVar6);
      uVar6 = uVar6 + 1;
    } while (uVar6 < *(uint *)(param_1 + 0x3f7c));
  }
  *(undefined4 *)(param_1 + 0x3f7c) = 0;
  *(undefined4 *)(param_1 + 0x3f84) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x3f88) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x3f8c) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x3f90) = 0xffffffff;
  return;
}

