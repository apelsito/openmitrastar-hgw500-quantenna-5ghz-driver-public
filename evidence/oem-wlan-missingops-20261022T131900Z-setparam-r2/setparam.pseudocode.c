
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_ram_00041664(int param_1,undefined4 param_2,undefined4 param_3,int *param_4)

{
  undefined1 uVar1;
  ushort uVar2;
  char cVar3;
  byte bVar4;
  undefined2 uVar5;
  uint *puVar6;
  char *pcVar7;
  byte bVar8;
  ushort uVar9;
  undefined4 uVar10;
  uint uVar11;
  ushort *puVar12;
  int *piVar13;
  int iVar14;
  uint uVar15;
  uint uVar16;
  byte bVar17;
  undefined4 *puVar18;
  int iVar19;
  uint uVar20;
  byte *pbVar21;
  int iVar22;
  int iVar23;
  int iVar24;
  int iVar25;
  uint *puVar26;
  byte *pbVar27;
  code *pcVar28;
  bool bVar29;
  bool bVar30;
  int local_44;
  char local_3c;
  undefined1 local_3b;
  int *local_38;
  uint uStack_34;
  uint uStack_30;
  undefined2 local_2a;
  
  iVar24 = _soc_shared_params;
  piVar13 = (int *)(param_1 + 0x300);
  uVar20 = param_4[1];
  iVar23 = *(int *)(param_1 + 0x404);
  iVar25 = *param_4;
  if (*(int *)(param_1 + 0x814) == 0) {
    pbVar27 = (byte *)0x0;
  }
  else {
    pbVar27 = (byte *)(*(int *)(param_1 + 0x814) + 0xc4);
  }
  if (0x1f0 < iVar25 - 2U) goto switchD_ram_000416d4_caseD_b;
  bVar4 = (byte)uVar20;
  uVar9 = (ushort)uVar20;
  uVar11 = (int)uVar20 >> 0x10;
  switch(iVar25) {
  case 2:
    if (*(int *)(iVar23 + 0x1c) != 0) {
      return -0x5f;
    }
    if ((*(char *)(iVar23 + 0xbf5) == '\x02') && (*(int *)(iVar23 + 0x24) == 1)) {
      *(uint *)(param_1 + 0x3e0) = uVar20;
    }
    uVar11 = (*(code *)&chip_id)();
    if (uVar11 < 0x40) goto LAB_ram_00046d30;
    *(undefined4 *)(iVar23 + 0x4a44) = 4;
    ieee80211_param_to_qdrv(piVar13,2,uVar20,0,0);
    ieee80211_wireless_reassoc(piVar13,0,1);
    if (*(int *)(param_1 + 0x748) == 6) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x53cc))(iVar23);
      break;
    }
    goto LAB_ram_00046d0c;
  case 3:
    if (*(int *)(param_1 + 0x814) == 0) {
      return -0xe;
    }
    if (4 < uVar20 - 1) {
      return -0x16;
    }
    iVar24 = ieee80211_authenticator_get(uVar20);
    if (iVar24 == 0) {
      return -0x16;
    }
    switch(uVar20) {
    default:
      *(uint *)(param_1 + 0x738) = *(uint *)(param_1 + 0x738) & 0xfe7fffff;
      break;
    case 5:
      uVar20 = 3;
    }
    *(char *)(*(int *)(param_1 + 0x814) + 0x2c) = (char)uVar20;
    *(int *)(param_1 + 0xb48) = iVar24;
    *(undefined1 *)(param_1 + 0xf68) = 0;
    goto LAB_ram_00046c44;
  case 4:
    if (2 < (int)uVar20) {
      return -0x16;
    }
    *(uint *)(iVar23 + 0x5060) = uVar20;
    if ((*(int *)(iVar23 + 0x4e0c) != 0xffff) &&
       ((uVar20 = *(uint *)(*(int *)(iVar23 + 0x4e0c) + 4), (uVar20 & 0xc0) == 0xc0 ||
        ((uVar20 & 0x480) == 0x480)))) goto LAB_ram_00046c44;
    goto LAB_ram_00046d0c;
  case 5:
    switch(uVar20) {
    case 0:
      uVar11 = 1;
      break;
    case 1:
      uVar11 = 2;
      break;
    case 2:
      uVar11 = 4;
      break;
    case 3:
      uVar11 = 8;
      break;
    default:
      uVar11 = 0;
      break;
    case 5:
      uVar11 = 0x20;
    }
    if (((uVar11 & *(uint *)(param_1 + 0x740)) == 0) &&
       (iVar24 = ieee80211_crypto_available(uVar20), iVar24 == 0)) {
      return -0x16;
    }
    if (pbVar27 == (byte *)0x0) {
      return -0xe;
    }
    if ((*(short *)(iVar23 + 0x5170) == 0x348) && (uVar20 != 3)) {
      (*(code *)&printk)("%s: invalid cipher %d ignored\n","ieee80211_ioctl_setparam",uVar20);
    }
    else {
      *pbVar27 = bVar4;
    }
    goto LAB_ram_0004188e;
  case 6:
    if (0xf < uVar20 - 1) {
      return -0x16;
    }
    if (pbVar27 == (byte *)0x0) {
      return -0xe;
    }
    pbVar27[1] = bVar4;
    goto LAB_ram_0004188e;
  case 7:
    if (pbVar27 == (byte *)0x0) {
      return -0xe;
    }
    iVar24 = 1;
    uVar11 = 0;
    bVar4 = 1;
    do {
      if (((int)uVar20 >> bVar4 & 1U) != 0) {
        switch(iVar24) {
        case 0:
          uVar15 = 1;
          break;
        case 1:
          uVar15 = 2;
          break;
        case 2:
          uVar15 = 4;
          break;
        case 3:
          uVar15 = 8;
          break;
        default:
          uVar15 = 0;
          break;
        case 5:
          uVar15 = 0x20;
        }
        if (((uVar15 & *(uint *)(param_1 + 0x740)) != 0) ||
           (iVar23 = ieee80211_crypto_available(iVar24), iVar23 != 0)) {
          uVar11 = uVar11 | 1 << ((byte)iVar24 & 0x1f);
        }
      }
      iVar24 = iVar24 + 1;
      bVar4 = (byte)iVar24 & 0x1f;
    } while (iVar24 != 0x20);
    if (uVar11 == 0) {
      return -0x16;
    }
    pbVar27[2] = (byte)uVar11;
    goto LAB_ram_0004188e;
  case 8:
    switch(uVar20) {
    case 0:
      uVar11 = 1;
      break;
    case 1:
      uVar11 = 2;
      break;
    case 2:
      uVar11 = 4;
      break;
    case 3:
      uVar11 = 8;
      break;
    default:
      uVar11 = 0;
      break;
    case 5:
      uVar11 = 0x20;
    }
    if (((uVar11 & *(uint *)(param_1 + 0x740)) == 0) &&
       (iVar24 = ieee80211_crypto_available(uVar20), iVar24 == 0)) {
      return -0x16;
    }
    if (pbVar27 == (byte *)0x0) {
      return -0xe;
    }
    pbVar27[3] = bVar4;
    goto LAB_ram_0004188e;
  case 9:
    if (0xf < uVar20 - 1) {
      return -0x16;
    }
    if (pbVar27 == (byte *)0x0) {
      return -0xe;
    }
    local_44 = 0;
    pbVar27[4] = bVar4;
    break;
  case 10:
    if (3 < (int)uVar20) {
      return -0x16;
    }
    puVar6 = (uint *)(param_1 + 0x738);
    uVar11 = *puVar6 & 0xfe7fffff;
    *puVar6 = uVar11;
    if (uVar20 == 2) {
      uVar11 = uVar11 | 0x1000000;
    }
    else if (uVar20 == 3) {
      uVar11 = uVar11 | 0x1800000;
    }
    else {
      if (uVar20 != 1) goto LAB_ram_00046c44;
      uVar11 = uVar11 | 0x800000;
    }
LAB_ram_000422f6:
    *puVar6 = uVar11;
    goto LAB_ram_00046c44;
  case 0xb:
  case 0x11:
  case 0x1a:
  case 0x1b:
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
  case 0x25:
  case 0x43:
  case 0x4b:
  case 0x4c:
  case 0x4f:
  case 0x5a:
  case 0x5b:
  case 0x5c:
  case 0x66:
  case 0x6c:
  case 0x7e:
  case 0x7f:
  case 0xa2:
  case 0xa3:
  case 0xaa:
  case 0xbd:
  case 0xc2:
  case 0xc6:
  case 0xcb:
  case 0xd6:
  case 0xd8:
  case 0xdc:
  case 0xe0:
  case 0xea:
  case 0xf7:
  case 0xf8:
  case 0xfa:
  case 0xfe:
  case 0x103:
  case 0x108:
  case 0x121:
  case 0x123:
  case 0x136:
  case 0x138:
  case 0x142:
  case 0x148:
  case 0x15a:
  case 0x160:
  case 0x170:
  case 0x179:
  case 0x18d:
  case 0x19d:
  case 0x1a0:
  case 0x1a3:
  case 0x1aa:
  case 0x1ac:
  case 0x1b4:
  case 0x1b6:
  case 0x1bc:
  case 0x1bd:
  case 0x1c0:
  case 0x1c2:
  case 0x1c3:
  case 0x1c5:
  case 0x1c6:
  case 0x1c7:
  case 0x1ca:
  case 0x1ce:
  case 0x1d1:
  case 0x1d3:
  case 0x1e2:
  case 0x1e5:
  case 0x1e6:
  case 0x1e7:
  case 0x1e8:
  case 0x1e9:
    goto switchD_ram_000416d4_caseD_b;
  case 0xc:
    if (2 < uVar20) {
      return -0x16;
    }
    local_44 = 0;
    *(uint *)(iVar23 + 0x4e50) = uVar20;
    break;
  case 0xd:
    puVar6 = (uint *)(param_1 + 0x738);
    if (uVar20 == 0) {
      uVar11 = *puVar6 & 0xffffffef;
    }
    else {
      uVar11 = *puVar6 | 0x10;
    }
    *puVar6 = uVar11;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0xd,uVar20,0,0);
    break;
  case 0xe:
    puVar6 = (uint *)(param_1 + 0x738);
    if (uVar20 == 0) {
      uVar20 = *puVar6;
      bVar29 = false;
      *puVar6 = uVar20 & 0xfbffffff;
      uVar20 = uVar20 >> 0x1a & 1;
    }
    else {
      if ((*puVar6 & 0x1800000) == 0) {
        return -0x16;
      }
      bVar29 = true;
      uVar20 = 0;
      *puVar6 = *puVar6 | 0x4000000;
    }
    if (uVar20 != 0 || bVar29) {
      pcVar7 = "invoked";
      if (!bVar29) {
        pcVar7 = "cleared";
      }
      local_44 = 0;
      ieee80211_eventf(param_1,"%sTKIP countermeasures %s","Dot11Msg:",pcVar7);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0xf:
    puVar6 = (uint *)(param_1 + 0x738);
    if (uVar20 == 0) {
      uVar11 = *puVar6 & 0xfdffffff;
    }
    else {
      uVar11 = *puVar6 | 0x2000000;
    }
    *puVar6 = uVar11;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0xf,uVar20,0,0);
    break;
  case 0x10:
    local_44 = 0;
    *(uint *)(param_1 + 0x740) = uVar20;
    break;
  case 0x12:
    if ((*(uint *)(iVar23 + 0xa4) & 0x4000000) != 0) {
      iVar24 = *(int *)(param_1 + 0x404);
      puVar6 = (uint *)(param_1 + 0x738);
      if (uVar20 == 0) {
        *puVar6 = *puVar6 & 0xffffdfff;
        uVar20 = *(uint *)(iVar24 + 0x98) & 0xffffdfff;
      }
      else {
        *puVar6 = *puVar6 | 0x2000;
        uVar20 = *(uint *)(iVar24 + 0x98) | 0x2000;
      }
      *(uint *)(iVar24 + 0x98) = uVar20;
      goto LAB_ram_00046c44;
    }
    goto LAB_ram_00046d0c;
  case 0x13:
    uVar11 = *(uint *)(param_1 + 0x738);
    if (uVar20 != 0) {
      uVar15 = uVar11 | 0x8000000;
    }
    else {
      uVar15 = uVar11 & 0xf7ffffff;
    }
    *(uint *)(param_1 + 0x738) = uVar15;
    if (((uint)(uVar20 != 0) != (uVar11 >> 0x1b & 1)) && ((*(uint *)(param_1 + 0x74c) & 4) != 0)) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x5444))(piVar13);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x14:
    puVar6 = (uint *)(param_1 + 0x738);
    if (uVar20 == 0) {
      local_44 = 0;
      *puVar6 = *puVar6 | 0x10000000;
    }
    else {
      local_44 = 0;
      *puVar6 = *puVar6 & 0xefffffff;
    }
    break;
  case 0x15:
    if (pbVar27 == (byte *)0x0) {
      return -0xe;
    }
    if (uVar20 != 0x21) {
      if ((int)uVar20 < 0x22) {
        if (uVar20 == 2) {
          if (*(char *)(param_1 + 0xe01) == '\x03') {
LAB_ram_00041a16:
            bVar8 = 6;
            goto LAB_ram_00041a08;
          }
        }
        else {
          bVar8 = 0;
          if (uVar20 == 4) {
LAB_ram_00041a08:
            pbVar27[5] = bVar8;
            goto LAB_ram_0004188e;
          }
        }
      }
      else {
        if (uVar20 == 0x80) {
          bVar8 = 5;
          goto LAB_ram_00041a08;
        }
        if (uVar20 == 0x100) goto LAB_ram_00041a16;
      }
    }
    pbVar27[5] = bVar4;
    goto LAB_ram_0004188e;
  case 0x16:
    if (pbVar27 == (byte *)0x0) {
      return -0xe;
    }
    *(ushort *)(pbVar27 + 8) = uVar9;
LAB_ram_0004188e:
    uVar20 = *(uint *)(param_1 + 0x738);
