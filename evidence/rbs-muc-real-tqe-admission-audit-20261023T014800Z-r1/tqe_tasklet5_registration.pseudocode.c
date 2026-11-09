
/* WARNING: Removing unreachable block (ram,0x0015699c) */
/* WARNING: Removing unreachable block (ram,0x001569d4) */
/* WARNING: Removing unreachable block (ram,0x001569a4) */
/* WARNING: Removing unreachable block (ram,0x001569ac) */
/* WARNING: Removing unreachable block (ram,0x001569dc) */
/* WARNING: Removing unreachable block (ram,0x001569e4) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_0015696c(void)

{
  undefined4 uVar1;
  
  _DAT_ram_e1035000 = _DAT_ram_e1035000 & 0x7fffffff;
  _DAT_ram_e1035008 = &DAT_ram_8003a3d8;
  _DAT_ram_e1035004 = 0x100;
  uVar1 = FUN_ram_0015659c();
  FUN_ram_000e83ec(5,&LAB_ram_00159f5c,uVar1,0,&LAB_ram_0015972c,0);
  FUN_ram_0015692c(1);
  return;
}

