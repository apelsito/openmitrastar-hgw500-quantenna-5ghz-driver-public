
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_00171000(uint param_1,uint param_2,byte param_3,int param_4)

{
  uint uVar1;
  uint uVar2;
  int in_r7;
  
  if ((param_1 < 0x200) || (0x3ff < param_1)) {
    DAT_ram_0018c380 = param_1;
    return;
  }
  if (param_1 == 0x268) {
    in_r7 = 1;
  }
  uVar1 = 0x168;
  if (param_1 != 0x268) {
    uVar1 = param_1 - 0x100;
    in_r7 = 0;
    if (0x1ff < uVar1) {
      DAT_ram_0018c38e = (undefined2)uVar1;
      DAT_ram_0018c384 = DAT_ram_0018c384 | 0x40;
      uVar1 = param_1 - 0x200;
      DAT_ram_0018c393 = param_3;
      goto LAB_ram_00171098;
    }
  }
  DAT_ram_0018c384 = DAT_ram_0018c384 & 0xffffffbf;
  DAT_ram_0018c38c = (undefined2)uVar1;
  DAT_ram_0018c392 = param_3;
  DAT_ram_0018c39c = DAT_ram_0018c384;
LAB_ram_00171098:
  uVar2 = DAT_ram_0018c384;
  if (in_r7 == 0) {
    uVar2 = *(uint *)(uVar1 + 0xe6040000) >> 7;
  }
  uVar2 = param_4 << (param_3 & 0x1f) | ~param_2 & uVar2;
  DAT_ram_0018c398 = DAT_ram_0018c384;
  _DAT_ram_e6040168 = DAT_ram_0018c384;
  *(uint *)(uVar1 + 0xe6040000) = uVar2;
  if (in_r7 == 0) {
    return;
  }
  DAT_ram_0018c384 = uVar2;
  return;
}

