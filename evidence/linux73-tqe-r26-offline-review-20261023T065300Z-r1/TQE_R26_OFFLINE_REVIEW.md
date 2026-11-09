# TQE y R26: contrato offline de pausa/reset y prepare AuC

El reset ARC observado escribe0x4180(NETSS|MAC|BB), sin resetSRAM. TQE
publica elringantesdelstartQDRV; reinicializarelheapdespuésdelreset perdería
suownership. El controladoraislado mantiene allocation_owner, bus ycount;
despublica/reprograma el mismo ring y no libera ni reconstruye SRAM.

Los readbacks están ligados a los registros existentes: IRQ/reset enCSR,
STATUSempty, QPTRread/write conwrap yTXSTARTnready. Dos snapshots consecutivos
deidle preceden ring0/count0+CPUIFreset. El poll esacotado; discrepanciasretienen
laidentidad en cuarentena.24casos/83checks y objetoARCcompilado sirven sólo
para elcontrolador. Callbacks MMIO/Linux/productores son simulados.

Falta elcontrato real de productores remotos y delbank/batch/tasklet LHOST.
IRQ/NAPIdrain no demuestra esa pausa; NAPIpuede reactivarIRQalcompletar. El
provideractual no ofrece esa barreraexclusiva. Sinella elcontrolador devuelve
EOPNOTSUPP antesdecambiarregistros. Noestá conectadoaldriver ni habilitareset.
El softwarelease de resetconarenaempty tampoco acepta unringvivo mediante
una bandera: necesitacontrato real de ownership/quiesce distinto.

La pruebaR26 incluye qdrv_auc_init, auc_clear_mem y qdrv_fw_load_prepared
reales.46casos: hash y8destinosde loader precedenprepare; los4clearspans se
validan antesdelprimerclearmap. Hash/tamaño/metadata y cadaerror de mapase
rechazan sin publicarentry ni habilitarAuC. Callback positivoinesperado se
normalizaaEPROTO. SHA256hostOpenSSLreal; recursos/lease/MMIO son fakes,
ASAN/UBSAN pasan. Noejecución físicade firmwareo reset.

ElpipelineR26 cierra elclearAuCpreviamentefuera dellease, pero no convierte
unmaprefusado en rollbackatomico:clearsanteriorespuedenquedar. Tampocoprueba
queNETSSpuedaresetearTQE condescriptores/firmwares activos. LareservaSRAM,
productorfence y pruebasfísicas siguenpendientes. R24/R26fuentesintactas.
