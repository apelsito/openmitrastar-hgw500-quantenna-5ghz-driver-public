# Exact remaining OEM private records

Measured at 2026-10-22T16:28:01.451406+00:00 against `aa00ffd4fa3fba0f7871fdea14c76f2a6816c7add9fd3ff0d9a7b0c9fa6c2c02`.

OEM: 607 records. Qualified candidate: 586. OEM matches: 583. Missing: 24. Port additions: 3.

These are table records. Full firmware behaviors and physical acceptance are separate.

| ID | Direction | Name | Remaining dependency |
| --- | --- | --- | --- |
| 449 | set | `20_40_bss_coex` | 20/40coexistence consumers absent or not reconciled; requires append-only ABIstate and actual beacon/control integration |
| 457 | set | `nac_mode` | Sharedpointeralreadyallocated/published; zero denominatorguard-EAGAIN prototypePASSED; setter needsMuCtimer/monitor lifecycle evidence |
| 457 | get | `get_nac_mode` | Sharedpointeralreadyallocated/published; zero denominatorguard-EAGAIN prototypePASSED; setter needsMuCtimer/monitor lifecycle evidence |
| 461 | set | `set_bw_autosel` | OEM channel-set selection requires 80-byte tables, allocation/free, seven metric-history cells, scan callbacks and band/width policies. Fresh start/pick REA retained; matching subsystem is absent from port. |
| 461 | get | `get_bw_autosel` | OEM channel-set selection requires 80-byte tables, allocation/free, seven metric-history cells, scan callbacks and band/width policies. Fresh start/pick REA retained; matching subsystem is absent from port. |
| 467 | get | `dfs_chan_avail` | Portcallbackic_dfs_chans_available_for_cac hasdifferentbool/sideeffects; missingrefreshexport/source, noequivalence |
| 466 | set | `rbs_dfs_msr_cac` | RBSspecificstateabsent; mustnotmaptoSTAstrictfieldwithdifferentvalidation |
| 466 | get | `g_rbsdfs_msrcac` | RBSspecificstateabsent; mustnotmaptoSTAstrictfieldwithdifferentvalidation |
| 468 | set | `rbs_dfs_end_tx` | RBSclosingtimefieldabsent; OEMinit500, STAtimefielddifferentcontract |
| 468 | get | `g_rbsdfs_end_tx` | RBSclosingtimefieldabsent; OEMinit500, STAtimefielddifferentcontract |
| 492 | set | `neigh_repo` | 11kneighborreport IEconsumersabsent ornotreconciled; appendhoststate + IEgenerationneeded |
| 492 | get | `get_neigh_repo` | 11kneighborreport IEconsumersabsent ornotreconciled; appendhoststate + IEgenerationneeded |
| 478 | set | `wea_cac_en` | WeatherCACpolicyfieldabsent; consumerrecovery/regulatorygate required |
| 478 | get | `get_wea_cac_en` | WeatherCACpolicyfieldabsent; consumerrecovery/regulatorygate required |
| 481 | set | `set_vopt` | Currentportlackstwofields; effectivevaluewriterneededbeforeclaimingbehavior |
| 481 | get | `get_vopt` | Currentportlackstwofields; effectivevaluewriterneededbeforeclaimingbehavior |
| 491 | set | `coc_mv_to_ndfs` | CoCmovementstateabsent, behaviorconsumerunknown |
| 491 | get | `g_coc_mv_to_ndfs` | CoCmovementstateabsent, behaviorconsumerunknown |
| 493 | set | `bss_tm` | BSSTMextendedcap/managementactionconsumerrecoveryneeded |
| 493 | get | `get_bss_tm` | BSSTMextendedcap/managementactionconsumerrecoveryneeded |
| 494 | set | `mdid` | MDIE/FTauthsourceabsent; callbackmapsOEMbeaconupdate; cannotclaimFTsupportfromfieldsetter |
| 494 | get | `get_mdid` | MDIE/FTauthsourceabsent; callbackmapsOEMbeaconupdate; cannotclaimFTsupportfromfieldsetter |
| 495 | set | `ft_over_ds` | FTOverDShandlersabsent; getstubPASSED butsetterstillrequiresconsumers |
| 490 | set | `fix_legacy_rate` | Fullspanningcontrolflow/validation andPHYmodeconsumers; avoidnaivealiasexistingfixedtxrate |
