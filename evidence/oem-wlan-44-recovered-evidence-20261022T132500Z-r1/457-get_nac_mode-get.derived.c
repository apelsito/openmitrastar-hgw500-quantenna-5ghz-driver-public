/* Derived REA pseudocode; verify assembly and exact OEM ABI. */
  case 0x1c9:
    puVar7 = *(ushort **)(_soc_shared_params + 0x184);
    uVar3 = puVar7[1];
    uVar1 = puVar7[2];
    uVar2 = (*(code *)&__divsi3)((uint)*puVar7 * 100);
    local_1c = 0;
    *param_4 = (uint)uVar3 << 0x10 | (uint)(byte)uVar1 | (uVar2 & 0xff) << 8;
    break;
