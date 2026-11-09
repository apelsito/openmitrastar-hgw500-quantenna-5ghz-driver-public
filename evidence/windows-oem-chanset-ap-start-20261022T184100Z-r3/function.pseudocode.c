
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 om_oem_scanap_start(int param_1,undefined4 *param_2)

{
  bool bVar1;
  ushort uVar2;
  undefined4 uVar3;
  int iVar4;
  short *psVar5;
  uint uVar6;
  uint uVar7;
  byte *pbVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  ushort *puVar13;
  int in_lp_count;
  char in_is_in_loop;
  
  iVar12 = param_2[0x41];
  puVar13 = (ushort *)(param_1 + 0x43a);
  *puVar13 = 0;
  if (*(short *)(iVar12 + 0x4e30) == 0) {
    pbVar8 = (byte *)(iVar12 + 0xc04);
    iVar10 = 1;
    for (iVar9 = 0; iVar9 < *(int *)(iVar12 + 0xbf8); iVar9 = iVar9 + 1) {
      iVar11 = iVar12 + iVar9 * 0x34 + 0xbfc;
      if ((iVar11 != 0) &&
         (((int)(uint)*(byte *)(iVar12 + (uint)(*pbVar8 >> 3) + 0x4bb0) >> (*pbVar8 & 7) & 1U) != 0)
         ) {
        uVar7 = *(uint *)(pbVar8 + -4);
        if ((uVar7 & 0x10) == 0) {
          if ((uVar7 & 0xa0) != 0xa0) goto LAB_ram_00011d7a;
          in_lp_count = (*(int *)(iVar12 + 0xbf8) - iVar10) + 1;
          _DAT_auxregs_000c = 0x11d1c;
          in_is_in_loop = '\x01';
          _DAT_auxregs_0008 = (code *)&LAB_ram_00011cee;
          iVar4 = iVar10 * 0x34;
          do {
            psVar5 = (short *)(iVar12 + 0xbfc + iVar4);
            iVar4 = iVar4 + 0x34;
            in_lp_count = in_lp_count + -1;
            if (in_lp_count == 0) {
              in_lp_count = 1;
              if (-1 < iVar9) {
                in_lp_count = iVar9 + 1;
              }
              _DAT_auxregs_000c = 0x11d7a;
              in_is_in_loop = '\x01';
              _DAT_auxregs_0008 = (code *)&LAB_ram_00011d46;
              goto LAB_ram_00011d7a;
            }
          } while ((*psVar5 != *(short *)(pbVar8 + -8)) ||
                  ((uVar6 = *(uint *)(psVar5 + 2), (uVar6 & 0xc0) != 0xc0 &&
                   ((uVar6 & 0x480) != 0x480))));
        }
        else if (((*(byte *)(param_2 + 0x111) & 8) == 0) && ((uVar7 & 0x2150) == 0x2150)) {
LAB_ram_00011d7a:
                    /* WARNING: Could not recover jumptable at 0x00011d7a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          if ((in_is_in_loop != '\0') && (bVar1 = in_lp_count != 1, in_lp_count = 0, bVar1)) {
            uVar3 = (*_DAT_auxregs_0008)();
            return uVar3;
          }
          in_is_in_loop = '\0';
          if ((uVar7 & 0x1000) == 0) {
            uVar2 = *puVar13;
            if (0xfe < uVar2) break;
            if (((*(ushort *)(param_1 + 0x10) & 0x20) == 0) || ((uVar7 & 0x100000) == 0)) {
              *(int *)(param_1 + (uint)uVar2 * 4 + 0x3c) = iVar11;
              *puVar13 = uVar2 + 1;
            }
          }
        }
      }
      iVar10 = iVar10 + 1;
      pbVar8 = pbVar8 + 0x34;
    }
  }
  else {
    uVar7 = (*(code *)&ieee80211_get_chanflags)();
    if ((((*(byte *)(param_2 + 0x111) & 1) != 0) && (uVar7 != 0x2150)) &&
       (uVar7 = 0xd0, *(short *)(iVar12 + 0x4e30) != 3)) {
      uVar7 = 0x150;
    }
    pbVar8 = (byte *)(iVar12 + 0xc04);
    for (iVar9 = 0; iVar9 < *(int *)(iVar12 + 0xbf8); iVar9 = iVar9 + 1) {
      iVar10 = iVar12 + iVar9 * 0x34 + 0xbfc;
      if ((iVar10 != 0) &&
         (((int)(uint)*(byte *)(iVar12 + (uint)(*pbVar8 >> 3) + 0x4bb0) >> (*pbVar8 & 7) & 1U) != 0)
         ) {
        uVar6 = *(uint *)(pbVar8 + -4);
        if (((uVar7 & uVar6) == uVar7) &&
           (((uVar6 & 0x10) == 0 || ((*(byte *)(param_2 + 0x111) & 8) == 0)))) {
          uVar2 = *(ushort *)(param_1 + 0x43a);
          if (0xfe < uVar2) break;
          if (((((uVar6 & 0x2000) == 0) || (*(short *)(iVar12 + 0x4e30) == 0xf)) &&
              ((uVar6 & 0x1000) == 0)) &&
             (((*(ushort *)(param_1 + 0x10) & 0x20) == 0 || ((uVar6 & 0x100000) == 0)))) {
            *(int *)(param_1 + (uint)uVar2 * 4 + 0x3c) = iVar10;
            *(ushort *)(param_1 + 0x43a) = uVar2 + 1;
          }
        }
      }
      pbVar8 = pbVar8 + 0x34;
    }
  }
  *(undefined2 *)(param_1 + 0x438) = 0;
  uVar3 = (*(code *)&msecs_to_jiffies)(*(undefined2 *)(iVar12 + 0x5134));
  *(undefined4 *)(param_1 + 0x440) = uVar3;
  uVar3 = (*(code *)&msecs_to_jiffies)(*(undefined2 *)(iVar12 + 0x5138));
  *(undefined4 *)(param_1 + 0x448) = uVar3;
  uVar3 = (*(code *)&msecs_to_jiffies)(*(undefined2 *)(iVar12 + 0x513a));
  *(undefined4 *)(param_1 + 0x44c) = uVar3;
  uVar3 = (*(code *)&msecs_to_jiffies)(*(undefined2 *)(iVar12 + 0x5136));
  *(undefined4 *)(param_1 + 0x444) = uVar3;
  if ((param_2[0x42] & 0x200000) != 0) {
    (*(code *)&printk)("%s: scan set ",*param_2);
    (*(code *)&ieee80211_scan_dump_channels)(param_1);
    (*(code *)&printk)(" dwell min %ld max %ld\n",*(undefined4 *)(param_1 + 0x440),
                       *(undefined4 *)(param_1 + 0x448));
  }
  return 0;
}

