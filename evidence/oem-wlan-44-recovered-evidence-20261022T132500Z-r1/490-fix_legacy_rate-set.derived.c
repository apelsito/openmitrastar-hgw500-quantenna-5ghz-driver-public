/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1ea:
    iVar24 = *(int *)(param_1 + 0x404);
    if ((int)uVar20 < 0) {
      if (*(int *)(iVar24 + 0x1c) == 0) goto LAB_ram_00046c22;
      iVar25 = *(int *)(iVar24 + 0x18);
      *(undefined4 *)(iVar24 + 0x1c) = 0;
      *(undefined4 *)(iVar24 + 0x18) = 0;
      iVar23 = -1;
      if (5 < iVar25 - 1U) {
        iVar23 = iVar25;
      }
      uVar20 = 0xff;
    }
    else {
      if (0xc < (int)uVar20) {
LAB_ram_00046c22:
        local_44 = 0x16;
        break;
      }
      uVar20 = uVar20 | 0x7f000000 | uVar20 << 8 | uVar20 << 0x10;
      if ((ushort)(*(short *)(iVar24 + 0x4de) - 1U) < 6) {
        iVar23 = -1;
      }
      else {
        iVar23 = *(int *)(iVar24 + 0x14);
        if ((((iVar23 == 1) || (iVar23 == 7)) || (iVar23 == 10)) || (iVar23 - 0xbU < 4)) {
          iVar23 = 1;
        }
        else {
          iVar23 = 3;
        }
      }
      if (*(int *)(iVar24 + 0x1c) == 0) {
        *(uint *)(iVar24 + 0x18) = (uint)*(ushort *)(iVar24 + 0x4de);
        *(undefined4 *)(iVar24 + 0x1c) = 1;
      }
    }
    ieee80211_param_to_qdrv(piVar13,0x3e,uVar20,0,0);
    if (iVar23 != -1) {
      if ((*(char *)(iVar24 + 0xbf5) == '\x02') && (*(int *)(iVar24 + 0x24) == 1)) {
        iVar25 = *(int *)(iVar24 + 0x14);
        if (((iVar25 == 1) || ((iVar25 == 7 || (iVar25 == 10)))) || (iVar25 - 0xbU < 4)) {
          *(uint *)(param_1 + 0x3e0) = (uint)(10 < iVar23);
        }
        else {
          *(uint *)(param_1 + 0x3c8) = (uint)(10 < iVar23);
        }
      }
      *(short *)(iVar24 + 0x4e30) = (short)iVar23;
      *(int *)(iVar24 + 0x14) = iVar23;
      ieee80211_setmode(iVar24,iVar23);
      *(undefined4 *)(iVar24 + 0x4a44) = 4;
      ieee80211_wireless_reassoc(piVar13,0,1);
      if (*(int *)(param_1 + 0x748) == 6) {
        local_44 = 0;
        (**(code **)(iVar24 + 0x53cc))(iVar24);
        break;
      }
    }
    local_44 = 0;
    break;
