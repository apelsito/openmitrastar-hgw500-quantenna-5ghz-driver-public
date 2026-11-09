# Real management TX ingress: conditional TQE path

This immutable receipt extends the preceding full-map audit. It preserves original MuC `71dcd32849f122c09397490914227af6f77e51c34d15615028faa3d0fabb0fe4` and analysis-only ELF `f5878c74735e08516b4345e36a30e9a5c9f610c7439b471bf8cb2b9264722906`. No hardware, radio, SSH, UART, protected-partition or firmware access occurred. No R24 source was edited.

Result SHA256: `a7f84cecf7a600e687a7cd3e26cc85d6eefaa9aa10c34cf7e82dd4916e6fc75d`.
Input pins SHA256: `78ac6f0264ad76769512ddc6fd561cdac955f7fafe87e29503cab39016c946b0`.

## Established source branch

R24 `qdrv_wlan.c:8914` initializes `QDRV_WLAN_FLAG_AUC_TX` (flags_ext bit0x20). `IEEE80211_PARAM_AUC_TX` at4792 permits changing it. This is a source default, not a live-device claim. `fw_no_mu` and the selected default/nomu carrier are independent of this flag.

With AUC_TX enabled, host management takes `qdrv_tx_sch_enqueue_80211` -> `qdrv_tx_sch_enqueue_to_node` -> `qdrv_tx_to_auc`. The packet goes to TQE MuC port with node in misc_user and TID_MGMT/WLAN; it does not take mailbox1. The host frees its node reference before publication, and firmware later resolves a shared node. A future RBS adapter must preserve that actual ownership model.

With AUC_TX disabled, hardstart still uses HTXD and mailbox1. The exact mailbox1 firmware TX consumer remains unresolved. The mailbox structure was proved in the preceding receipt; this audit must not relabel that alternate consumer as recovered.

## Actual firmware chain

`0xe83ec` (160 bytes) is actual tasklet registration:28-byte record at8002be4c, bottom-half pointer+0, args+4/+8, state+16, top-half+20, context+24. `0x15696c` (202 bytes) initializes TQE ingress and registers tasklet5 bottom-half159f5c/top-half15972c. The previously proved scheduler80023900 invokes this stored callback.

`0x159f5c` pops a valid16-byte TQE descriptor. AuC ingress port7/type0 gives feedback; **LHOST ingress port4 gives a packet** to159eb8. The earlier feedback-only interpretation was incomplete; that evidence remains immutable and this newer chain supplies the correction.

`0x159eb8` (164 bytes) validates the HBM packet pointer and forwards pointer/length/node to `0x155ec8` with normal LHOST flags. `0x155ec8` (1252 bytes) identifies itself through diagnostic `qtn_auc_tqe_mgmt_xmit`, resolves the shared node, builds a108-byte FCS with `0x12d3c8`, and publishes six TQE PPCTL words before triggering e1035038. The GNU instruction window establishes actual write ordering; Ghidra pseudocode may reorder MMIO assignments and cannot be used alone as the operational model.

FCS constructor `0x12d3c8` has1530 executable bytes in two ranges:12d3c8..12d5f9 and12d638..12d9ff. The gap is a switch table, not a missing function body. It sets frame subtype/protection/retry/rate/node fields and returns the flag used for SKIP_RATE_RETRY1. Normal LHOST entry passes OCS/probe flags0. No role/channel/deadline-based CAC grant contract is established here.

## AuC boundary

The GPL enum binds OCS_TX_SUSPEND to command21. Wrapper156550 (16 bytes) invokes sync IPC1563ac withcmd21/booleanarg1/zeroarg2/arg3/irq1. Its halt/resume callers also alter global netdev/state40b4. This is not an exact-frame permission and must not be substituted for one. FCS_GIVE is sourceenum19; its carrier instruction consumer is still unqualified.

M2A management misc_user flags are0x1 skip first rate retry,0x2 OCS frame,0x4 event,0x8 probe,0x20 group. None by its definition proves CAC permission. Exact carrier instruction acceptance must now show what happens to the descriptor/FCS and OCS policy before WMAC admission, revocation and completion.

AuC default SHA256 `e1fd397ef56b061af7c91cbd422973dcfcf1382933afa226f8f428dd315f65af` and nomu SHA256 `c10018f4fe384d753a800bbbcd5ef3c85c568d0852c221009063282d30fd85f6` are separately pinned. Neither received instruction analysis in this lane.

## Receipt state

Six fresh REA queries passed and unchanged target/input hashes were rechecked. Complete pseudocode, body ranges, callers/callees, limitations, GNU windows, enum/flag definitions and pins are saved. All handles64310,88084,23359,32753,85547,52999 completed and were collected; no background job remains.

**0 qualified ops added;0 adapters created; exact CAC provider unresolved.** Preserve NULL provider and keep ioctl26/global TX plus command21/global OCS outside the exact-frame provider. Next acquisition is the selected AuC carrier's actual M2A descriptor/FCS consumer and scheduling policy, alongside the unresolved alternate mailbox1 consumer. Segment3 remains mapped at physical46e004, never overlaid on segment2 at80020000.
