/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1ef:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    local_44 = 0;
    *(byte *)(param_1 + 0xf8e) = bVar4;
    break;
