/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1c3:
    if (*(char *)(iVar8 + 0x5c28) == '\x01') {
      local_1c = 0;
      *param_4 = 1;
    }
    else if (*(char *)(iVar8 + 0x5c28) == '\x02') {
      local_1c = 0;
      *param_4 = 2;
    }
    else if ((*(uint *)(iVar8 + 0x9c) & 0x40000) == 0) {
      local_1c = 0;
      *param_4 = 0;
    }
    else {
      *param_4 = 3;
      local_1c = 0;
    }
    break;
