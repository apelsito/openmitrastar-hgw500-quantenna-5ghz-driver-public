# R20: límites de carga normal para AELITA

El enlace offline y la tabla610/44 no establecen que QDRV normal pueda cargarse.

1. `qdrv.ko` depende de `switch_tqe.ko`. Este módulo exige `w3_switch_tqe_only_enable=1`; por defecto0 devuelve`-EPERM`.
2. Aun habilitando expresamente ese gate, el ringTQE solicitaSRAM. El proveedor actual devuelveNULL salvo fallbackDRAMmanual de laboratorio; la inicialización falla.

El scheduler tiene una rutaSRAMopcional bajoCONFIG_ARCH_RUBY_NUMA y fallbackkmalloc; no es el bloqueo inevitable.

Existe el allocatorOEM en `linux_2.6.35.12/arch/arc/kernel/module.c`: requiere__sram_end, mapa de bancos yNUMA propios del kernelOEM. No está portado ni validado en7.3.

El nombre `qtn_mproc_sync_soc_sharedparams_init` no aparece en los árboles actuales/OEM seleccionados. La implementación encontrada es `qdrv_soc_publish_params`, conDMAcoherente y publicación mediante `qtn_mproc_sync_shared_params_set`; esto no demuestra aceptación física del bus.

No activar fallback de laboratorio ni inventar mapaSRAM para declararviabilidad. Conservar estas limitaciones en AELITA hasta recuperar/portar elbackendreal y probar suscontratos físicos.
