
void FUN_ram_0006db88(int *param_1,int *param_2)

{
  int iVar1;
  
  if (*(int **)(*param_2 + 0x514) != param_2) {
    iVar1 = (*(code *)&memcmp)(*param_2 + 0x42c,param_2 + 0x28b,6);
    if ((iVar1 != 0) && (*param_1 = *param_1 + 1, (param_2[0x398] & 0x800U) != 0)) {
      param_1[1] = param_1[1] + 1;
    }
  }
  return;
}

