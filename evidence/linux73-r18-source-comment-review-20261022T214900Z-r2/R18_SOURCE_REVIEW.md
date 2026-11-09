# R18: revisión privada de comentarios y 44 entradas

Candidato: `linux73-full44-candidate-20261023T002000Z-r18`. Hay 43 alias originales exactos y un alias468 distinto; ninguna operación funcional nueva queda cualificada por esta revisión.

Catálogo: 174 ficheros, 7137 comentarios y 720910 bytes exactos. No se modificaron fuentes.

El getter468 original se llama `g_rbsdfs_end_tx`; R18 usa `g_rbsdfs_endtx`. Hay que conservar el alias original en una revisión nueva antes de afirmar compatibilidad literal completa.

| Cmd | Nombre original | Nombre en R18 | Estado de la fuente |
|---|---|---|---|
| 331 | get_cca_fixed | get_cca_fixed | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 246 | tx_maxmpdu | tx_maxmpdu | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 246 | get_tx_maxmpdu | get_tx_maxmpdu | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 275 | get_auto_cca_en | get_auto_cca_en | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 484 | txamsdu_11n | txamsdu_11n | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 484 | get_txamsdu_11n | get_txamsdu_11n | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 498 | set_dup_rts | set_dup_rts | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 498 | get_dup_rts | get_dup_rts | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 449 | 20_40_bss_coex | 20_40_bss_coex | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 450 | get_min_cac | get_min_cac | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 457 | nac_mode | nac_mode | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 457 | get_nac_mode | get_nac_mode | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 451 | get_dev_mode | get_dev_mode | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 460 | set_max_bw | set_max_bw | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 460 | get_max_bw | get_max_bw | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 461 | set_bw_autosel | set_bw_autosel | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 461 | get_bw_autosel | get_bw_autosel | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 465 | ignr_icac_sel | ignr_icac_sel | FAITHFUL_OEM_UNSUPPORTED_SOURCE_PRESENT |
| 467 | dfs_chan_avail | dfs_chan_avail | BOUNDED_AP_QUERY_SOURCE_PRESENT |
| 464 | check_cur_chan | check_cur_chan | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 466 | rbs_dfs_msr_cac | rbs_dfs_msr_cac | SOURCE_ADAPTER_PRESENT_REAL_FRAME_PROVIDER_ABSENT |
| 466 | g_rbsdfs_msrcac | g_rbsdfs_msrcac | SOURCE_ADAPTER_PRESENT_REAL_FRAME_PROVIDER_ABSENT |
| 468 | rbs_dfs_end_tx | rbs_dfs_end_tx | SOURCE_ADAPTER_PRESENT_REAL_FRAME_PROVIDER_ABSENT |
| 468 | g_rbsdfs_end_tx | g_rbsdfs_endtx | SOURCE_ADAPTER_PRESENT_REAL_FRAME_PROVIDER_ABSENT |
| 492 | neigh_repo | neigh_repo | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 492 | get_neigh_repo | get_neigh_repo | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 463 | set_dyn_sifs | set_dyn_sifs | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 478 | wea_cac_en | wea_cac_en | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 478 | get_wea_cac_en | get_wea_cac_en | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 481 | set_vopt | set_vopt | VOPT_WORKER_AND_CONTROL_SOURCE_PRESENT |
| 481 | get_vopt | get_vopt | VOPT_WORKER_AND_CONTROL_SOURCE_PRESENT |
| 483 | bbdf_war_en | bbdf_war_en | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 483 | get_bbdfwar_en | get_bbdfwar_en | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 491 | coc_mv_to_ndfs | coc_mv_to_ndfs | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 491 | g_coc_mv_to_ndfs | g_coc_mv_to_ndfs | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 493 | bss_tm | bss_tm | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 493 | get_bss_tm | get_bss_tm | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 494 | mdid | mdid | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 494 | get_mdid | get_mdid | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 495 | ft_over_ds | ft_over_ds | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 495 | get_ft_over_ds | get_ft_over_ds | FAITHFUL_OEM_UNSUPPORTED_SOURCE_PRESENT |
| 496 | get_sretry_lmt | get_sretry_lmt | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 497 | get_lretry_lmt | get_lretry_lmt | SOURCE_RECORD_AND_DISPATCH_PRESENT |
| 490 | fix_legacy_rate | fix_legacy_rate | SOURCE_RECORD_AND_DISPATCH_PRESENT |

RBS466/468 sigue sin proveedor real de grants/tramas. La consulta467 y VOPT mantienen sus límites. MDIE no implica autenticaciónFT. Los getters OEM no soportados se conservan como tales.

R18registersgpio_chipwithoutTHIS_MODULEownerandwithoutmodule_exit. Mustfixownershipbeforeload/unloadacceptance.
R18alignedallocator lacksnull/zero/power2align/multiply/addoverflow admission; SRAMbackend freeclassification reliesonoldmacros absentin73. Minimalbounds/backendownershipfixneeded.

La evidencia sirve para AELITA como orientación de fuente y catálogo; no demuestra tráfico, aceptación física o release.
