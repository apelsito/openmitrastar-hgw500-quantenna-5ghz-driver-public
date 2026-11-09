/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1d4:
    if (999 < uVar20 - 1) {
      return -0x16;
    }
    local_44 = 0;
    *(uint *)(iVar23 + 0x5da8) = uVar20;
    break;
