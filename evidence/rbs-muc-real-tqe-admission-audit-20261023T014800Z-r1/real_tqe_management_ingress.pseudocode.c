
void FUN_ram_00159eb8(undefined4 param_1,uint *param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = *param_2;
  uVar3 = param_2[2];
  uVar1 = param_2[1];
  if (((uVar3 < 0x6ec400) || (0x26ec3ff < uVar3)) && ((uVar3 < 0x26fc400 || (0x48fc3ff < uVar3)))) {
    FUN_ram_000ee260("tqe: buf %p not from hbm pool\n",uVar3);
  }
  else {
    FUN_ram_80025588(uVar3,(short)uVar1);
    FUN_ram_00155ec8(param_1,uVar3,(short)uVar1,uVar2 >> 0x10 & 0xff,1,0,0,0,0);
  }
  return;
}

