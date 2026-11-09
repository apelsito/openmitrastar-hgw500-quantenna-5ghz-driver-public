
undefined4 om_oem_vopt_effective_apply(int param_1,char param_2)

{
  char cVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  uint local_1c;
  
  puVar3 = *(undefined4 **)(param_1 + 8);
  uVar4 = *puVar3;
  local_1c = 0;
  *(char *)(param_1 + 0x4df5) = param_2;
  if (param_2 == '\0') {
    if (*(char *)(param_1 + 0x4df6) != '\0') {
      ieee80211_param_to_qdrv(puVar3,0x4e,*(char *)(param_1 + 0x4df6),0,0);
      ieee80211_param_to_qdrv(puVar3,0x44,0x11,0,0);
    }
    cVar1 = *(char *)(param_1 + 0x4df8);
    if (cVar1 != '\0') {
      ieee80211_param_to_qdrv(puVar3,0x9e,cVar1,0,0);
      *(char *)(param_1 + 0xbe1) = *(char *)(param_1 + 0x4df8);
    }
    if (*(uint *)(param_1 + 0x4dfc) != 0) {
      local_1c = *(uint *)(param_1 + 0x4dfc) | 0x10000;
      iVar2 = ieee80211_param_scs_set(uVar4,puVar3,local_1c);
      if (iVar2 == 0) {
        ieee80211_param_to_qdrv(puVar3,0x93,local_1c,0,0);
      }
    }
    if (*(int *)(&DAT_ram_00004e00 + param_1) != 0) {
      local_1c = *(ushort *)(param_1 + 0x8fc) | 0x10000;
      ieee80211_param_ocac_set(uVar4,puVar3,local_1c);
    }
    if ((*(int *)(param_1 + 0x4e04) != 0) && (*(code **)(param_1 + 0x5548) != (code *)0x0)) {
      (**(code **)(param_1 + 0x5548))(param_1,0,1);
    }
    if (*(char *)(param_1 + 0x4df7) != '\0') {
      ieee80211_param_to_qdrv(puVar3,0xc3,*(char *)(param_1 + 0x4df7),0,0);
    }
    cVar1 = *(char *)(param_1 + 0x4df9);
    if (cVar1 != '\0') {
      *(char *)(param_1 + 0x505d) = cVar1;
      ieee80211_param_to_qdrv(puVar3,0x120,cVar1,0,0);
    }
    ieee80211_param_to_qdrv(puVar3,0x1e2,(int)*(char *)(*(int *)(param_1 + 0x4e08) + 0xc),0,0);
    *(undefined1 *)(param_1 + 0x535e) = 0;
    ieee80211_param_to_qdrv(puVar3,0x19e,0,0,0);
  }
  else {
    ieee80211_param_to_qdrv(puVar3,0x44,0x10,0,0);
    ieee80211_param_to_qdrv(puVar3,0x4e,0,0,0);
    if (((*(uint *)(param_1 + 0x98) & 0x40000000) == 0) ||
       ((*(uint *)(param_1 + 0x9c) & 0x800000) == 0)) {
      ieee80211_param_to_qdrv(puVar3,0x9e,0,0,0);
    }
    *(undefined1 *)(param_1 + 0xbe1) = 0;
    ieee80211_param_to_qdrv(puVar3,0xc3,0,0,0);
    *(undefined1 *)(param_1 + 0x505d) = 0;
    ieee80211_param_to_qdrv(puVar3,0x120,0,0,0);
    local_1c = 0x10000;
    iVar2 = ieee80211_param_scs_set(uVar4,puVar3);
    if (iVar2 == 0) {
      ieee80211_param_to_qdrv(puVar3,0x93,local_1c,0,0);
    }
    local_1c = 0x20000;
    ieee80211_param_ocac_set(uVar4,puVar3);
    if (((*(code **)(param_1 + 0x554c) != (code *)0x0) && (*(int *)(param_1 + 0x5548) != 0)) &&
       (iVar2 = (**(code **)(param_1 + 0x554c))(param_1,0,&local_1c), iVar2 == 0)) {
      *(uint *)(param_1 + 0x4e04) = local_1c;
      (**(code **)(param_1 + 0x5548))(param_1,0,0);
    }
    ieee80211_param_to_qdrv(puVar3,0x1e2,0,0,0);
    *(undefined1 *)(param_1 + 0x535e) = 3;
    ieee80211_param_to_qdrv(puVar3,0x19e,3,0,0);
  }
  ieee80211_wireless_reassoc(puVar3,0,0);
  return 0;
}

