
/* WARNING: This function may have set the stack pointer */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void entry(void)

{
  Auxregs0028 = 0;
  Auxregs0094 = 0xe5000000;
  Auxregs0800 = 0;
  _DAT_ram_e0000174 = 0xffff;
  _DAT_ram_e0000178 = 0xffff0000;
  DAT_ram_e5103be8 = 0x57ac6e9d;
  FUN_ram_e5007434();
  return;
}

