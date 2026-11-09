/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0xf6:
    if (2 < uVar20) goto LAB_ram_00046d30;
    if (*(byte *)(param_1 + 0xf56) != uVar20) {
      *(byte *)(param_1 + 0xf56) = bVar4;
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0xf6,uVar20,0,0);
      break;
    }
    goto LAB_ram_00046d0c;
