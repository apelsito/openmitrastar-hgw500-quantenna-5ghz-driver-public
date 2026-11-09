
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_ram_0003741c(int param_1,undefined4 param_2,undefined4 param_3,uint *param_4)

{
  ushort uVar1;
  uint uVar2;
  ushort uVar3;
  int iVar4;
  byte *pbVar5;
  uint *puVar6;
  ushort *puVar7;
  int iVar8;
  undefined *puVar9;
  int iVar10;
  uint uVar11;
  byte bVar12;
  uint uVar13;
  bool bVar14;
  undefined4 local_1c;
  uint uStack_18;
  uint uStack_14;
  
  iVar10 = *(int *)(param_1 + 0x814);
  iVar4 = param_1 + 0x300;
  iVar8 = *(int *)(param_1 + 0x404);
  if (iVar10 == 0) {
    pbVar5 = (byte *)0x0;
  }
  else {
    pbVar5 = (byte *)(iVar10 + 0xc4);
  }
  uVar2 = *param_4;
  uVar13 = uVar2 & 0xffff;
  if (0x1f0 < uVar13 - 2) goto switchD_ram_0003747c_caseD_b;
  uVar11 = (int)uVar2 >> 0x10;
  switch(uVar13) {
  case 2:
    iVar10 = *(int *)(iVar8 + 0x14);
    if ((((iVar10 == 1) || (iVar10 == 7)) || (iVar10 == 10)) || (iVar10 - 0xbU < 4)) {
      ieee80211_param_from_qdrv(iVar4,2,param_4,0,0);
      local_1c = 0;
    }
    else {
      local_1c = 0;
      *param_4 = *(uint *)(iVar8 + 0x9c) >> 0x1e & 1;
    }
    break;
  case 3:
    if ((*(uint *)(param_1 + 0x738) & 0x1800000) != 0) {
      *param_4 = 5;
      return 0;
    }
    if (iVar10 != 0) {
      *param_4 = (uint)*(byte *)(iVar10 + 0x2c);
      return 0;
    }
    goto LAB_ram_000397de;
  case 4:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5060);
    break;
  case 5:
    if (pbVar5 != (byte *)0x0) {
      *param_4 = (uint)*pbVar5;
      return 0;
    }
    goto LAB_ram_000397de;
  case 6:
    if (pbVar5 != (byte *)0x0) {
      *param_4 = (uint)pbVar5[1];
      return 0;
    }
    goto LAB_ram_000397de;
  case 7:
    if (pbVar5 != (byte *)0x0) {
      *param_4 = (uint)pbVar5[2];
      return 0;
    }
    goto LAB_ram_000397de;
  case 8:
    if (pbVar5 != (byte *)0x0) {
      *param_4 = (uint)pbVar5[3];
      return 0;
    }
    goto LAB_ram_000397de;
  case 9:
    if (pbVar5 != (byte *)0x0) {
      *param_4 = (uint)pbVar5[4];
      return 0;
    }
    goto LAB_ram_000397de;
  case 10:
    uVar2 = *(uint *)(param_1 + 0x738) & 0x1800000;
    if (uVar2 == 0x1000000) {
      local_1c = 0;
      *param_4 = 2;
    }
    else if (uVar2 == 0x1800000) {
      local_1c = 0;
      *param_4 = 3;
    }
    else if (uVar2 == 0x800000) {
      local_1c = 0;
      *param_4 = 1;
    }
    else {
      local_1c = 0;
      *param_4 = 0;
    }
    break;
  default:
    goto switchD_ram_0003747c_caseD_b;
  case 0xc:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x4e50);
    break;
  case 0xd:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x738) >> 4 & 1;
    break;
  case 0xe:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x738) >> 0x1a & 1;
    break;
  case 0xf:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x738) >> 0x19 & 1;
    break;
  case 0x10:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x740);
    break;
  case 0x12:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x738) >> 0xd & 1;
    break;
  case 0x13:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x738) >> 0x1b & 1;
    break;
  case 0x14:
    local_1c = 0;
    *param_4 = ~(*(uint *)(param_1 + 0x738) >> 0x1c) & 1;
    break;
  case 0x15:
    if (pbVar5 != (byte *)0x0) {
      *param_4 = (uint)pbVar5[5];
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x16:
    if (pbVar5 != (byte *)0x0) {
      *param_4 = (uint)*(ushort *)(pbVar5 + 8);
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x17:
    local_1c = 0;
    *param_4 = *(int *)(param_1 + 0x784) * 5;
    break;
  case 0x18:
    local_1c = 0;
    *param_4 = *(int *)(param_1 + 0x780) * 5;
    break;
  case 0x19:
    local_1c = 0;
    *param_4 = *(int *)(param_1 + 0x77c) * 5;
    break;
  case 0x1c:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x80a);
    break;
  case 0x1d:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x4e2);
    break;
  case 0x1e:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x98) >> 0x1e & 1;
    break;
  case 0x1f:
    if (((*(uint *)(iVar8 + 0x98) & 0x40000000) != 0) && ((*(uint *)(iVar8 + 0x9c) & 0x800000) != 0)
       ) {
      *param_4 = (uint)*(byte *)(iVar8 + 0x529c);
      return 0;
    }
    goto switchD_ram_0003747c_caseD_b;
  case 0x25:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x738) >> 5 & 1;
    break;
  case 0x26:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 0x12 & 1;
    break;
  case 0x27:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x73c) & 1;
    break;
  case 0x28:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x738) >> 0xe & 1;
    break;
  case 0x29:
    uVar2 = (*(code *)&jiffies_to_msecs)(*(undefined4 *)(param_1 + 0x7e0));
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0x2a:
    uVar2 = (*(code *)&__udivsi3)(*(undefined4 *)(param_1 + 0x7e4),200);
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0x2b:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0xb78);
    break;
  case 0x2c:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x4f4);
    break;
  case 0x2d:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 1 & 1;
    break;
  case 0x2e:
    uVar2 = (*(code *)&__udivsi3)(*(undefined4 *)(param_1 + 0x7e8),200);
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0x2f:
    local_1c = 0;
    *param_4 = (int)*(char *)(param_1 + 0x7ec);
    break;
  case 0x30:
    local_1c = 0;
    *param_4 = (int)*(char *)(param_1 + 0x7ee);
    break;
  case 0x31:
    local_1c = 0;
    *param_4 = (int)*(char *)(param_1 + 0x7ed);
    break;
  case 0x32:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x7f0);
    break;
  case 0x33:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x7f2);
    break;
  case 0x34:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x7f1);
    break;
  case 0x35:
    if (*(int *)(param_1 + 0x748) == 6) {
      if ((*(uint *)(param_1 + 0x73c) & 0x10) != 0) {
        *param_4 = 1;
        return 0;
      }
      *param_4 = 0;
      return 0;
    }
    if (*(int *)(param_1 + 0x748) == 1) {
      *param_4 = (uint)*(byte *)(param_1 + 0x810);
      return 0;
    }
    goto LAB_ram_000397f6;
  case 0x36:
    if (iVar10 != 0) {
      *param_4 = *(ushort *)(iVar10 + 0x2e) & 0x10;
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x39:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x73c) & 0x40;
    break;
  case 0x3a:
  case 0xe7:
    if ((*(uint *)(iVar8 + 0x9c) & 0x80) == 0) {
      local_1c = 0;
      *param_4 = 0;
    }
    else {
      local_1c = 0;
      *param_4 = 1;
    }
    break;
  case 0x3b:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 8 & 1;
    break;
  case 0x3c:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x73c) & 0x800;
    break;
  case 0x3d:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0xa4) >> 0xf & 1;
    break;
  case 0x3e:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0xd74);
    break;
  case 0x3f:
  case 0x41:
  case 0x4d:
  case 0x4e:
  case 0x5a:
  case 0x5b:
  case 0x6e:
  case 0x70:
  case 0x7c:
  case 0x7d:
  case 0x81:
  case 0x8a:
  case 0xa0:
  case 0xa1:
  case 0xa5:
  case 0xb6:
  case 0xb7:
  case 0xba:
  case 0xbb:
  case 0xdc:
  case 0xdf:
  case 0xef:
  case 0xf0:
  case 0xf2:
  case 0x103:
  case 0x108:
  case 0x113:
  case 0x124:
  case 0x129:
  case 0x12a:
  case 0x132:
  case 0x14b:
  case 0x15d:
  case 0x170:
  case 0x171:
  case 0x1a5:
  case 0x1ac:
  case 0x1ca:
  case 0x1df:
  case 0x1e3:
  case 0x1f0:
  case 0x1f1:
    ieee80211_param_from_qdrv(iVar4,uVar13,param_4,0,0);
    local_1c = 0;
    break;
  case 0x42:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x408);
    break;
  case 0x47:
    uVar2 = ieee80211_get_bw(iVar8);
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0x5d:
    if (*(int *)(iVar8 + 0x14) - 0xbU < 4) {
      local_1c = 0;
      *param_4 = *(byte *)(iVar8 + 0x567) >> 5 & 1;
    }
    else {
      uVar2 = 0;
      if (((*(uint *)(param_1 + 0xd78) & 0x800) != 0) &&
         (uVar2 = 1, (*(ushort *)(iVar8 + 0x500) & 0x60) == 0)) {
        uVar2 = 0;
      }
      local_1c = 0;
      *param_4 = uVar2;
    }
    break;
  case 99:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xd8a);
    break;
  case 0x66:
    if ((*(uint *)(iVar8 + 0x98) & 0x80) == 0) {
      local_1c = 0;
      *param_4 = 0;
    }
    else {
      local_1c = 0;
      *param_4 = 1;
    }
    break;
  case 0x67:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(param_1 + 0xd8c);
    break;
  case 0x69:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0xbe4);
    break;
  case 0x6a:
    ieee80211_param_from_qdrv(iVar4,0x6a,param_4,0,0);
    if (uVar11 - 1 < 0xff) {
      uVar2 = uVar11;
      if ((int)uVar11 < 0) {
        uVar2 = uVar11 + 7;
      }
      uVar13 = uVar11 & 0x80000007;
      bVar14 = (int)uVar13 < 0;
      if (bVar14) {
        uVar13 = uVar13 - 1;
      }
      bVar12 = (byte)uVar13;
      if (bVar14) {
        bVar12 = (bVar12 | 0xf8) + 1;
      }
      if ((((int)(uint)*(byte *)(iVar8 + ((int)uVar2 >> 3) + 0x4bb0) >> (bVar12 & 0x1f) & 1U) != 0)
         && (iVar4 = findchannel(iVar8,uVar11,0), iVar4 != 0)) {
        if (*param_4 == 1) {
          *param_4 = 10;
          return 0;
        }
        *param_4 = (int)*(char *)(iVar4 + 0xc);
        return 0;
      }
    }
    goto LAB_ram_000397de;
  case 0x6c:
    *param_4 = 0;
    (*(code *)&printk)("RX stats (delta)\n");
    (*(code *)&printk)("  dup:\t%u\n",*(undefined4 *)(param_1 + 0x42c));
    (*(code *)&printk)("  beacon:\t%u\n",*(undefined4 *)(param_1 + 0x454));
    (*(code *)&printk)("  elem_missing:\t%u\n",*(undefined4 *)(param_1 + 0x45c));
    (*(code *)&printk)("  badchan:\t%u\n",*(undefined4 *)(param_1 + 0x46c));
    (*(code *)&printk)("  chanmismatch:\t%u\n",*(undefined4 *)(param_1 + 0x470));
    (*(code *)&memset)(param_1 + 0x420,0,0x174);
    local_1c = 0;
    break;
  case 0x6f:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 0x14 & 1;
    break;
  case 0x71:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x505);
    break;
  case 0x72:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 0x15 & 1;
    break;
  case 0x73:
    iVar4 = ieee80211_countryid_to_country_string(*(undefined2 *)(iVar8 + 0x5170),&uStack_14);
    if (iVar4 != 0) {
      uStack_14 = 0;
    }
    local_1c = 0;
    *param_4 = uStack_14;
    break;
  case 0x7a:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x4ec);
    break;
  case 0x7b:
    if ((*(uint *)(param_1 + 0xd74) & 0xff000000) == 0x7f000000) {
      local_1c = 0;
      *param_4 = *(uint *)(param_1 + 0xd74) & 0xff;
    }
    else {
      (*(code *)&printk)("11N MCS is not set\n");
      local_1c = 0xffffffa1;
    }
    break;
  case 0x7e:
    ieee80211_iterate_dev_nodes(param_1,iVar8 + 0x4e5c,&LAB_ram_0002b368,0,0);
    local_1c = 0;
    *param_4 = 0;
    break;
  case 0x80:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(param_1 + 0xd94);
    break;
  case 0x85:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5134);
    break;
  case 0x86:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5136);
    break;
  case 0x87:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5138);
    break;
  case 0x88:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x513a);
    break;
  case 0x93:
    uVar2 = uVar2 >> 0x10;
    if (0x42 < uVar2) {
      (*(code *)&printk)("<4>%s: SCS: invalid config cmd %u\n",param_1,uVar2);
      return 0xffffffea;
    }
    if (1 < *(short *)(iVar8 + 0x5c2)) {
      (*(code *)&printk)("SCS: get param %u\n",uVar2);
    }
    switch(uVar2) {
    case 0:
    case 1:
      uVar13 = (uint)*(ushort *)(iVar8 + 0x5c0);
      goto LAB_ram_000383e0;
    default:
      if (1 < *(short *)(iVar8 + 0x5c2)) {
        (*(code *)&printk)("SCS: get param %u not supported\n",uVar2);
        return 0xffffffea;
      }
      goto LAB_ram_000397de;
    case 4:
      puVar6 = (uint *)(iVar8 + 0x594);
      break;
    case 5:
      puVar6 = (uint *)(iVar8 + 0x598);
    }
    uVar13 = *puVar6;