LAB_ram_0004189e:
    if ((uVar20 & 0x1800000) != 0) {
LAB_ram_00046c44:
      if (((*(uint *)(*piVar13 + 0xe4) & 0x41) == 0x41) &&
         (*(int *)(*(int *)(param_1 + 0x404) + 0x4e50) == 1)) {
        local_44 = ieee80211_open();
      }
      else {
        local_44 = 0;
      }
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x17:
    uVar10 = (*(code *)&__divsi3)(uVar20,5);
    local_44 = 0;
    *(undefined4 *)(param_1 + 0x784) = uVar10;
    break;
  case 0x18:
    uVar10 = (*(code *)&__divsi3)(uVar20,5);
    local_44 = 0;
    *(undefined4 *)(param_1 + 0x780) = uVar10;
    break;
  case 0x19:
    uVar10 = (*(code *)&__divsi3)(uVar20,5);
    local_44 = 0;
    *(undefined4 *)(param_1 + 0x77c) = uVar10;
    break;
  case 0x1c:
    if ((*(int *)(param_1 + 0x748) != 6) && (*(int *)(param_1 + 0x748) != 0)) {
      return -0x16;
    }
    if (uVar20 - 1 < 0xf) {
      *(byte *)(param_1 + 0x80a) = bVar4;
      local_44 = -0x16;
      (**(code **)(iVar23 + 0x5444))(piVar13);
      break;
    }
    goto LAB_ram_00046d30;
  case 0x1d:
    if ((*(int *)(param_1 + 0x748) != 6) && (*(int *)(param_1 + 0x748) != 0)) {
      return -0x16;
    }
    if (uVar20 - 0x19 < 0x1370) {
      *(ushort *)(*(int *)(param_1 + 0x404) + 0x4e2) = uVar9;
      local_44 = -0x16;
      ieee80211_beacon_interval_set(*(undefined4 *)(param_1 + 0x404),uVar20);
      break;
    }
    goto LAB_ram_00046d30;
  case 0x1e:
    if (uVar20 == 0) {
      local_44 = 0;
      *(uint *)(iVar23 + 0x98) = *(uint *)(iVar23 + 0x98) & 0xbfffffff;
    }
    else {
      local_44 = 0;
      *(uint *)(iVar23 + 0x98) = *(uint *)(iVar23 + 0x98) | 0x40000000;
    }
    break;
  case 0x1f:
    local_3c = -100;
    if (((((*(uint *)(iVar23 + 0x98) & 0x40000000) == 0) ||
         ((*(uint *)(iVar23 + 0x9c) & 0x800000) == 0)) || (*(int *)(param_1 + 0x748) != 6)) ||
       (*(int *)(iVar23 + 0x4e0c) == 0xffff)) goto switchD_ram_000416d4_caseD_b;
    local_38 = piVar13;
    ieee80211_iterate_nodes(iVar23 + 0x4e5c,get_max_in_minpwr,&local_3c,1);
    uVar20 = uVar20 & 0xffff;
    iVar24 = (int)*(char *)(*(int *)(iVar23 + 0x4e0c) + 9);
    if (iVar24 <= (int)uVar20) {
      local_44 = 0x16;
      (*(code *)&printk)("power constraint(%d) >= current channel max regulatory power(%d)\n");
      break;
    }
    if ((local_3c != -100) && ((int)(iVar24 - uVar20) < (int)local_3c)) {
      local_44 = 0x16;
      (*(code *)&printk)(
                        "power constraint(%d) make local max transmit power(%d) less than the max value(%d) of min power in associated STAs\n"
                        );
      break;
    }
    *(char *)(iVar23 + 0x529c) = (char)uVar20;
    if (*(int *)(param_1 + 0x74c) == 4) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x5444))(piVar13);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x20:
    iVar24 = *(int *)(param_1 + 0x814);
    if (iVar24 == 0) {
      return -0xe;
    }
    local_44 = 0;
    (**(code **)(*(int *)(iVar24 + 4) + 0x5374))(iVar24,0x20,0);
    break;
  case 0x26:
    if ((*(int *)(param_1 + 0x748) == 2) && ((*(ushort *)(param_1 + 0xce0) & 3) - 1 < 2)) {
      if ((*(uint *)(param_1 + 0x408) & 0x40000000) == 0) {
        return -0x16;
      }
      ieee80211_note(piVar13,"%s can\'t config repeater since it\'s a extender\n",*piVar13);
      return -0x16;
    }
    if (*(int *)(*(int *)(iVar23 + 8) + 0x448) != 1) {
      return -0x16;
    }
    if (uVar20 == 0) {
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) & 0xfffbffff;
    }
    else {
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) | 0x40000;
      ieee80211_new_state(*(int *)(iVar23 + 8),0,0);
    }
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x26,uVar20,0,0);
    break;
  case 0x27:
    pbVar27 = (byte *)(param_1 + 0xbd0);
    if (uVar20 == 0) {
      bVar4 = *pbVar27 | 2;
    }
    else {
      bVar4 = *pbVar27 & 0xfd;
    }
    *pbVar27 = bVar4;
    local_44 = 0;
    ieee80211_bridgemode_set(piVar13,1);
    break;
  case 0x28:
    if (uVar20 == 0) {
      *(uint *)(param_1 + 0x738) = *(uint *)(param_1 + 0x738) & 0xffffbfff;
      local_44 = 0;
      ieee80211_cancel_scan(piVar13);
    }
    else {
      if ((*(uint *)(param_1 + 0x740) & 0x20000000) == 0) {
        return -0x16;
      }
      local_44 = 0;
      *(uint *)(param_1 + 0x738) = *(uint *)(param_1 + 0x738) | 0x4000;
    }
    break;
  case 0x29:
    if (99 < (int)uVar20) {
      uVar10 = (*(code *)&msecs_to_jiffies)(uVar20);
      local_44 = 0;
      *(undefined4 *)(param_1 + 0x7e0) = uVar10;
      break;
    }
    goto LAB_ram_00046d30;
  case 0x2a:
    if (0xe < (int)uVar20) {
      local_44 = 0;
      *(uint *)(param_1 + 0x7e4) = uVar20 * 200;
      *(uint *)(iVar23 + 0x5c90) = uVar20 * 200;
      break;
    }
    goto LAB_ram_00046d30;
  case 0x2b:
    if (uVar20 - 0x100 < 0xd1f1) {
      local_44 = 0;
      *(uint *)(param_1 + 0xb78) = uVar20;
      break;
    }
    goto LAB_ram_00046d30;
  case 0x2c:
    if (0x1f < uVar20) goto LAB_ram_00046d30;
    *(byte *)(iVar23 + 0x4f4) = bVar4;
    if (((*(uint *)(*piVar13 + 0xe4) & 0x41) == 0x41) &&
       (*(int *)(*(int *)(param_1 + 0x404) + 0x4e50) == 1)) {
      local_44 = 0;
      ieee80211_new_state(piVar13,1,0);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x2d:
    if (uVar20 == 0) {
      uVar20 = *(uint *)(iVar23 + 0x9c) & 0xfffffffd;
    }
    else {
      uVar20 = *(uint *)(iVar23 + 0x9c) | 2;
    }
    goto LAB_ram_00042226;
  case 0x2e:
    local_44 = 0;
    *(uint *)(param_1 + 0x7e8) = uVar20 * 200;
    break;
  case 0x2f:
    local_44 = 0;
    *(byte *)(param_1 + 0x7ec) = bVar4;
    break;
  case 0x30:
    local_44 = 0;
    *(byte *)(param_1 + 0x7ee) = bVar4;
    break;
  case 0x31:
    local_44 = 0;
    *(byte *)(param_1 + 0x7ed) = bVar4;
    break;
  case 0x32:
    local_44 = 0;
    *(byte *)(param_1 + 0x7f0) = bVar4;
    break;
  case 0x33:
    local_44 = 0;
    *(byte *)(param_1 + 0x7f2) = bVar4;
    break;
  case 0x34:
    local_44 = 0;
    *(byte *)(param_1 + 0x7f1) = bVar4;
    break;
  case 0x35:
    if (*(int *)(param_1 + 0x748) == 6) {
      if ((*(uint *)(iVar23 + 0xa4) & 0x40000000) != 0) {
        puVar6 = (uint *)(param_1 + 0x73c);
        if (uVar20 == 0) {
          uVar11 = *puVar6 & 0xffffffef;
        }
        else {
          uVar11 = *puVar6 | 0x10;
        }
        goto LAB_ram_000422f6;
      }
    }
    else if (*(int *)(param_1 + 0x748) == 1) {
      *(byte *)(param_1 + 0x810) = bVar4;
      *(uint *)(param_1 + 0x73c) = *(uint *)(param_1 + 0x73c) | 0x10;
      goto LAB_ram_00046c44;
    }
    goto LAB_ram_00046d0c;
  case 0x36:
    if (*(int *)(param_1 + 0x814) == 0) {
      return -0xe;
    }
    puVar6 = (uint *)(param_1 + 0x73c);
    if (uVar20 == 0) {
      uVar20 = *puVar6 & 0xffffffdf;
    }
    else {
      uVar20 = *puVar6 | 0x20;
    }
    *puVar6 = uVar20;
    iVar24 = *(int *)(param_1 + 0x814);
    uVar20 = Auxregs0028;
    if ((uVar20 & 6) != 0) {
      Auxregs0028 = uVar20 & 0xfffffff9;
    }
    *(int *)(iVar24 + 0x24) = *(int *)(iVar24 + 0x24) + 1;
    Auxregs0028 = uVar20;
    local_44 = 0;
    ieee80211_send_nulldata(*(int *)(param_1 + 0x814));
    break;
  case 0x37:
    if (*(int *)(param_1 + 0x748) != 6) {
      iVar24 = *(int *)(param_1 + 0x814);
      if (iVar24 == 0) {
        return -0xe;
      }
      uVar11 = Auxregs0028;
      if ((uVar11 & 6) != 0) {
        Auxregs0028 = uVar11 & 0xfffffff9;
      }
      *(int *)(iVar24 + 0x24) = *(int *)(iVar24 + 0x24) + 1;
      Auxregs0028 = uVar11;
      local_44 = 0;
      ieee80211_send_qosnulldata(*(int *)(param_1 + 0x814),uVar20);
      break;
    }
    iVar24 = ieee80211_find_node_by_idx(iVar23,0,uVar20 & 0xffff);
    if (iVar24 != 0) {
      local_44 = 0;
      ieee80211_send_qosnulldata(iVar24,1);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x38:
    if (*(int *)(param_1 + 0x814) == 0) {
      return -0xe;
    }
    local_44 = 0;
    ieee80211_send_pspoll();
    break;
  case 0x39:
    if (*(int *)(param_1 + 0x748) == 6) {
      puVar6 = (uint *)(param_1 + 0x73c);
      if (uVar20 == 0) {
        local_44 = 0;
        *puVar6 = *puVar6 & 0xffffffbf;
      }
      else {
        local_44 = 0;
        *puVar6 = *puVar6 | 0x40;
      }
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x3a:
    if (uVar20 == 0) {
      uVar11 = *(uint *)(iVar23 + 0x9c) & 0xffffff7f;
    }
    else {
      uVar11 = *(uint *)(iVar23 + 0x9c) | 0x80;
    }
    *(uint *)(iVar23 + 0x9c) = uVar11;
    local_44 = 0;
    (**(code **)(iVar23 + 0x5524))(uVar20);
    break;
  case 0x3b:
    if (uVar20 == 0) {
      uVar20 = *(uint *)(iVar23 + 0x9c) & 0xfffffeff;
    }
    else {
      uVar20 = *(uint *)(iVar23 + 0x9c) | 0x100;
    }
LAB_ram_00042226:
    *(uint *)(iVar23 + 0x9c) = uVar20;
    goto LAB_ram_00046c44;
  case 0x3c:
    puVar6 = (uint *)(param_1 + 0x73c);
    if (uVar20 == 0) {
      uVar11 = *puVar6 & 0xfffff7ff;
    }
    else {
      uVar11 = *puVar6 | 0x800;
    }
    *puVar6 = uVar11;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x3c,uVar20,0,0);
    break;
  case 0x3d:
    if (uVar20 == 0) {
      uVar11 = *(uint *)(iVar23 + 0xa4) & 0xffff7fff;
      uVar15 = *(uint *)(iVar23 + 0x98) & 0xfffbffff | 0x200000;
    }
    else {
      uVar11 = *(uint *)(iVar23 + 0xa4) | 0x8000;
      uVar15 = *(uint *)(iVar23 + 0x98) & 0xffdfffff | 0x40000;
    }
    *(uint *)(iVar23 + 0x98) = uVar15;
    *(uint *)(iVar23 + 0xa4) = uVar11;
    ieee80211_param_to_qdrv(piVar13,0x3d,uVar20,0,0);
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,1);
    break;
  case 0x3e:
    if ((uVar20 & 0xff000000) == 0x7e000000) {
      if (3 < *(int *)(iVar23 + 0x14) - 0xbU) {
        return -0x16;
      }
      uVar11 = (int)(uVar20 & 0xf0) >> 4;
      if (9 < (uVar20 & 0xf)) {
        return -0x16;
      }
      if (3 < uVar11) {
        return -0x16;
      }
      iVar24 = ieee80211_swfeat_is_supported(5,0);
      if ((iVar24 == 0) && (iVar24 = ieee80211_swfeat_is_supported(6,0), iVar24 == 0)) {
        iVar24 = ieee80211_swfeat_is_supported(7,0);
        if (iVar24 == 0) goto LAB_ram_000430e6;
        bVar29 = uVar11 == 2;
        bVar30 = uVar11 < 2;
      }
      else {
        bVar29 = uVar11 == 1;
        bVar30 = uVar11 == 0;
      }
LAB_ram_000430e2:
      if (!bVar30 && !bVar29) {
        return -0x16;
      }
    }
    else if ((uVar20 & 0xff000000) == 0x7f000000) {
      uVar11 = uVar20 & 0x7f;
      if (0x4c < uVar11) {
        return -0x16;
      }
      if (uVar11 == 0x20) {
        return -0x16;
      }
      iVar24 = ieee80211_swfeat_is_supported(5,0);
      if ((iVar24 == 0) && (iVar24 = ieee80211_swfeat_is_supported(6,0), iVar24 == 0)) {
        iVar24 = ieee80211_swfeat_is_supported(7,0);
        if (iVar24 == 0) goto LAB_ram_000430e6;
        if (uVar11 - 0x18 < 9) {
          return -0x16;
        }
        bVar29 = uVar11 == 0x34;
        bVar30 = uVar11 < 0x34;
      }
      else {
        if (uVar11 - 0x10 < 0x11) {
          return -0x16;
        }
        bVar29 = uVar11 == 0x26;
        bVar30 = uVar11 < 0x26;
      }
      goto LAB_ram_000430e2;
    }
LAB_ram_000430e6:
    (*(code *)&printk)("Warning: %s MCS rate is fixed at 0x%08x\n","ieee80211_ioctl_setparam",uVar20
                      );
    ieee80211_param_to_qdrv(piVar13,0x3e,uVar20,0,0);
    local_44 = 0;
    *(uint *)(param_1 + 0xd74) = uVar20;
    break;
  default:
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,iVar25,uVar20,0,0);
    break;
  case 0x40:
    if ((uVar20 == 0) && (*(int *)(iVar23 + 0x14) - 0xbU < 4)) {
      return -0x5f;
    }
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x40,uVar20,0,0);
    break;
  case 0x42:
    local_44 = 0;
    *(uint *)(param_1 + 0x408) = uVar20;
    break;
  case 0x45:
  case 0x47:
    if ((((uVar20 != 0xa0) && (uVar20 != 0x50)) && (uVar20 != 0x28)) && (uVar20 != 0x14)) {
      return -0x16;
    }
    if ((*(char *)(iVar23 + 0xbf5) == '\x02') && (*(int *)(iVar23 + 0x24) == 1)) {
      iVar24 = *(int *)(iVar23 + 0x14);
      if (((iVar24 == 1) || ((iVar24 == 7 || (iVar24 == 10)))) || (iVar24 - 0xbU < 4)) {
        *(uint *)(param_1 + 0x3e4) = uVar20;
      }
      else {
        *(uint *)(param_1 + 0x3cc) = uVar20;
      }
    }
    if ((int)uVar20 < 0x29) {
      if (uVar20 == 0x28) goto LAB_ram_0004325e;
    }
    else {
      iVar24 = ieee80211_swfeat_is_supported(4,1);
      if (iVar24 == 0) goto switchD_ram_000416d4_caseD_b;
LAB_ram_0004325e:
      if ((*(short *)(iVar23 + 0x4de) == 1) ||
         ((1 < *(int *)(iVar23 + 0x14) - 8U &&
          ((*(uint *)(*(int *)(iVar23 + 0x4e08) + 4) & 0x80) != 0)))) {
        return -0x5f;
      }
    }
    *(byte *)(iVar23 + 0x4df1) = bVar4;
    ieee80211_change_bw(piVar13,uVar20,1);
    ieee80211_start_obss_scan_timer(piVar13);
    *(undefined4 *)(iVar23 + 0x4a44) = 4;
    if (((*(uint *)(*piVar13 + 0xe4) & 0x41) == 0x41) &&
       (*(int *)(*(int *)(param_1 + 0x404) + 0x4e50) == 1)) {
      ieee80211_wireless_reassoc(piVar13,0);
      local_44 = 0;
      ieee80211_new_state(piVar13,1,0);
      break;
    }
    if (*(int *)(param_1 + 0x748) == 6) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x53cc))(iVar23);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x4e:
    *(byte *)(iVar23 + 0x4df6) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x4e,uVar20,0,0);
    break;
  case 0x55:
    local_44 = 0;
    g_channel_fixed = bVar4;
    break;
  case 0x5d:
    ieee80211_param_to_qdrv(piVar13,0x5d,uVar20,0,0);
    iVar24 = *(int *)(iVar23 + 8);
    while (iVar24 != 0) {
      iVar25 = *(int *)(iVar24 + 0xf8);
      if (uVar20 == 0) {
        uVar11 = *(uint *)(iVar24 + 0xa78) & 0xfffff7ff;
        uVar15 = *(uint *)(iVar24 + 0xa7c) & 0xffffffdf;
      }
      else {
        uVar11 = *(uint *)(iVar24 + 0xa78) | 0x800;
        uVar15 = *(uint *)(iVar24 + 0xa7c) | 0x20;
      }
      *(uint *)(iVar24 + 0xa78) = uVar11;
      *(uint *)(iVar24 + 0xa7c) = uVar15;
      if ((*(int *)(iVar24 + 0x448) == 6) && (*(int *)(iVar24 + 0x44c) == 4)) {
        (**(code **)(iVar23 + 0x5444))(iVar24);
      }
      ieee80211_wireless_reassoc(iVar24,0,1);
      iVar24 = iVar25;
    }
    goto LAB_ram_00046d0c;
  case 0x5e:
    local_44 = 0;
    *(byte *)(param_1 + 0xbcc) = bVar4;
    break;
  case 0x5f:
    if (*(int *)(param_1 + 0x814) == 0) {
      return -0xe;
    }
    if (*(int *)(param_1 + 0x748) != 1) {
      local_44 = 0;
      (*(code *)&printk)("<3>%s: can\'t set beacon miss threshold for non STA\n",param_1);
      break;
    }
    if ((int)uVar20 < 1) {
      local_44 = 0;
      (*(code *)&printk)("<3>%s: bad value %d, beacon miss threshold must be >= 1\n",param_1,uVar20)
      ;
      break;
    }
    (*(code *)&printk)("<6>%s: set beacon miss threshold to %d\n",param_1,uVar20);
    *(uint *)(param_1 + 0xbc8) = uVar20;
    uVar5 = (*(code *)&__divsi3)
                      (uVar20 * 0x400 * (uint)*(ushort *)(*(int *)(param_1 + 0x814) + 0xa42),5000);
    *(undefined2 *)(param_1 + 0xba4) = uVar5;
    if (*(ushort *)(param_1 + 0xbc6) != 0) {
      local_44 = 0;
      uVar5 = (*(code *)&__divsi3)(uVar5,*(ushort *)(param_1 + 0xbc6) + 1);
      *(undefined2 *)(param_1 + 0xba4) = uVar5;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x60:
    if (uVar20 == 0xffffffff) {
      iVar24 = ieee80211_find_node(*(int *)(param_1 + 0x404) + 0x4e5c,param_1 + 0x72c);
      if (iVar24 == 0) goto LAB_ram_00046d0c;
      (*(code *)&printk)("Clearing force SMPS mode in driver\n");
      uVar11 = (uint)*(byte *)(iVar24 + 0x198);
      ieee80211_free_node(iVar24);
    }
    else {
      uVar11 = (uint)(short)uVar9;
    }
    if (uVar11 == 2) {
      local_44 = 0;
      (*(code *)&printk)("Ignoring invalid SMPS mode (%04X) at WLAN driver\n",2);
      break;
    }
    if (*(int *)(param_1 + 0x748) == 1) {
      (*(code *)&memset)(&local_3c,0,8);
      local_3c = '\a';
      local_3b = 1;
      if (uVar11 == 1) {
        local_38 = (int *)0x3;
      }
      else {
        local_38 = (int *)0x0;
        if ((uVar11 != 3) && (local_38 = (int *)(uint)(uVar11 == 0), uVar11 != 0)) {
          local_44 = 0;
          (*(code *)&printk)("Not sending ACTION frame\n");
          break;
        }
      }
      puVar12 = (ushort *)(param_1 + 0xd88);
      (*(code *)&printk)("STA Sending HT Action frame to change PS mode (%02X->%02X)\n",
                         *(undefined1 *)puVar12,uVar11);
      if (uVar20 == 0xffffffff) {
        uVar9 = *puVar12 & 0x7fff;
      }
      else {
        uVar9 = (ushort)uVar11 | 0x8000;
      }
      *puVar12 = uVar9;
      local_44 = 0;
      (**(code **)(*(int *)(*(int *)(param_1 + 0x814) + 4) + 0x5374))
                (*(int *)(param_1 + 0x814),0xd0,&local_3c);
      break;
    }
    goto LAB_ram_00046d0c;
  case 99:
    if ((uint)*(byte *)(param_1 + 0xd8a) != (uVar20 & 0xffff)) {
      (*(code *)&printk)("New implicit BA value (%04X) - remove all associations\n");
      *(byte *)(param_1 + 0xd8a) = bVar4;
      local_44 = 0;
      ieee80211_wireless_reassoc(piVar13,0,1);
      break;
    }
    goto LAB_ram_00046d0c;
  case 100:
    (*(code *)&printk)("Removing clients, not forcing deauth\n");
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,1,0);
    break;
  case 0x65:
    if (uVar20 == 1) {
      local_44 = 0;
      (*(code *)&printk)("WLAN bytes: allocated=%d freed=%d balance=%d\n     times: allocated=%d freed=%d\n     nodes: allocated=%d freed=%d current=%d refs=%u\n     tmp:   allocated=%d freed=%d\n"
                         ,g_wlan_tot_alloc,g_wlan_tot_free,g_wlan_balance,g_wlan_tot_alloc_cnt,
                         g_wlan_tot_free_cnt,g_wlan_tot_node_alloc,g_wlan_tot_node_free,
                         *(undefined4 *)(iVar23 + 0x54fc),*(undefined4 *)(*piVar13 + 500),
                         g_wlan_tot_node_alloc_tmp,g_wlan_tot_node_free_tmp);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x67:
    uVar2 = *(ushort *)(param_1 + 0xd8c);
    if ((uint)uVar2 != (uVar20 & 0xffff)) {
      *(ushort *)(param_1 + 0xd8c) = uVar9;
      *(ushort *)(param_1 + 0xd8e) = uVar2;
      local_44 = 0;
      ieee80211_wireless_reassoc(piVar13,0,1);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x68:
    pbVar27 = (byte *)(param_1 + 0xbd2);
    if (uVar20 == 0) {
      local_44 = 0;
      *pbVar27 = *pbVar27 | 1;
      (*(code *)&printk)("Disabling associations with no SSID\n");
    }
    else {
      local_44 = 0;
      *pbVar27 = *pbVar27 & 0xfe;
      (*(code *)&printk)("Enabling associations with no SSID\n");
    }
    break;
  case 0x69:
    *(byte *)(iVar23 + 0xbe4) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x69,uVar20,0,0);
    break;
  case 0x6a:
    uVar10 = 0;
    goto LAB_ram_000437ea;
  case 0x6b:
    local_44 = 0;
    _DAT_ram_0f00006c = uVar20;
    break;
  case 0x6e:
    ieee80211_param_to_qdrv(piVar13,0x6e,uVar20,0,0);
    pbVar27 = (byte *)(iVar23 + 0x567);
    puVar26 = (uint *)(iVar23 + 0x568);
    puVar6 = (uint *)(iVar23 + 0x53c);
    iVar24 = *(int *)(iVar23 + 8);
    while (iVar24 != 0) {
      iVar25 = *(int *)(iVar24 + 0xf8);
      if (uVar20 == 0) {
        *(uint *)(iVar24 + 0xa78) = *(uint *)(iVar24 + 0xa78) & 0xffffefff;
        *(uint *)(iVar24 + 0xa7c) = *(uint *)(iVar24 + 0xa7c) & 0xffffffef;
        *pbVar27 = *pbVar27 & 0xef;
        *puVar26 = *puVar26 & 0xff000000 | *puVar26 & 0xffffff;
        uVar11 = *puVar6 & 0xffffffef;
      }
      else {
        *(uint *)(iVar24 + 0xa78) = *(uint *)(iVar24 + 0xa78) | 0x1000;
        *(uint *)(iVar24 + 0xa7c) = *(uint *)(iVar24 + 0xa7c) | 0x10;
        *pbVar27 = *pbVar27 | 0x10;
        *puVar26 = *puVar26 & 0xff000000 | *puVar26 & 0xffffff;
        uVar11 = *puVar6 | 0x10;
      }
      *puVar6 = uVar11;
      if ((*(int *)(iVar24 + 0x448) == 6) && (*(int *)(iVar24 + 0x44c) == 4)) {
        (**(code **)(iVar23 + 0x5444))(iVar24);
      }
      ieee80211_wireless_reassoc(iVar24,0,1);
      iVar24 = iVar25;
    }
    goto LAB_ram_00046d0c;
  case 0x6f:
    if (uVar20 == 0) {
      local_44 = 0;
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) & 0xffefffff;
    }
    else if (((*(char *)(iVar23 + 0x5313) == '\0') ||
             (iVar24 = findchannel(iVar23,*(char *)(iVar23 + 0x5313),
                                   *(undefined2 *)(iVar23 + 0x4e30)), iVar24 == 0)) ||
            (iVar24 = (**(code **)(iVar23 + 0x548c))(iVar23,iVar24), iVar24 == 2)) {
      local_44 = 0;
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) | 0x100000;
    }
    else {
      local_44 = 0x16;
    }
    break;
  case 0x70:
    if ((uVar20 != 0) && ((*(ushort *)(iVar23 + 0x500) & 2) == 0)) goto LAB_ram_00046d30;
    ieee80211_param_to_qdrv(piVar13,0x70,uVar20,0,0);
    if ((*(int *)(param_1 + 0x748) == 6) && (*(int *)(param_1 + 0x74c) == 4)) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x5444))(piVar13);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x71:
    if (7 < uVar20) {
      return -0x16;
    }
    if (*(byte *)(iVar23 + 0x505) != uVar20) {
      *(byte *)(iVar23 + 0x505) = bVar4;
      ieee80211_wireless_reassoc(piVar13,0,1);
    }
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x71,uVar20,0,0);
    break;
  case 0x72:
    if (uVar20 == 0) {
      local_44 = 0;
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) & 0xffdfffff;
    }
    else {
      local_44 = 0;
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) | 0x200000;
    }
    break;
  case 0x73:
    uStack_30 = uVar20 & 0xffffff;
    local_2a = 0;
    local_44 = ieee80211_country_string_to_countryid(&uStack_30,&local_2a);
    if (local_44 == 0) {
      *(undefined2 *)(iVar23 + 0x5170) = local_2a;
      (**(code **)(iVar23 + 0x5510))(iVar23,*(undefined4 *)(iVar23 + 0xbf8),iVar23 + 0xbfc);
      (**(code **)(iVar23 + 0x5514))(iVar23,*(undefined4 *)(iVar23 + 0xbf8),iVar23 + 0xbfc);
      local_44 = ieee80211_region_to_operating_class(iVar23,&uStack_30);
      if (local_44 < 0) {
        *(uint *)(param_1 + 0x73c) = *(uint *)(param_1 + 0x73c) | 0x2000000;
      }
    }
    goto LAB_ram_00046c3a;
  case 0x7a:
    *(uint *)(iVar23 + 0x4ec) = uVar20;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x7a,uVar20,0,0);
    break;
  case 0x7b:
    if ((0x4c < uVar20) || (uVar20 == 0x20)) goto LAB_ram_00042710;
    iVar24 = ieee80211_swfeat_is_supported(5,0);
    if ((iVar24 == 0) && (iVar24 = ieee80211_swfeat_is_supported(6,0), iVar24 == 0)) {
      iVar24 = ieee80211_swfeat_is_supported(7,0);
      if (iVar24 != 0) {
        if (uVar20 - 0x18 < 9) goto LAB_ram_00042710;
        bVar29 = uVar20 == 0x34;
        bVar30 = uVar20 < 0x34;
        goto LAB_ram_0004270c;
      }
    }
    else {
      if (uVar20 - 0x10 < 0x11) goto LAB_ram_00042710;
      bVar29 = uVar20 == 0x26;
      bVar30 = uVar20 < 0x26;
LAB_ram_0004270c:
      if (!bVar30 && !bVar29) {
LAB_ram_00042710:
        (*(code *)&printk)("Invalid MCS in 11n mode\n");
        return -0x16;
      }
    }
    *(uint *)(param_1 + 0xd74) =
         uVar20 & 0xff | 0x7f000000 | (uVar20 & 0xff) << 0x10 | (uVar20 & 0xff) << 8;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x7b,uVar20,0,0);
    break;
  case 0x80:
    if ((int)uVar20 < 0x3ff) {
      *(ushort *)(param_1 + 0xd94) = uVar9;
    }
    else {
      *(ushort *)(param_1 + 0xd94) = 0x3ff;
    }
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,0);
    break;
  case 0x83:
    if (uVar20 < 3) {
      ieee80211_param_to_qdrv(piVar13,0x83,uVar20,0,0);
      local_44 = 0;
      *(byte *)(iVar23 + 0x5340) = bVar4;
      break;
    }
    goto LAB_ram_00046d30;
  case 0x85:
    local_44 = 0;
    *(ushort *)(iVar23 + 0x5134) = uVar9;
    break;
  case 0x86:
    local_44 = 0;
    *(ushort *)(iVar23 + 0x5136) = uVar9;
    break;
  case 0x87:
    local_44 = 0;
    *(ushort *)(iVar23 + 0x5138) = uVar9;
    break;
  case 0x88:
    local_44 = 0;
    *(ushort *)(iVar23 + 0x513a) = uVar9;
    break;
  case 0x8b:
    local_44 = 0;
    *(uint *)(param_1 + 0xd98) = uVar20;
    break;
  case 0x8c:
    if (*(char *)(iVar23 + 0xbf5) == '\x02') {
      return -0x5f;
    }
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x8c,uVar20,0,0);
    break;
  case 0x93:
    iVar24 = ieee80211_should_disable_scs(iVar23);
    if (iVar24 == 0) {
      return -0x5f;
    }
    iVar24 = ieee80211_param_scs_set(param_1,piVar13,uVar20);
    if (iVar24 == 0) {
      ieee80211_param_to_qdrv(piVar13,0x93,uVar20,0,0);
      local_44 = 0;
      *(uint *)(iVar23 + 0x4dfc) = uVar20 & 0xffff;
      break;
    }
    goto LAB_ram_00046d30;
  case 0x94:
    uVar20 = uVar20 & 0xff;
    if (uVar20 == 0) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5313) = bVar4;
    }
    else if (((*(byte *)(*(int *)(iVar23 + 0x4e08) + 8) == uVar20) ||
             (iVar24 = findchannel(iVar23,uVar20,*(undefined2 *)(iVar23 + 0x4e30)), iVar24 == 0)) ||
            (((*(uint *)(iVar23 + 0x9c) & 0x100000) != 0 &&
             (iVar24 = (**(code **)(iVar23 + 0x548c))(iVar23,iVar24), iVar24 != 2)))) {
      local_44 = 0x16;
    }
    else {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5313) = bVar4;
    }
    break;
  case 0x95:
    if (uVar20 == 0) {
      local_44 = 0;
      *(uint *)(iVar23 + 0xa0) = *(uint *)(iVar23 + 0xa0) & 0xfffffffe;
    }
    else {
      local_44 = 0;
      *(uint *)(iVar23 + 0xa0) = *(uint *)(iVar23 + 0xa0) | 1;
    }
    break;
  case 0x96:
    *(byte *)(iVar23 + 0xbe5) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x96,uVar20,0,0);
    break;
  case 0x97:
    if (uVar20 - 5 < 0x704) {
      local_44 = 0;
      *(uint *)(iVar23 + 0x5318) = uVar20 * 200;
      break;
    }
    goto LAB_ram_00046d30;
  case 0x98:
    if (uVar20 == 0) {
      local_44 = 0;
      *(uint *)(iVar23 + 0xa0) = *(uint *)(iVar23 + 0xa0) & 0xfffffffd;
    }
    else {
      local_44 = 0;
      *(uint *)(iVar23 + 0xa0) = *(uint *)(iVar23 + 0xa0) | 2;
    }
    break;
  case 0x99:
    iVar24 = 0;
    do {
      bVar8 = (byte)iVar24;
      iVar24 = iVar24 + 8;
      if (0xb < ((int)uVar20 >> (bVar8 & 0x1f) & 0xffU)) {
        local_44 = 0x16;
        goto LAB_ram_00046c86;
      }
    } while (iVar24 != 0x20);
    (*(code *)&printk)("Forcing multicast rate to %sMbps\n",
                       (&PTR_DAT_ram_0009cebc_ram_000881b4)[uVar20 >> 0x18]);
    *(byte *)(param_1 + 0xda0) = bVar4;
    ieee80211_param_to_qdrv(piVar13,0x99,uVar20,0,0);
    if (*(int *)(param_1 + 0x748) == 6) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x5444))(piVar13);
    }
    else {
      local_44 = 0;
    }
    break;
  case 0x9a:
    puVar6 = (uint *)(param_1 + 0xd78);
    if (uVar20 == 0) {
      uVar20 = *puVar6 & 0xffffdfff;
    }
    else {
      uVar20 = *puVar6 | 0x2000;
    }
    *puVar6 = uVar20;
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,1);
    break;
  case 0x9b:
    local_44 = 0;
    *(bool *)(param_1 + 0xda1) = uVar20 != 0;
    break;
  case 0x9c:
    local_44 = 0;
    *(bool *)(param_1 + 0xda3) = uVar20 != 0;
    break;
  case 0x9d:
    local_44 = 0;
    *(bool *)(param_1 + 0xda4) = uVar20 != 0;
    break;
  case 0x9e:
    if (((*(uint *)(iVar23 + 0x98) & 0x40000000) == 0) ||
       ((*(uint *)(iVar23 + 0x9c) & 0x800000) == 0)) {
      *(byte *)(iVar23 + 0x4df8) = bVar4;
      *(byte *)(iVar23 + 0xbe1) = bVar4;
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x9e,uVar20,0,0);
      break;
    }
    goto switchD_ram_000416d4_caseD_b;
  case 0xa0:
    ieee80211_param_to_qdrv(piVar13,0xa0,uVar20,0,0);
    iVar24 = *(int *)(iVar23 + 8);
    while (iVar24 != 0) {
      iVar25 = *(int *)(iVar24 + 0xf8);
      if (uVar20 == 0) {
        uVar11 = *(uint *)(iVar24 + 0xa78) & 0xffffbfff;
        uVar15 = *(uint *)(iVar24 + 0xa7c) & 0xffffff7f;
      }
      else {
        uVar11 = *(uint *)(iVar24 + 0xa78) | 0x4000;
        uVar15 = *(uint *)(iVar24 + 0xa7c) | 0x80;
      }
      *(uint *)(iVar24 + 0xa78) = uVar11;
      *(uint *)(iVar24 + 0xa7c) = uVar15;
      if ((*(int *)(iVar24 + 0x448) == 6) && (*(int *)(iVar24 + 0x44c) == 4)) {
        (**(code **)(iVar23 + 0x5444))(iVar24);
      }
      ieee80211_wireless_reassoc(iVar24,0,1);
      iVar24 = iVar25;
    }
    goto LAB_ram_00046d0c;
  case 0xab:
    if ((uVar20 & 4) == 0) {
      uVar11 = *(uint *)(iVar23 + 0xa4) | 0x80000000;
    }
    else {
      uVar11 = *(uint *)(iVar23 + 0xa4) & 0x7fffffff;
    }
    *(uint *)(iVar23 + 0xa4) = uVar11;
    ieee80211_param_to_qdrv(piVar13,0xab,uVar20,0,0);
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,1);
    break;
  case 0xac:
  case 0x10d:
    iVar24 = *(int *)(param_1 + 0x404);
    if (uVar20 - 1 < 4) {
      iVar23 = ieee80211_swfeat_is_supported(5,0);
      if ((iVar23 == 0) && (iVar23 = ieee80211_swfeat_is_supported(6,0), iVar23 == 0)) {
        iVar23 = ieee80211_swfeat_is_supported(7,0);
        if (iVar23 != 0) {
          iVar23 = -3;
          bVar29 = uVar20 == 3;
          bVar30 = SBORROW4(uVar20,3);
          goto LAB_ram_00042cf0;
        }
      }
      else {
        iVar23 = -2;
        bVar29 = uVar20 == 2;
        bVar30 = SBORROW4(uVar20,2);
LAB_ram_00042cf0:
        if (!bVar29 && bVar30 == (int)(uVar20 + iVar23) < 0) goto LAB_ram_00042d60;
      }
      if (iVar25 == 0xac) {
        *(uint *)(iVar24 + 0xac) = uVar20;
      }
      else {
        if (iVar25 != 0x10d) goto LAB_ram_00042d60;
        *(byte *)(iVar24 + 0x54d) = bVar4 - 1;
        *(byte *)(iVar24 + 0x578) = bVar4 - 1;
        *(uint *)(iVar24 + 0xb0) = uVar20;
      }
      ieee80211_param_to_qdrv(piVar13,iVar25,uVar20,0,0);
      local_44 = 0;
      ieee80211_wireless_reassoc(piVar13,0,1);
    }
    else {
LAB_ram_00042d60:
      local_44 = 0x16;
    }
    break;
  case 0xad:
    if ((*(int *)(param_1 + 0x748) == 6) || ((*(uint *)(iVar23 + 0x9c) & 0x40000) != 0)) {
      if (uVar20 < 0x67) {
        iVar25 = 0x20;
        *(ushort *)(iVar23 + 0x5068) = uVar9;
        iVar24 = 0;
        _DAT_auxregs_0008 = 0x4435e;
        _DAT_auxregs_000c = 0x4437c;
        do {
          iVar22 = iVar24 * 6;
          uVar9 = *(ushort *)(iVar23 + 0x5068);
          iVar24 = iVar24 + 1;
          iVar22 = iVar22 + iVar23;
          *(undefined2 *)(iVar22 + 0x5076) = 0;
          *(ushort *)(iVar22 + 0x5074) = uVar9;
          iVar25 = iVar25 + -1;
        } while (iVar25 != 0);
        for (puVar18 = *(undefined4 **)(iVar23 + 0x4e60); puVar18 != (undefined4 *)0x0;
            puVar18 = (undefined4 *)puVar18[3]) {
          ieee80211_wireless_reassoc(*puVar18,0,0);
        }
        local_44 = 0;
      }
      else {
        local_44 = 0x16;
        (*(code *)&printk)("%s: Max configurable limit is %d\n","ieee80211_ioctl_set_assoc_limit",
                           0x66);
      }
    }
    else {
      local_44 = 0x16;
    }
    break;
  case 0xae:
    local_44 = 0;
    *(byte *)(iVar23 + 0x4f5) = bVar4;
    break;
  case 0xb4:
    (*(code *)&memset)(iVar23 + 0x4a50,0,0xc0);
    iVar25 = 0x20;
    iVar24 = 0;
    _DAT_auxregs_0008 = 0x44462;
    _DAT_auxregs_000c = 0x44474;
    do {
      *(undefined4 *)(iVar23 + iVar24 * 4 + 0x4b10) = 0;
      iVar24 = iVar24 + 1;
      iVar25 = iVar25 + -1;
    } while (iVar25 != 0);
    goto LAB_ram_00046d0c;
  case 0xb5:
    local_44 = 0;
    (*(code *)&memset)(iVar23 + 0x3ffc,0,0x248);
    break;
  case 0xb8:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    (*(code *)&br_set_ap_isolate)(uVar20);
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0xb8,uVar20,0,0);
    break;
  case 0xb9:
    *(uint *)(_soc_shared_params + 0x48) = uVar20;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0xb9,uVar20,0,0);
    break;
  case 0xbc:
    if (((*(int *)(param_1 + 0x748) == 6) || ((*(uint *)(iVar23 + 0x9c) & 0x40000) != 0)) &&
       (uVar11 = uVar20 >> 0x10, uVar11 - 1 < 0x1f)) {
      if (((uVar20 & 0xffff) <= (uint)*(ushort *)(iVar23 + 0x5068)) &&
         (iVar24 = uVar11 * 6 + iVar23, (uint)*(ushort *)(iVar24 + 0x5076) <= (uVar20 & 0xffff))) {
        *(ushort *)(iVar24 + 0x5074) = uVar9;
        for (piVar13 = *(int **)(iVar23 + 0x4e60); piVar13 != (int *)0x0;
            piVar13 = (int *)piVar13[3]) {
          if (*(byte *)(*piVar13 + 0x8f6) == uVar11) {
            ieee80211_wireless_reassoc(*piVar13,0,0);
          }
        }
        local_44 = 0;
        break;
      }
    }
    local_44 = 0x16;
    break;
  case 0xc3:
    *(byte *)(iVar23 + 0x4df7) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0xc3,uVar20,0,0);
    break;
  case 199:
    if (uVar20 == 0) {
      local_44 = 0;
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) & 0xffbfffff;
    }
    else {
      local_44 = 0;
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) | 0x400000;
    }
    break;
  case 0xce:
    if (((*(uint *)(iVar23 + 0x98) & 0x40000000) == 0) ||
       ((*(uint *)(iVar23 + 0x9c) & 0x800000) == 0)) {
      if (3 < uVar20) {
        return -0x16;
      }
      *(byte *)(iVar23 + 0xbe3) = bVar4;
      ieee80211_param_to_qdrv(piVar13,0xce,uVar20,0,0);
      local_44 = 0;
      ieee80211_wireless_reassoc(piVar13,0,1);
      break;
    }
    goto switchD_ram_000416d4_caseD_b;
  case 0xcf:
    if (uVar20 != 0) {
      local_44 = 0;
      *(ushort *)(iVar23 + 0x513c) = uVar9;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0xd0:
    if (uVar20 != 0) {
      local_44 = 0;
      *(ushort *)(iVar23 + 0x513e) = uVar9;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0xd1:
    *(ushort *)(iVar23 + 0x514c) = uVar9;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0xd1,uVar20,0,0);
    break;
  case 0xd2:
    uVar11 = (uVar20 & 0xff0000) >> 0x10;
    uVar20 = (uVar20 & 0xff00) >> 8;
    if (uVar20 < uVar11) {
      local_44 = -0x16;
    }
    else {
      for (iVar24 = 0; iVar24 < *(int *)(iVar23 + 0xbf8); iVar24 = iVar24 + 1) {
        iVar25 = iVar24 * 0x34 + iVar23;
        uVar15 = (uint)*(byte *)(iVar25 + 0xc04);
        if ((uVar11 <= uVar15) && (uVar15 <= uVar20)) {
          *(byte *)(iVar25 + 0xc05) = bVar4;
        }
      }
      local_44 = 0;
    }
    break;
  case 0xd4:
    local_44 = 0;
    *(uint *)(iVar23 + 0x52d0) = uVar20;
    break;
  case 0xd7:
    *(byte *)(iVar23 + 0xbe7) = bVar4;
    ieee80211_param_to_qdrv(piVar13,0xd7,uVar20,0,0);
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,1);
    break;
  case 0xd9:
    if (((*(uint *)(iVar23 + 0x98) & 0x40000000) != 0) &&
       ((*(uint *)(iVar23 + 0x9c) & 0x800000) != 0)) {
      local_44 = ieee80211_tpc_query_config_interval(iVar23 + 0x5a84,uVar20);
      goto LAB_ram_00046c3a;
    }
    goto switchD_ram_000416d4_caseD_b;
  case 0xda:
    if (((*(uint *)(iVar23 + 0x98) & 0x40000000) != 0) &&
       ((*(uint *)(iVar23 + 0x9c) & 0x800000) != 0)) {
      if (uVar20 == 0) {
        local_44 = 0;
        ieee80211_tpc_query_stop();
      }
      else {
        local_44 = 0;
        ieee80211_tpc_query_start(iVar23 + 0x5a84);
      }
      break;
    }
    goto switchD_ram_000416d4_caseD_b;
  case 0xdb:
    if ((*(uint *)(iVar23 + 0x98) & 0x40000000) == 0) goto switchD_ram_000416d4_caseD_b;
    uVar20 = (uint)(uVar20 != 0);
    if (uVar20 != (*(uint *)(iVar23 + 0x9c) >> 0x17 & 1)) {
      pcVar7 = (char *)(iVar23 + 0xbe1);
      if (uVar20 == 0) {
        (*(code *)&printk)("Disable tpc feature\n");
        *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) & 0xff7fffff;
        ieee80211_tpc_query_stop(iVar23 + 0x5a84);
        cVar3 = *(char *)(iVar23 + 0xbe2);
        *pcVar7 = cVar3;
        if (cVar3 != '\0') {
          ieee80211_param_to_qdrv(piVar13,0x9e,1,0,0);
        }
      }
      else {
        (*(code *)&printk)("Enable tpc feature\n");
        *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) | 0x800000;
        *(char *)(iVar23 + 0xbe2) = *pcVar7;
        if (*pcVar7 != '\0') {
          ieee80211_param_to_qdrv(piVar13,0x9e,0,0,0);
          *pcVar7 = '\0';
        }
      }
      for (iVar24 = *(int *)(iVar23 + 8); iVar24 != 0; iVar24 = *(int *)(iVar24 + 0xf8)) {
        if ((*(int *)(iVar24 + 0x448) == 6) && (*(int *)(iVar24 + 0x44c) == 4)) {
          if (uVar20 == 0) {
            ieee80211_ppqueue_remove_with_cat_action(iVar24 + 0xaa8,0,3);
          }
          (**(code **)(iVar23 + 0x5444))(piVar13);
        }
      }
    }
    goto LAB_ram_00046d0c;
  case 0xdd:
    if (uVar20 != *(uint *)(param_1 + 0x81c)) {
      if (uVar20 < 0x10001) {
        *(uint *)(param_1 + 0x81c) = uVar20;
        local_44 = 0;
        ieee80211_param_to_qdrv(piVar13,0xdd,uVar20,0,0);
        break;
      }
      goto LAB_ram_00046d30;
    }
    goto LAB_ram_00046d0c;
  case 0xe3:
    ieee80211_param_to_qdrv(piVar13,0xe3,uVar20,0,0);
    local_44 = 0;
    *(uint *)(param_1 + 0xdfc) = uVar20;
    break;
  case 0xe4:
    if (((*(int *)(iVar23 + 0x14) - 0xbU < 4) ||
        ((*(int *)(iVar23 + 0x14) - 8U < 2 && ((*(uint *)(iVar23 + 0x9c) & 0x40000000) != 0)))) &&
       (iVar24 = FUN_ram_0002b0e4(uVar20,0x28), -1 < iVar24)) {
      ieee80211_param_to_qdrv(piVar13,0xe4,iVar24,0,0);
      local_44 = 0;
      *(uint *)(param_1 + 0xd74) = uVar20 & 0xffffff | 0x7e000000;
      break;
    }
    goto LAB_ram_00046d30;
  case 0xe7:
    if (*(int *)(param_1 + 0x748) == 1) {
      uVar10 = *(undefined4 *)(param_1 + 0x814);
      if (uVar20 == 0) {
        uVar11 = *(uint *)(iVar23 + 0x9c) & 0xffffff7f;
      }
      else {
        uVar11 = *(uint *)(iVar23 + 0x9c) | 0x80;
      }
      *(uint *)(iVar23 + 0x9c) = uVar11;
      if (*(int *)(param_1 + 0x74c) == 4) {
        if (0 < *(short *)(iVar23 + 0x5c2)) {
          pcVar7 = "Enabled";
          if (uVar20 == 0) {
            pcVar7 = "Disabled";
          }
          (*(code *)&printk)("SCS: send qtn DFS report (DFS %s)\n",pcVar7);
        }
        ieee80211_send_action_dfs_report(uVar10);
      }
      local_44 = 0;
      (**(code **)(iVar23 + 0x5528))(uVar20);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0xe8:
  case 0x177:
    iVar24 = ieee80211_param_ocac_set(param_1,piVar13,uVar20);
    if (iVar24 == 0) {
      ieee80211_param_to_qdrv(piVar13,iVar25,uVar20,0,0);
      local_44 = 0;
      *(uint *)(&DAT_ram_00004e00 + iVar23) = uVar20;
      break;
    }
    goto LAB_ram_00046d30;
  case 0xeb:
    local_44 = 0;
    *(uint *)(iVar23 + 0x20) = uVar20;
    break;
  case 0xec:
    iVar24 = ieee80211_tdls_cfg_disc_int(piVar13,uVar20);
    if (iVar24 != 0) goto LAB_ram_00046d30;
    goto LAB_ram_00046d0c;
  case 0xed:
    local_44 = 0;
    *(byte *)(param_1 + 0x59d) = bVar4;
    break;
  case 0xf1:
    if (*(int *)(iVar23 + 0x24) != 6) goto LAB_ram_00044eaa;
    uVar11 = uVar20 & 0xffff;
    iVar24 = findchannel_any(iVar23,uVar11,*(undefined2 *)(iVar23 + 0x4e30));
    if ((iVar24 == 0) || (iVar24 == 0xffff)) {
      local_44 = -0x16;
      break;
    }
    uVar15 = uVar20 >> 0x18;
    if ((uVar20 >> 0x10 & 0xff) == 0) {
      bVar4 = bVar4 & 7;
      iVar25 = (uVar11 >> 3) + iVar23;
      pbVar27 = (byte *)(iVar25 + 0x4cd0);
      bVar8 = *pbVar27;
      if ((((int)(uint)bVar8 >> bVar4 & 1U) == 0) ||
         ((((int)(uint)*(byte *)(iVar25 + 0x4d10) >> bVar4 ^ uVar20 >> 0x1a) & 1) != 0)) {
        pbVar21 = (byte *)(iVar25 + 0x4cf0);
        if ((uVar15 & 2) == 0) {
          if (((int)(uint)*pbVar21 >> bVar4 & 1U) == 0) {
            *pbVar27 = (byte)(1 << bVar4) | bVar8;
            (*(code *)&printk)("channel %u is added into non-primary channel list\n",uVar11);
            goto LAB_ram_00044c4a;
          }
        }
        else {
          bVar17 = (byte)(1 << bVar4);
          *pbVar27 = bVar17 | bVar8;
          (*(code *)&printk)("channel %u is added into non-primary channel list\n",uVar11);
          *pbVar21 = bVar17 | *pbVar21;
LAB_ram_00044c4a:
          iVar25 = (uVar11 >> 3) + iVar23;
          if ((uVar15 & 4) == 0) {
            pbVar27 = (byte *)(iVar25 + 0x4d10);
            *pbVar27 = *pbVar27 & ~(byte)(1 << bVar4);
          }
          else {
            pbVar27 = (byte *)(iVar25 + 0x4d10);
            *pbVar27 = (byte)(1 << bVar4) | *pbVar27;
            (*(code *)&printk)("channel %u is non-primary for auto channel selection only\n",uVar11)
            ;
          }
        }
        iVar25 = ieee80211_get_bw(iVar23);
        if (0x27 < iVar25) {
          uVar20 = ieee80211_find_sec_chan(iVar24);
          if (uVar20 == 0) {
            uVar20 = 1;
          }
          else {
            uVar11 = uVar20 & 0x80000007;
            if ((int)uVar20 < 0) {
              uVar20 = uVar20 + 7;
            }
            bVar29 = (int)uVar11 < 0;
            if (bVar29) {
              uVar11 = uVar11 - 1;
            }
            bVar4 = (byte)uVar11;
            if (bVar29) {
              bVar4 = (bVar4 | 0xf8) + 1;
            }
            uVar20 = (int)(uint)*(byte *)(((int)uVar20 >> 3) + iVar23 + 0x4cd0) >> (bVar4 & 0x1f) &
                     1;
          }
          uVar11 = uVar20;
          if (0x4f < iVar25) {
            uVar15 = ieee80211_find_sec40u_chan(iVar24);
            if (uVar15 != 0) {
              uVar11 = uVar15 & 0x80000007;
              if ((int)uVar15 < 0) {
                uVar15 = uVar15 + 7;
              }
              bVar29 = (int)uVar11 < 0;
              if (bVar29) {
                uVar11 = uVar11 - 1;
              }
              bVar4 = (byte)uVar11;
              if (bVar29) {
                bVar4 = (bVar4 | 0xf8) + 1;
              }
              uVar11 = 0;
              if (((int)(uint)*(byte *)(((int)uVar15 >> 3) + iVar23 + 0x4cd0) >> (bVar4 & 0x1f) & 1U
                  ) != 0) {
                uVar11 = uVar20;
              }
            }
            uVar20 = ieee80211_find_sec40l_chan(iVar24);
            if (uVar20 != 0) {
              uVar15 = uVar20 & 0x80000007;
              if ((int)uVar20 < 0) {
                uVar20 = uVar20 + 7;
              }
              bVar29 = (int)uVar15 < 0;
              if (bVar29) {
                uVar15 = uVar15 - 1;
              }
              bVar4 = (byte)uVar15;
              if (bVar29) {
                bVar4 = (bVar4 | 0xf8) + 1;
              }
              if (((int)(uint)*(byte *)(((int)uVar20 >> 3) + iVar23 + 0x4cd0) >> (bVar4 & 0x1f) & 1U
                  ) == 0) goto LAB_ram_00044dc8;
            }
          }
          if (uVar11 != 0) {
            (*(code *)&printk)("Warning: all the sub channels are not in primary channel list!\n");
          }
        }
LAB_ram_00044dc8:
        if ((*(int *)(iVar23 + 0x4e0c) == 0xffff) ||
           (bVar4 = *(byte *)(*(int *)(iVar23 + 0x4e0c) + 8),
           ((int)(uint)*(byte *)((uint)(bVar4 >> 3) + iVar23 + 0x4cd0) >> (bVar4 & 7) & 1U) == 0)) {
          bVar29 = false;
        }
        else {
          (*(code *)&printk)("current channel is %d, cannot be used as primary channel\n");
          bVar29 = true;
        }
        iVar24 = *(int *)(iVar23 + 0x4e1c);
        if ((iVar24 == 0xffff) ||
           (bVar4 = *(byte *)(iVar24 + 8),
           ((int)(uint)*(byte *)((uint)(bVar4 >> 3) + iVar23 + 0x4cd0) >> (bVar4 & 7) & 1U) == 0)) {
          if (!bVar29) goto LAB_ram_00044eaa;
        }
        else {
          (*(code *)&printk)("Channel %d cannot be used as desired channel\n");
          *(int *)(iVar23 + 0x4e1c) = 0xffff;
        }
        if (((*(uint *)(*piVar13 + 0xe4) & 0x41) == 0x41) &&
           (*(int *)(*(int *)(param_1 + 0x404) + 0x4e50) == 1)) {
          local_44 = 0;
          ieee80211_new_state(piVar13,1,0);
          break;
        }
      }
    }
    else {
      bVar4 = bVar4 & 7;
      iVar23 = (uVar11 >> 3) + iVar23;
      pbVar27 = (byte *)(iVar23 + 0x4cd0);
      bVar8 = *pbVar27;
      if (((int)(uint)bVar8 >> bVar4 & 1U) != 0) {
        pbVar21 = (byte *)(iVar23 + 0x4cf0);
        if ((uVar15 & 2) != 0) {
          bVar4 = (byte)(1 << bVar4);
          *pbVar27 = bVar8 & ~bVar4;
          (*(code *)&printk)("channel %u is removed from non-primary channel list\n",uVar11);
          local_44 = 0;
          *pbVar21 = bVar4 | *pbVar21;
          break;
        }
        if (((int)(uint)*pbVar21 >> bVar4 & 1U) == 0) {
          *pbVar27 = bVar8 & ~(byte)(1 << bVar4);
          local_44 = 0;
          (*(code *)&printk)("channel %u is removed from non-primary channel list\n",uVar11);
          break;
        }
      }
    }
LAB_ram_00044eaa:
    local_44 = 0;
    break;
  case 0xf6:
    if (2 < uVar20) goto LAB_ram_00046d30;
    if (*(byte *)(param_1 + 0xf56) != uVar20) {
      *(byte *)(param_1 + 0xf56) = bVar4;
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0xf6,uVar20,0,0);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0xf9:
    puVar6 = (uint *)(param_1 + 0x73c);
    if (uVar20 == 0) {
      local_44 = 0;
      *puVar6 = *puVar6 & 0xefffffff;
    }
    else {
      local_44 = 0;
      *puVar6 = *puVar6 | 0x10000000;
    }
    break;
  case 0xfb:
    iVar24 = ieee80211_find_node_by_node_idx(piVar13,uVar20 & 0xffff);
    if (iVar24 != 0) {
      ieee80211_node_training_start(iVar24,1);
      local_44 = 0;
      ieee80211_free_node(iVar24);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x100:
    iVar24 = ieee80211_swfeat_is_supported(0x16,1);
    if (iVar24 == 0) {
      local_44 = 0x5f;
    }
    else if ((int)uVar20 < 4) {
      *(byte *)(param_1 + 0xe00) = bVar4;
      uVar11 = Auxregs0028;
      if ((uVar11 & 6) != 0) {
        Auxregs0028 = uVar11 & 0xfffffff9;
      }
      ieee80211_adjust_wme_by_vappri(iVar23);
      Auxregs0028 = uVar11;
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x100,uVar20,0,0);
    }
    else {
      local_44 = 0x16;
    }
    break;
  case 0x104:
    uStack_34 = uVar20 & 0xffffff;
    local_2a = 0;
    local_44 = ieee80211_country_string_to_countryid(&uStack_34,&local_2a);
    if (local_44 == 0) {
      *(undefined2 *)(iVar23 + 0x5172) = local_2a;
      ieee80211_build_countryie(iVar23);
      local_44 = 0;
      break;
    }
    local_44 = -local_44;
    goto LAB_ram_00046c3a;
  case 0x107:
    (*(code *)&printk)("Change auto WME param based on VAP priority from %u to %u\n",
                       *(byte *)(iVar23 + 0x505c),uVar20);
    *(byte *)(iVar23 + 0x505c) = bVar4;
    uVar20 = Auxregs0028;
    if ((uVar20 & 6) != 0) {
      Auxregs0028 = uVar20 & 0xfffffff9;
    }
    ieee80211_adjust_wme_by_vappri(iVar23);
    Auxregs0028 = uVar20;
    local_44 = 0;
    break;
  case 0x109:
    if (uVar20 != 1) {
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,iVar25,uVar20,0,0);
      break;
    }
    (*(code *)&printk)("channel   max_pwr   min_pwr   pwr_80M   pwr_40M   pwr_20M\n");
    for (iVar24 = 0; iVar24 < *(int *)(iVar23 + 0xbf8); iVar24 = iVar24 + 1) {
      iVar25 = iVar23 + iVar24 * 0x34 + 0xbfc;
      if (((int)(uint)*(byte *)((uint)(*(byte *)(iVar25 + 8) >> 3) + iVar23 + 0x4bb0) >>
           (*(byte *)(iVar25 + 8) & 7) & 1U) != 0) {
        iVar22 = 0;
        do {
          iVar19 = 0;
          iVar14 = 0;
          do {
            iVar19 = iVar19 + 1;
            iVar14 = iVar14 + iVar22 * 0xc + iVar25;
            (*(code *)&printk)("%7d   %7d   %7d   %7d   %7d   %7d\n",*(undefined1 *)(iVar25 + 8),
                               (int)*(char *)(iVar25 + 10),(int)*(char *)(iVar25 + 0xb),
                               (int)*(char *)(iVar14 + 0x10),(int)*(char *)(iVar14 + 0xf),
                               (int)*(char *)(iVar14 + 0xe));
            iVar14 = iVar19 * 3;
          } while (iVar19 != 4);
          iVar22 = iVar22 + 1;
        } while (iVar22 != 2);
      }
    }
    goto LAB_ram_00046d0c;
  case 0x10a:
    *(byte *)(iVar23 + 0xbf0) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x10a,uVar20,0,0);
    break;
  case 0x10b:
    uVar15 = uVar20 >> 0x14 & 0xf;
    uVar11 = (uVar20 >> 0x10 & 0xf) - 1 & 0xff;
    iVar24 = *(int *)(param_1 + 0x404);
    if (((uVar15 < 2) && (uVar11 < 4)) && (uVar16 = uVar20 >> 8 & 0xf, uVar16 < 3)) {
      for (iVar23 = 0; iVar23 < *(int *)(iVar24 + 0xbf8); iVar23 = iVar23 + 1) {
        if ((uint)*(byte *)(iVar23 * 0x34 + iVar24 + 0xc04) == uVar20 >> 0x18) {
          *(byte *)(iVar23 * 0x34 + uVar15 * 0xc + uVar11 * 3 + uVar16 + iVar24 + 0xc0a) = bVar4;
          if (uVar11 == 0 && uVar15 == 0) {
            iVar25 = ieee80211_get_bw(iVar24);
            if (iVar25 == 0x14) {
              if (uVar16 != 0) {
LAB_ram_000438a2:
                if (iVar25 != 0x50) goto LAB_ram_000438de;
                bVar29 = uVar16 == 2;
                goto LAB_ram_000438ac;
              }
            }
            else {
              if (iVar25 != 0x28) goto LAB_ram_000438a2;
              bVar29 = uVar16 == 1;
LAB_ram_000438ac:
              if (!bVar29) goto LAB_ram_000438de;
            }
            iVar24 = iVar23 * 0x34 + iVar24;
            *(byte *)(iVar24 + 0xc08) = bVar4;
            *(byte *)(iVar24 + 0xc06) = bVar4;
          }
LAB_ram_000438de:
          local_44 = 0;
          ieee80211_param_to_qdrv(piVar13,0x10b,uVar20,0,0);
          goto LAB_ram_00046c86;
        }
      }
    }
    goto LAB_ram_00046d30;
  case 0x10c:
    if (uVar20 == 0) {
      local_44 = 0;
      ieee80211_cancel_scan(piVar13);
    }
    else {
      local_44 = 0;
      ieee80211_cancel_scan_no_wait(piVar13);
    }
    break;
  case 0x10e:
    if ((uVar20 & 0x10) == 0) {
LAB_ram_00042be4:
      *(byte *)(iVar23 + 0xbe6) = bVar4;
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x10e,uVar20,0,0);
    }
    else {
      iVar24 = ieee80211_get_bw(iVar23);
      if (iVar24 == 0) {
        local_44 = 0;
        (*(code *)&printk)("current bw is invalid!\n");
        break;
      }
      uVar11 = uVar20 & 3;
      if (iVar24 == 0x14) {
        if (uVar11 == 0) {
LAB_ram_00042bb6:
          if (iVar24 != 0x50) goto LAB_ram_00042be4;
          iVar25 = -2;
          bVar29 = uVar11 == 2;
          bVar30 = SBORROW4(uVar11,2);
          goto LAB_ram_00042bc0;
        }
      }
      else {
        if (iVar24 != 0x28) goto LAB_ram_00042bb6;
        iVar25 = -1;
        bVar29 = uVar11 == 1;
        bVar30 = SBORROW4(uVar11,1);
LAB_ram_00042bc0:
        if (bVar29 || bVar30 != (int)(uVar11 + iVar25) < 0) goto LAB_ram_00042be4;
      }
      local_44 = 0;
      (*(code *)&printk)("Can\'t set fixed qtn_bw %u because current bw is %u\n",uVar11,iVar24);
    }
    break;
  case 0x110:
    if (*(int *)(param_1 + 0x814) == 0) {
      return -0xe;
    }
    for (iVar24 = 0; iVar24 < (int)uVar20; iVar24 = iVar24 + 1) {
      ieee80211_send_tuning_data(*(int *)(param_1 + 0x814));
    }
    goto LAB_ram_00046d0c;
  case 0x112:
    if (((uVar20 == 0) || (uVar20 == 2)) || (uVar20 == 3)) {
      *(byte *)(param_1 + 0xe01) = bVar4;
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x112,uVar20,0,0);
      break;
    }
    goto LAB_ram_00046d30;
  case 0x116:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    if (uVar20 == 0) {
      uVar11 = *(uint *)(param_1 + 0x2ec) & 0xbfffffff;
    }
    else {
      uVar11 = *(uint *)(param_1 + 0x2ec) | 0x40000000;
    }
    *(uint *)(param_1 + 0x2ec) = uVar11;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x116,uVar20,0,0);
    break;
  case 0x117:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    if (uVar20 == 0) {
      uVar11 = *(uint *)(param_1 + 0x2ec) & 0xdfffffff;
    }
    else {
      uVar11 = *(uint *)(param_1 + 0x2ec) | 0x20000000;
    }
    *(uint *)(param_1 + 0x2ec) = uVar11;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x117,uVar20,0,0);
    break;
  case 0x118:
    if (3 < uVar20 - 1) {
LAB_ram_00046d3c:
      local_44 = -0x16;
      break;
    }
    iVar24 = *(int *)(iVar23 + 8);
    uVar1 = (undefined1)(uVar20 - 1);
    *(undefined1 *)(iVar23 + 0x54c) = uVar1;
    *(undefined1 *)(iVar23 + 0x577) = uVar1;
    for (; iVar24 != 0; iVar24 = *(int *)(iVar24 + 0xf8)) {
      if ((*(int *)(iVar24 + 0x448) == 6) && (*(int *)(iVar24 + 0x44c) == 4)) {
        (**(code **)(iVar23 + 0x5444))(iVar24);
      }
    }
    goto LAB_ram_00046d0c;
  case 0x119:
    iVar24 = *(int *)(param_1 + 0x404);
    uVar11 = uVar20 & 0xffff;
    if (6 < uVar20 >> 0x10) {
      local_44 = 0x16;
      (*(code *)&printk)("<4>%s: WOWLAN set: invalid config cmd %u, arg=%u\n",param_1);
      break;
    }
    switch(uVar20 >> 0x10) {
    case 1:
      if (1 < uVar11) goto LAB_ram_00046d30;
      if (*(ushort *)(iVar24 + 0x5b1c) != uVar11) {
        ieee80211_wireless_set_sta_bmps(piVar13,iVar24,uVar11);
      }
      break;
    case 2:
      if (2 < uVar11) goto LAB_ram_00046d30;
      if (*(ushort *)(iVar24 + 0x5b1e) != uVar11) {
        *(ushort *)(iVar24 + 0x5b1e) = uVar9;
        _g_wowlan_match_type = uVar9;
      }
      break;
    case 3:
      if (uVar11 < 0x600) goto LAB_ram_00046d30;
      if (*(ushort *)(iVar24 + 0x5b20) != uVar11) {
        *(ushort *)(iVar24 + 0x5b20) = uVar9;
        _g_wowlan_l2_ether_type = uVar9;
      }
      break;
    case 4:
      if (*(ushort *)(iVar24 + 0x5b22) != uVar11) {
        *(ushort *)(iVar24 + 0x5b22) = uVar9;
        _g_wowlan_l3_udp_port = uVar9;
      }
    }
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x119,uVar20,0,0);
    break;
  case 0x11a:
    if ((*(uint *)(iVar23 + 0x9c) & 0x40000) != 0) {
      if ((*(uint *)(param_1 + 0x408) & 0x40000000) != 0) {
        local_44 = 1;
        ieee80211_note(piVar13,"%s can\'t config WDS mode since it\'s a repeater\n",*piVar13);
        break;
      }
      goto LAB_ram_00046d18;
    }
    if (*(int *)(param_1 + 0x748) == 2) {
      iVar24 = 1;
      if ((*(ushort *)(param_1 + 0xce0) & 3) != 2) {
        iVar24 = 0;
      }
    }
    else {
      iVar24 = 0;
    }
    puVar12 = (ushort *)(param_1 + 0xce0);
    *puVar12 = uVar9 | *puVar12 & ~(ushort)(uVar20 >> 0x10);
    if (*(int *)(param_1 + 0x748) == 2) {
      iVar25 = 1;
      if ((*puVar12 & 3) != 2) {
        iVar25 = 0;
      }
    }
    else {
      iVar25 = 0;
    }
    if ((*(int *)(param_1 + 0x748) == 2) && (iVar24 != iVar25)) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x5540))(iVar23);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x11b:
    if ((*(uint *)(iVar23 + 0x9c) & 0x40000) == 0) {
      iVar24 = ieee80211_swfeat_is_supported(0xe,1);
      if (iVar24 == 0) {
        local_44 = 0x5f;
        break;
      }
      if (*(byte *)(iVar23 + 0x5c28) != uVar20) {
        *(byte *)(iVar23 + 0x5c28) = bVar4;
        if (2 < (uVar20 & 0xff)) {
          local_44 = 0x16;
          break;
        }
        if ((uVar20 & 0xff) == 1) {
          iVar24 = iVar23 + 0x40;
          pcVar28 = (code *)&memcpy;
        }
        else {
          iVar24 = 0;
          pcVar28 = (code *)&memset;
        }
        (*pcVar28)(iVar23 + 0x5c2f,iVar24,6);
        *(undefined1 *)(iVar23 + 0x5c36) = 0;
        (*(code *)&memset)(iVar23 + 0x5c37,0,0x30);
        if (*(int *)(param_1 + 0x748) == 6) {
          (**(code **)(iVar23 + 0x5444))(piVar13);
        }
        ieee80211_param_to_qdrv(piVar13,0x11b,uVar20,0,0);
        for (iVar24 = *(int *)(iVar23 + 8); iVar24 != 0; iVar24 = *(int *)(iVar24 + 0xf8)) {
          if (*(int *)(iVar24 + 0x448) == 2) {
            ieee80211_vap_wds_mode_change(iVar24);
          }
        }
        if ((*(int *)(param_1 + 0x748) == 6) &&
           (ieee80211_extender_cleanup_wds_link(piVar13), *(char *)(iVar23 + 0x5c28) == '\x02')) {
          local_44 = 0;
          (*(code *)&mod_timer)(iVar23 + 0x5c70,_jiffies);
          break;
        }
      }
      local_44 = 0;
      break;
    }
    if ((*(uint *)(param_1 + 0x408) & 0x40000000) != 0) {
      local_44 = 1;
      ieee80211_note(piVar13,"%s can\'t config extender role since it\'s a repeater\n",*piVar13);
      break;
    }
