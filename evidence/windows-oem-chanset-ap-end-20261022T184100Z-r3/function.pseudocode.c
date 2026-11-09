
bool om_oem_scanap_end(int *param_1,undefined4 *param_2,int param_3,undefined4 param_4)

{
  ushort uVar1;
  uint uVar2;
  int iVar3;
  undefined2 *puVar4;
  uint uVar5;
  int iVar6;
  bool bVar7;
  int iVar8;
  undefined1 auStack_f0 [128];
  undefined2 *local_70;
  
  iVar6 = param_2[0x41];
  iVar8 = param_1[2];
  if (param_2[0x112] != 6) {
    (*(code *)&printk)("wrong opmode %u");
    (*(code *)&show_stacktrace)(0,0);
    (*(code *)&printk)("Kernel BUG in file %s: function %s: line %d!\n",
                       "../drivers/wlan/ieee80211_scan_ap.c","ap_end",0x3f6);
  }
  uVar1 = *(ushort *)(param_1 + 4);
  if ((uVar1 & 0x200) != 0) {
    return true;
  }
  if ((uVar1 & 0x40) != 0) {
    (**(code **)(iVar6 + 0x547c))();
    return true;
  }
  if ((uVar1 & 0x80) == 0) {
    (*(code *)&memset)(auStack_f0,0,0xd0);
    iVar3 = (**(code **)(iVar6 + 0x54a4))(iVar6);
    if (0 < iVar3) {
      if ((*(uint *)(*param_1 + 0x108) & 0x200000) == 0) {
        param_4 = 0x110000;
      }
      else {
        param_4 = 0x110000;
        (*(code *)&ieee80211_note)(*param_1,"%s: pick dfs channels only: for eu ICAC\n","ap_end");
      }
    }
    local_70 = (undefined2 *)om_oem_scanap_pick_channel(iVar6,param_1,param_4);
    if (local_70 == (undefined2 *)0x0) {
      if ((*(uint *)(*param_1 + 0x108) & 0x200000) != 0) {
        (*(code *)&ieee80211_note)(*param_1,"%s: no suitable channel! Go back!\n",*param_2);
      }
      iVar3 = (**(code **)(iVar6 + 0x54a4))(iVar6);
      if (0 < iVar3) {
        return false;
      }
      if (*(undefined2 **)(iVar6 + 0x4e0c) == (undefined2 *)0xffff) {
        iVar6 = (*(code *)&ieee80211_chanset_scan_finished)(iVar6);
        return iVar6 == 0;
      }
      bVar7 = false;
      local_70 = *(undefined2 **)(iVar6 + 0x4e0c);
    }
    else {
      if (((*(uint *)(local_70 + 2) & 0x2010) == 0x10) &&
         (local_70 = (undefined2 *)
                     (*(code *)&ieee80211_find_channel)
                               (iVar6,*local_70,*(uint *)(local_70 + 2) & 0xffffffef),
         local_70 == (undefined2 *)0x0)) {
        return false;
      }
      if ((*(undefined2 **)(iVar6 + 0x4e0c) == (undefined2 *)0xffff) ||
         ((*(ushort *)(param_1 + 4) & 1) == 0)) {
        bVar7 = true;
      }
      else {
        bVar7 = true;
        local_70 = *(undefined2 **)(iVar6 + 0x4e0c);
      }
    }
    if ((*(int *)(iVar6 + 0x4e24) != 0) && (*(int *)(iVar6 + 0x4e28) == 0)) {
      puVar4 = (undefined2 *)(*(code *)&ieee80211_find_channel_by_ieee)(iVar6);
      *(undefined4 *)(iVar6 + 0x4e2c) = 0;
      iVar3 = (**(code **)(iVar6 + 0x53c8))(iVar6,puVar4,0,1);
      if ((iVar3 != 0) &&
         (((int)(uint)*(byte *)(iVar6 + (uint)(*(byte *)(puVar4 + 4) >> 3) + 0x4cd0) >>
           (*(byte *)(puVar4 + 4) & 7) & 1U) == 0)) {
        local_70 = puVar4;
      }
      *(undefined4 *)(iVar6 + 0x4e24) = 0;
    }
    *(undefined2 **)(iVar6 + 0x4e1c) = local_70;
    if (param_3 == 0) {
      param_3 = *(int *)(param_1[1] + 0x44);
    }
    *(int *)(iVar8 + 0x21c4) = param_3;
    (*(code *)&memcpy)(iVar8 + 0x20f4,auStack_f0,0xd0);
    uVar2 = Auxregs0028;
    if ((uVar2 & 6) != 0) {
      Auxregs0028 = uVar2 & 0xfffffff9;
    }
    uVar5 = *(uint *)(iVar8 + 0x20e4);
    *(uint *)(iVar8 + 0x20e4) = uVar5 | 1;
    Auxregs0028 = uVar2;
    if ((uVar5 & 1) == 0) {
      (*(code *)&__tasklet_schedule)();
    }
    return bVar7;
  }
  *(undefined2 *)((int)param_1 + 0x12) = 0x101;
  return true;
}