LAB_ram_000383e0:
    *param_4 = uVar13;
    if (1 < *(short *)(iVar8 + 0x5c2)) {
      (*(code *)&printk)("SCS: get param %u: value 0x%x completed successfully\n",uVar2,*param_4);
      return 0;
    }
    goto LAB_ram_000397f6;
  case 0x94:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5313);
    break;
  case 0x96:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0xbe5);
    break;
  case 0x97:
    uVar2 = (*(code *)&__udivsi3)(*(undefined4 *)(iVar8 + 0x5318),200);
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0x9a:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0xd78) >> 0xd & 1;
    break;
  case 0x9b:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xda1);
    break;
  case 0x9c:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xda3);
    break;
  case 0x9d:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xda4);
    break;
  case 0x9e:
    if (((*(uint *)(iVar8 + 0x98) & 0x40000000) == 0) || ((*(uint *)(iVar8 + 0x9c) & 0x800000) == 0)
       ) {
      *param_4 = (uint)*(byte *)(iVar8 + 0xbe1);
      return 0;
    }
    goto switchD_ram_0003747c_caseD_b;
  case 0xa2:
    local_1c = 0;
    *param_4 = (uint)CONCAT11(*(undefined1 *)(iVar8 + 0x5788),*(undefined1 *)(iVar8 + 0x5789));
    break;
  case 0xa3:
    local_1c = 0;
    *param_4 = (uint)CONCAT11(*(undefined1 *)(iVar8 + 0x578a),*(undefined1 *)(iVar8 + 0x578b));
    break;
  case 0xac:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0xac);
    break;
  case 0xad:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5068);
    break;
  case 0xb8:
    uVar2 = (*(code *)&br_get_ap_isolate)();
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0xb9:
    local_1c = 0;
    *param_4 = *(uint *)(_soc_shared_params + 0x48);
    break;
  case 0xbc:
    if ((uVar2 >> 0x10) - 1 < 0x1f) {
      *param_4 = (uint)*(ushort *)((uVar2 >> 0x10) * 6 + iVar8 + 0x5074);
      return 0;
    }
    goto LAB_ram_000397de;
  case 0xc2:
    local_1c = 0;
    *param_4 = *(uint *)(_soc_shared_params + 0x3c);
    break;
  case 199:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 0x16 & 1;
    break;
  case 0xcb:
    local_1c = 0;
    *param_4 = *(uint *)(*(int *)(*(int *)(iVar8 + 0x4e4c) + 0xc) + 0x3e00);
    break;
  case 0xce:
    if (((*(uint *)(iVar8 + 0x98) & 0x40000000) == 0) || ((*(uint *)(iVar8 + 0x9c) & 0x800000) == 0)
       ) {
      *param_4 = (uint)*(byte *)(iVar8 + 0xbe3);
      return 0;
    }
    goto switchD_ram_0003747c_caseD_b;
  case 0xcf:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x513c);
    break;
  case 0xd0:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x513e);
    break;
  case 0xd1:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x514c);
    break;
  case 0xd2:
    if (uVar11 - 1 < 0xff) {
      uVar2 = uVar11;
      if ((int)uVar11 < 0) {
        uVar2 = uVar11 + 7;
      }
      uVar13 = uVar11 & 0x80000007;
      bVar14 = (int)uVar13 < 0;
      if (bVar14) {
        uVar13 = uVar13 - 1;
      }
      bVar12 = (byte)uVar13;
      if (bVar14) {
        bVar12 = (bVar12 | 0xf8) + 1;
      }
      if (((int)(uint)*(byte *)(iVar8 + ((int)uVar2 >> 3) + 0x4bb0) >> (bVar12 & 0x1f) & 1U) != 0) {
        iVar4 = findchannel(iVar8,uVar11,0);
        if (iVar4 != 0) {
          *param_4 = (int)*(char *)(iVar4 + 9);
          return 0;
        }
        goto LAB_ram_000397de;
      }
    }
    goto LAB_ram_000397f6;
  case 0xd4:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x52d0);
    break;
  case 0xd7:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0xbe7);
    break;
  case 0xd8:
    iVar4 = *(int *)(*(int *)(iVar8 + 0x4e4c) + 0xc);
    if ((((iVar4 != 0) && ((int)uVar11 < 0xff)) && (0 < (int)uVar11)) &&
       (((int)(uint)*(byte *)(iVar8 + ((int)uVar2 >> 0x13) + 0x4bb0) >> ((byte)(uVar2 >> 0x10) & 7)
        & 1U) != 0)) {
      uVar2 = (uint)*(ushort *)(iVar4 + uVar11 * 2 + 0x29ea);
      if (uVar2 == 0xffff) {
        *param_4 = 0xffffffff;
        return 0;
      }
      *param_4 = uVar2;
      return 0;
    }
    goto LAB_ram_000397de;
  case 0xd9:
    if (((*(uint *)(iVar8 + 0x98) & 0x40000000) != 0) && ((*(uint *)(iVar8 + 0x9c) & 0x800000) != 0)
       ) {
      uVar2 = ieee80211_tpc_query_get_interval(iVar8 + 0x5a84);
      *param_4 = uVar2;
      return 0;
    }
    goto switchD_ram_0003747c_caseD_b;
  case 0xda:
    if (((*(uint *)(iVar8 + 0x98) & 0x40000000) != 0) && ((*(uint *)(iVar8 + 0x9c) & 0x800000) != 0)
       ) {
      uVar2 = ieee80211_tpc_query_state(iVar8 + 0x5a84);
      *param_4 = uVar2;
      return 0;
    }
    goto switchD_ram_0003747c_caseD_b;
  case 0xdb:
    if ((*(uint *)(iVar8 + 0x98) & 0x40000000) != 0) {
      *param_4 = *(uint *)(iVar8 + 0x9c) >> 0x17 & 1;
      return 0;
    }
