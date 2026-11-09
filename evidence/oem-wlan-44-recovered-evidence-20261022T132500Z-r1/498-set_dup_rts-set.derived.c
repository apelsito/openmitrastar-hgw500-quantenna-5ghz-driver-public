/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1f2:
    *(byte *)(iVar23 + 0x535c) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x1f2,uVar20,0,0);