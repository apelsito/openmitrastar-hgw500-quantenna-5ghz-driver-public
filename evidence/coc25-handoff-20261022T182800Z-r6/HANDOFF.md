# SOURCE25 CoC491 offline handoff — 22 October 2026

Selected qualification: `outputs/rea/driver5g-ops-20261022-coc25-qualification-r6/result.json`, SHA256 `934048082f6f04f694356695198eb78cbc099d689e8f133c52c4572c22c98814`, status `PASS_OFFLINE_COC25_INITIALIZED_RETAINED_IC_SEVEN_MODULE_CLOSURE`.

The new source atop qualified SOURCE23 implements both original private records `coc_mv_to_ndfs` and `g_coc_mv_to_ndfs` (491) and their real PM/CSA consumer. It has 591 private records, 25 original gaps recovered and 19 original gaps still absent. Two inherited original operations remain faithful EOPNOTSUPP records. This is offline control-flow and compile/link qualification, not 25 physically accepted features. No hardware, SSH, UART, flash, boot environment, RF or network access occurred in this lane.

## Selected source and compiled closure

- Source: `outputs/rea/coc491-source25-postcompletion-20261022T181500Z-r6`.
- First full source compilation: `outputs/rea/driver5g-ops-20261022-coc25-full-closure-r6`; second independent full source compilation: `outputs/rea/driver5g-ops-20261022-coc25-second-source-r6`. Every candidate pass compiles all 58 C units fresh (116 total). No baseline or first-pass objects were reused in the second pass. Both standalone receipts preserve the expected modpost failure caused by the new paired WLAN export absent from old dependency files.
- Selected joint compositions: `outputs/rea/driver5g-ops-20261022-coc25-joint-a-r6` and `outputs/rea/driver5g-ops-20261022-coc25-joint-b-r6`. Canonical `compose_hgw5g_joint_modules_dedup.py` resolves all strong imports, validates unique export contracts, rejects contradictory declarations and reports empty joint modpost diagnostics. All seven final modules are byte-identical between these independent candidate compilations.
- Same-header parent control source: `outputs/rea/coc25-header-control-source-20261022T180300Z-r2`. Control object closure: `outputs/rea/driver5g-ops-20261022-coc25-exact-control-r6`; joint control: `outputs/rea/driver5g-ops-20261022-coc25-control-joint-r6`. It reuses 54 whole candidate objects only after exact source/header/receipt identity checks and recompiles the four different units (`qdrv_wlan`, `ieee80211`, `ieee80211_wireless`, `ieee80211_node`). It is explicitly an incremental control, not 58 fresh control compilations or a second independent candidate build. The control WLAN exactly matches qualified SOURCE23.

## Module identity

| Module | Bytes | SHA256 |
| --- | ---: | --- |
| qdrv.ko | 684776 | 7c53487f459328d12eb7952b9672a7410d621302da1215a539d600c5de75f1c5 |
| wlan.ko | 633096 | 06680ba7a6a67ef2003e61f84378aa16d6e4697b78c98fcd300b6c64c2731f29 |
| wlan_scan_ap.ko | 15508 | 0345ee6674524126d322a00a10e2a74c4038dd54b18225ffd82cd96e11437093 |
| wlan_scan_sta.ko | 18740 | 8a1825a68082dd2086c6ba496ee70bd1d1663a2c0aa3acbf2f0c5da085b290a5 |
| wlan_ccmp.ko | 9792 | 7c54b944fd3f2d214dd8a4d0c98615a48b6fed1acb0b29d86176dff05c9d700f |
| wlan_tkip.ko | 13472 | 5a625b63d6a86b8f43bc28a2cae90f5a8270cdc6298711029a9aac59f58a4f22 |
| wlan_xauth.ko | 3340 | 0cc908723d5d097f11acad4eb0c4b33d5ee5b183a134657e0291c8540c145989 |

## Reconstructed behavior and Linux 5.10 adaptation

Fresh REA on the exact OEM-derived analysis image recovered the 390-byte channel-switch consumer at `0x83da0`, 256-byte worker at `0x83f28` and 184-byte completion callback at `0x84028`. The two initially unnamed latter entries were added as symbols only to a separate analysis derivative; every allocated section remained byte/address/flag-identical. These derivatives are analysis-only and must never be loaded or flashed.

The 491 setter/getter preserve the entire u32 value. The policy is consumed in the existing PM work: check current BSS CAC, region callback, active STA VAP, DFS channel and OCAC; select the exact SCS flag set, validate the candidate channel, then enter existing CSA with count 10/mode 1/flags 3. Successful idle-to-non-DFS movement stores its own last-switch state 9; a subsequent node-join trigger 13 can permit return. The trigger projection preserves the only OEM distinction consumed by this policy (13 versus all other reasons), without aliasing a channel diagnostic field.

