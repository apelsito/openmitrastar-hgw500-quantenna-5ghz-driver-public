
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void om_auc_ipc_config_dispatch(undefined4 param_1,undefined4 param_2,uint param_3,uint param_4)

{
  undefined4 uVar1;
  byte bVar2;
  uint uVar3;
  char cVar4;
  ushort uVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  uint uVar9;
  undefined *puVar10;
  int iVar11;
  int iVar12;
  int unaff_gp;
  bool bVar13;
  ulonglong uVar14;
  undefined4 uStack_c;
  int iStack_8;
  
  uVar14 = FUN_ram_e5005d64();
  uVar7 = (uint)(uVar14 >> 0x20);
  iVar8 = (int)uVar14;
  switch(iVar8) {
  case 2:
  case 3:
    uVar7 = uVar7 & 0xff;
    FUN_ram_e5004c68();
    if (iVar8 == 2) {
      iVar8 = FUN_ram_e5004dcc();
      if (iVar8 == 0) {
        iVar8 = FUN_ram_e5004df4();
      }
    }
    else {
      iVar8 = FUN_ram_e5004df4();
      if (iVar8 == 0) {
        iVar8 = FUN_ram_e5004dcc();
      }
    }
    if (iVar8 != 0) {
      FUN_ram_e5005f74(iVar8,0xbc);
    }
    uVar1 = 0;
    if (iVar8 == 0) goto LAB_ram_e5005660;
    *(undefined2 *)(iVar8 + 0x58) = 0;
    iVar12 = FUN_ram_e5004d48(&DAT_ram_8006c02c,1);
    if (iVar12 < 0) {
      iVar12 = FUN_ram_e5004d48(&DAT_ram_005fcd60,4);
      if (iVar12 < 0) {
        puVar10 = (undefined *)0x0;
      }
      else {
        FUN_ram_e5005c34(0x84);
        puVar10 = &DAT_ram_005f8440 + iVar12 * 0xa0;
      }
    }
    else {
      FUN_ram_e5005c34(0x82);
      puVar10 = &DAT_ram_8006b62c + iVar12 * 0xa0;
    }
    if (puVar10 != (undefined *)0x0) {
      FUN_ram_e5005f74(puVar10,0xa0);
    }
    *(undefined **)(iVar8 + 0x14) = puVar10;
    if (puVar10 == (undefined *)0x0) {
      FUN_ram_e5007540(s_fw_auc_fw_tx_nodes_c_ram_00472633,0x3a2,
                       s_must_never_happen__preallocated_n_ram_00472922);
      return;
    }
    FUN_ram_e5005f74(&DAT_ram_80062318 + uVar7 * 0x38,0x38);
    (&DAT_ram_e5101b48)[uVar7] = iVar8;
    iVar8 = (uVar7 >> 5) * 4;
    *(uint *)(&DAT_ram_80069a90 + iVar8) =
         *(uint *)(&DAT_ram_80069a90 + iVar8) & ~(1 << ((byte)(uVar14 >> 0x20) & 0x1f));
    break;
  case 4:
    FUN_ram_e5004c68(uVar7 & 0xff);
    break;
  case 5:
  case 6:
    uVar7 = uVar7 & 0xff;
    param_3 = param_3 & 0xff;
    iVar12 = FUN_ram_e5004e1c(uVar7,param_3);
    uVar9 = param_4 & 0xffff;
    if (iVar12 == 0) {
LAB_ram_e50053b6:
      iVar11 = 0;
    }
    else {
      if (iVar8 == 5) {
        iVar11 = FUN_ram_e5004e40();
        if ((iVar11 == 0) && (iVar11 = FUN_ram_e5004e68(), iVar11 == 0)) {
          iVar11 = FUN_ram_e5004e90();
        }
      }
      else {
        iVar11 = FUN_ram_e5004e90();
        if ((iVar11 == 0) && (iVar11 = FUN_ram_e5004e68(), iVar11 == 0)) {
          iVar11 = FUN_ram_e5004e40();
        }
      }
      if (iVar11 == 0) goto LAB_ram_e50053b6;
      FUN_ram_e5005f74(iVar11,0xe8);
      *(undefined4 *)(iVar11 + 0x68) = 0;
      *(undefined4 *)(iVar11 + 100) = 0;
      *(short *)(iVar11 + 0x60) = 0x58c4 - (short)*(undefined4 *)(unaff_gp + 0x124);
      FUN_ram_e5000288(iVar11 + 0x30);
      *(undefined4 *)(iVar11 + 0x38) = 0;
      *(int *)(iVar11 + 0x3c) = iVar11 + 0x38;
      *(undefined2 *)(iVar11 + 0x54) = 0;
      if (_DAT_ram_e00000bc == 0) {
        *(undefined4 *)(unaff_gp + -0x50) = 0;
      }
      else {
        *(undefined4 *)(unaff_gp + -0x50) = *(undefined4 *)(_DAT_ram_e00000bc + 0x38);
      }
      *(int *)(iVar12 + 0x18 + param_3 * 4) = iVar11;
    }
    if (iVar11 != 0) {
      FUN_ram_e5004f84(uVar7,param_3);
      if (param_3 == 1) {
        bVar2 = 1;
      }
      else if (param_3 == 5) {
        bVar2 = 2;
      }
      else if (param_3 == 6 || param_3 == 4) {
        bVar2 = 3;
      }
      else {
        bVar2 = -(param_3 == 7) & 3;
      }
      if ((bVar2 == 2) && (*(char *)(unaff_gp + 0x85) == '\0')) {
        uVar3 = 0x100;
      }
      else {
        uVar3 = 0x80;
      }
      uVar9 = uVar9 * (uVar9 >= 9) + (uint)(uVar9 < 9) * 8;
      uVar3 = uVar9 * (uVar3 > uVar9) + uVar3 * (uVar3 <= uVar9) & 0xffff;
      FUN_ram_e5000288(&uStack_c);
      uVar9 = 0;
      uVar5 = 0;
      if (uVar3 != 0) {
        do {
          uVar5 = (ushort)uVar9;
          iVar12 = FUN_ram_e500148c(iVar8 == 5,0);
          if (iVar12 == 0) break;
          FUN_ram_e5000290(&uStack_c,iVar12,0);
          uVar6 = uVar9 + 1;
          uVar9 = uVar6 & 0xffff;
          uVar5 = (ushort)uVar6;
        } while (uVar9 < uVar3);
      }
      if (iStack_8 != 0) {
        *(undefined4 *)(iStack_8 + 8) = uStack_c;
      }
      if (uVar5 < 8) {
        FUN_ram_e5005c34(0x36);
        FUN_ram_e5004f50(uStack_c);
        uVar5 = 0;
      }
      else {
        if (uVar3 != uVar5) {
          FUN_ram_e5005c34(0x35);
        }
        FUN_ram_e5004ecc(uVar7,param_3,uStack_c,uVar5);
        *(undefined4 *)(&DAT_ram_80048000 + param_3 * 4 + uVar7 * 0x40) = uStack_c;
      }
      if (uVar5 != 0) {
        *(short *)(iVar11 + 0x56) = (short)param_4;
        *(undefined2 *)(iVar11 + 0x58) = 0;
        *(undefined4 *)(iVar11 + 0x94) = 0x58fc0;
        break;
      }
      FUN_ram_e5004e1c(uVar7,param_3);
    }
  default:
switchD_ram_e50051ae_default:
    uVar1 = 0;
    goto LAB_ram_e5005660;
  case 7:
    FUN_ram_e5004f84(uVar7 & 0xff,param_3 & 0xff);
    FUN_ram_e5004e1c(uVar7 & 0xff,param_3 & 0xff);
    break;
  case 8:
    FUN_ram_e5004fb8(uVar7 & 0xff,param_3 & 0xffff,1);
    uVar1 = 1;
    goto LAB_ram_e5005660;
  case 9:
    FUN_ram_e5004fb8(uVar7 & 0xff,param_3 & 0xffff,0);
    break;
  case 10:
    param_3 = param_3 & 0xffff;
    if (param_3 != 0) {
      cVar4 = '\0';
      do {
        if ((param_3 & 1) != 0) {
          iVar8 = FUN_ram_e5001668(uVar7 & 0xff,cVar4);
          if (iVar8 == 0) {
            bVar13 = true;
          }
          else {
            iVar12 = FUN_ram_e50006d4();
            if (iVar12 == 0) {
              bVar13 = *(short *)(iVar8 + 0x58) == 0;
            }
            else {
              bVar13 = false;
            }
          }
          if (!bVar13) goto switchD_ram_e50051ae_default;
        }
        param_3 = (int)param_3 >> 1;
        cVar4 = cVar4 + '\x01';
      } while (param_3 != 0);
    }
    break;
  case 0xb:
    uVar9 = uVar7 & 0xff;
    uVar3 = (uVar7 & 0xf000) >> 0xc;
    iVar8 = FUN_ram_e5001668(uVar9,(uVar7 & 0xf00) >> 8);
    iVar12 = FUN_ram_e50002e4(uVar9);
    *(char *)(iVar8 + 0x86) = (char)uVar3;
    if (uVar3 == 1) {
      bVar2 = *(byte *)(iVar8 + 0x87);
      *(byte *)(iVar8 + 0x87) = bVar2 | 4;
      if ((uVar7 >> 0x10 & 2) == 0) {
        *(byte *)(iVar8 + 0x87) = bVar2 & 0xbf | 4;
      }
      else {
        *(byte *)(iVar8 + 0x87) = bVar2 | 0x44;
      }
      *(char *)(iVar8 + 0x8c) = (char)param_3;
      *(ushort *)(iVar8 + 0x5a) = (ushort)(param_3 >> 8) & 0xfff;
      *(char *)(iVar8 + 0x88) = (char)param_4;
      *(undefined2 *)(iVar8 + 0x84) = 0x1000;
      if ((uVar7 >> 0x10 & 1) == 0) {
        if ((*(uint *)(iVar12 + 100) & 0x80) == 0) {
          FUN_ram_e5001828(uVar9,iVar8,*(undefined2 *)(iVar8 + 0x54));
        }
        uVar7 = *(uint *)(iVar8 + 0x7c);
        *(uint *)(iVar8 + 0x7c) = uVar7 | 2;
        if ((uVar7 & 4) != 0) {
          *(uint *)(iVar8 + 0x7c) = uVar7 & 0xfffffffb | 2;
          FUN_ram_e5001740(iVar8,1);
        }
      }
      else {
        *(uint *)(iVar8 + 0x7c) = *(uint *)(iVar8 + 0x7c) & 0xfffffffd;
      }
      if ((*(uint *)(iVar8 + 0x7c) & 1) == 0) {
        *(undefined4 *)(iVar8 + 0x78) = 0;
        *(undefined4 *)(iVar8 + 0x74) = 0;
        *(short *)(iVar8 + 0x70) = 0x58e0 - (short)*(undefined4 *)(unaff_gp + 0x124);
        if (*(undefined4 **)(iVar8 + 0x78) != (undefined4 *)0x0) {
          **(undefined4 **)(iVar8 + 0x78) = 0;
          *(undefined4 *)(iVar8 + 0x78) = 0;
          *(undefined4 *)(iVar8 + 0x74) = 0;
        }
        FUN_ram_e50001d4(iVar8 + 0x70,0xa00);
        *(uint *)(iVar8 + 0x7c) = *(uint *)(iVar8 + 0x7c) | 1;
      }
    }
    else {
      if (uVar3 == 5 || uVar3 == 6) {
        *(uint *)(iVar8 + 0x7c) = *(uint *)(iVar8 + 0x7c) & 0xfffffffb;
        FUN_ram_e5001740(iVar8,1);
      }
      *(byte *)(iVar8 + 0x87) = *(byte *)(iVar8 + 0x87) & 0xbb;
    }
    break;
  case 0xc:
    iVar8 = FUN_ram_e50002e4(uVar7 & 0xff);
    *(char *)(iVar8 + 0x7d) = (char)param_3;
    break;
  case 0xd:
    bVar2 = 0;
    uVar9 = 0;
    do {
      uVar3 = 0;
      do {
        iVar8 = uVar3 * 3 + uVar9 * 0xc + unaff_gp;
        *(byte *)(iVar8 + 0xa4) = (byte)(uVar7 >> (bVar2 & 0x1f)) & 0xf;
        *(byte *)(iVar8 + 0xa5) = (byte)(param_3 >> (bVar2 & 0x1f)) & 0xf;
        *(byte *)(iVar8 + 0xa6) = (byte)(param_4 >> (bVar2 & 0x1f)) & 0xf;
        uVar3 = uVar3 + 1 & 0xff;
        bVar2 = bVar2 + 4;
      } while (uVar3 < 4);
      uVar9 = uVar9 + 1 & 0xff;
      DAT_ram_e51035e4 = uVar7;
      DAT_ram_e51035e8 = param_3;
      DAT_ram_e51035ec = param_4;
    } while (uVar9 < 2);
    break;
  case 0xe:
    DAT_ram_e51035f0 = uVar7;
    *(uint *)(unaff_gp + 0xbc) = uVar7;
    break;
  case 0xf:
    FUN_ram_e50002f8(&LAB_ram_e5005984);
    break;
  case 0x10:
    if ((uVar7 == 0) || (0x7fffffffff < uVar14)) {
      if (uVar7 != 999) {
        uVar7 = 0;
        do {
          FUN_ram_e5005060(uVar7);
          uVar7 = uVar7 + 1;
        } while (uVar7 < 0x80);
      }
    }
    else {
      FUN_ram_e5005060(uVar7);
    }
    iVar8 = 0;
    do {
      FUN_ram_e5007494(s_contention_queue__d_counter__u_ram_0047297a,iVar8,
                       *(undefined1 *)(unaff_gp + iVar8 + 0x20));
      iVar8 = iVar8 + 1;
    } while (iVar8 < 4);
    FUN_ram_e5007494(s_chain_header_available__u_descrs_ram_0047299a,
                     *(undefined2 *)(unaff_gp + 0x98),*(undefined2 *)(unaff_gp + 0x94),
                     *(undefined2 *)(unaff_gp + 0x88),*(undefined2 *)(unaff_gp + 0x86),
                     *(undefined4 *)(unaff_gp + 0xe4));
    break;
  case 0x11:
    uVar9 = uVar7 & 0xfffffff;
    switch(uVar7 >> 0x1c) {
    case 1:
      *(uint *)(unaff_gp + -0xc4) =
           *(uint *)(unaff_gp + -0xc4) & 0xfffffffe | (uint)((uVar14 & 0xfffffff00000000) != 0);
      break;
    case 4:
      iVar12 = FUN_ram_e5001668((uVar7 & 0xff00000) >> 0x14,(uVar7 & 0xf0000) >> 0x10);
      *(uint *)(iVar12 + 0xb8) = ((uVar7 & 0xf800) >> 0xb) * 5;
      iVar8 = (uVar7 & 0x7ff) * 0x1400;
      *(int *)(iVar12 + 0xbc) = iVar8;
      *(int *)(iVar12 + 0xc4) = iVar8;
      *(undefined4 *)(iVar12 + 0xcc) = 0;
      *(undefined4 *)(iVar12 + 0xc0) = *(undefined4 *)(unaff_gp + 0x138);
      *(int *)(iVar12 + 200) = *(int *)(unaff_gp + 0x138) + *(int *)(iVar12 + 0xb8) * 10;
      break;
    case 7:
      *(uint *)(unaff_gp + -0xc4) = uVar9;
      break;
    case 8:
      *(uint *)(unaff_gp + 200) = uVar9;
      break;
    case 9:
      *(short *)(unaff_gp + 0xd6) = (short)((uVar7 & 0xff) << 4);
      *(short *)(unaff_gp + 0xd4) = (short)((uVar9 >> 8 & 0xff) << 4);
      *(ushort *)(unaff_gp + 0xd8) = (ushort)(uVar9 >> 0x10) & 0xff;
      break;
    case 10:
      *(char *)(unaff_gp + 0xcc) = (char)uVar9;
      break;
    case 0xb:
      *(char *)(unaff_gp + 0x85) = (char)uVar9;
    }
    break;
  case 0x12:
    goto switchD_ram_e50051ae_caseD_12;
  case 0x13:
    param_4 = param_4 & 0xff;
    if (1 < param_4) {
      FUN_ram_e5007540(s_fw_auc_fw_tx_fcs_c_ram_004724b6,0x6f,s_Invalid_wmac_id__d__ram_00472965);
      return;
    }
    if (*(byte *)(param_4 + unaff_gp + 0x96) <= *(byte *)(unaff_gp + param_4 + -0x48))
    goto switchD_ram_e50051ae_default;
    om_auc_management_fcs_enqueue(uVar7);
    break;
  case 0x14:
    FUN_ram_e50007b8(uVar7 & 0xff);
    break;
  case 0x15:
    if (uVar7 != *(uint *)(unaff_gp + 0xdc)) {
      *(uint *)(unaff_gp + 0xdc) = uVar7;
      if (uVar7 == 0) {
        *(undefined4 *)(unaff_gp + 0xe0) = *(undefined4 *)(unaff_gp + 0x138);
        if (*(int *)(unaff_gp + 0x60) != 0) {
          iVar12 = **(int **)(*(int *)(unaff_gp + 100) + 4);
          iVar8 = *(int *)(unaff_gp + 0x60);
          do {
            iVar11 = *(int *)(iVar8 + 0x18);
            FUN_ram_e5001770(iVar8,1);
            bVar13 = iVar8 != iVar12;
            iVar8 = iVar11;
          } while (bVar13 && iVar11 != 0);
        }
        DAT_ram_e5103690 = DAT_ram_e5103690 + 1;
      }
      else {
        DAT_ram_e510368c = DAT_ram_e510368c + 1;
      }
    }
    goto switchD_ram_e50051ae_caseD_12;
  case 0x16:
    DAT_ram_e5103434 = FUN_ram_e5005e50(uVar7 << 0xd,100);
    break;
  case 0x17:
    DAT_ram_e510342c = uVar7;
  }
switchD_ram_e50057be_caseD_2:
  uVar1 = 1;
LAB_ram_e5005660:
  FUN_ram_e5005db8(uVar1);
  return;
switchD_ram_e50051ae_caseD_12:
  *(uint *)(unaff_gp + 0xe4) = uVar7;
  goto switchD_ram_e50057be_caseD_2;
}