LAB_ram_00046d18:
    local_44 = 1;
    break;
  case 0x11c:
    iVar24 = ieee80211_swfeat_is_supported(0xe,1);
    if (iVar24 != 0) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5c2b) = bVar4;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x11d:
    iVar24 = ieee80211_swfeat_is_supported(0xe,1);
    if (iVar24 != 0) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5c2c) = bVar4;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x11e:
    iVar24 = ieee80211_swfeat_is_supported(0xe,1);
    if (iVar24 != 0) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5c29) = bVar4;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x11f:
    iVar24 = ieee80211_swfeat_is_supported(0xe,1);
    if (iVar24 != 0) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5c2a) = bVar4;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x120:
    if ((int)uVar20 < 2) {
      *(byte *)(iVar23 + 0x4df9) = bVar4;
      *(byte *)(iVar23 + 0x505d) = bVar4;
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x120,uVar20,0,0);
      break;
    }
    goto LAB_ram_00046d30;
  case 0x122:
    *(uint *)(param_1 + 0x40c) = uVar20;
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,1,0);
    break;
  case 0x125:
    local_44 = 0;
    *(uint *)(param_1 + 0x410) = uVar20;
    break;
  case 0x126:
    local_44 = 0;
    *(uint *)(param_1 + 0x414) = uVar20;
    break;
  case 0x127:
    local_44 = 0;
    *(uint *)(param_1 + 0x41c) = uVar20;
    break;
  case 0x128:
    local_44 = 0;
    *(uint *)(param_1 + 0x418) = uVar20;
    break;
  case 299:
    iVar24 = *(int *)(param_1 + 0x404);
    iVar23 = iVar24 + 0x5afc;
    if (uVar20 == 0) {
      local_44 = 0;
      (*(code *)&del_timer)(iVar23);
    }
    else {
      (*(code *)&init_timer_key)(iVar23,0,0);
      *(code **)(iVar24 + 0x5b0c) = ieee80211_ba_setup_detect_rssi;
      *(int *)(iVar24 + 0x5b10) = iVar24;
      *(int *)(iVar24 + 0x5b04) = _jiffies;
      local_44 = 0;
      (*(code *)&add_timer)(iVar23);
    }
    break;
  case 0x12d:
    local_44 = 0;
    *(uint *)(_soc_shared_params + 0x170) = uVar20;
    break;
  case 0x12e:
    if (*(byte *)(param_1 + 0xf54) != uVar20) {
      *(byte *)(param_1 + 0xf54) = bVar4;
      ieee80211_param_to_qdrv(piVar13,0x12e,uVar20,0,0);
      local_44 = 0;
      ieee80211_wireless_reassoc(piVar13,0,1);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x12f:
    uVar15 = uVar20 & 0xffff;
    uVar11 = uVar11 & 0xff;
    uVar16 = uVar20 >> 0x18;
    if (*(int *)(iVar23 + 0x4e0c) == 0xffff) {
      (*(code *)&printk)("bss channel not set");
      (*(code *)&show_stacktrace)(0,0);
      (*(code *)&printk)("Kernel BUG in file %s: function %s: line %d!\n",
                         "../drivers/wlan/ieee80211_wireless.c","ieee80211_ioctl_setparam",0x38c9);
    }
    if (((uVar15 != 0) && (uVar11 == 0)) && (uVar16 == 0)) {
      uVar15 = 0x13;
      uVar11 = 0x2a;
      uVar16 = 4;
    }
    if ((((*(uint *)(iVar23 + 0x98) & 0x40000000) == 0) || ((*(uint *)(iVar23 + 0x9c) & 2) == 0)) ||
       ((*(uint *)(iVar23 + 0x9c) & 0x800000) != 0)) {
      local_44 = 0x5f;
      (*(code *)&printk)("power constraint override needs to have TPC disabled\n");
      break;
    }
    if (*(int *)(param_1 + 0x748) != 6) goto switchD_ram_000416d4_caseD_b;
    pcVar7 = (char *)(iVar23 + 0x52c4);
    if (uVar20 == 0) {
      if (*pcVar7 != -1) {
        (*(code *)&printk)("PCO Disabled\n");
        *(undefined2 *)(iVar23 + 0x52c0) = 0;
        *(undefined1 *)(iVar23 + 0x52c3) = 0;
        *(undefined1 *)(iVar23 + 0x52c2) = 0;
        FUN_ram_0002f3a0(piVar13);
        (*(code *)&del_timer)(iVar23 + 0x52a4);
        local_44 = 0;
        *(char *)(iVar23 + 0x529c) = *pcVar7;
        *pcVar7 = -1;
        break;
      }
    }
    else if (*pcVar7 == -1) {
      (*(code *)&printk)("pwr-cons=%d rssi-thr=%d sec-off=%d\n",uVar15,-uVar11,uVar16);
      if ((int)uVar15 < (int)*(char *)(*(int *)(iVar23 + 0x4e0c) + 9)) {
        *(char *)(iVar23 + 0x52c2) = (char)uVar11;
        *(short *)(iVar23 + 0x52c0) = (short)uVar15;
        *(char *)(iVar23 + 0x52c3) = (char)uVar16;
        *pcVar7 = *(char *)(iVar23 + 0x529c);
        (*(code *)&init_timer_key)(iVar23 + 0x52a4,0,0);
        *(int **)(iVar23 + 0x52b8) = piVar13;
        *(code **)(iVar23 + 0x52b4) = FUN_ram_0002f3a0;
        *(int *)(iVar23 + 0x52ac) = _jiffies + 1000;
        local_44 = 0;
        (*(code *)&add_timer)(iVar23 + 0x52a4);
      }
      else {
        local_44 = 0x16;
        (*(code *)&printk)("power constraint(%d) >= current channel max regulatory power(%d)\n",
                           uVar15);
      }
      break;
    }
    goto LAB_ram_00046d30;
  case 0x130:
    *(uint *)(iVar23 + 0x5348) = uVar20;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x130,uVar20,0,0);
    break;
  case 0x131:
    *(uint *)(iVar23 + 0x534c) = uVar20;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x131,uVar20,0,0);
    break;
  case 0x133:
    if (*(int *)(param_1 + 0x748) != 6) goto switchD_ram_000416d4_caseD_b;
    if (uVar20 - 1 < 100) {
      *(uint *)(iVar23 + 0x5c98) = uVar20;
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x133,uVar20,0,0);
      break;
    }
    goto LAB_ram_00046d30;
  case 0x134:
    if (uVar20 != *(uint *)(iVar23 + 0x5c9c)) {
      *(uint *)(iVar23 + 0x5c9c) = uVar20;
      local_44 = 0;
      ieee80211_scan_flush(iVar23);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x137:
    *(bool *)(param_1 + 0x6b0) = uVar20 != 0;
    if (uVar20 != 0) {
      local_44 = 0;
      ieee80211_tdls_free_peer_ps_info(piVar13);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x139:
    *(uint *)(param_1 + 0x6b8) = uVar20;
    if ((*(uint *)(param_1 + 0x73c) & 0x9000000) == 0) {
      local_44 = 0;
      ieee80211_tdls_update_link_timeout(piVar13);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x13a:
    local_44 = 0;
    *(uint *)(param_1 + 0x6bc) = uVar20;
    break;
  case 0x13b:
    local_44 = 0;
    *(uint *)(param_1 + 0x6c0) = uVar20;
    break;
  case 0x13c:
    local_44 = 0;
    *(uint *)(param_1 + 0x6c4) = uVar20;
    break;
  case 0x13d:
    if (*(int *)(param_1 + 0x814) == 0) {
      return -0x16;
    }
    *(uint *)(param_1 + 0x6c8) = uVar20;
    local_44 = 0;
    (**(code **)(iVar23 + 0x54c4))(*(int *)(param_1 + 0x814),6,uVar20);
    break;
  case 0x13e:
    local_44 = 0;
    *(uint *)(param_1 + 0x6cc) = uVar20;
    break;
  case 0x13f:
    local_44 = 0;
    *(uint *)(param_1 + 0x6d0) = uVar20;
    break;
  case 0x140:
    local_44 = 0;
    *(uint *)(param_1 + 0x6d4) = uVar20;
    break;
  case 0x141:
    if (*(byte *)(param_1 + 0x59c) != uVar20) {
      *(byte *)(param_1 + 0x59c) = bVar4;
      local_44 = 0;
      ieee80211_tdls_update_uapsd_indicication_windows(piVar13);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x143:
    if (uVar20 == 2) {
      local_44 = 0;
      *(uint *)(param_1 + 0x73c) = *(uint *)(param_1 + 0x73c) & 0xfdffffff | 0x4000000;
    }
    else {
      puVar6 = (uint *)(param_1 + 0x73c);
      if (uVar20 == 1) {
        local_44 = 0;
        *puVar6 = *puVar6 & 0xfbffffff | 0x2000000;
      }
      else {
        local_44 = 0;
        *puVar6 = *puVar6 & 0xf9ffffff;
      }
    }
    break;
  case 0x144:
    local_44 = 0;
    *(byte *)(param_1 + 0x6f8) = bVar4;
    break;
  case 0x145:
    if (((uVar20 == 0) || (uVar20 == 0x14)) ||
       ((uVar20 == 0x28 || ((uVar20 == 0x50 || (uVar20 == 0xa0)))))) {
      local_44 = 0;
      *(byte *)(param_1 + 0x6f9) = bVar4;
      break;
    }
    goto LAB_ram_00046d30;
  case 0x146:
    *(uint *)(param_1 + 0x598) = uVar20;
    if ((*(uint *)(param_1 + 0x73c) & 0x9000000) == 0) {
      local_44 = 0;
      ieee80211_tdls_start_node_expire_timer(piVar13);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x147:
    local_44 = 0;
    ieee80211_node_dbgref_history_dump();
    break;
  case 0x149:
    if ((int)uVar20 < 0x19) {
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x149,uVar20,0,0);
    }
    else {
      local_44 = 0;
      (*(code *)&printk)("%s: feature id %d is invalid\n","ieee80211_ioctl_swfeat_disable",uVar20);
    }
    break;
  case 0x14a:
    if (uVar20 == 0) {
      local_44 = 0;
      *(uint *)(iVar23 + 0xa0) = *(uint *)(iVar23 + 0xa0) & 0xffffdfff;
    }
    else {
      local_44 = 0;
      *(uint *)(iVar23 + 0xa0) = *(uint *)(iVar23 + 0xa0) | 0x2000;
    }
    break;
  case 0x14e:
    iVar24 = ieee80211_swfeat_is_supported(0xe,1);
    if (iVar24 != 0) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5c2d) = bVar4;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x14f:
    if (uVar20 != 0) {
      local_44 = 0;
      ieee80211_scan_flush(iVar23);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x150:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5c8c) = (uint)(uVar20 != 0);
    break;
  case 0x152:
    local_44 = 0;
    ieee80211_send_vht_opmode_to_all(iVar23,uVar20 & 0xff);
    break;
  case 0x153:
    iVar24 = ieee80211_swfeat_is_supported(9,1);
    if (iVar24 == 0) goto switchD_ram_000416d4_caseD_b;
    if (*(int *)(param_1 + 0x748) == 6) {
      local_44 = 0;
      *(byte *)(param_1 + 0xf5f) = bVar4;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x154:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    local_44 = 0;
    *(bool *)(param_1 + 0xf60) = uVar20 != 0;
    break;
  case 0x155:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    local_44 = 0;
    *(bool *)(param_1 + 0xf61) = uVar20 != 0;
    break;
  case 0x159:
    *(undefined4 *)(_soc_shared_params + 0x17c) = 0;
    local_44 = 0;
    (*(code *)&printk)(
                      "pm_corrupt_debug can\'t be activated when QTN_HAL_PM_CORRUPT_DEBUG is undefined\n"
                      );
    break;
  case 0x15b:
    uVar11 = FUN_ram_0002b0e4(uVar20 & 0xff,0x50);
    iVar24 = FUN_ram_0002b0e4((int)uVar20 >> 8 & 0xff,0x50);
    iVar25 = ieee80211_swfeat_is_supported(0x11,1);
    if (iVar25 == 0) {
      local_44 = 0x5f;
    }
    else if (((*(int *)(iVar23 + 0x14) - 0xbU < 4) && (-1 < (int)uVar11)) && (-1 < iVar24)) {
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x15b,iVar24 << 8 | uVar11,0,0);
    }
    else {
      local_44 = 0x16;
    }
    break;
  case 0x15c:
    *(ushort *)(iVar23 + 0x5358) = uVar9;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x15c,uVar20,0,0);
    break;
  case 0x15d:
    iVar24 = ieee80211_swfeat_is_supported(0x11,1);
    if (iVar24 == 0) {
      return -0x5f;
    }
    if (*(int *)(param_1 + 0x748) == 1) {
      if (_soc_shared_params == 0) {
        return -0x5f;
      }
      if (*(int *)(_soc_shared_params + 0x38) < 6) {
        return -0x5f;
      }
    }
    puVar6 = (uint *)(iVar23 + 0x568);
    uVar11 = *puVar6;
    *(undefined1 *)(iVar23 + 0x567) = *(undefined1 *)(iVar23 + 0x567);
    *puVar6 = *puVar6 & 0xff000000 | (uVar11 << 8 & 0xffe7ffff) >> 8;
    *(byte *)(iVar23 + 0x535a) = bVar4;
    if ((uVar20 & 0xff) == 0) {
      if (*(char *)(iVar23 + 0x589) != -1) {
        *(char *)(iVar23 + 0x577) = *(char *)(iVar23 + 0x589);
      }
      if (*(char *)(iVar23 + 0x55e) != -1) {
        *(char *)(iVar23 + 0x54c) = *(char *)(iVar23 + 0x55e);
      }
    }
    else {
      iVar24 = *(int *)(param_1 + 0x748);
      if (iVar24 == 6) {
        uVar20 = 0x80000;
      }
      else if (iVar24 == 1) {
        uVar20 = 0x100000;
      }
      else {
        uVar20 = uVar11 << 8 & 0x180000;
      }
      uVar1 = *(undefined1 *)(iVar23 + 0x577);
      *(undefined1 *)(iVar23 + 0x577) = 3;
      *(undefined1 *)(iVar23 + 0x589) = uVar1;
      puVar6 = (uint *)(iVar23 + 0x568);
      *(undefined1 *)(iVar23 + 0x567) = *(undefined1 *)(iVar23 + 0x567);
      *puVar6 = *puVar6 & 0xff000000 | (uVar20 | *puVar6 << 8) >> 8;
      uVar1 = *(undefined1 *)(iVar23 + 0x54c);
      *(undefined1 *)(iVar23 + 0x54c) = 3;
      *(undefined1 *)(iVar23 + 0x55e) = uVar1;
    }
    ieee80211_param_to_qdrv(piVar13,0x15d,*(undefined1 *)(iVar23 + 0x535a),0,0);
    iVar24 = *(int *)(iVar23 + 8);
    while (iVar24 != 0) {
      iVar23 = *(int *)(iVar24 + 0xf8);
      ieee80211_wireless_reassoc(iVar24,0,1);
      iVar24 = iVar23;
    }
    goto LAB_ram_00046d0c;
  case 0x163:
    local_44 = 0;
    *(uint *)(*(int *)(_soc_shared_params + 0x50) + 0xe14) = uVar20;
    break;
  case 0x164:
    local_44 = 0;
    *(uint *)(*(int *)(_soc_shared_params + 0x50) + 0xe18) = uVar20;
    break;
  case 0x16b:
    if ((0 < (int)uVar20) && (uVar20 != *(uint *)(iVar23 + 0x5ca0))) {
      *(uint *)(iVar23 + 0x5ca0) = uVar20;
      local_44 = 0;
      (*(code *)&mod_timer)(iVar23 + 0x5ca4,uVar20 * 200 + _jiffies);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x16c:
    local_44 = 0;
    *(uint *)(param_1 + 0x6b4) = uVar20;
    break;
  case 0x16d:
    if (2 < (int)uVar20) {
      return -0x16;
    }
    local_44 = 0;
    *(uint *)(*(int *)(_soc_shared_params + 0x50) + 0x1194) = uVar20;
    break;
  case 0x16e:
    if (2 < (int)uVar20) {
      return -0x16;
    }
    local_44 = 0;
    *(uint *)(*(int *)(_soc_shared_params + 0x50) + 0x1190) = uVar20;
    break;
  case 0x172:
    uVar10 = 3;
LAB_ram_000437ea:
    iVar24 = FUN_ram_00033bfc(piVar13,uVar20,uVar10);
    if (iVar24 == 0) {
      local_44 = 0;
      break;
    }
    goto LAB_ram_00046d30;
  case 0x173:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    puVar18 = (undefined4 *)(iVar23 + 0x5454);
    goto LAB_ram_00045ae0;
  case 0x174:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    puVar18 = (undefined4 *)(iVar23 + 0x5458);
LAB_ram_00045ae0:
    iVar24 = (*(code *)*puVar18)(piVar13,uVar20);
    return iVar24;
  case 0x178:
    local_44 = 0;
    *(uint *)(*(int *)(_soc_shared_params + 0x50) + 0x1198) = uVar20;
    break;
  case 0x17a:
    if (-1 < (int)uVar20) {
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x17a,uVar20,0,0);
      break;
    }
LAB_ram_00046d30:
    local_44 = 0x16;
    break;
  case 0x17b:
    local_44 = 0;
    (*(code *)&topaz_congest_set_unicast_queue_count)(uVar20);
    break;
  case 0x17c:
    ieee80211_param_to_qdrv(piVar13,0x17c,uVar20,0,0);
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,1);
    break;
  case 0x17d:
    local_44 = 0;
    *(byte *)(_soc_shared_params + 7) = bVar4;
    break;
  case 0x17e:
    if (*(int *)(iVar23 + 0x24) == 1) {
      pbVar27 = (byte *)(param_1 + 0xf62);
      if (uVar20 == 0) {
        local_44 = 0;
        *pbVar27 = *pbVar27 & 0xf7;
      }
      else {
        *pbVar27 = *pbVar27 | 8;
        local_44 = 0;
        ieee80211_send_20_40_bss_coex(piVar13);
      }
      break;
    }
    goto switchD_ram_000416d4_caseD_b;
  case 0x17f:
    if (*(int *)(param_1 + 0x748) == 6) {
      *(ushort *)(iVar23 + 0x5cec) = uVar9;
      local_44 = 0;
      (**(code **)(iVar23 + 0x5444))(piVar13);
      break;
    }
    goto switchD_ram_000416d4_caseD_b;
  case 0x180:
    local_44 = 0;
    *(uint *)(param_1 + 0x3f4) = uVar20;
    break;
  case 0x181:
    if ((((uVar20 != 0xa0) && (uVar20 != 0x50)) && (uVar20 != 0x28)) && (uVar20 != 0x14)) {
      return -0x16;
    }
    if ((*(char *)(iVar23 + 0xbf5) == '\x02') && (*(int *)(iVar23 + 0x24) == 1)) {
      *(uint *)(param_1 + 0x3cc) = uVar20;
    }
    iVar24 = *(int *)(iVar23 + 0x14);
    if ((iVar24 - 2U < 2) || (iVar24 - 8U < 2)) {
      if ((0x27 < (int)uVar20) && (iVar24 == 3)) {
        return -0x5f;
      }
      *(byte *)(iVar23 + 0x4df1) = bVar4;
      ieee80211_change_bw(piVar13,uVar20,1);
      ieee80211_start_obss_scan_timer(piVar13);
      *(undefined4 *)(iVar23 + 0x4a44) = 4;
      if (((*(uint *)(*piVar13 + 0xe4) & 0x41) == 0x41) &&
         (*(int *)(*(int *)(param_1 + 0x404) + 0x4e50) == 1)) {
        ieee80211_wireless_reassoc(piVar13,0);
        local_44 = 0;
        ieee80211_new_state(piVar13,1,0);
        break;
      }
    }
    goto LAB_ram_00046d0c;
  case 0x182:
    if (*(short *)(iVar23 + 0x5170) == 0x348) {
      return -0x5f;
    }
    iVar24 = *(int *)(iVar23 + 8);
    while (iVar24 != 0) {
      iVar25 = *(int *)(iVar24 + 0xf8);
      *(byte *)(iVar24 + 0xc63) = bVar4;
      if ((*(int *)(iVar24 + 0x448) == 6) && (*(int *)(iVar24 + 0x44c) == 4)) {
        (**(code **)(iVar23 + 0x5444))(iVar24);
      }
      ieee80211_wireless_reassoc(iVar24,0,1);
      iVar24 = iVar25;
    }
    goto LAB_ram_00046d0c;
  case 0x185:
    if (uVar20 != 0) {
      local_44 = 0;
      *(ushort *)(iVar23 + 0x5140) = uVar9;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x186:
    if (uVar20 != 0) {
      local_44 = 0;
      *(ushort *)(iVar23 + 0x5142) = uVar9;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x187:
    if (uVar20 != 0) {
      local_44 = 0;
      *(ushort *)(iVar23 + 0x5144) = uVar9;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x188:
    if (uVar20 != 0) {
      local_44 = 0;
      *(ushort *)(iVar23 + 0x5146) = uVar9;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x189:
    local_44 = 0;
    *(ushort *)(iVar23 + 0x5148) = uVar9;
    break;
  case 0x18a:
    local_44 = 0;
    *(ushort *)(iVar23 + 0x514a) = uVar9;
    break;
  case 0x18b:
    if ((*(uint *)(param_1 + 0x408) & 0x40000000) != 0) {
      ieee80211_note(piVar13,"%s block state is from %d to %d\n",*piVar13,
                     *(undefined1 *)(param_1 + 0xf64),uVar20);
    }
    local_44 = 0;
    *(bool *)(param_1 + 0xf64) = uVar20 != 0;
    break;
  case 0x18c:
    if (uVar20 == 0) {
      uVar11 = *(uint *)(iVar23 + 0x9c) & 0xbfffffff;
    }
    else {
      uVar11 = *(uint *)(iVar23 + 0x9c) | 0x40000000;
    }
    *(uint *)(iVar23 + 0x9c) = uVar11;
    ieee80211_param_to_qdrv(piVar13,0x18c,uVar20,0,0);
    for (iVar24 = *(int *)(iVar23 + 8); iVar24 != 0; iVar24 = *(int *)(iVar24 + 0xf8)) {
      ieee80211_wireless_reassoc(iVar24,0,0);
    }
    goto LAB_ram_00046d0c;
  case 0x18e:
    if ((*(int *)(param_1 + 0x748) == 6) &&
       (bVar29 = uVar20 != 0, (bool)*(char *)(iVar23 + 0xbf2) != bVar29)) {
      if (*(int *)(iVar23 + 0x57d4) < 5) {
        FUN_ram_0002bafc(iVar23);
        *(undefined4 *)(iVar23 + 0x5e0c) = 0x19;
        ieee80211_pm_queue_work_custom(iVar23,10000);
      }
      iVar24 = (**(code **)(iVar23 + 0x5410))(piVar13,0x18e,bVar29);
      if (iVar24 < 0) {
        if ((*(uint *)(param_1 + 0x408) & 0x40000000) != 0) {
          local_44 = -0x16;
          ieee80211_note(piVar13,"%s: Beaconing scheme update to value [%d] failed\n",
                         "ieee80211_ioctl_setparam",bVar29);
          break;
        }
        goto LAB_ram_00046d3c;
      }
    }
    goto LAB_ram_00046d0c;
  case 399:
    local_44 = ieee80211_wireless_set_sta_bmps(piVar13,iVar23,uVar20);
LAB_ram_00046c3a:
    if (local_44 != 0x66) break;
    goto LAB_ram_00046c44;
  case 400:
    pbVar27 = (byte *)(param_1 + 0xf62);
    bVar4 = *pbVar27;
    if (((ushort)(pbVar27 == (byte *)0x0) == (bVar4 >> 1 & 1)) ||
       ((ushort)(pbVar27 == (byte *)0x0) ==
        (*(ushort *)(*(int *)(param_1 + 0x404) + 0x500) >> 0xe & 1))) {
      puVar12 = (ushort *)(*(int *)(param_1 + 0x404) + 0x500);
      if (uVar20 == 0) {
        *pbVar27 = bVar4 & 0xfd;
        uVar9 = *puVar12 & 0xbfff;
      }
      else {
        *pbVar27 = bVar4 | 2;
        uVar9 = *puVar12 | 0x4000;
      }
      *puVar12 = uVar9;
      ieee80211_change_bw(piVar13,0x14,0);
      if (*(int *)(param_1 + 0x748) == 1) {
        local_44 = 0;
        ieee80211_send_20_40_bss_coex(piVar13);
        break;
      }
    }
    goto LAB_ram_00046d0c;
  case 0x192:
    *(byte *)(param_1 + 0xf65) = bVar4;
    ieee80211_param_to_qdrv(piVar13,0x192,uVar20,0,0);
    (*(code *)&printk)("%s: TX Block Ack establishment disable configured to %u\n",param_1,uVar20);
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,0);
    break;
  case 0x193:
    *(byte *)(param_1 + 0xf66) = bVar4;
    (*(code *)&printk)("%s: RX Block Ack decline configured to %u\n",param_1,uVar20);
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,0);
    break;
  case 0x194:
    local_44 = 0;
    *(bool *)(param_1 + 0xf67) = uVar20 != 0;
    break;
  case 0x195:
    local_44 = 0;
    (**(code **)(iVar23 + 0x5d28))(piVar13,uVar20);
    break;
  case 0x196:
    if (*(int *)(param_1 + 0x748) != 6) goto switchD_ram_000416d4_caseD_b;
    uVar20 = *(uint *)(param_1 + 0x738);
    *(byte *)(param_1 + 0xf68) = bVar4;
    goto LAB_ram_0004189e;
  case 0x197:
    bVar29 = uVar20 != 0;
    if ((bool)*(char *)(iVar23 + 0x5ce4) != bVar29) {
      *(char *)(iVar23 + 0x5ce4) = bVar29;
      if (bVar29) {
        local_44 = 0;
        ieee80211_start_obss_scan_timer(piVar13);
      }
      else {
        local_44 = 0;
        (*(code *)&del_timer)(iVar23 + 0x5cc8);
      }
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x198:
    if (uVar20 == 0) {
      uVar11 = *(uint *)(iVar23 + 0xa4) & 0xffffbfff;
      uVar15 = *(uint *)(iVar23 + 0x98) & 0xfffffbff;
    }
    else {
      uVar11 = *(uint *)(iVar23 + 0xa4) | 0x4000;
      uVar15 = *(uint *)(iVar23 + 0x98) | 0x400;
    }
    *(uint *)(iVar23 + 0x98) = uVar15;
    *(uint *)(iVar23 + 0xa4) = uVar11;
    ieee80211_param_to_qdrv(piVar13,0x198,uVar20,0,0);
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,1);
    break;
  case 0x199:
    *(byte *)(iVar23 + 0x535b) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x199,uVar20,0,0);
    break;
  case 0x19a:
    *(byte *)(iVar23 + 0x535d) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x19a,uVar20,0,0);
    break;
  case 0x19b:
    if (uVar20 < 3) {
      *(uint *)(*(int *)(param_1 + 0x404) + 0xa8) = uVar20;
      ieee80211_param_to_qdrv(piVar13,0x19b,uVar20,0,0);
      local_44 = 0;
      ieee80211_wireless_reassoc(piVar13,0,1);
    }
    else {
      local_44 = 0x16;
    }
    break;
  case 0x19c:
    *(ushort *)(iVar23 + 0xbdc) = uVar9;
    iVar24 = *(int *)(iVar23 + 8);
    while (iVar25 = iVar24, iVar25 != 0) {
      iVar24 = *(int *)(iVar25 + 0xf8);
      if ((*(int *)(iVar25 + 0x448) == 6) && (*(int *)(iVar25 + 0x44c) == 4)) {
        (**(code **)(iVar23 + 0x5444))();
      }
    }
    goto LAB_ram_00046d0c;
  case 0x19e:
    ieee80211_param_to_qdrv(piVar13,0x19e,uVar20,0,0);
    if (*(byte *)(iVar23 + 0x535e) != uVar20) {
      ieee80211_wireless_reassoc(piVar13,0,1);
    }
    local_44 = 0;
    *(byte *)(iVar23 + 0x535e) = bVar4;
    break;
  case 0x19f:
    if ((*(char *)(iVar23 + 0xbf5) == '\x02') &&
       ((*(int *)(iVar23 + 0x14) - 2U < 2 || (*(int *)(iVar23 + 0x14) - 8U < 2)))) {
      if (uVar20 == 0) {
        *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) & 0x7fffffff;
        (**(code **)(iVar23 + 0x5538))(piVar13,0);
      }
      else {
        *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) | 0x80000000;
      }
      local_44 = 0;
      ieee80211_wireless_reassoc(piVar13,0,1);
      break;
    }
