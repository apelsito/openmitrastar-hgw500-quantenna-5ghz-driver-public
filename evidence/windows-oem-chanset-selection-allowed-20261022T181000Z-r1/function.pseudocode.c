
bool ieee80211_chan_selection_allowed(int param_1)

{
  bool bVar1;
  
  if (*(int *)(param_1 + 0x24) == 6) {
    bVar1 = *(int *)(param_1 + 0x14) - 2U < 2 || *(int *)(param_1 + 0x14) - 8U < 2;
  }
  else {
    bVar1 = false;
  }
  return bVar1;
}