Linux 5.10's QoS provider uses mutexes and blocking notifiers. The OEM timer callback therefore cannot invoke it directly. The selected callback finishes existing CSA and publishes a captured completion level to the existing PM worker; process context performs QoS/log/event publication. The queue lock protects completion/request publication and the stop gate. A separate pending bit prevents completion-only runs from inventing new PM transactions and prevents coalesced producers from being lost. Each consumed request captures its trigger under the same lock; a later producer remains pending for another run. BSS is resampled after completion publication.

Invalid or ANYC BSS pointers refuse a PM transaction, rather than reproducing OEM unchecked access. The local interrupt state is always restored, including the active-STA refusal path.

## Verification evidence

- Full ARC ABI: `outputs/rea/coc491-source25-full-abi-20261022T181000Z-r1`. All 376 old IC members (including SOURCE23 legacy state), 210 VAP members and 72 QDRV members preserve widths/prefix offsets. IC grows 22,652→22,672 and QDRV WLAN 30,564→30,584; QDRV fields following embedded IC shift exactly 20 bytes. VAP/QDRV VAP and wire widths remain unchanged. Absent conditional fields are explicitly reported. This witness used r5 headers; r6 headers are verified byte-identical.
- Worker/reference fixture: `outputs/rea/coc491-source25-postcompletion-fixture-20261022T181600Z-r1` — 165,888 worker cases, 66 completion callback cases, 64 queue profiles and six basic stop assertions.
- Actual 491 dispatch snippet fixture: `outputs/rea/coc491-source25-ioctl-fixture-20261022T181100Z-r1` — 262,144 signed/u32 boundary cases; r6 wireless source is hash-identical to that fixture's selected source.
- Deterministic producer/completion interleavings: `outputs/rea/coc491-source25-postcompletion-interleaving-20261022T181700Z-r1` — nine schedules including producer after request consumption, coalesced node join, completion-only publication, immediate completion scheduling and stop-before-producer.
- Independent running-worker cancel/join model: `outputs/rea/coc491-source25-r6-independent-canceljoin-20261022T205900Z-r1`. Actual helper bodies run against a Windows model of the Linux cancellation contract; worker blocked in QoS is joined before stop returns and cannot arm new CSA after the stop guard.
- Independent source review: `outputs/rea/coc491-source25-r6-independent-review-20261022T210000Z-r1/result.json`, SHA256 `b4aceaac13131e1a316dc7c7e3d25713095a65b1ca9425cb75677ed5f346ac67`. Final module receipt review is separate.
- Complete module audit: `outputs/rea/driver5g-ops-20261022-coc25-qualification-r6/complete-controlled-audit.json`. WLAN admits six fixture-bound changed bodies and five new helpers, checks seven exact diagnostic encodings, 947 other function bodies and 9,079 residual relocations. QDRV admits only `qdrv_wlan_exit`, with 830 other bodies and 13,968 residual relocations. All old named symbol attributes, allocated data, private record/count deltas, export contract deltas and anonymous executable bodies with their following owners are checked. Exact anonymous NOP changes are declared (WLAN +2 bytes, QDRV -2 bytes). Five provider modules are byte-identical to control.
- Parsed negative audit: `outputs/rea/driver5g-ops-20261022-coc25-negative-audit-r1` — all eleven deliberate unrelated function, anonymous executable, data, relocation, symbol and diagnostic changes rejected by the actual auditors.
- Qualification verifies 2,378 hashes before and after its audit; tools and final source remain frozen after PASS.

## Scope limitations and next gate

This qualification is limited to initialized, retained IC objects. The inherited QDRV initialization path ignores `ieee80211_ifattach` failure, so preinitialization failure cleanup is not proven. F355 blocks normal unload after core start; forced unload and recovery are outside this proof. Existing CSA completion and other external CSA producers are not certified as a complete IC/VAP teardown protocol. The running-worker fixture models the contract and does not exercise the physical Linux scheduler. Scalar state is not a coherent tuple.

No PM transition, CSA transition, 5 GHz client association, RF behavior, traffic, calibration/regulatory acceptance, physical boot, persistence, rollback or release is claimed. The new candidate is not firmware-eligible. SOURCE23 remains the independently reviewed prior milestone; SOURCE25 final module review and root consolidation are the next actions. Root requested no new feature work until this checkpoint is consolidated.
