# Exact AuC nomu checkpoint for Analiza firmware Movistar REA

Owner priority now permits compiling without functional5GHz. This lane stops at the sealed static AuC checkpoint; no new driver implementation, radio action or further REA job is pending.

Exact carrier: `auc_driver.nomu.0.bin`,33342 bytes, SHA256 `c10018f4fe384d753a800bbbcd5ef3c85c568d0852c221009063282d30fd85f6`, pinned by R24 firmware profile. Default AuC SHA256 `e1fd397ef56b061af7c91cbd422973dcfcf1382933afa226f8f428dd315f65af` is distinct and was not analyzed.

Result SHA256 `898c1634ff5d93d429633600d67184c3b4c8c0a4d5fb4d271c1944f9faa2a7db`; pins SHA256 `e7d16857525dc03966a144bd68b9d29bde8111d328d328244ae8c110467eb3b1`.

## Where to resume

MuC chain from previous receipt: AUC_TX source default inR24 → hostmanagementTQE/MuCport → registeredtasklet5/159f5c →159eb8 →155ec8 realmanagementproducer →12d3c8 FCS108B →M2A PPCTL. NormalLHOST flagsOCS/probe0. Mailbox1 is the alternate AUC_TX-disabled path and its exactconsumer remains unresolved.

AuC executable code now proves the M2A misc_user flag ingestion: e5004956 reads TQEdescriptorword+4 high16; bit0 setsinternalframe+78 bit0; bit1 at496e invokes09e4 and setsinternalframe+78 bit2; bit3 at49e0 setsinternalframe+78 bits3/4. Their matchingGPL definitions are skipfirstretry, OCS andprobe. This is internalflag propagation, not a CACgrant orRFadmission proof.

Signedhalfwordcommandtable at e50051b0 mapscmd21 to **e50056f4**. FreshREA80-byte clause e50056f4..e5005743 proves globalarg1→GP+dc=e51011a4, mirroredGP+e4=e51011ac. Unsuspend zero records timestampGP+e0 and walksreadyqueueGP+60 through1770. Counters e510368c/3690 match power-managementsleep/awake stats. Noframeidentity, role,channel ordeadline in this clause. The scheduling policy later checksGP+dc at e5003f78..3f82; completeWMAC/CAC semantics remain unqualified. **Do not substitute command21/global OCS orioctl26/globalTX as an exact-frame provider.**

Cmd19/FCS_GIVE maps e50056c8 and checkswmac<2 plusFCSringavailability before0cf4 enqueue. It does not establishCACpermission.

## Correct temporal loading

Initial analysis ELF SHA256 `d52e91bdaba98c86611cfe13866a809c3bf14156469bb3e46683d781b2dc0a51` preserves paddr-first loader bytes and NOBITSzero-fill, withFWINFO nonallocated. It keeps segment7nativebootstrap e50073b0 separate from segment8loadcopy63b3b0.

Entrye5000000→e50073b0 setsGP=e51010c8/SP=e5104000. Originale5007404 copiesseg8from63b3b0 toe50073b0 length0xb34 throughmemcpy5c50, then continues normalstartup. New **poststart model** SHA256 `501513f36ec7f347a3722f9fb806472445ccbd40f9a8a9f8dcb877d16dbb1293` places exactly those copiedbytes at their executionaddress. It is inferredstaticstate, never physicalboot evidence. Exceptione50001ac separately copiesseg6from63bee4 toe5000000 length0x1e0; that alternate overlay is not applied to this normalpoststart model.

## Material decoder limit

AuC uses custominstructions that GNU reports as `.word`/r57 and the available GhidraARCdecoder does not fully qualify. Fivequeryresults returnedexit0 andoneinitialIRQlookup failedbecause nofunctionautocreated; allreceipts are preserved. Decompilerwarnings, terminalflows and seededforensicsymbolsplit ofcmd21 fromdispatchbody must remain explicit. Source/profile/hash/mapping and boundedGNUflag/tablebytes are useful; fullschedule→WMAC/CAC cannot be asserted from them.

Next useful acquisition is qualifiedAuCcustomISA and exactM2A/FCSconsumer throughqueue/WMAC, plus perframeOCS/probe exception versus legitimateCACmeasurementpermission, grantrevoke andcompletion. No adapter can be implemented from globalcmd21 alone. PreserveNULLprovider and0opspromotion.

## Artifacts and process state

`result.json` lists exactquerydirectories andranges. Fullpseudocode/dossiers and boundedGNUwindows are adjacent; `ipc-relative-command-table.json` is decoded from exactcarrierbytes. Reproducers are `tools/derive_rbs_auc_nomu_loaded_image.py`, `tools/derive_rbs_auc_nomu_poststart_entries.py`, and `tools/seal_rbs_auc_nomu_checkpoint.py`.

Handles80873,65835,71754,31811,50538,36058 allcompleted andwerecollected. **0activejobs;0hardware;0R24edits;0opsadded;0adapter.** Compilation, runtime5GHz, clienttraffic, persistence andrelease remain distinct. RootR26 work was not touched by this lane.
