/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1e1:
    if (*(int *)(iVar23 + 0x24) != 6) {
      return -0x5f;
    }
    if (2 < (int)uVar20) {
      return -0x5f;
    }
    if ((int)uVar20 < 0) {
      return -0x5f;
    }
    local_44 = 0;
    *(byte *)(iVar23 + 0x4df4) = bVar4;
    break;
