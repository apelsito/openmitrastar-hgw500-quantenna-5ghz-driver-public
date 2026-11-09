/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1ee:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    *(ushort *)(param_1 + 0xf8c) = uVar9;
    local_44 = 0;
    (**(code **)(iVar23 + 0x5444))(piVar13);
    break;
