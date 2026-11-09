/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1ed:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5e1c) & 1;
    break;
