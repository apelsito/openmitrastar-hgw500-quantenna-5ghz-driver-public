/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1ec:
    uVar11 = *(uint *)(iVar23 + 0x5e18);
    if (uVar20 == 1) {
      uVar11 = uVar11 | 1;
    }
    else {
      uVar11 = uVar11 & 0xfffffffe;
    }
    *(uint *)(iVar23 + 0x5e18) = uVar11;
    local_44 = 0;
    ieee80211_beacon_update_all(iVar23);
    break;
