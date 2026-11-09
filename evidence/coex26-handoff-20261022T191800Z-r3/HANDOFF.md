# Coex26 r3 — OEM449 recovery handoff, 22 October 2026

Selected source: `outputs/rea/coex449-source26-bss-geometry-20261022T192600Z-r3`. Selected qualification: `outputs/rea/driver5g-ops-20261022-coex26-qualification-r3/result.json`, status `PASS_OFFLINE_COEX26_R3_OEM_BSS_GEOMETRY_TIMER_RX_BW_MODULE_CLOSURE`, SHA256 `4bd311834052a7b421094bd148f61a341b71c41efbfdff10a2f301c0b095adb3`.

The candidate recovers the exact original setter record `(449,18433,0,"20_40_bss_coex")`, its default-enabled boolean policy, and the actual timer/scan/RX bandwidth consumers. This lane now has 592 private records, 26 original gaps recovered and 18 original gaps absent. Two inherited recovered records are faithful EOPNOTSUPP records. These counts are private-record recovery, not physical feature acceptance; do not combine them with separate prototype counts.

No router, SSH, UART, flash, reboot, RF, traffic or boot-environment access occurred. The selected candidate is not firmware-eligible.

## Real behavior recovered

Fresh immutable OEM REA evidence identifies policy field `IC+0x5dac`: default 1 in QDRV initialization and setter stores `!!value`. Consumers are `ieee80211_start_obss_scan_timer`, scan inventory bandwidth checks, public coexistence actions, AP association intolerance, current-BSS STA HT information and non-HT neighboring AP beacons. The retained source had these consumers without the OEM policy gates and used a narrower BW71 forwarding sequence.

The standalone recovered `om_ieee80211_change_bw` helper has SHA256 `f28fb457437b00acb7b8008ad26507b0e170b68fd68a89978e52a95368e81e18` at `outputs/rea/coex449-changebw-helper-20261022T191000Z-r1/om_coex449_change_bw.c`. It preserves OEM order: forward parameter 71, select the 20/40/80 MHz active-channel bitmap, update maximum-power and normal-power values from the existing country/power tables for active channels, then optionally invoke the existing channel callback. It does not create regulatory entries, edit calibration or establish completion of the void callbacks. Its arithmetic and order passed 9,216 cases with 256 channel rows per case; the actual typed ARC helper compiled.

The OEM 20/40 geometric checks use `ic_bsschan` (`IC+0x4e08`), while the retained source used `ic_curchan`. R3 reconciles this material difference with a single BSS snapshot, OEM low16 frequency bounds and signed8 scan input conversion. Current-channel movement during scanning therefore does not substitute for BSS geometry. NULL/ANYC BSS refuses the operation instead of reproducing OEM unchecked dereference.

The public coexistence parser requires the three-byte coexistence IE payload, its declared length exactly 1, and a present channel report's declared bytes before node mutation. This is an explicit safety adaptation of the retained parser's short/OOB reads. Existing beacon IE producers are preserved: no flag-controlled beacon IE removal was observed in the exact OEM policy consumer evidence.

## ABI and compilation

The u8 policy occupies prior IC padding byte **22667**, between `om_coc_pm_pending` at 22666 and `om_coc_completed_level` at 22668. The actual ARC witness checks all 384 previous IC members, 210 VAP members and 72 QDRV members, preserving offsets, widths and sizes. IC remains 22672, QDRV WLAN 30584, VAP 3084 and QDRV VAP 3540. Wire layouts remain unchanged. Conditional absent members are explicitly listed.

Header additions share existing declaration lines; the previous 2327 line positions remain unchanged, so unrelated inline `__LINE__` diagnostic encodings are not shifted. Every other header byte is compared after the two exact declared additions.

The original r2 stage compiled four changed units independently per lane (eight compilations). R3 recompiles only `ieee80211_input` twice for the new BSS geometry and retains the six other already paired r2 changed-unit compilations. Each final lane reuses 54 whole qualified CoC25 objects (108 across both lanes) only under exact unchanged-source, unchanged old-member ABI and preserved header line positions. No 116-fresh-compilation claim is made for Coex26.

Selected r3 joint compositions are `driver5g-ops-20261022-coex26-joint-a-r3` and `driver5g-ops-20261022-coex26-joint-b-r3`. They validate all strong imports, unique export contracts, empty joint modpost diagnostics and byte-identical module pairs.

| Module | Bytes | SHA256 |
| --- | ---: | --- |
| qdrv.ko | 684792 | 5a5212e13b6d173b5774a261b142d6ca0e8b6efbfc6f6dfdc430351da27c561e |
| wlan.ko | 633956 | f10246f46abf150e01648bacab88a5d62642d800c0c3a3f8e5d8c3bcee1ff587 |
| wlan_scan_ap.ko | 15508 | 0345ee6674524126d322a00a10e2a74c4038dd54b18225ffd82cd96e11437093 |
| wlan_scan_sta.ko | 18740 | 8a1825a68082dd2086c6ba496ee70bd1d1663a2c0aa3acbf2f0c5da085b290a5 |
| wlan_ccmp.ko | 9792 | 7c54b944fd3f2d214dd8a4d0c98615a48b6fed1acb0b29d86176dff05c9d700f |
| wlan_tkip.ko | 13472 | 5a625b63d6a86b8f43bc28a2cae90f5a8270cdc6298711029a9aac59f58a4f22 |
| wlan_xauth.ko | 3340 | 0cc908723d5d097f11acad4eb0c4b33d5ee5b183a134657e0291c8540c145989 |

## Qualification and reviewed limits

Qualification verifies 3471 hashes before and after the exact module audit. QDRV admits only its initialization body plus exact diagnostic changes; 829 other function bodies and 13679 residual relocations match. WLAN admits six exact source/fixture-bound bodies (including BSS geometry), the new bandwidth helper and seven exact diagnostic encodings; 952 other bodies and 8608 residual relocations match. Old named symbol attributes, allocated data, private table/count, new export contracts and all anonymous executable body owners are checked. The five provider modules match their qualified CoC25 modules byte for byte. All eleven deliberate unrelated function/anonymous/data/relocation/symbol/diagnostic mutations are rejected.

Actual fixture scopes are:

- 3538944 public-parser length/policy/report cases and 256 coexistence IE length cases.
- 248254 timer, scan inventory, association, STA BSS identity and non-HT beacon gate cases.
- 196608 signed/u32 setter boundary cases.
- 9216 bandwidth-helper cases × 256 channel rows, plus actual typed ARC compilation.
- 33030144 geometric pair cases and 1956864 report-channel cases, with BSS/current deliberately different and four invalid-BSS refusal checks.
- 1441792 raw public-parser cases calling the actual BSS geometric helper.

Independent current source review is `outputs/rea/coex449-r3-independent-review-20261022T215800Z-r1/result.json`, SHA256 `e3753379acd7a7312f11b8d7dfe76dde007ed63fb8cd8b2fd69bdc63995eeb3b`. Its earlier r2 NO_GO is retained separately: fake allowed-channel fixtures did not cover the BSS/current mismatch. R3 closes that source blocker with actual geometric and parser tests. R2 module artifacts are not selected for R3.

Other received management branches have their changed decision fragments tested; their complete frame parsers have not all been replayed. No physical BSS/current lifetime, RF, PHY, MuC bandwidth transition, regulatory/calibration correctness, client traffic, persistence, recovery or release acceptance is established. Inherited CoC25 initialized/retained-IC, NAC snapshot and faithful unsupported-record limitations remain. Root owns integration into its later source candidate and must preserve these distinctions; this handoff itself authorizes no device operation.
