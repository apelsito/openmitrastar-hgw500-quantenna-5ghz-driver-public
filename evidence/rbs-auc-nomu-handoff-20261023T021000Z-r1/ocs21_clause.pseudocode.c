
void om_auc_ocs_suspend_command21_clause(void)

{
  int unaff_r13;
  int iVar1;
  int iVar2;
  int iVar3;
  int unaff_gp;
  bool bVar4;
  
  if (unaff_r13 != *(int *)(unaff_gp + 0xdc)) {
    *(int *)(unaff_gp + 0xdc) = unaff_r13;
    if (unaff_r13 == 0) {
      *(undefined4 *)(unaff_gp + 0xe0) = *(undefined4 *)(unaff_gp + 0x138);
      if (*(int *)(unaff_gp + 0x60) != 0) {
        iVar3 = **(int **)(*(int *)(unaff_gp + 100) + 4);
        iVar1 = *(int *)(unaff_gp + 0x60);
        do {
          iVar2 = *(int *)(iVar1 + 0x18);
          FUN_ram_e5001770(iVar1,1);
          bVar4 = iVar1 != iVar3;
          iVar1 = iVar2;
        } while (bVar4 && iVar2 != 0);
      }
      DAT_ram_e5103690 = DAT_ram_e5103690 + 1;
    }
    else {
      DAT_ram_e510368c = DAT_ram_e510368c + 1;
    }
  }
  *(int *)(unaff_gp + 0xe4) = unaff_r13;
  FUN_ram_e5005db8(1);
  return;
}

