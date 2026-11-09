
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void om_oem_vopt_peer_policy(int param_1)

{
  char cVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  int *piVar11;
  int iVar12;
  int *piVar13;
  char *pcVar14;
  int local_40;
  int local_3c;
  
  iVar12 = *(int *)(param_1 + 0x4e5c);
  uVar3 = Auxregs0028;
  if ((uVar3 & 6) != 0) {
    Auxregs0028 = uVar3 & 0xfffffff9;
  }
  uVar4 = Auxregs0028;
  if ((uVar4 & 6) != 0) {
    Auxregs0028 = uVar4 & 0xfffffff9;
  }
  piVar13 = (int *)(param_1 + 0x4e60);
  piVar9 = (int *)*piVar13;
  if (piVar9 != (int *)0x0) {
    pcVar14 = (char *)(iVar12 + 0x8cc);
    do {
      if (piVar9 == *(int **)(*piVar9 + 0x514)) {
LAB_ram_00076c10:
        piVar9 = (int *)piVar9[3];
      }
      else {
        piVar10 = piVar9 + 0x28b;
        iVar6 = (*(code *)&memcmp)(*piVar9 + 0x42c,piVar10,6);
        if ((iVar6 == 0) || (iVar6 = ieee80211_blacklist_check(piVar9), iVar6 != 0))
        goto LAB_ram_00076c10;
        if ((piVar9[0x30] != 0) && ((piVar9[0x2f] + 200) - _jiffies < 0)) {
          (*(code *)&consume_skb)();
          piVar9[0x30] = 0;
        }
        iVar6 = *piVar9;
        if ((*(uint *)(iVar6 + 0x108) & 0x1000080) != 0) {
          iVar8 = *(int *)(piVar9[0x9c] + 0x2c);
          iVar7 = *(int *)(piVar9[0x9c] + 0x128);
          ieee80211_note_mac(iVar6,piVar10,"sta ni_inact=%u rx=%u/%u/%u ack=%u/%u/%u",
                             (int)(short)piVar9[0x2cb],iVar7,piVar9[0x39a],iVar7 - piVar9[0x39a],
                             iVar8,piVar9[0x39b],iVar8 - piVar9[0x39b]);
          iVar6 = *piVar9;
        }
        piVar11 = piVar9 + 0x2cb;
        if ((*(int *)(iVar6 + 0x448) != 2) ||
           (iVar6 = (*(code *)&memcmp)(piVar10,iVar6 + 0x91a,6), iVar6 != 0)) {
          iVar6 = piVar9[0x9c];
          if ((*(int *)(iVar6 + 0x128) == piVar9[0x39a]) &&
             (*(int *)(iVar6 + 0x2c) == piVar9[0x39b])) {
            if (0 < (short)*piVar11) {
              *(short *)piVar11 = (short)*piVar11 + -1;
            }
          }
          else {
            *(short *)piVar11 = *(short *)((int)piVar9 + 0xb2e);
          }
          piVar9[0x39a] = *(int *)(iVar6 + 0x128);
          piVar9[0x39b] = *(int *)(iVar6 + 0x2c);
          if ((short)*piVar11 < 1) {
            uVar5 = Auxregs0028;
            if ((uVar5 & 6) != 0) {
              Auxregs0028 = uVar5 & 0xfffffff9;
            }
            piVar9[9] = piVar9[9] + 1;
            Auxregs0028 = uVar5;
            iVar6 = (*(code *)&schedule_work)(piVar9 + 0x2cc);
            if (iVar6 == 0) {
              ieee80211_free_node(piVar9);
              piVar9 = (int *)piVar9[3];
              goto LAB_ram_00076c14;
            }
          }
          else {
            if ((*(short *)((int)piVar9 + 0x3a) == 0) && (*(int *)(iVar12 + 0x24) != 0)) {
              if (*(int *)(iVar12 + 0x24) != 3) goto LAB_ram_00076c10;
              iVar6 = *piVar9;
            }
            else {
              iVar6 = *piVar9;
            }
            if (((int)(short)*piVar11 <= *(int *)(iVar6 + 0x488)) || (*pcVar14 != '\0')) {
              uVar5 = Auxregs0028;
              if ((uVar5 & 6) != 0) {
                Auxregs0028 = uVar5 & 0xfffffff9;
              }
              piVar9[9] = piVar9[9] + 1;
              Auxregs0028 = uVar5;
              iVar6 = (*(code *)&schedule_work)(piVar9 + 0x2cc);
              if (iVar6 == 0) {
                ieee80211_free_node(piVar9);
                piVar9 = (int *)piVar9[3];
                goto LAB_ram_00076c14;
              }
            }
          }
          goto LAB_ram_00076c10;
        }
        if ((0 < (short)*piVar11) &&
           ((*(int *)(iVar12 + 0x4e0c) == 0xffff ||
            ((*(uint *)(*(int *)(iVar12 + 0x4e0c) + 4) & 0x1100000) != 0x1100000)))) {
          *(short *)piVar11 = (short)*piVar11 + -1;
        }
        uVar5 = Auxregs0028;
        if ((uVar5 & 6) != 0) {
          Auxregs0028 = uVar5 & 0xfffffff9;
        }
        piVar9[9] = piVar9[9] + 1;
        Auxregs0028 = uVar5;
        iVar6 = (*(code *)&schedule_work)(piVar9 + 0x2cc);
        if (iVar6 != 0) goto LAB_ram_00076c10;
        ieee80211_free_node(piVar9);
        piVar9 = (int *)piVar9[3];
      }
LAB_ram_00076c14:
    } while (piVar9 != (int *)0x0);
  }
  Auxregs0028 = uVar4;
  Auxregs0028 = uVar3;
  for (iVar12 = *(int *)(param_1 + 8); iVar12 != 0; iVar12 = *(int *)(iVar12 + 0xf8)) {
    while ((*(int *)(iVar12 + 0x448) != 1 || (*(int *)(iVar12 + 0x44c) != 4))) {
      iVar12 = *(int *)(iVar12 + 0xf8);
      if (iVar12 == 0) goto LAB_ram_00076e58;
    }
    piVar9 = *(int **)(iVar12 + 0x514);
    uVar3 = Auxregs0028;
    if ((uVar3 & 6) != 0) {
      Auxregs0028 = uVar3 & 0xfffffff9;
    }
    piVar9[9] = piVar9[9] + 1;
    Auxregs0028 = uVar3;
    if ((*(uint *)(*piVar9 + 0x108) & 0x1000080) != 0) {
      ieee80211_note_mac(*piVar9,piVar9 + 0x28b,"ap ni_inact=%u ack=%u/%u/%u",
                         (int)(short)piVar9[0x2cb],*(int *)(piVar9[0x9c] + 0x2c),piVar9[0x39b],
                         *(int *)(piVar9[0x9c] + 0x2c) - piVar9[0x39b]);
    }
    piVar10 = piVar9 + 0x2cb;
    if (*(int *)(piVar9[0x9c] + 0x2c) == piVar9[0x39b]) {
      if (0 < (short)*piVar10) {
        *(short *)piVar10 = (short)*piVar10 + -1;
      }
    }
    else {
      *(short *)piVar10 = *(short *)((int)piVar9 + 0xb2e);
    }
    piVar9[0x39b] = *(int *)(piVar9[0x9c] + 0x2c);
    ieee80211_send_nulldata(piVar9);
  }
LAB_ram_00076e58:
  cVar1 = *(char *)(param_1 + 0x4df5);
  iVar12 = DAT_ram_00093734;
  if (*(char *)(param_1 + 0x4df4) != cVar1) {
    local_3c = 0;
    local_40 = 0;
    (*(code *)&local_bh_disable)();
    iVar6 = *(int *)(param_1 + 0x4f6c) + 1;
    *(int *)(param_1 + 0x4f6c) = iVar6;
    iVar12 = *piVar13;
    while (iVar12 != 0) {
      while (iVar6 == *(int *)(iVar12 + 0x28)) {
        iVar12 = *(int *)(iVar12 + 0xc);
        if (iVar12 == 0) goto LAB_ram_00076f14;
      }
      *(int *)(iVar12 + 0x28) = iVar6;
      uVar3 = Auxregs0028;
      if ((uVar3 & 6) != 0) {
        Auxregs0028 = uVar3 & 0xfffffff9;
      }
      *(int *)(iVar12 + 0x24) = *(int *)(iVar12 + 0x24) + 1;
      Auxregs0028 = uVar3;
      FUN_ram_0006db88(&local_40,iVar12);
      ieee80211_free_node(iVar12);
      iVar12 = *piVar13;
    }
LAB_ram_00076f14:
    (*(code *)&local_bh_enable)();
    cVar2 = *(char *)(param_1 + 0x4df4);
    if (cVar2 == '\x01') {
      om_oem_vopt_effective_apply(param_1,1);
      DAT_ram_00093734 = 0;
      iVar12 = DAT_ram_00093734;
    }
    else if (cVar2 == '\0') {
      om_oem_vopt_effective_apply(param_1,0);
      DAT_ram_00093734 = 0;
      iVar12 = DAT_ram_00093734;
    }
    else {
      iVar12 = DAT_ram_00093734;
      if ((cVar2 == '\x02') && (iVar12 = DAT_ram_00093734 + -1, DAT_ram_00093734 == 0)) {
        if ((local_40 == local_3c) && (cVar1 != '\x01')) {
          om_oem_vopt_effective_apply(param_1,1);
        }
        iVar12 = DAT_ram_00093734;
        if ((local_3c != local_40) && (cVar1 != '\0')) {
          om_oem_vopt_effective_apply(param_1,0);
          DAT_ram_00093734 = 10;
          iVar12 = DAT_ram_00093734;
        }
      }
    }
  }
  DAT_ram_00093734 = iVar12;
  *(int *)(param_1 + 0x50) = _jiffies + 1000;
  (*(code *)&add_timer)(param_1 + 0x48);
  return;
}

