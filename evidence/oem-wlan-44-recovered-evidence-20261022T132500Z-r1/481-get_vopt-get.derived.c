/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1e1:
    if (*(int *)(iVar8 + 0x24) == 6) {
      local_1c = 0;
      *param_4 = (*(byte *)(iVar8 + 0x4df4) & 0xf) << 4 | (uint)*(byte *)(iVar8 + 0x4df5);
    }
    else {
      local_1c = 0;
      *param_4 = 0;
    }
    break;
