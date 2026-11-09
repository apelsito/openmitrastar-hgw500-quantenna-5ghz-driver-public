/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1c9:
    if (*(int *)(param_1 + 0x748) == 6) {
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x1c9,uVar20,0,0);
      break;
    }
    goto LAB_ram_00046d0c;
