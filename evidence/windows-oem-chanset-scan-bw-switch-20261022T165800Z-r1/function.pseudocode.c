
void FUN_ram_00013418(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 1;
  do {
    iVar1 = ieee80211_dual_sec_chan_supported(param_1,iVar2);
    if (iVar1 != 0) {
      iVar1 = findchannel_any(param_1,iVar2,*(undefined2 *)(param_1 + 0x4e30));
      if ((iVar1 == 0) || (iVar1 == 0xffff)) {
        if (-1 < *(int *)(param_1 + 0x5e08)) {
          (*(code *)&printk)("%s: fail to find channel %d\n","ieee80211_set_chanset_sec_chan",iVar2)
          ;
        }
      }
      else {
        if (1 < *(int *)(param_1 + 0x5e08)) {
          (*(code *)&printk)("%s: set sec_chan_offset to %d for channel %d\n",
                             "ieee80211_set_chanset_sec_chan",param_2,iVar2);
        }
        ieee80211_update_sec_chan_offset(iVar1,param_2);
      }
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 != 0xf);
  return;
}

