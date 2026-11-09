
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 qdrv_wlan_init(undefined4 *param_1,int param_2,uint param_3,undefined4 param_4)

{
  byte bVar1;
  undefined1 uVar2;
  ushort uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint *puVar7;
  int iVar8;
  byte *pbVar9;
  undefined4 uVar10;
  int *piVar11;
  uint uVar12;
  int iVar13;
  undefined1 *puVar14;
  int *piVar15;
  undefined4 *puVar16;
  undefined4 *puVar17;
  byte *pbVar18;
  undefined4 *puVar19;
  undefined2 *puVar20;
  undefined1 *puVar21;
  byte *pbVar22;
  undefined2 uStack_144;
  byte local_140 [200];
  undefined *puStack_78;
  undefined4 local_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  byte *local_3c;
  
  if ((((_g_dbg_log_module & 1) != 0) && (9 < _g_dbg_log_level)) && ((_g_dbg_log_func & 1) != 0)) {
    (*(code *)&printk)("%s: -->Enter\n","qdrv_wlan_init");
  }
  iVar4 = (*(code *)&kmem_cache_alloc)(_DAT_ram_0f000ffc,0xd0);
  if (iVar4 == 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to allocate wlan structure\n","qdrv_wlan_init");
    }
    if ((_g_dbg_log_module & 1) == 0) {
      return 0xfffffff4;
    }
    if (_g_dbg_log_level < 10) {
      return 0xfffffff4;
    }
    if ((_g_dbg_log_func & 1) != 0) {
      (*(code *)&printk)("%s: <--Exit\n","qdrv_wlan_init");
      return 0xfffffff4;
    }
    return 0xfffffff4;
  }
  if ((_g_dbg_log_module & 1) != 0) goto LAB_ram_00044f48;
LAB_ram_0004498c:
  (*(code *)&memset)(iVar4,0,0x8004);
  param_1[0x7f] = iVar4;
  piVar15 = (int *)(iVar4 + 0x5e3c);
  *(uint *)(iVar4 + 0x5e34) = *(uint *)(iVar4 + 0x5e34) | 0x28;
  (*(code *)&reg_congest_queue_stats)(qdrv_update_cgq_stats,iVar4);
  *piVar15 = (int)param_1;
  uVar10 = *param_1;
  pbVar18 = (byte *)(iVar4 + 0x5e3a);
  *pbVar18 = (byte)(param_3 >> 8) & 0xf;
  *(undefined4 *)(iVar4 + 0x5e20) = uVar10;
  *(char *)(iVar4 + 0x5e3b) = (char)((param_3 & 0xf000) >> 0xc);
  *(undefined2 *)(iVar4 + 0x5e38) = 0;
  *(int *)(iVar4 + 0x5f50) = param_1[0x82] + 0x94;
  *(byte *)(_soc_shared_params + 6) = *pbVar18;
  iVar5 = (*(code *)&strcmp)("topaz_dbdc_config","qtm710_rgmii_config");
  if (((iVar5 == 0) ||
      (iVar5 = (*(code *)&strcmp)("topaz_dbdc_config","topaz_rgmii_config"), iVar5 == 0)) ||
     ((iVar5 = (*(code *)&strcmp)("topaz_dbdc_config","topaz_vzn_config"), iVar5 == 0 ||
      (iVar5 = (*(code *)&strcmp)("topaz_dbdc_config","topaz_pcie_config"), iVar5 == 0)))) {
    *(undefined2 *)(iVar4 + 0x7fba) = 1;
  }
  else {
    *(undefined2 *)(iVar4 + 0x7fba) = 0;
  }
  *(undefined1 *)(iVar4 + 0x7fbe) = 0;
  *(undefined2 *)(iVar4 + 0x7fbc) = 0;
  iVar5 = (*(code *)&dev_get_by_name)(&init_net,&DAT_ram_000a07fc);
  *(int *)(iVar4 + 0x6e28) = iVar5;
  if ((iVar5 == 0) && (1 < _g_dbg_log_level)) {
    (*(code *)&printk)("%s: ERROR - Could not get bridge device\n","qdrv_wlan_init");
  }
  qdrv_br_create(iVar4 + 0x6ea4);
  *(undefined4 *)(iVar4 + 0x7ec8) = 6;
  iVar13 = -1;
  *(undefined4 *)(iVar4 + 0x7ecc) = 0xc;
  *(undefined4 *)(iVar4 + 0x7ed0) = 5;
  *(undefined4 *)(iVar4 + 0x7ec0) = 0;
  *(undefined4 *)(iVar4 + 0x7ec4) = 0;
  *(undefined1 *)(iVar4 + 0x7ed4) = 0xff;
  iVar5 = qdrv_sch_shared_data_init(0x400,0x7f);
  if (iVar5 == 0) {
    (*(code *)&panic)("%s: could not allocate tx_sch shared data\n","qdrv_wlan_tx_sch_init");
    goto LAB_ram_000477cc;
  }
  *(int *)(iVar4 + 0x7fb4) = iVar5;
  *(undefined1 *)(iVar4 + 0x7ed5) = 0xff;
  *(int *)(iVar4 + 0x7f84) = _jiffies;
  *(undefined1 *)(iVar4 + 0x7ed6) = 0xff;
  uStack_70 = 0;
  puStack_78 = &DAT_ram_0003d710;
  local_74 = iVar4;
  iVar5 = qdrv_mac_set_handler(*piVar15,10,&puStack_78);
  if (iVar5 == 0) {
    uVar6 = Auxregs0028;
    if ((uVar6 & 6) != 0) {
      Auxregs0028 = uVar6 & 0xfffffff9;
    }
    **(uint **)(*piVar15 + 0x11c) = **(uint **)(*piVar15 + 0x11c) | 0x400;
    Auxregs0028 = uVar6;
  }
  else if (1 < _g_dbg_log_level) {
    (*(code *)&printk)("%s: ERROR - Could not set csa irq handler\n","qdrv_init_csa_irqhandler");
  }
  *(undefined4 *)(iVar4 + 0x7f60) = 0;
  iVar5 = iVar4 + 0x7f64;
  *(int *)(iVar4 + 0x7f68) = iVar5;
  *(int *)iVar5 = iVar5;
  *(undefined **)(iVar4 + 0x7f6c) = &DAT_ram_0003d59c;
  *(undefined4 *)(iVar4 + 0x7f88) = 0;
  iVar5 = iVar4 + 0x7f8c;
  *(int *)(iVar4 + 0x7f90) = iVar5;
  *(int *)iVar5 = iVar5;
  *(undefined **)(iVar4 + 0x7f94) = &DAT_ram_00039480;
  *(undefined4 *)(iVar4 + 0x7f70) = 0;
  iVar5 = iVar4 + 0x7f74;
  *(int *)(iVar4 + 0x7f78) = iVar5;
  *(int *)iVar5 = iVar5;
  *(undefined1 **)(iVar4 + 0x7f7c) = &LAB_ram_00039454;
  uStack_70 = 0;
  puStack_78 = &DAT_ram_0003dbd8;
  local_74 = iVar4;
  iVar5 = qdrv_mac_set_handler(*piVar15,0xb,&puStack_78);
  if (iVar5 == 0) {
    uVar6 = Auxregs0028;
    if ((uVar6 & 6) != 0) {
      Auxregs0028 = uVar6 & 0xfffffff9;
    }
    **(uint **)(*piVar15 + 0x11c) = **(uint **)(*piVar15 + 0x11c) | 0x800;
    Auxregs0028 = uVar6;
  }
  else if (1 < _g_dbg_log_level) {
    (*(code *)&printk)("%s: ERROR - Could not set cca irq handler\n","qdrv_init_cca_irqhandler");
  }
  *(undefined4 *)(iVar4 + 0x7f2c) = 0;
  iVar5 = iVar4 + 0x7f30;
  *(int *)(iVar4 + 0x7f34) = iVar5;
  *(int *)iVar5 = iVar5;
  *(undefined **)(iVar4 + 0x7f38) = &DAT_ram_0003d904;
  uStack_70 = 0;
  puStack_78 = &DAT_ram_00036e40;
  local_74 = iVar4;
  iVar5 = qdrv_mac_set_handler(*piVar15,8,&puStack_78);
  if (iVar5 == 0) {
    uVar6 = Auxregs0028;
    if ((uVar6 & 6) != 0) {
      Auxregs0028 = uVar6 & 0xfffffff9;
    }
    **(uint **)(*piVar15 + 0x11c) = **(uint **)(*piVar15 + 0x11c) | 0x100;
    Auxregs0028 = uVar6;
  }
  else if (1 < _g_dbg_log_level) {
    (*(code *)&printk)("%s: ERROR - Could not set measurement irq handler\n",
                       "qdrv_init_meas_irqhandler");
  }
  *(undefined4 *)(iVar4 + 0x7f3c) = 0;
  iVar5 = iVar4 + 0x7f40;
  *(int *)(iVar4 + 0x7f44) = iVar5;
  *(int *)iVar5 = iVar5;
  *(undefined1 **)(iVar4 + 0x7f48) = &LAB_ram_0003dc34;
  uStack_70 = 0;
  puStack_78 = &DAT_ram_0003e19c;
  local_74 = iVar4;
  iVar5 = qdrv_mac_set_handler(*piVar15,0xc,&puStack_78);
  if (iVar5 == 0) {
    uVar6 = Auxregs0028;
    if ((uVar6 & 6) != 0) {
      Auxregs0028 = uVar6 & 0xfffffff9;
    }
    **(uint **)(*piVar15 + 0x11c) = **(uint **)(*piVar15 + 0x11c) | 0x1000;
    Auxregs0028 = uVar6;
  }
  else if (1 < _g_dbg_log_level) {
    (*(code *)&printk)("%s: ERROR - BG_SCAN: could not set irq handler\n",
                       "qdrv_init_scan_irqhandler");
  }
  *(undefined4 *)(iVar4 + 0x7f4c) = 0;
  iVar5 = iVar4 + 0x7f50;
  *(int *)(iVar4 + 0x7f54) = iVar5;
  *(int *)iVar5 = iVar5;
  *(undefined1 **)(iVar4 + 0x7f58) = &LAB_ram_0003e04c;
  param_3 = *(uint *)(param_2 + 0x1c);
  iVar5 = *piVar15;
  uVar6 = nocache_to_muc(param_3);
  piVar15 = (int *)(iVar4 + 0x6e38);
  if (uVar6 != 0) {
    param_3 = uVar6;
  }
  uVar10 = bus_to_virt(param_3);
  iVar13 = (*(code *)&ioremap_nocache)(uVar10,0x2008);
  *piVar15 = iVar13;
  if (iVar13 != 0) {
    (*(code *)&tasklet_init)(iVar4 + 0x6e3c,&DAT_ram_00042a08,iVar4);
    *(undefined4 *)(iVar4 + 0x6e78) = 0;
    iVar13 = iVar4 + 0x6e7c;
    *(int *)(iVar4 + 0x6e80) = iVar13;
    *(int *)iVar13 = iVar13;
    *(undefined1 **)(iVar4 + 0x6e84) = &LAB_ram_00036eb0;
    (*(code *)&init_timer_key)(iVar4 + 0x6e88,0,0);
    uStack_70 = 0;
    puStack_78 = &DAT_ram_00036e50;
    local_74 = iVar4;
    param_3 = qdrv_mac_set_handler(iVar5,0xd,&puStack_78);
    if (param_3 == 0) goto LAB_ram_000450ec;
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Could not initialize VSP update irq handler\n",
                         "qdrv_wlan_vsp_irq_init");
    }
    (*(code *)&iounmap)(*piVar15);
  }
  (*(code *)&panic)("Could not initialize VSP stats IRQ");