switchD_ram_0003747c_caseD_b:
    local_1c = 0xffffffa1;
    break;
  case 0xdd:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x81c);
    break;
  case 0xe3:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0xdfc);
    break;
  case 0xe4:
    if ((*(uint *)(param_1 + 0xd74) & 0xff000000) == 0x7e000000) {
      local_1c = 0;
      *param_4 = *(uint *)(param_1 + 0xd74) & 0xf;
    }
    else {
      (*(code *)&printk)("VHT rate is not set\n");
      local_1c = 0xffffffa1;
    }
    break;
  case 0xe8:
  case 0x177:
    if (uVar2 >> 0x10 == 1) {
      *param_4 = (uint)*(byte *)(iVar8 + 0x8f8);
      return 0;
    }
    if (uVar2 >> 0x10 == 2) {
      iVar4 = FUN_ram_0002f0c4(iVar4);
      *param_4 = (uint)(iVar4 == 0);
      return 0;
    }
    goto LAB_ram_000397de;
  case 0xec:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x594);
    break;
  case 0xed:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x59d);
    break;
  case 0xf1:
    uVar2 = FUN_ram_00029190(iVar8,0);
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0xf6:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xf56);
    break;
  case 0xf9:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x73c) >> 0x1c & 1;
    break;
  case 0x100:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xe00);
    break;
  case 0x104:
    iVar4 = ieee80211_countryid_to_country_string(*(undefined2 *)(iVar8 + 0x5172),&uStack_18);
    if (iVar4 != 0) {
      uStack_18 = 0;
    }
    local_1c = 0;
    *param_4 = uStack_18;
    break;
  case 0x10a:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0xbf0);
    break;
  case 0x10b:
    if ((((int)(uint)*(byte *)(iVar8 + (uVar2 >> 0x1b) + 0x4bb0) >> ((byte)(uVar2 >> 0x18) & 7) & 1U
         ) != 0) && ((uVar11 & 0xf) < 5)) {
      uVar13 = (int)uVar2 >> 0x14 & 0xf;
      iVar4 = findchannel(iVar8,uVar2 >> 0x18,0);
      if ((iVar4 != 0) && ((uVar13 < 2 && (uVar2 = (uVar11 & 0xf) - 1, uVar2 < 4)))) {
        iVar4 = uVar2 * 3 + uVar13 * 0xc + iVar4;
        *param_4 = (uint)*(byte *)(iVar4 + 0x10) << 0x10 | (uint)*(byte *)(iVar4 + 0xf) << 8 |
                   (uint)*(byte *)(iVar4 + 0xe);
        return 0;
      }
    }
    goto LAB_ram_000397de;
  case 0x10d:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0xb0);
    break;
  case 0x10e:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0xbe6);
    break;
  case 0x112:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xe01);
    break;
  case 0x116:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x2ec) >> 0x1e & 1;
    break;
  case 0x117:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x2ec) >> 0x1d & 1;
    break;
  case 0x118:
    local_1c = 0;
    *param_4 = *(byte *)(iVar8 + 0x577) + 1;
    break;
  case 0x119:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5b1c) << 0x1f | (uint)*(ushort *)(iVar8 + 0x5b1e) << 0x1d
               | (uint)*(ushort *)(iVar8 + 0x5b20) | (*(ushort *)(iVar8 + 0x5b22) & 0x1fff) << 0x10;
    break;
  case 0x11a:
    if (*(int *)(param_1 + 0x748) == 2) {
      uVar3 = *(ushort *)(param_1 + 0xce0) & 3;
      if (uVar3 == 2) {
        uVar2 = 1;
      }
      else {
        if (uVar3 != 1) goto LAB_ram_00038cc6;
        uVar2 = 0;
      }
    }
    else {
LAB_ram_00038cc6:
      uVar2 = 2;
    }
    *param_4 = uVar2;
    if (*(int *)(param_1 + 0x748) == 2) {
      uVar3 = *(ushort *)(param_1 + 0xce0) & 3;
      if (uVar3 == 1) {
        puVar9 = &DAT_ram_00097988;
      }
      else {
        if (uVar3 != 2) goto LAB_ram_00038d02;
        puVar9 = &DAT_ram_0009798c;
      }
    }
    else {
LAB_ram_00038d02:
      puVar9 = &DAT_ram_00097990;
    }
    if ((*(uint *)(param_1 + 0x408) & 0x40000001) != 0) {
      ieee80211_note(iVar4,"Legay VAP QHOP mode: %s\n",puVar9);
      return 0;
    }
