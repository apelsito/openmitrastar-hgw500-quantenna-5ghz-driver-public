
void ieee80211_init_chanset_ranking_params(int param_1)

{
  undefined *puVar1;
  
  if ((*(int *)(param_1 + 0x14) - 2U < 2) || (*(int *)(param_1 + 0x14) - 8U < 2)) {
    puVar1 = &DAT_ram_00086120;
  }
  else if (*(short *)(param_1 + 0x5c0) == 0) {
    puVar1 = &DAT_ram_00086144;
  }
  else {
    puVar1 = &DAT_ram_00086168;
  }
  (*(code *)&memcpy)(param_1 + 0x5db8,puVar1,0x24);
  if (1 < *(int *)(param_1 + 0x5e08)) {
    (*(code *)&printk)("Chanset ranking params:\ncci_instnt_factor\t%d\naci_instnt_factor\t%d\ncci_longterm_factor\t%d\naci_longterm_factor\t%d\nrange_factor\t\t%d\ndfs_factor\t\t%d\nmin_cochan_rssi\t\t%d\nmaxbw_minbenefit\t\t%d\ndense_cci_span\t\t%dMHz\n"
                       ,*(undefined4 *)(param_1 + 0x5db8),*(undefined4 *)(param_1 + 0x5dbc),
                       *(undefined4 *)(param_1 + 24000),*(undefined4 *)(param_1 + 0x5dc4),
                       *(undefined4 *)(param_1 + 0x5dc8),*(undefined4 *)(param_1 + 0x5dcc),
                       *(undefined4 *)(param_1 + 0x5dd0),*(undefined4 *)(param_1 + 0x5dd4),
                       *(undefined4 *)(param_1 + 0x5dd8));
  }
  return;
}

