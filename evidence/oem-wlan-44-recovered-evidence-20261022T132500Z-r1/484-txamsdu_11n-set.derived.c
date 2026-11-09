/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1e4:
    if ((bool)*(char *)(param_1 + 0xf55) != (uVar20 != 0)) {
      *(char *)(param_1 + 0xf55) = uVar20 != 0;
      ieee80211_param_to_qdrv(piVar13,0x1e4,uVar20,0,0);
      local_44 = 0;
      ieee80211_wireless_reassoc(piVar13,0,1);
      break;
    }
LAB_ram_00046d0c:
    local_44 = 0;
    break;
