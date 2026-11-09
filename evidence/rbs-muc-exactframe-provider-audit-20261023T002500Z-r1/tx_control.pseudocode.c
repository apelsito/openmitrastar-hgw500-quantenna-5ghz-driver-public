
void FUN_ram_0016fe74(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = 0;
  if (param_1 == 0) {
    uVar2 = 3;
    uVar1 = 0x3fffff;
  }
  else {
    uVar2 = 0;
  }
  if (*(char *)(DAT_ram_0018c2f0 * 0x1c + 0x214398) == '\x05') {
    if (param_1 == 0) {
      uRam002142bc = 3;
      uRam002142f8 = 3;
      uRam00214334 = 3;
      uRam00214370 = 3;
    }
    else {
      uRam002142bc = 7;
      uRam002142f8 = 7;
      uRam00214334 = 7;
      uRam00214370 = 7;
    }
  }
  FUN_ram_001710d0(0x250,0xffffff,0,uVar1);
  FUN_ram_001710d0(0x254,0xffffff,0,uVar1);
  FUN_ram_001710d0(600,0x3fffff,0,uVar1);
  FUN_ram_001710d0(0x25c,0x3fffff,0,uVar1);
  FUN_ram_001710d0(0x250,0xc00000,0x16,uVar2);
  return;
}