LAB_ram_00044f48:
  if ((5 < _g_dbg_log_level) && ((_g_dbg_log_func & 0x200) != 0)) {
    (*(code *)&printk)("%s: qw 0x%08x ic 0x%08x\n","qdrv_wlan_init",iVar4,iVar4);
  }
  goto LAB_ram_0004498c;
LAB_ram_000450ec:
  puVar7 = *(uint **)(iVar5 + 0x11c);
  uVar6 = Auxregs0028;
  if ((uVar6 & 6) != 0) {
    Auxregs0028 = uVar6 & 0xfffffff9;
  }
  *puVar7 = *puVar7 | 0x2000;
  Auxregs0028 = uVar6;
  iVar5 = (*(code *)&dma_alloc_coherent)(0,0x10048,iVar4 + 0x7f9c,0x80d1);
  piVar15 = (int *)(iVar4 + 0x7f98);
  *piVar15 = iVar5;
  if (iVar5 == 0) {
    if (_g_dbg_log_level < 2) {
      return 0xfffffff4;
    }
    (*(code *)&printk)("%s: ERROR - Failed to allocate per node stats pool","qdrv_wlan_init");
    return 0xfffffff4;
  }
  piVar11 = (int *)(iVar4 + 0x7fa4);
  iVar13 = 0x76;
  *(undefined4 *)(iVar4 + 0x7fa0) = 0;
  *piVar11 = iVar4 + 0x7fa0;
  iVar5 = 0;
  _DAT_auxregs_0008 = 0x4516c;
  _DAT_auxregs_000c = 0x451b0;
  do {
    *(undefined4 *)(*piVar15 + iVar5 + 0x224) = 0;
    *(int *)(*piVar15 + iVar5 + 0x228) = *piVar11;
    *(int *)*piVar11 = *piVar15 + iVar5;
    iVar8 = *piVar15 + iVar5;
    iVar5 = iVar5 + 0x22c;
    *piVar11 = iVar8 + 0x224;
    iVar13 = iVar13 + -1;
  } while (iVar13 != 0);
  iVar5 = qdrv_tx_init(param_1,param_2,param_4);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to initialize TX\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = qdrv_rx_init(iVar4,param_2);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to initialize RX\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = qdrv_scan_init(iVar4,param_2);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to initialize Scan\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = qdrv_hostlink_init(iVar4,param_2);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to initialize hostlink\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = qdrv_txbf_init(iVar4);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to initialize beamforming\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = qdrv_rx_start(param_1);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to start RX\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = qdrv_tx_start(param_1);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to start TX\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = qdrv_scan_start(param_1);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to start Scan\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = qdrv_hostlink_start(param_1);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to start hostlink\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = FUN_ram_00034628(iVar4,1);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to set A rates\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = FUN_ram_00034628(iVar4,2);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to set B rates\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = FUN_ram_00034628(iVar4,3);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to set G rates\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = FUN_ram_00034628(iVar4,8);
  if ((iVar5 < 0) || (iVar5 = FUN_ram_00034628(iVar4,9), iVar5 < 0)) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to set NG rates\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = FUN_ram_00034628(iVar4,7);
  if ((iVar5 < 0) || (iVar5 = FUN_ram_00034628(iVar4,10), iVar5 < 0)) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to set NA rates\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = FUN_ram_00034628(iVar4,0xb);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to set AC rates VHT20\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = FUN_ram_00034628(iVar4,0xc);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to set AC rates VHT40\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  iVar5 = FUN_ram_00034628(iVar4,0xd);
  if (iVar5 < 0) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to set AC rates VHT80\n","qdrv_wlan_init");
    }
    goto LAB_ram_00045abc;
  }
  bVar1 = *pbVar18;
  if (bVar1 == 0) {
    uVar10 = 0xd;
  }
  else {
    uVar10 = 0x1e;
    if (bVar1 != 1) {
      uVar10 = 0x2b;
    }
  }
  FUN_ram_00037d88(iVar4,uVar10);
  *(undefined4 *)(iVar4 + 0x88) = 0x25040059;
  if (_soc_shared_params == 0) {
    uStack_144 = 0;
  }
  else {
    uStack_144 = (undefined2)*(undefined4 *)(_soc_shared_params + 0x38);
  }
  *(byte *)(iVar4 + 0xbf5) = bVar1;
  *(undefined2 *)(iVar4 + 0x8c) = uStack_144;
  *(code **)(iVar4 + 0x5534) = FUN_ram_00037d88;
  *(undefined2 *)(iVar4 + 0x8e) = 0x1cc;
  *(undefined1 **)(iVar4 + 0x5390) = &LAB_ram_0003e704;
  *(undefined1 **)(iVar4 + 0x5394) = &LAB_ram_00038e5c;
  *(undefined1 **)(iVar4 + 0x5398) = &LAB_ram_00036584;
  *(undefined1 **)(iVar4 + 0x538c) = &LAB_ram_00034000;
  *(undefined1 **)(iVar4 + 0x5384) = &LAB_ram_00033e54;
  *(undefined1 **)(iVar4 + 0x5380) = &LAB_ram_00033efc;
  *(undefined1 **)(iVar4 + 0x5388) = &LAB_ram_00036428;
  *(undefined1 **)(iVar4 + 0x537c) = &LAB_ram_0004ac48;
  *(code **)(iVar4 + 0x5468) = qdrv_wlan_80211_stats;
  *(undefined1 **)(iVar4 + 0x5440) = &LAB_ram_00042188;
  *(code **)(iVar4 + 0x5444) = FUN_ram_000388ec;
  *(undefined1 **)(iVar4 + 0x5448) = &LAB_ram_00035918;
  *(undefined1 **)(iVar4 + 0x5454) = &LAB_ram_0004abc4;
  *(code **)(iVar4 + 0x5458) = FUN_ram_0004aa68;
  *(undefined1 **)(iVar4 + 0x545c) = &LAB_ram_00032764;
  *(undefined1 **)(iVar4 + 0x5460) = &LAB_ram_000388dc;
  *(code **)(iVar4 + 0x5464) = qdrv_mac_reserved;
  *(code **)(iVar4 + 0x5420) = FUN_ram_00048224;
  *(undefined1 **)(iVar4 + 0x5424) = &LAB_ram_00040730;
  *(undefined **)(iVar4 + 0x5428) = &DAT_ram_0003887c;
  *(undefined **)(iVar4 + 0x542c) = &DAT_ram_00038848;
  *(undefined1 **)(iVar4 + 0x5430) = &LAB_ram_00038830;
  *(undefined1 **)(iVar4 + 0x5438) = &LAB_ram_00035ec4;
  iVar5 = *(int *)(iVar4 + 0x14);
  *(undefined1 **)(iVar4 + 0x5d38) = &LAB_ram_0003881c;
  *(undefined1 **)(iVar4 + 0x543c) = &LAB_ram_000356d8;
  *(undefined1 **)(iVar4 + 0x544c) = &LAB_ram_000362a4;
  *(undefined1 **)(iVar4 + 0x5450) = &LAB_ram_00036114;
  *(undefined1 **)(iVar4 + 0x546c) = &LAB_ram_00034fdc;
  *(undefined1 **)(iVar4 + 0x5500) = &LAB_ram_0003c7fc;
  *(undefined4 *)(iVar4 + 0x90) = 0x580abd30;
  puVar21 = (undefined1 *)(iVar4 + 0x535a);
  *(undefined1 *)(iVar4 + 0xbec) = 2;
  *(undefined4 *)(iVar4 + 0x5c98) = 0x4b;
  *(undefined4 *)(iVar4 + 0x5344) = 200;
  *(undefined1 *)(iVar4 + 0x5341) = 4;
  *(undefined2 *)(iVar4 + 0xbdc) = 0xffff;
  *(undefined1 *)(iVar4 + 0xbef) = 1;
  *(undefined1 *)(iVar4 + 0x535f) = 0;
  *(undefined ***)(iVar4 + 0x5504) = &PTR_LAB_ram_00021004_ram_00038804;
  *(undefined1 **)(iVar4 + 0x54bc) = &LAB_ram_00035c6c;
  *(undefined1 *)(iVar4 + 0x505c) = 1;
  *(undefined1 *)(iVar4 + 0x505d) = 1;
  *(undefined1 **)(iVar4 + 0x54c0) = &LAB_ram_00035a60;
  *(undefined1 **)(iVar4 + 0x5508) = &LAB_ram_00035244;
  *(undefined1 **)(iVar4 + 0x550c) = &LAB_ram_0003f464;
  *(undefined1 **)(iVar4 + 0x54c4) = &LAB_ram_00035634;
  *(undefined1 **)(iVar4 + 0x54c8) = &LAB_ram_00035590;
  *(undefined4 *)(iVar4 + 0x24) = 1;
  *(undefined2 *)(iVar4 + 0x5170) = 0;
  *(undefined2 *)(iVar4 + 0x5172) = 0;
  *(undefined1 *)(iVar4 + 0xbf2) = 0;
  *(code **)(iVar4 + 0x5410) = qdrv_wlan_80211_set_bcn_scheme;
  *(undefined1 *)(iVar4 + 0x535e) = 0;
  *(undefined1 *)(iVar4 + 0x5340) = 0;
  *(undefined4 *)(iVar4 + 0xa8) = 2;
  *(undefined4 *)(iVar4 + 0xa4) = 0xe581ed10;
  *puVar21 = 0;
  if (3 < iVar5 - 0xbU) {
    if (iVar5 == 7) {
      *(undefined4 *)(iVar4 + 0x14) = 0xb;
    }
    else if (iVar5 == 10) {
      *(undefined4 *)(iVar4 + 0x14) = 0xc;
    }
    else {
      *(undefined4 *)(iVar4 + 0x14) = 0xd;
    }
  }
  iVar13 = (*(code *)&ieee80211_swfeat_is_supported)(4,1);
  if (iVar13 == 0) {
    *(int *)(iVar4 + 0x14) = iVar5;
  }
  if ((((_g_dbg_log_module & 1) != 0) && (9 < _g_dbg_log_level)) && ((_g_dbg_log_func & 0x200) != 0)
     ) {
    (*(code *)&printk)("%s: 802.11ac mode = %d\n","qdrv_wlan_80211_set_11ac_mode",
                       *(undefined4 *)(iVar4 + 0x14));
  }
  iVar5 = _soc_shared_params;
  *(undefined1 *)(iVar4 + 0x503) = 2;
  *(ushort *)(iVar4 + 0x500) = *(ushort *)(iVar4 + 0x500) | 0x9e2;
  *(undefined1 *)(iVar4 + 0x504) = 3;
  *(undefined2 *)(iVar4 + 0x506) = 0xeff;
  iVar13 = (*(code *)&board_slow_ethernet)();
  if (iVar13 == 0) {
    *(undefined2 *)(iVar4 + 0x508) = 3;
LAB_ram_000458d8:
    uVar3 = *(ushort *)(iVar5 + 4);
  }
  else {
    *(undefined2 *)(iVar4 + 0x508) = 0;
    if ((((_g_dbg_log_module & 1) == 0) || (_g_dbg_log_level < 10)) || ((_g_dbg_log_func & 1) == 0))
    goto LAB_ram_000458d8;
    (*(code *)&printk)("%s: Slow Ethernet WAR: RXAMPDU %d bytes\n","qdrv_wlan_80211_cfg_ht",0);
    uVar3 = *(ushort *)(iVar5 + 4);
  }
  if (uVar3 < 0x40) {
    *(undefined1 *)(iVar4 + 0x505) = 6;
  }
  else {
    *(undefined1 *)(iVar4 + 0x505) = 5;
  }
  *(undefined1 *)(iVar4 + 0x519) = 0x18;
  *(undefined1 *)(iVar4 + 0x51a) = 0xe6;
  pbVar18 = (byte *)(iVar4 + 0x51b);
  *(undefined2 *)(iVar4 + 0x514) = 0;
  *pbVar18 = *pbVar18 | 7;
  iVar5 = (*(code *)&ieee80211_swfeat_is_supported)(5,0);
  if (iVar5 == 0) {
    iVar5 = (*(code *)&ieee80211_swfeat_is_supported)(7,0);
    if (iVar5 == 0) {
      *pbVar18 = *pbVar18 | 0xe0;
      *(undefined4 *)(iVar4 + 0xac) = 4;
      *(byte *)(iVar4 + 0x51c) = *(byte *)(iVar4 + 0x51c) | 0x19;
    }
    else {
      *pbVar18 = *pbVar18 & 0x1f | 0x40;
      *(undefined4 *)(iVar4 + 0xac) = 3;
      *(byte *)(iVar4 + 0x51c) = *(byte *)(iVar4 + 0x51c) & 0xe7 | 0x11;
    }
  }
  else {
    *pbVar18 = *pbVar18 & 0x9f | 0xa0;
    *(undefined4 *)(iVar4 + 0xac) = 2;
    *(byte *)(iVar4 + 0x51c) = *(byte *)(iVar4 + 0x51c) & 0xe6 | 8;
  }
  puVar14 = (undefined1 *)(iVar4 + 0x50a);
  (*(code *)&memset)(puVar14,0,10);
  iVar5 = *(int *)(iVar4 + 0xac);
  if (iVar5 == 1) {
    *puVar14 = 0xff;
  }
  else if (iVar5 == 2) {
    iVar13 = *(int *)(iVar4 + 0xa4);
    *puVar14 = 0xff;
    *(undefined1 *)(iVar4 + 0x50b) = 0xff;
    if (iVar13 < 0) {
      *(undefined1 *)(iVar4 + 0x50e) = 0x7e;
    }
  }
  else if (iVar5 == 3) {
    *puVar14 = 0xff;
    *(undefined1 *)(iVar4 + 0x50b) = 0xff;
    *(undefined1 *)(iVar4 + 0x50c) = 0xff;
    if (*(int *)(iVar4 + 0xa4) < 0) {
      *(undefined1 *)(iVar4 + 0x50e) = 0xfe;
      *(undefined1 *)(iVar4 + 0x50f) = 0xff;
      *(undefined1 *)(iVar4 + 0x510) = 0x1f;
    }
  }
  else if (iVar5 == 4) {
    *puVar14 = 0xff;
    *(undefined1 *)(iVar4 + 0x50b) = 0xff;
    *(undefined1 *)(iVar4 + 0x50c) = 0xff;
    *(undefined1 *)(iVar4 + 0x50d) = 0xff;
    if (*(int *)(iVar4 + 0xa4) < 0) {
      *(undefined1 *)(iVar4 + 0x50e) = 0xfe;
      *(undefined1 *)(iVar4 + 0x50f) = 0xff;
      *(undefined1 *)(iVar4 + 0x510) = 0xff;
      *(undefined1 *)(iVar4 + 0x511) = 0xff;
      *(undefined1 *)(iVar4 + 0x512) = 0xff;
      *(undefined1 *)(iVar4 + 0x513) = 0x1f;
    }
  }
  pbVar18 = (byte *)(iVar4 + 0x518);
  puVar14 = (undefined1 *)(iVar4 + 0x502);
  *puVar14 = 0;
  *pbVar18 = 1;
  if ((iVar5 == 4) && (iVar5 = (*(code *)&ieee80211_swfeat_is_supported)(6,0), iVar5 != 0)) {
    bVar1 = *pbVar18;
    iVar5 = *(int *)(iVar4 + 0xa4);
    *puVar14 = 1;
    *pbVar18 = bVar1 | 2;
    if (iVar5 < 0) {
      *pbVar18 = bVar1 | 0x12;
    }
  }
  uVar2 = *puVar21;
  *(undefined1 *)(iVar4 + 0x531) = 0;
  *(undefined1 *)(iVar4 + 0x532) = 0;
  *(undefined1 *)(iVar4 + 0x52e) = 0;
  iVar5 = FUN_ram_0004461c(iVar4 + 0x567,iVar4 + 0x58a,iVar4 + 0xb0,0,*(undefined4 *)(iVar4 + 0x24),
                           uVar2);
  if ((iVar5 != 0) ||
     (iVar5 = FUN_ram_0004461c(iVar4 + 0x53c,iVar4 + 0x55f,iVar4 + 0xb4,1,
                               *(undefined4 *)(iVar4 + 0x24),0), iVar5 != 0)) {
    if (1 < _g_dbg_log_level) {
      (*(code *)&printk)("%s: ERROR - Failed to initialize 802.11 (ieee80211com)\n","qdrv_wlan_init"
                        );
    }
LAB_ram_00045abc:
    if ((_g_dbg_log_module & 1) == 0) {
      return 0xffffffff;
    }
    if (9 < _g_dbg_log_level) {
      if ((_g_dbg_log_func & 1) != 0) {
        (*(code *)&printk)("%s: <--Exit\n","qdrv_wlan_init");
        return 0xffffffff;
      }
      return 0xffffffff;
    }
    return 0xffffffff;
  }
  (*(code *)&memcpy)(iVar4 + 0x40,(int)param_1 + 0x45,6);
  (*(code *)&ieee80211_ifattach)(iVar4);
  *(undefined1 *)(iVar4 + 0x5304) = 0x50;
  *(undefined1 **)(iVar4 + 0x53a4) = &LAB_ram_00042808;
  *(undefined1 **)(iVar4 + 0x53c0) = &LAB_ram_000386ac;
  *(code **)(iVar4 + 0x5478) = qdrv_radar_select_newchan;
  *(undefined1 **)(iVar4 + 0x539c) = &LAB_ram_0003ee10;
  *(undefined1 **)(iVar4 + 0x53c4) = &LAB_ram_000385bc;
  *(undefined1 **)(iVar4 + 0x53c8) = &LAB_ram_0004ae68;
  *(undefined **)(iVar4 + 0x53cc) = &DAT_ram_0003f210;
  *(code **)(iVar4 + 0x53d4) = hal_get_tsf;
  *(undefined **)(iVar4 + 0x53f4) = &DAT_ram_0003fd50;
  *(code **)(iVar4 + 0x53f8) = qdrv_async_cca_read;
  *(code **)(iVar4 + 0x53fc) = qtn_do_measurement;
  *(undefined1 **)(iVar4 + 0x5400) = &ieee80211_action_finish_measurement;
  *(undefined **)(iVar4 + 0x5404) = &DAT_ram_00038568;
  *(undefined1 **)(iVar4 + 0x5418) = &LAB_ram_000342d0;
  *(undefined1 **)(iVar4 + 0x541c) = &LAB_ram_00034228;
  *(undefined1 **)(iVar4 + 0x5368) = &LAB_ram_00034180;
  *(undefined1 **)(iVar4 + 0x536c) = &LAB_ram_000340c0;
  *(undefined1 **)(iVar4 + 0x5370) = &LAB_ram_00032758;
  *(code **)(iVar4 + 0x5474) = qdrv_radar_detected;
  *(code **)(iVar4 + 0x547c) = qdrv_dfs_action_scan_done;
  *(code **)(iVar4 + 0x5480) = qdrv_dfs_is_eu_region;
  iVar13 = 0x1f;
  *(code **)(iVar4 + 0x5510) = FUN_ram_00032790;
  *(code **)(iVar4 + 0x54f8) = qdrv_radar_test_mode_enabled;
  *(code **)(iVar4 + 0x5514) = FUN_ram_00037bfc;
  *(undefined1 **)(iVar4 + 0x5518) = &LAB_ram_00037be8;
  *(code **)(iVar4 + 0x5520) = qdrv_sta_set_xmit;
  *(code **)(iVar4 + 0x5524) = qdrv_set_radar;
  *(code **)(iVar4 + 0x5528) = qdrv_sta_dfs_enable;
  *(code **)(iVar4 + 0x552c) = qdrv_radar_detections_num;
  *(undefined2 *)(iVar4 + 0x5068) = 0x66;
  *(code **)(iVar4 + 0x5530) = qdrv_cac_instant_completed;
  *(undefined1 **)(iVar4 + 4) = &findchannel;
  *(undefined2 *)(iVar4 + 0x5074) = 0x66;
  *(undefined2 *)(iVar4 + 0x5078) = 0;
  *(undefined2 *)(iVar4 + 0x5076) = 0;
  iVar5 = 1;
  _DAT_auxregs_0008 = 0x45ea0;
  _DAT_auxregs_000c = 0x45ed0;
  do {
    iVar8 = iVar4 + iVar5 * 6;
    *(undefined2 *)(iVar8 + 0x5074) = *(undefined2 *)(iVar4 + 0x5068);
    *(undefined2 *)(iVar8 + 0x5076) = 0;
    *(undefined2 *)(iVar8 + 0x5078) = 0;
    iVar5 = iVar5 + 1;
    iVar13 = iVar13 + -1;
  } while (iVar13 != 0);
  *(undefined1 *)(iVar4 + 0xbf0) = 1;
  *(undefined2 *)(iVar4 + 0x66a) = 4;
  *(undefined4 *)(iVar4 + 0x59c) = 0x10;
  *(undefined1 **)(iVar4 + 0x53d8) = &LAB_ram_000433ac;
  *(undefined1 **)(iVar4 + 0x53dc) = &LAB_ram_0003c35c;
  *(undefined **)(iVar4 + 0x53e0) = &DAT_ram_00037b84;
  *(undefined1 **)(iVar4 + 0x53e4) = &LAB_ram_00040124;
  *(undefined1 **)(iVar4 + 0x53e8) = &LAB_ram_0003d4a8;
  *(undefined ***)(iVar4 + 0x5484) = &PTR_LAB_ram_00031014_ram_000336a0;
  *(undefined1 **)(iVar4 + 0x5488) = &LAB_ram_00037598;
  *(undefined1 **)(iVar4 + 0x548c) = &LAB_ram_00037604;
  *(undefined ***)(iVar4 + 0x5490) = &PTR_LAB_ram_00041014_ram_00041a74;
  *(undefined1 **)(iVar4 + 0x5498) = &LAB_ram_00037a6c;
  *(code **)(iVar4 + 0x5494) = qdrv_ic_dump_chan_availability_status;
  *(undefined1 **)(iVar4 + 0x549c) = &LAB_ram_00037458;
  *(undefined1 **)(iVar4 + 0x54a0) = &LAB_ram_0003740c;
  *(undefined1 **)(iVar4 + 0x54a4) = &LAB_ram_00032930;
  *(undefined1 **)(iVar4 + 0x54a8) = &LAB_ram_00032940;
  *(undefined **)(iVar4 + 0x54b0) = &DAT_ram_000368a8;
  *(undefined **)(iVar4 + 0x54ac) = &DAT_ram_00036d00;
  *(undefined1 **)(iVar4 + 0x54b4) = &LAB_ram_00041924;
  *(undefined1 **)(iVar4 + 0x5564) = &LAB_ram_00037404;
  *(undefined1 **)(iVar4 + 0x5568) = &LAB_ram_000373fc;
  *(undefined2 *)(iVar4 + 0x5c0) = 0;
  *(undefined2 *)(iVar4 + 0x5c4) = 0;
  *(undefined1 *)(iVar4 + 0x5c6) = 0;
  *(undefined2 *)(iVar4 + 0x5c2) = 0;
  *(undefined2 *)(iVar4 + 0x624) = 0;
  *(undefined4 *)(iVar4 + 0x598) = 5;
  *(undefined4 *)(iVar4 + 0x594) = 0x14;
  *(undefined4 *)(iVar4 + 0x5a0) = 200;
  *(undefined4 *)(iVar4 + 0x5a4) = 5;
  *(undefined4 *)(iVar4 + 0x5a8) = 0x3c;
  *(undefined4 *)(iVar4 + 0x5b0) = 0x28;
  *(undefined1 *)(iVar4 + 0x600) = 0x4b;
  *(undefined1 *)(iVar4 + 0x602) = 0x4b;
  *(undefined4 *)(iVar4 + 0x5b8) = 0x168;
  *(undefined4 *)(iVar4 + 0x5f8) = 300;
  *(undefined1 *)(iVar4 + 0x601) = 0x5a;
  *(undefined1 *)(iVar4 + 0x605) = 0x19;
  *(undefined2 *)(iVar4 + 0x65e) = 0x3c;
  *(undefined2 *)(iVar4 + 0x658) = 0x42;
  *(undefined2 *)(iVar4 + 0x666) = 0x32;
  *(undefined4 *)(iVar4 + 0x5ec) = 0x1e;
  *(undefined4 *)(iVar4 + 0x5bc) = 5;
  *(undefined4 *)(iVar4 + 0x5e4) = 0x28;
  *(undefined4 *)(iVar4 + 0x5e8) = 0x32;
  *(undefined4 *)(iVar4 + 0x5f0) = 0x14;
  *(undefined4 *)(iVar4 + 0x5f4) = 0;
  *(undefined1 *)(iVar4 + 0x603) = 0x19;
  *(undefined1 *)(iVar4 + 0x604) = 0xf;
  *(undefined1 *)(iVar4 + 0x606) = 5;
  *(undefined4 *)(iVar4 + 0x5fc) = 2;
  *(undefined2 *)(iVar4 + 0x626) = 0xffff;
  *(undefined4 *)(iVar4 + 0x62c) = 0x28;
  *(undefined4 *)(iVar4 + 0x630) = 0x42;
  *(undefined4 *)(iVar4 + 0x634) = 5000;
  *(undefined4 *)(iVar4 + 0x638) = 0x28;
  *(undefined4 *)(iVar4 + 0x63c) = 0;
  *(undefined4 *)(iVar4 + 0x640) = 100;
  *(undefined4 *)(iVar4 + 0x5b4) = 0x14;
  *(undefined2 *)(iVar4 + 0x654) = 0x42;
  *(undefined2 *)(iVar4 + 0x656) = 0x42;
  *(undefined2 *)(iVar4 + 0x65a) = 0x1e;
  *(undefined2 *)(iVar4 + 0x65c) = 0x32;
  *(undefined2 *)(iVar4 + 0x660) = 0x32;
  *(undefined2 *)(iVar4 + 0x662) = 0x32;
  *(undefined1 *)(iVar4 + 0x8f8) = 0;
  *(undefined1 *)(iVar4 + 0x8f9) = 0;
  *(undefined1 *)(iVar4 + 0x8fa) = 0;
  *(undefined2 *)(iVar4 + 0x670) = 0;
  *(undefined2 *)(iVar4 + 0x672) = 0x2a30;
  *(undefined2 *)(iVar4 + 0x674) = 3;
  *(undefined2 *)(iVar4 + 0x676) = 0xe10;
  *(undefined2 *)(iVar4 + 0x678) = 0;
  *(undefined2 *)(iVar4 + 0x67a) = 0;
  *(undefined2 *)(iVar4 + 0x8fc) = 0;
  (*(code *)&strncpy)(iVar4 + 0x900,&DAT_ram_000a0c6c,4);
  *(undefined2 *)(iVar4 + 0x906) = 0x17;
  *(undefined2 *)(iVar4 + 0x90a) = 0x2d0;
  *(undefined2 *)(iVar4 + 0x90c) = 0xf0;
  *(undefined2 *)(iVar4 + 0x90e) = 0x2e;
  *(undefined2 *)(iVar4 + 0x908) = 0x28;
  *(undefined2 *)(iVar4 + 0x918) = 0x5a;
  *(undefined2 *)(iVar4 + 0x91a) = 0x1e;
  *(undefined4 *)(iVar4 + 0x910) = 0x2d00;
  *(undefined4 *)(iVar4 + 0x914) = 0x10e9;
  *(undefined2 *)(iVar4 + 0x8fe) = 2;
  *(undefined2 *)(iVar4 + 0x924) = 2;
  *(undefined2 *)(iVar4 + 0x91e) = 0x14;
  *(undefined2 *)(iVar4 + 0x926) = 100;
  *(undefined2 *)(iVar4 + 0x91c) = 10;
  *(undefined2 *)(iVar4 + 0x904) = 1;
  *(undefined2 *)(iVar4 + 0x920) = 10;
  puVar17 = (undefined4 *)(iVar4 + 0x5544);
  *(undefined2 *)(iVar4 + 0x922) = 5;
  puVar19 = (undefined4 *)(iVar4 + 0x5548);
  *(undefined4 *)(iVar4 + 0x9c4) = 0;
  *(undefined1 **)(iVar4 + 0x5408) = &LAB_ram_0003f85c;
  *(undefined1 **)(iVar4 + 0x540c) = &LAB_ram_0003c29c;
  *(code **)(iVar4 + 0x54cc) = qdrv_muc_stats_rxtx_phy_rate;
  *(code **)(iVar4 + 0x54d0) = qdrv_muc_stats_rssi;
  *(code **)(iVar4 + 0x54d4) = qdrv_muc_stats_smoothed_rssi;
  *(code **)(iVar4 + 0x54d8) = qdrv_muc_stats_snr;
  *(code **)(iVar4 + 0x54dc) = qdrv_muc_stats_hw_noise;
  *(code **)(iVar4 + 0x54e0) = qdrv_muc_stats_max_queue;
  *(code **)(iVar4 + 0x54e8) = qdrv_muc_stats_mcs_to_phyrate;
  *(code **)(iVar4 + 0x54e4) = qdrv_muc_stats_tx_failed;
  *(code **)(iVar4 + 0x54ec) = qdrv_channel_switch_record;
  *(code **)(iVar4 + 0x54f0) = qdrv_channel_switch_reason_record;
  *(code **)(iVar4 + 0x54f4) = dfs_reentry_chan_switch_notify;
  *(undefined1 **)(iVar4 + 0x5538) = &LAB_ram_00034ecc;
  *(undefined4 *)(iVar4 + 0x52e0) = 0;
  (*(code *)&__init_waitqueue_head)(iVar4 + 0x52e4,&DAT_ram_00092110);
  *(undefined4 *)(iVar4 + 0x52f0) = 0;
  iVar5 = iVar4 + 0x52f4;
  *(int *)(iVar4 + 0x52f8) = iVar5;
  *(int *)iVar5 = iVar5;
  *(undefined1 **)(iVar4 + 0x52fc) = &ieee80211_csa_finish;
  uVar10 = (*(code *)&__create_workqueue_key)("csa_work_queue",0,0,0,0,0);
  *(undefined4 *)(iVar4 + 0x52ec) = uVar10;
  *(undefined1 *)(iVar4 + 0x5dac) = 1;
  *(undefined1 *)(iVar4 + 0x5ce4) = 1;
  *(undefined1 *)(iVar4 + 0x5ce5) = 0;
  *(undefined4 *)(iVar4 + 0x5300) = 0;
  (*(code *)&init_timer_key)(iVar4 + 0x5cc8,0,0);
  *(undefined2 *)(iVar4 + 0x5cee) = 200;
  *(undefined2 *)(iVar4 + 0x5cea) = 10;
  *(undefined2 *)(iVar4 + 0x5cf4) = 0x19;
  *(undefined4 *)(iVar4 + 0x5d18) = 0xffffffff;
  *(undefined2 *)(iVar4 + 0x5cf0) = 0x14;
  *(undefined2 *)(iVar4 + 0x5cf2) = 5;
  *(undefined2 *)(iVar4 + 0x5ce8) = 0x14;
  *(undefined2 *)(iVar4 + 0x5cec) = 200;
  *(undefined1 *)(iVar4 + 0x5d1c) = 3;
  *(undefined1 *)(iVar4 + 0x5d1d) = 0xf;
  *(code **)(iVar4 + 0x5cf8) = qdrv_wlan_coex_stats_update;
  *(code **)(iVar4 + 0x5550) = qdrv_wlan_vsp_cb_strm_ctrl;
  *(undefined **)(iVar4 + 0x553c) = &DAT_ram_000328d4;
  *(undefined **)(iVar4 + 0x5540) = &DAT_ram_000373d8;
  *puVar17 = &DAT_ram_000328d8;
  *puVar19 = &DAT_ram_00037340;
  *(undefined1 **)(iVar4 + 0x554c) = &LAB_ram_00037244;
  *(code **)(iVar4 + 0x5554) = qdrv_wlan_vsp_cb_cfg;
  puVar17 = (undefined4 *)(iVar4 + 0x5ae8);
  puVar19 = (undefined4 *)(iVar4 + 0x5aec);
  *(code **)(iVar4 + 0x5560) = qdrv_wlan_vsp_reset;
  puVar16 = (undefined4 *)(iVar4 + 0x5af0);
  puVar21 = (undefined1 *)(iVar4 + 0x52a0);
  *(code **)(iVar4 + 0x5558) = qdrv_wlan_vsp_cb_strm_ext_throttler;
  puVar20 = (undefined2 *)(iVar4 + 0x52c0);
  *(undefined1 **)(iVar4 + 0x53ec) = &LAB_ram_0003d54c;
  *(undefined1 **)(iVar4 + 0x53f0) = &LAB_ram_0004354c;
  *(undefined1 *)(iVar4 + 0x4f5) = 0;
  (*(code *)&memset)(iVar4 + 0x3ffc,0,0x248);
  (*(code *)&memset)(iVar4 + 0x4244,0,0x800);
  *(undefined1 **)(iVar4 + 0x551c) = &ieee80211_send_notify_chan_width_action;
  *(undefined1 **)(iVar4 + 0x5cc4) = &ieee80211_send_vht_grp_id_mgmt_action;
  (*(code *)&memcpy)(iVar4 + 0x57d4,&DAT_ram_00082954,0x58);
  uVar6 = *(uint *)(iVar4 + 0xa0);
  uVar12 = *(uint *)(iVar4 + 0x9c);
  *(undefined4 *)(iVar4 + 0x5350) = 2;
  *(undefined1 *)(iVar4 + 0x5c6c) = 0;
  *puVar17 = qdrv_wlan_reset_shared_node_stats;
  *puVar19 = qdrv_wlan_get_dscp2ac_map;
  *(code **)(iVar4 + 0x5af8) = qdrv_sch_get_dscp2tid_map;
  *(code **)(iVar4 + 0x5af4) = qdrv_sch_set_dscp2tid_map;
  *(undefined1 *)(iVar4 + 0x52c4) = 0xff;
  *(undefined1 *)(iVar4 + 0xbf3) = 1;
  *(code **)(iVar4 + 0x5aac) = qdrv_get_local_tx_power;
  *(code **)(iVar4 + 0x5ab0) = qdrv_get_local_link_margin;
  *(code **)(iVar4 + 0x5adc) = qdrv_wlan_get_shared_vap_stats;
  *(code **)(iVar4 + 0x5ae0) = qdrv_wlan_reset_shared_vap_stats;
  *(code **)(iVar4 + 0x5ae4) = qdrv_wlan_get_shared_node_stats;
  *puVar16 = qdrv_wlan_set_dscp2ac_map;
  *puVar21 = 0;
  *puVar20 = 0;
  *(undefined1 *)(iVar4 + 0x52c2) = 0;
  *(undefined1 *)(iVar4 + 0x52c3) = 0;
  puVar7 = (uint *)(iVar4 + 0x5cfc);
  *(undefined4 *)(iVar4 + 0x534c) = 2;
  *(code **)(iVar4 + 0x5b18) = ieee80211_get_cca_adjusting_status;
  *(uint *)(iVar4 + 0xa0) = uVar6 | 0x2000;
  *(uint *)(iVar4 + 0x9c) = uVar12 | 0x10;
  local_3c = (byte *)0x0;
  *puVar7 = 0;
  iVar5 = (*(code *)&bootcfg_get_var)("dm_txpower_factor",&puStack_78);
  if (((iVar5 != 0) && (iVar5 = (*(code *)&sscanf)(iVar5,&DAT_ram_000950f4,&local_3c), iVar5 == 1))
     && (local_3c < (byte *)0x9)) {
    *puVar7 = *puVar7 | 1;
    *(byte **)(iVar4 + 0x5d00) = local_3c;
  }
  iVar5 = (*(code *)&bootcfg_get_var)("dm_aci_factor",&puStack_78);
  if (((iVar5 != 0) && (iVar5 = (*(code *)&sscanf)(iVar5,&DAT_ram_000950f4,&local_3c), iVar5 == 1))
     && (local_3c + 4 < (byte *)0x5)) {
    *puVar7 = *puVar7 | 2;
    *(byte **)(iVar4 + 0x5d04) = local_3c;
  }
  iVar5 = (*(code *)&bootcfg_get_var)("dm_cci_factor",&puStack_78);
  if (((iVar5 != 0) && (iVar5 = (*(code *)&sscanf)(iVar5,&DAT_ram_000950f4,&local_3c), iVar5 == 1))
     && (local_3c + 4 < (byte *)0x5)) {
    *puVar7 = *puVar7 | 4;
    *(byte **)(iVar4 + 0x5d08) = local_3c;
  }
  iVar5 = (*(code *)&bootcfg_get_var)("dm_dfs_factor",&puStack_78);
  if (((iVar5 != 0) && (iVar5 = (*(code *)&sscanf)(iVar5,&DAT_ram_000950f4,&local_3c), iVar5 == 1))
     && (local_3c < (byte *)0x21)) {
    *puVar7 = *puVar7 | 8;
    *(byte **)(iVar4 + 0x5d0c) = local_3c;
  }
  iVar5 = (*(code *)&bootcfg_get_var)("dm_beacon_factor",&puStack_78);
  if (((iVar5 != 0) && (iVar5 = (*(code *)&sscanf)(iVar5,&DAT_ram_000950f4,&local_3c), iVar5 == 1))
     && (local_3c + 4 < (byte *)0x5)) {
    *puVar7 = *puVar7 | 0x10;
    *(byte **)(iVar4 + 0x5d10) = local_3c;
  }
  *(undefined1 *)(iVar4 + 0x5d78) = 0;
  *(undefined4 *)(iVar4 + 0x5d80) = 1000;
  *(undefined1 *)(iVar4 + 0x5361) = 1;
  *(undefined4 *)(iVar4 + 0x5da8) = 500;
  *(code **)(iVar4 + 0x5d20) = qdrv_muc_stats_tx_airtime;
  *(code **)(iVar4 + 0x5d24) = qdrv_muc_stats_tx_accum_airtime;
  *(code **)(iVar4 + 0x5d28) = qdrv_tx_airtime_control;
  *(code **)(iVar4 + 0x5d2c) = qdrv_muc_stats_rx_airtime;
  *(code **)(iVar4 + 0x5d30) = qdrv_muc_stats_rx_accum_airtime;
  *(undefined1 *)(iVar4 + 0x5d84) = 0;
  *(code **)(iVar4 + 0x5d34) = qdrv_mu_grp_update;
  *(undefined1 *)(iVar4 + 0x5d85) = 0;
  *(undefined1 *)(iVar4 + 0x5d86) = 0;
  *(undefined4 *)(iVar4 + 0x5d7c) = 0;
  *(undefined1 *)(iVar4 + 0x5360) = 0;
  *(undefined1 *)(iVar4 + 0x5362) = 0;
  (*(code *)&memset)(iVar4 + 0xbc8,0,0x10);
  *(undefined1 **)(iVar4 + 0x5db0) = &LAB_ram_00037234;
  *(int *)(iVar4 + 0x5fb0) = iVar4 + 0x5f84;
  *(undefined4 *)(iVar4 + 0x5fb4) = 8;
  puStack_78 = (undefined *)0x35396573;
  local_74 = 0;
  uStack_70 = 0;
  uStack_6c = 0;
  uStack_68 = 0;
  uStack_64 = 0;
  uStack_60 = 0;
  uStack_5c = 0;
  uStack_58 = 0;
  (*(code *)&memset)((int)&local_74 + 1,0,0xf);
  uStack_64 = CONCAT22(0x49,(undefined2)uStack_64);
  iVar13 = (*(code *)&i2c_get_adapter)(0);
  if (iVar13 == 0) {
    pbVar18 = (byte *)(iVar4 + 0x7fc0);
    pbVar18[0] = 0;
    pbVar18[1] = 0;
    pbVar18[2] = 0;
    pbVar18[3] = 0;
    (*(code *)&printk)("QDRV: I2C dapter not found\n");
  }
  else {
    pbVar18 = (byte *)(iVar4 + 0x7fc0);
    iVar5 = (*(code *)&i2c_new_device)(iVar13,&puStack_78);
    *(int *)pbVar18 = iVar5;
    if (iVar5 == 0) {
LAB_ram_000477cc:
      (*(code *)&i2c_put_adapter)(iVar13);
      if (1 < _g_dbg_log_level) {
        (*(code *)&printk)("%s: ERROR - Failed to instantiate temperature sensor device\n",
                           "qdrv_init_tsensor");
      }
    }
    else {
      iVar5 = (*(code *)&qtn_tsensor_get_temperature)(iVar5,&local_3c);
      if (iVar5 < 0) {
        (*(code *)&i2c_unregister_device)(*(int *)pbVar18);
        pbVar18[0] = 0;
        pbVar18[1] = 0;
        pbVar18[2] = 0;
        pbVar18[3] = 0;
        (*(code *)&i2c_put_adapter)(iVar13);
        (*(code *)&printk)("QDRV: no external temperature sensor found\n");
      }
    }
  }
  (*(code *)&memcpy)(&puStack_78,&DAT_ram_000830f0,0x3c);
  (*(code *)&memset)(local_140,0,200);
  iVar5 = (*(code *)&sys_open)("/tmp/tprofile.txt",0,0);
  iVar13 = (*(code *)&sys_open)("/proc/bootcfg/tprofile.txt",0,0);
  iVar8 = (*(code *)&sys_open)("/proc/bootcfg/pdetector.cal",0,0);
  if (iVar5 < 0) {
    if (iVar13 < 0) {
      if ((((_g_dbg_log_module & 1) != 0) && (7 < _g_dbg_log_level)) &&
         ((_g_dbg_log_func & 0x200) != 0)) {
        (*(code *)&printk)("%s: QDRV: using default temperature profile\n","qdrv_txpow_cal_init");
      }
      iVar13 = 0xf;
      (*(code *)&memcpy)(&DAT_ram_00089a6c,&puStack_78,0x3c);
      iVar5 = 0xf;
    }
    else {
      if (iVar13 < 0) goto LAB_ram_000476f8;
      (*(code *)&sys_read)(iVar13,local_140,200);
      (*(code *)&sys_close)(iVar13);
      pbVar9 = local_140;
      pbVar22 = local_140;
      while( true ) {
        bVar1 = (&_ctype)[*pbVar22];
        *pbVar9 = *pbVar22;
        if ((bVar1 & 0x20) == 0) {
          pbVar9 = pbVar9 + 1;
        }
        if (*pbVar22 == 0) break;
        pbVar22 = pbVar22 + 1;
      }
      iVar13 = 0;
      puVar17 = &DAT_ram_00089a6c;
      local_3c = local_140;
      do {
        uVar10 = (*(code *)&strsep)(&local_3c,&DAT_ram_000a405c);
        uVar10 = (*(code *)&simple_strtoul)(uVar10,0,0);
        *puVar17 = uVar10;
        puVar17 = puVar17 + 1;
        iVar13 = iVar13 + 1;
      } while (local_3c != (byte *)0x0);
      iVar5 = iVar13 * (uint)(iVar13 >= 2) + (uint)(iVar13 < 2);
    }
  }
  else if ((iVar13 < 0) && (-1 < iVar5)) {
    (*(code *)&sys_read)(iVar5,local_140,200);
    (*(code *)&sys_close)(iVar5);
    pbVar9 = local_140;
    pbVar22 = local_140;
    while( true ) {
      bVar1 = (&_ctype)[*pbVar22];
      *pbVar9 = *pbVar22;
      if ((bVar1 & 0x20) == 0) {
        pbVar9 = pbVar9 + 1;
      }
      if (*pbVar22 == 0) break;
      pbVar22 = pbVar22 + 1;
    }
    puVar17 = &DAT_ram_00089a6c;
    iVar13 = 0;
    local_3c = local_140;
    do {
      uVar10 = (*(code *)&strsep)(&local_3c,&DAT_ram_000a405c);
      uVar10 = (*(code *)&simple_strtoul)(uVar10,0,0);
      *puVar17 = uVar10;
      puVar17 = puVar17 + 1;
      iVar13 = iVar13 + 1;
    } while (local_3c != (byte *)0x0);
    iVar5 = iVar13 * (uint)(iVar13 >= 2) + (uint)(iVar13 < 2);
  }
  else {
LAB_ram_000476f8:
    (*(code *)&sys_read)(iVar13,local_140,200);
    (*(code *)&sys_close)(iVar13);
    pbVar9 = local_140;
    pbVar22 = local_140;
    while( true ) {
      bVar1 = (&_ctype)[*pbVar22];
      *pbVar9 = *pbVar22;
      if ((bVar1 & 0x20) == 0) {
        pbVar9 = pbVar9 + 1;
      }
      if (*pbVar22 == 0) break;
      pbVar22 = pbVar22 + 1;
    }
    puVar17 = &DAT_ram_00089a6c;
    iVar13 = 0;
    local_3c = local_140;
    do {
      uVar10 = (*(code *)&strsep)(&local_3c,&DAT_ram_000a405c);
      uVar10 = (*(code *)&simple_strtoul)(uVar10,0,0);
      *puVar17 = uVar10;
      puVar17 = puVar17 + 1;
      iVar13 = iVar13 + 1;
    } while (local_3c != (byte *)0x0);
    iVar5 = iVar13 * (uint)(iVar13 >= 2) + (uint)(iVar13 < 2);
  }
  piVar15 = &DAT_ram_00089a6c;
  _DAT_auxregs_0008 = 0x4709c;
  _DAT_auxregs_000c = 0x470ac;
  do {
    *piVar15 = *piVar15 * 100000;
    piVar15 = piVar15 + 1;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  if (iVar8 < 0) {
    if (*(int *)pbVar18 == 0) {
      if (2 < _g_dbg_log_level) {
        (*(code *)&printk)("%s: WARNING - QDRV: failed to initialize power calibration\n",
                           "qdrv_txpow_cal_init");
      }
      goto LAB_ram_00047170;
    }
    *(char *)(iVar4 + 0x7fcc) = (char)iVar13;
    if ((((_g_dbg_log_module & 1) != 0) && (7 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: QDRV: temperature sensor zone=%d, <%d, %d, %d, %d>\n",
                         "qdrv_txpow_cal_init",iVar13,DAT_ram_00089a6c,DAT_ram_00089a70,
                         *(undefined4 *)(iVar13 * 4 + 0x89a64),(&DAT_ram_00089a68)[iVar13]);
    }
    puVar17 = (undefined4 *)(iVar4 + 0x7fd0);
    *puVar17 = 0;
    iVar5 = iVar4 + 0x7fd4;
    *(int *)(iVar4 + 0x7fd8) = iVar5;
    *(int *)iVar5 = iVar5;
    *(undefined1 **)(iVar4 + 0x7fdc) = &LAB_ram_00039248;
    (*(code *)&init_timer_key)(iVar4 + 0x7fe0,0,0);
  }
  else {
    if ((((_g_dbg_log_module & 1) != 0) && (5 < _g_dbg_log_level)) &&
       ((_g_dbg_log_func & 0x200) != 0)) {
      (*(code *)&printk)("%s: QDRV: using %s for Tx gain calibration\n","qdrv_txpow_cal_init",
                         "/proc/bootcfg/pdetector.cal");
    }
    puVar17 = (undefined4 *)(iVar4 + 0x7fd0);
    *puVar17 = 0;
    iVar5 = iVar4 + 0x7fd4;
    *(int *)(iVar4 + 0x7fd8) = iVar5;
    *(int *)iVar5 = iVar5;
    *(undefined1 **)(iVar4 + 0x7fdc) = &LAB_ram_0003934c;
    (*(code *)&init_timer_key)(iVar4 + 0x7fe0,0,0);
  }
  (*(code *)&schedule_delayed_work)(puVar17,_jiffies + 1000);
LAB_ram_00047170:
  qdrv_radar_init(param_1);
  qdrv_pktlogger_init(iVar4);
  iVar5 = iVar4 + 0x7ed8;
  (*(code *)&init_timer_key)(iVar5,0,0);
  *(int *)(iVar4 + 0x7eec) = iVar4;
  *(undefined1 **)(iVar4 + 0x7ee8) = &LAB_ram_000393d0;
  *(int *)(iVar4 + 0x7ee0) = _jiffies + 600;
  (*(code *)&add_timer)(iVar5);
  *(undefined4 *)(iVar4 + 0x7fa8) = &LAB_ram_00041c70;
  (*(code *)&pm_qos_add_notifier)(4,(undefined4 *)(iVar4 + 0x7fa8));
  (*(code *)&arc_set_sram_safe_area)(&DAT_ram_8803e7c0,0x8803f7c0);
  (*(code *)&arc_set_troubleshoot_start_hook)(&LAB_ram_000393dc,iVar4);
  _br_fdb_check_active_sub_port_hook = qdrv_check_active_sub_port;
  _br_fdb_get_active_sub_port_hook = qdrv_get_active_sub_port;
  *(int *)(iVar4 + 0x7ffc) = _soc_shared_params;
  if ((_g_dbg_log_module & 1) != 0) {
    if (_g_dbg_log_level < 10) {
      return 0;
    }
    if ((_g_dbg_log_func & 1) != 0) {
      (*(code *)&printk)("%s: <--Exit\n","qdrv_wlan_init");
      return 0;
    }
  }
  return 0;
}

