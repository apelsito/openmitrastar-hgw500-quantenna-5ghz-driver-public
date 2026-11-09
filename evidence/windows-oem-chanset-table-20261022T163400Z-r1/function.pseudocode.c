
undefined4 * FUN_ram_00012f18(int param_1,int param_2,undefined4 *param_3)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  *param_3 = 0;
  if (param_1 == 0) {
    uVar2 = 0xe;
    if (param_2 == 0x14) {
      puVar1 = &DAT_ram_0008f380;
    }
    else {
      if (param_2 != 0x28) {
        return (undefined4 *)0x0;
      }
      puVar1 = (undefined4 *)&DAT_ram_0008f7e0;
      uVar2 = 0x12;
    }
  }
  else {
    uVar2 = 0x1c;
    if (param_2 == 0x28) {
      puVar1 = (undefined4 *)0x906e0;
    }
    else if (param_2 < 0x29) {
      if (param_2 != 0x14) {
        return (undefined4 *)0x0;
      }
      puVar1 = &DAT_ram_0008fd80;
      uVar2 = 0x1e;
    }
    else if (param_2 == 0x50) {
      puVar1 = (undefined4 *)0x90fa0;
      uVar2 = 0x18;
    }
    else {
      if (param_2 != 0xa0) {
        return (undefined4 *)0x0;
      }
      puVar1 = (undefined4 *)0x91720;
      uVar2 = 0x10;
    }
  }
  *param_3 = uVar2;
  return puVar1;
}

