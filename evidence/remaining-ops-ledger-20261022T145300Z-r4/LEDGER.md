# Missing OEM operation ledger — 22 October 2026

ExactOEMcase mappings and source constraints. No hardware actions.

| ID | Name | Direction | OEM evidence | Remaining requirement |
| --- | --- | --- | --- | --- |
|331|get_cca_fixed|get|ic.cca_fix_disable u8|Seven-entrySOURCE/fixture recovered; fullpairqualification separate|
|246|tx_maxmpdu|set|existingTXMAXMPDU setter withVHTsizevalidation|Twoaliases completeWLANmodulequalified separately|
|246|get_tx_maxmpdu|get|existingiv_tx_max_amsdu byte|Twoaliases completeWLANmodulequalified separately|
|275|get_auto_cca_en|get|ic.auto_cca_enable u8|Seven-entrySOURCE/fixture recovered; fullpairqualification separate|
|484|txamsdu_11n|set|ifchanged:!!value,SET_PARAMS11 id484,reassoc(vap,0,1)|Distinct11nAMSDUfieldabsent; retainediv_tx_amsdu (VAPTXAMSDU302) isdifferentfield; requiresMuCnode+0x80consumer|
|484|get_txamsdu_11n|get|u8|Distinct11nAMSDUfieldabsent; retainediv_tx_amsdu (VAPTXAMSDU302) isdifferentfield; requiresMuCnode+0x80consumer|
|498|set_dup_rts|set|low8+SET_PARAMS11 id498→MuCFUN11ee48|DupRTSstate/consumerabsent, MuChandlerhardwarecontractneeded|
|498|get_dup_rts|get|u8|DupRTSstate/consumerabsent, MuChandlerhardwarecontractneeded|
|449|20_40_bss_coex|set|!!value,return0|20/40coexistence consumers absent or not reconciled; requires append-only ABIstate and actual beacon/control integration|
|450|get_min_cac|get|constant70|ObservedOEMMIN_CAC_PERIOD baseline compatibility policy|
|457|nac_mode|set|HOSTAPonly genericSET_PARAMS11 id457→MuCFUN16548c|Sharedpointeralreadyallocated/published; zero denominatorguard-EAGAIN prototypePASSED; setter needsMuCtimer/monitor lifecycle evidence|
|457|get_nac_mode|get|(value1<<16)\|((value0*100/value1 &255)<<8)\|enabledlow8|Sharedpointeralreadyallocated/published; zero denominatorguard-EAGAIN prototypePASSED; setter needsMuCtimer/monitor lifecycle evidence|
|451|get_dev_mode|get|role1/2 else repeaterbit0x40000=>3 else0|existingEXTENDER_ROLE283 and REPEATER38 anchor samefields; SOURCEcandidatePASSED|
|460|set_max_bw|set|low8value,return0,noQDRVcommand|OEMexportedget_max_system_bw reads samebyte; portfunctionnamesic_max_system_bw; SOURCEcandidatePASSED|
|460|get_max_bw|get|u8|OEMexportedget_max_system_bw reads samebyte; portfunctionnamesic_max_system_bw; SOURCEcandidatePASSED|
|461|set_bw_autosel|set|low8value,return0|Portretainedic_max_system_bw adjacentlayout differs; newfieldnotidentified; preserveconsumersbeforeextension|
|461|get_bw_autosel|get|u8|Portretainedic_max_system_bw adjacentlayout differs; newfieldnotidentified; preserveconsumersbeforeextension|
|465|ignr_icac_sel|set|-EOPNOTSUPP default|NoimplementationinexactOEM; SOURCEstubPASSED|
|467|dfs_chan_avail|get|refresh_scan_module_chan_list(ic),pickchannel(ic,0x130000)→1 or-95|Portcallbackic_dfs_chans_available_for_cac hasdifferentbool/sideeffects; missingrefreshexport/source, noequivalence|
|464|check_cur_chan|set|value,return0|ExactOEMexportcleanup_before_reload clears4e2c andcurrentequivalentclearsic_chan_is_set; SOURCEcandidate andhostfixturePASS, ARCcompileblockedWSL|
|466|rbs_dfs_msr_cac|set|!!value,return0|RBSspecificstateabsent; mustnotmaptoSTAstrictfieldwithdifferentvalidation|
|466|g_rbsdfs_msrcac|get|u8|RBSspecificstateabsent; mustnotmaptoSTAstrictfieldwithdifferentvalidation|
|468|rbs_dfs_end_tx|set|1..1000 inclusive else-EINVAL|RBSclosingtimefieldabsent; OEMinit500, STAtimefielddifferentcontract|
|468|g_rbsdfs_end_tx|get|u32|RBSclosingtimefieldabsent; OEMinit500, STAtimefielddifferentcontract|
|492|neigh_repo|set|value==1setelseclear;beacon_update_all(ic)|11kneighborreport IEconsumersabsent ornotreconciled; appendhoststate + IEgenerationneeded|
|492|get_neigh_repo|get|bit0|11kneighborreport IEconsumersabsent ornotreconciled; appendhoststate + IEgenerationneeded|
|463|set_dyn_sifs|set|genericSET_PARAMS11 id463→MuCFUN11f01c|76Bpayload exact; signed/range/changedMuChardwareeffectsneedfunctionanalysis|
|478|wea_cac_en|set|!!value,return0|WeatherCACpolicyfieldabsent; consumerrecovery/regulatorygate required|
|478|get_wea_cac_en|get|u8|WeatherCACpolicyfieldabsent; consumerrecovery/regulatorygate required|
|481|set_vopt|set|ic_opmodeHOSTAP andvalue0..2 else-EOPNOTSUPP; storelow8|Currentportlackstwofields; effectivevaluewriterneededbeforeclaimingbehavior|
|481|get_vopt|get|HOSTAP:(request&15)<<4\|effective else0|Currentportlackstwofields; effectivevaluewriterneededbeforeclaimingbehavior|
|483|bbdf_war_en|set|WLANgenericforward; QDRV!!value thenSET_PARAMS11 id483→MuCDAT18c030bool|SOURCEprivateqdrv_wlan appendbool keepsalloldoffsets; witnesses131072cases+ARCunitsPASS; fullmoduleheaderdep rebuildandMuCgatepending|
|483|get_bbdfwar_en|get|WLANic_getparam→QDRVboolfield|SOURCEprivateqdrv_wlan appendbool keepsalloldoffsets; witnesses131072cases+ARCunitsPASS; fullmoduleheaderdep rebuildandMuCgatepending|
|491|coc_mv_to_ndfs|set|value,return0|CoCmovementstateabsent, behaviorconsumerunknown|
|491|g_coc_mv_to_ndfs|get|u32|CoCmovementstateabsent, behaviorconsumerunknown|
|493|bss_tm|set|value==1setelseclear;beacon_update_all(ic)|BSSTMextendedcap/managementactionconsumerrecoveryneeded|
|493|get_bss_tm|get|bit0|BSSTMextendedcap/managementactionconsumerrecoveryneeded|
|494|mdid|set|HOSTAPonly else-EINVAL;storelow16 thenic+0x5444callback|MDIE/FTauthsourceabsent; callbackmapsOEMbeaconupdate; cannotclaimFTsupportfromfieldsetter|
|494|get_mdid|get|u16|MDIE/FTauthsourceabsent; callbackmapsOEMbeaconupdate; cannotclaimFTsupportfromfieldsetter|
|495|ft_over_ds|set|HOSTAPonlyelse-EINVAL;storelow8|FTOverDShandlersabsent; getstubPASSED butsetterstillrequiresconsumers|
|495|get_ft_over_ds|get|-EOPNOTSUPP default|FTOverDShandlersabsent; getstubPASSED butsetterstillrequiresconsumers|
|496|get_sretry_lmt|get|QDRVreturn8|ExactOEMbaselinepolicyonly; seven-entrycandidatePASSEDoffline|
|497|get_lretry_lmt|get|QDRVreturn8|ExactOEMbaselinepolicyonly; seven-entrycandidatePASSEDoffline|
|490|fix_legacy_rate|set|value-1restoreor0..12 rateencoding0x7f000000\|v\|v<<8\|v<<16; SET_PARAMS62;media/channelcallbacks|Fullspanningcontrolflow/validation andPHYmodeconsumers; avoidnaivealiasexistingfixedtxrate|