switchD_ram_000416d4_caseD_b:
    local_44 = 0x5f;
    break;
  case 0x1a1:
    if (uVar20 == 0) {
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) & 0xffffdfff;
      if ((*(char *)(iVar23 + 0xbeb) != '\0') && (*(int *)(param_1 + 0x748) - 1U < 2)) {
        *(char *)(iVar23 + 0xbeb) = '\0';
        (**(code **)(iVar23 + 0x5518))(iVar23);
      }
    }
    else {
      *(uint *)(iVar23 + 0x9c) = *(uint *)(iVar23 + 0x9c) | 0x2000;
    }
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,1);
    break;
  case 0x1a4:
    ieee80211_param_to_qdrv(piVar13,0x1a4,uVar20,0,0);
    local_44 = 0;
    *(byte *)(iVar23 + 0x535f) = bVar4;
    break;
  case 0x1a6:
    if (2 < uVar20) {
      return -0x16;
    }
    local_44 = 0;
    *(byte *)(param_1 + 0xda2) = bVar4;
    break;
  case 0x1a7:
    if ((int)uVar20 < 0) {
      return -0x16;
    }
    local_44 = 0;
    *(byte *)(param_1 + 0xf8f) = bVar4;
    break;
  case 0x1a8:
  case 0x1a9:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5cc0) = uVar20;
    break;
  case 0x1ad:
    iVar24 = ieee80211_swfeat_is_supported(0xe,1);
    if (iVar24 != 0) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5c2e) = bVar4;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x1ae:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    *(ushort *)(param_1 + 0xf74) = uVar9;
    if ((uVar20 & 0xffff) != 0) {
      *(undefined2 *)(param_1 + 0xf76) = 0;
      *(int *)(param_1 + 0xf78) = _jiffies + 200;
      *(undefined2 *)(param_1 + 0xf7c) = 0;
      local_44 = 0;
      *(int *)(param_1 + 0xf80) = _jiffies + 200;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x1af:
    if ((int)uVar20 < 0) {
      uVar20 = uVar20 & 0x7fffffff;
      if (uVar20 < 0x3d) {
        uVar11 = 1;
        if (uVar20 != 0) {
          uVar11 = uVar20;
        }
      }
      else {
        uVar11 = 0x3c;
      }
      local_44 = 0;
      ieee80211_off_channel_suspend(piVar13,uVar11);
    }
    else {
      local_44 = 0;
      ieee80211_off_channel_resume(piVar13);
    }
    break;
  case 0x1b0:
    if ((*(int *)(param_1 + 0x748) == 6) && (uVar20 - 1 < 0x1f)) {
      for (puVar18 = *(undefined4 **)(iVar23 + 0x4e60); puVar18 != (undefined4 *)0x0;
          puVar18 = (undefined4 *)puVar18[3]) {
        ieee80211_wireless_reassoc(*puVar18,0,0);
      }
      local_44 = 0;
      *(byte *)(param_1 + 0xbf6) = bVar4;
    }
    else {
      local_44 = 0x16;
    }
    break;
  case 0x1b1:
    if ((*(int *)(param_1 + 0x748) == 6) || ((*(uint *)(iVar23 + 0x9c) & 0x40000) != 0)) {
      uVar11 = uVar20 >> 0x10;
      uVar15 = 1;
      if (uVar11 - 1 < 0x1f) {
        iVar24 = 0x1f;
        _DAT_auxregs_0008 = 0x46374;
        _DAT_auxregs_000c = 0x46390;
        uVar16 = uVar20 & 0xffff;
        do {
          if (uVar15 != uVar11) {
            uVar16 = uVar16 + *(ushort *)(uVar15 * 6 + iVar23 + 0x5076);
          }
          uVar15 = uVar15 + 1;
          iVar24 = iVar24 + -1;
        } while (iVar24 != 0);
        iVar24 = uVar11 * 6 + iVar23;
        if (((uVar20 & 0xffff) <= (uint)*(ushort *)(iVar24 + 0x5074)) &&
           ((int)uVar16 <= (int)(uint)*(ushort *)(iVar23 + 0x5068))) {
          *(ushort *)(iVar24 + 0x5076) = uVar9;
          for (puVar18 = *(undefined4 **)(iVar23 + 0x4e60); puVar18 != (undefined4 *)0x0;
              puVar18 = (undefined4 *)puVar18[3]) {
            ieee80211_wireless_reassoc(*puVar18,0,0);
          }
          local_44 = 0;
          break;
        }
      }
    }
    local_44 = 0x16;
    break;
  case 0x1b2:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    if (uVar20 - 1 < 0x45) {
      return -0x16;
    }
    if ((0 < (int)uVar20) && (iVar24 = (**(code **)(iVar23 + 0x5480))(), iVar24 == 0)) {
      return -0x16;
    }
    if (*(code **)(iVar23 + 0x54a8) != (code *)0x0) {
      (**(code **)(iVar23 + 0x54a8))(iVar23,uVar20);
    }
    if (*(code **)(iVar23 + 0x54ac) != (code *)0x0) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x54ac))(iVar23);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x1b3:
    *(byte *)(iVar23 + 0x5d3c) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x1b3,uVar20,0,0);
    break;
  case 0x1b7:
    if ((*(int *)(param_1 + 0x748) == 6) &&
       (iVar24 = (**(code **)(iVar23 + 0x54a4))(iVar23), 0 < iVar24)) {
      local_44 = 0;
      (**(code **)(iVar23 + 0x54b0))(iVar23);
      (*(code *)&printk)("<7>ICAC: Aborted ICAC due to set channel request\n");
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x1b8:
    if (*(int *)(param_1 + 0x748) != 1) {
      return -0x5f;
    }
    iVar25 = (**(code **)(iVar23 + 0x5480))();
    if (iVar25 == 0) {
      return -0x5f;
    }
    if (uVar20 == 0) {
      uVar11 = *(uint *)(iVar23 + 0x9c) & 0xffffff7f;
    }
    else {
      uVar11 = *(uint *)(iVar23 + 0x9c) | 0x80;
    }
    *(uint *)(iVar23 + 0x9c) = uVar11;
    if (*(int *)(param_1 + 0x74c) == 4) {
      if (0 < *(short *)(iVar23 + 0x5c2)) {
        pcVar7 = "Enabled";
        if (uVar20 == 0) {
          pcVar7 = "Disabled";
        }
        (*(code *)&printk)("SCS: send qtn DFS report (DFS %s)\n",pcVar7);
      }
      ieee80211_send_action_dfs_report(*(undefined4 *)(param_1 + 0x814));
    }
    bVar29 = uVar20 != 0;
    *(bool *)(*(int *)(iVar24 + 0x68) + 0x20) = bVar29;
    local_44 = 0;
    (**(code **)(iVar23 + 0x5528))(bVar29);
    (**(code **)(iVar23 + 0x5524))(bVar29);
    *(bool *)(iVar23 + 0x5d84) = bVar29;
    break;
  case 0x1b9:
    if (*(int *)(param_1 + 0x748) != 1) {
      return -0x16;
    }
    iVar24 = (**(code **)(iVar23 + 0x5480))();
    if (iVar24 == 0) {
      return -0x16;
    }
    local_44 = 0;
    *(bool *)(iVar23 + 0x5d85) = uVar20 != 0;
    break;
  case 0x1ba:
    if (999 < uVar20 - 1) {
      return -0x16;
    }
    local_44 = 0;
    *(uint *)(iVar23 + 0x5d80) = uVar20;
    break;
  case 0x1bb:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    if (uVar11 == 0) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5d1c) = bVar4;
      break;
    }
    if (uVar11 == 1) {
      local_44 = 0;
      *(byte *)(iVar23 + 0x5d1d) = bVar4;
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x1be:
    *(uint *)(iVar23 + 0x5350) = uVar20;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x1be,uVar20,0,0);
    break;
  case 0x1bf:
    if ((*(int *)(param_1 + 0x748) == 6) && (0 < (int)uVar20)) {
      iVar24 = *(int *)(iVar23 + 8);
      do {
        if (iVar24 == 0) {
          local_44 = 0;
          *(byte *)(iVar23 + 0xbf3) = bVar4;
          goto LAB_ram_00046c86;
        }
        iVar25 = *(int *)(iVar24 + 0xf8);
        piVar13 = (int *)(iVar24 + 0x514);
        iVar24 = iVar25;
      } while ((*piVar13 == 0) ||
              (iVar25 = (*(code *)&__divsi3)((uint)*(ushort *)(*piVar13 + 0xa42) << 10,5000),
              (int)(uVar20 * iVar25) < 200));
    }
    return -0x16;
  case 0x1c1:
    local_44 = 0;
    *(bool *)(iVar23 + 0x5dac) = uVar20 != 0;
    break;
  case 0x1c4:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x5f;
    }
    puVar6 = (uint *)(param_1 + 0xf88);
    if (uVar20 == 0) {
      local_44 = 0;
      *puVar6 = *puVar6 & 0xfffffffe;
    }
    else {
      local_44 = 0;
      *puVar6 = *puVar6 | 1;
    }
    break;
  case 0x1c8:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5e08) = uVar20;
    break;
  case 0x1c9:
    if (*(int *)(param_1 + 0x748) == 6) {
      local_44 = 0;
      ieee80211_param_to_qdrv(piVar13,0x1c9,uVar20,0,0);
      break;
    }
    goto LAB_ram_00046d0c;
  case 0x1cb:
    local_44 = 0;
    *(byte *)(iVar23 + 0x5362) = bVar4;
    break;
  case 0x1cc:
    local_44 = 0;
    *(byte *)(iVar23 + 0x4df1) = bVar4;
    break;
  case 0x1cd:
    local_44 = 0;
    *(byte *)(iVar23 + 0x4df0) = bVar4;
    break;
  case 0x1d0:
    local_44 = 0;
    *(uint *)(iVar23 + 0x4e2c) = uVar20;
    break;
  case 0x1d2:
    local_44 = 0;
    *(bool *)(iVar23 + 0x5d88) = uVar20 != 0;
    break;
  case 0x1d4:
    if (999 < uVar20 - 1) {
      return -0x16;
    }
    local_44 = 0;
    *(uint *)(iVar23 + 0x5da8) = uVar20;
    break;
  case 0x1d5:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5db8) = uVar20;
    break;
  case 0x1d6:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5dbc) = uVar20;
    break;
  case 0x1d7:
    local_44 = 0;
    *(uint *)(iVar23 + 24000) = uVar20;
    break;
  case 0x1d8:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5dc4) = uVar20;
    break;
  case 0x1d9:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5dc8) = uVar20;
    break;
  case 0x1da:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5dcc) = uVar20;
    break;
  case 0x1db:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5dd0) = uVar20;
    break;
  case 0x1dc:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5dd4) = uVar20;
    break;
  case 0x1dd:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5dd8) = uVar20;
    break;
  case 0x1de:
    local_44 = 0;
    *(bool *)(iVar23 + 0xbf4) = uVar20 != 0;
    break;
  case 0x1e0:
    ieee80211_param_to_qdrv(piVar13,0x1e0,uVar20,0,0);
    local_44 = 0;
    ieee80211_wireless_reassoc(piVar13,0,1);
    break;
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
  case 0x1ea:
    iVar24 = *(int *)(param_1 + 0x404);
    if ((int)uVar20 < 0) {
      if (*(int *)(iVar24 + 0x1c) == 0) goto LAB_ram_00046c22;
      iVar25 = *(int *)(iVar24 + 0x18);
      *(undefined4 *)(iVar24 + 0x1c) = 0;
      *(undefined4 *)(iVar24 + 0x18) = 0;
      iVar23 = -1;
      if (5 < iVar25 - 1U) {
        iVar23 = iVar25;
      }
      uVar20 = 0xff;
    }
    else {
      if (0xc < (int)uVar20) {
LAB_ram_00046c22:
        local_44 = 0x16;
        break;
      }
      uVar20 = uVar20 | 0x7f000000 | uVar20 << 8 | uVar20 << 0x10;
      if ((ushort)(*(short *)(iVar24 + 0x4de) - 1U) < 6) {
        iVar23 = -1;
      }
      else {
        iVar23 = *(int *)(iVar24 + 0x14);
        if ((((iVar23 == 1) || (iVar23 == 7)) || (iVar23 == 10)) || (iVar23 - 0xbU < 4)) {
          iVar23 = 1;
        }
        else {
          iVar23 = 3;
        }
      }
      if (*(int *)(iVar24 + 0x1c) == 0) {
        *(uint *)(iVar24 + 0x18) = (uint)*(ushort *)(iVar24 + 0x4de);
        *(undefined4 *)(iVar24 + 0x1c) = 1;
      }
    }
    ieee80211_param_to_qdrv(piVar13,0x3e,uVar20,0,0);
    if (iVar23 != -1) {
      if ((*(char *)(iVar24 + 0xbf5) == '\x02') && (*(int *)(iVar24 + 0x24) == 1)) {
        iVar25 = *(int *)(iVar24 + 0x14);
        if (((iVar25 == 1) || ((iVar25 == 7 || (iVar25 == 10)))) || (iVar25 - 0xbU < 4)) {
          *(uint *)(param_1 + 0x3e0) = (uint)(10 < iVar23);
        }
        else {
          *(uint *)(param_1 + 0x3c8) = (uint)(10 < iVar23);
        }
      }
      *(short *)(iVar24 + 0x4e30) = (short)iVar23;
      *(int *)(iVar24 + 0x14) = iVar23;
      ieee80211_setmode(iVar24,iVar23);
      *(undefined4 *)(iVar24 + 0x4a44) = 4;
      ieee80211_wireless_reassoc(piVar13,0,1);
      if (*(int *)(param_1 + 0x748) == 6) {
        local_44 = 0;
        (**(code **)(iVar24 + 0x53cc))(iVar24);
        break;
      }
    }
    local_44 = 0;
    break;
  case 0x1eb:
    local_44 = 0;
    *(uint *)(iVar23 + 0x5e14) = uVar20;
    break;
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
  case 0x1ed:
    uVar11 = *(uint *)(iVar23 + 0x5e1c);
    if (uVar20 == 1) {
      uVar11 = uVar11 | 1;
    }
    else {
      uVar11 = uVar11 & 0xfffffffe;
    }
    *(uint *)(iVar23 + 0x5e1c) = uVar11;
    local_44 = 0;
    ieee80211_beacon_update_all(iVar23);
    break;
  case 0x1ee:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    *(ushort *)(param_1 + 0xf8c) = uVar9;
    local_44 = 0;
    (**(code **)(iVar23 + 0x5444))(piVar13);
    break;
  case 0x1ef:
    if (*(int *)(param_1 + 0x748) != 6) {
      return -0x16;
    }
    local_44 = 0;
    *(byte *)(param_1 + 0xf8e) = bVar4;
    break;
  case 0x1f2:
    *(byte *)(iVar23 + 0x535c) = bVar4;
    local_44 = 0;
    ieee80211_param_to_qdrv(piVar13,0x1f2,uVar20,0,0);
  }
LAB_ram_00046c86:
  return -local_44;
}