LAB_ram_000397f6:
    local_1c = 0;
    break;
  case 0x11b:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5c28);
    break;
  case 0x11c:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5c2b);
    break;
  case 0x11d:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5c2c);
    break;
  case 0x11e:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5c29);
    break;
  case 0x11f:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5c2a);
    break;
  case 0x120:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x505d);
    break;
  case 0x122:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x40c);
    break;
  case 0x123:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(param_1 + 0xe04);
    break;
  case 0x125:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x410);
    break;
  case 0x126:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x414);
    break;
  case 0x127:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x41c);
    break;
  case 0x128:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x418);
    break;
  case 0x12d:
    local_1c = 0;
    *param_4 = *(uint *)(_soc_shared_params + 0x170);
    break;
  case 0x12e:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xf54);
    break;
  case 0x12f:
    local_1c = 0;
    *param_4 = (uint)CONCAT11(*(undefined1 *)(iVar8 + 0x52a0),*(undefined1 *)(iVar8 + 0x529c));
    break;
  case 0x130:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5348);
    break;
  case 0x131:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x534c);
    break;
  case 0x133:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5c98);
    break;
  case 0x134:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5c9c);
    break;
  case 0x135:
    local_1c = 0;
    *param_4 = *(uint *)(_soc_shared_params + 0x174);
    break;
  case 0x137:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x6b0);
    break;
  case 0x138:
    if ((*(uint *)(param_1 + 0x73c) & 0x1000000) == 0) {
      local_1c = 0;
      *param_4 = 1;
    }
    else {
      local_1c = 0;
      *param_4 = 0;
    }
    break;
  case 0x139:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6b8);
    break;
  case 0x13a:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6bc);
    break;
  case 0x13b:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6c0);
    break;
  case 0x13c:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6c4);
    break;
  case 0x13d:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6c8);
    break;
  case 0x13e:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6cc);
    break;
  case 0x13f:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6d0);
    break;
  case 0x140:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6d4);
    break;
  case 0x141:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x59c);
    break;
  case 0x143:
    if ((*(uint *)(param_1 + 0x73c) & 0x4000000) == 0) {
      if ((*(uint *)(param_1 + 0x73c) & 0x2000000) == 0) {
        local_1c = 0;
        *param_4 = 0;
      }
      else {
        local_1c = 0;
        *param_4 = 1;
      }
    }
    else {
      local_1c = 0;
      *param_4 = 2;
    }
    break;
  case 0x144:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x6f8);
    break;
  case 0x145:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0x6f9);
    break;
  case 0x146:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x598);
    break;
  case 0x14e:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5c2d);
    break;
  case 0x150:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5c8c);
    break;
  case 0x16b:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5ca0);
    break;
  case 0x16c:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x6b4);
    break;
  case 0x173:
    local_1c = 0;
    *param_4 = (uint)g_l2_ext_filter;
    break;
  case 0x174:
    uVar2 = (**(code **)(iVar8 + 0x545c))();
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0x17d:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(_soc_shared_params + 7);
    break;
  case 0x17f:
    if (((*(int *)(iVar8 + 0x24) == 1) && (iVar10 != 0)) &&
       ((*(ushort *)(iVar10 + 0x3a) & 0x3fff) != 0)) {
      local_1c = 0;
      *param_4 = (uint)*(ushort *)(iVar10 + 0xeaa);
    }
    else {
      local_1c = 0;
      *param_4 = (uint)*(ushort *)(iVar8 + 0x5cec);
    }
    break;
  case 0x180:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x3f4);
    break;
  case 0x181:
    if ((*(int *)(iVar8 + 0x14) - 2U < 2) || (*(int *)(iVar8 + 0x14) - 8U < 2)) {
      uVar2 = ieee80211_get_bw(iVar8);
      *param_4 = uVar2;
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x182:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xf63);
    break;
  case 0x185:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5140);
    break;
  case 0x186:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5142);
    break;
  case 0x187:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5144);
    break;
  case 0x188:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5146);
    break;
  case 0x189:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x5148);
    break;
  case 0x18a:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0x514a);
    break;
  case 0x18b:
    local_1c = 0;
    *param_4 = (uint)(*(char *)(param_1 + 0xf64) != '\0');
    break;
  case 0x18c:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 0x1e & 1;
    break;
  case 0x18e:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0xbf2);
    break;
  case 399:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0x10cc);
    break;
  case 400:
    if (*(int *)(iVar8 + 0x14) - 2U < 2) {
      local_1c = 0;
      *param_4 = *(byte *)(param_1 + 0xf62) >> 1 & 1;
    }
    else if (*(int *)(iVar8 + 0x14) - 8U < 2) {
      local_1c = 0;
      *param_4 = *(ushort *)(iVar8 + 0x500) >> 0xe & 1;
    }
    else {
      local_1c = 0;
      *param_4 = 0;
    }
    break;
  case 0x192:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xf65);
    break;
  case 0x193:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xf66);
    break;
  case 0x194:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xf67);
    break;
  case 0x197:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5ce4);
    break;
  case 0x198:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0xa4) >> 0xe & 1;
    break;
  case 0x199:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x535b);
    break;
  case 0x19a:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x535d);
    break;
  case 0x19b:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0xa8);
    break;
  case 0x19c:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(iVar8 + 0xbdc);
    break;
  case 0x19e:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x535e);
    break;
  case 0x19f:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 0x1f;
    break;
  case 0x1a1:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x9c) >> 0xd & 1;
    break;
  case 0x1a4:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x535f);
    break;
  case 0x1a6:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xda2);
    break;
  case 0x1a8:
  case 0x1a9:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5cc0);
    break;
  case 0x1ad:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5c2e);
    break;
  case 0x1b0:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xbf6);
    break;
  case 0x1b1:
    if ((uVar2 >> 0x10) - 1 < 0x1f) {
      *param_4 = (uint)*(ushort *)((uVar2 >> 0x10) * 6 + iVar8 + 0x5076);
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x1b3:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5d3c);
    break;
  case 0x1b4:
    uVar2 = (**(code **)(iVar8 + 0x5480))();
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0x1b6:
    *param_4 = 0;
    (**(code **)(iVar8 + 0x5494))(iVar8);
    local_1c = 0;
    break;
  case 0x1b8:
    if ((*(int *)(param_1 + 0x748) == 1) && (iVar4 = (**(code **)(iVar8 + 0x5480))(), iVar4 != 0)) {
      *param_4 = (uint)*(byte *)(iVar8 + 0x5d84);
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x1b9:
    if ((*(int *)(param_1 + 0x748) == 1) && (iVar4 = (**(code **)(iVar8 + 0x5480))(), iVar4 != 0)) {
      *param_4 = (uint)*(byte *)(iVar8 + 0x5d85);
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x1ba:
    if ((*(int *)(param_1 + 0x748) == 1) && (iVar4 = (**(code **)(iVar8 + 0x5480))(), iVar4 != 0)) {
      *param_4 = *(uint *)(iVar8 + 0x5d80);
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x1bb:
    if (*(int *)(param_1 + 0x748) == 6) {
      if (uVar2 >> 0x10 == 0) {
        pbVar5 = (byte *)(iVar8 + 0x5d1c);
      }
      else {
        if (uVar2 >> 0x10 != 1) {
          uVar2 = 0;
          goto LAB_ram_000392b2;
        }
        pbVar5 = (byte *)(iVar8 + 0x5d1d);
      }
      uVar2 = (uint)*pbVar5;
LAB_ram_000392b2:
      *param_4 = uVar2;
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x1bc:
    if ((*(int *)(param_1 + 0x748) == 6) || (*(int *)(param_1 + 0x748) == 1)) {
      uVar2 = ieee80211_get_type_of_neighborhood(iVar8);
      *param_4 = uVar2;
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x1bd:
    if ((*(int *)(param_1 + 0x748) == 6) || (*(int *)(param_1 + 0x748) == 1)) {
      *param_4 = *(uint *)(iVar8 + 0x5d18);
      return 0;
    }
    goto LAB_ram_000397de;
  case 0x1be:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5350);
    break;
  case 0x1bf:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0xbf3);
    break;
  case 0x1c0:
    iVar4 = ieee80211_find_channel_by_ieee(iVar8,uVar2 >> 0x10);
    if (iVar4 != 0) {
      uVar2 = ieee80211_is_on_weather_channel(iVar8,iVar4);
      *param_4 = uVar2;
      return 0;
    }
LAB_ram_000397de:
    local_1c = 0xffffffea;
    break;
  case 0x1c2:
    local_1c = 0;
    *param_4 = 0x46;
    break;
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
  case 0x1c4:
    local_1c = 0;
    *param_4 = *(uint *)(param_1 + 0xf88) & 1;
    break;
  case 0x1c8:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5e08);
    break;
  case 0x1c9:
    puVar7 = *(ushort **)(_soc_shared_params + 0x184);
    uVar3 = puVar7[1];
    uVar1 = puVar7[2];
    uVar2 = (*(code *)&__divsi3)((uint)*puVar7 * 100);
    local_1c = 0;
    *param_4 = (uint)uVar3 << 0x10 | (uint)(byte)uVar1 | (uVar2 & 0xff) << 8;
    break;
  case 0x1cc:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x4df1);
    break;
  case 0x1cd:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x4df0);
    break;
  case 0x1d2:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x5d88);
    break;
  case 0x1d3:
    uVar2 = (**(code **)(iVar8 + 0x54a0))(iVar8,iVar4);
    local_1c = 0;
    *param_4 = uVar2;
    break;
  case 0x1d4:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5da8);
    break;
  case 0x1d5:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5db8);
    break;
  case 0x1d6:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5dbc);
    break;
  case 0x1d7:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 24000);
    break;
  case 0x1d8:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5dc4);
    break;
  case 0x1d9:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5dc8);
    break;
  case 0x1da:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5dcc);
    break;
  case 0x1db:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5dd0);
    break;
  case 0x1dc:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5dd4);
    break;
  case 0x1dd:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5dd8);
    break;
  case 0x1de:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0xbf4);
    break;
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
  case 0x1e4:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(param_1 + 0xf55);
    break;
  case 0x1eb:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5e14);
    break;
  case 0x1ec:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5e18) & 1;
    break;
  case 0x1ed:
    local_1c = 0;
    *param_4 = *(uint *)(iVar8 + 0x5e1c) & 1;
    break;
  case 0x1ee:
    local_1c = 0;
    *param_4 = (uint)*(ushort *)(param_1 + 0xf8c);
    break;
  case 0x1f2:
    local_1c = 0;
    *param_4 = (uint)*(byte *)(iVar8 + 0x535c);
  }
  return local_1c;
}

