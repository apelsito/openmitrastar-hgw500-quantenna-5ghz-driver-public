# SOURCE23 op490 offline handoff — 22 October 2026

`fix_legacy_rate` is reconstructed from the exact OEM case and implemented in a new source candidate atop qualified NAC22. The selected candidate has passed complete source/header closure and strict module delta qualification. No router, UART, RF, flash, reboot, boot environment or network access occurred in this lane. Firmware eligibility remains false.

## Selected evidence

- Candidate: `outputs/rea/legacy490-source23-20261022T173000Z-r1`.
- Paired full build: `outputs/rea/driver5g-ops-20261022-legacy23-full-closure-r1/result.json` — 58 fresh C units per lane, seven modules in each lane, every paired module byte-identical, no baseline objects reused.
- Same-header parent control: `outputs/rea/legacy23-header-control-source-20261022T174000Z-r1`, built under `outputs/rea/driver5g-ops-20261022-legacy23-header-control-closure-r1/result.json` — 58 fresh units and seven modules. This control is source-identical NAC22 under SOURCE23 headers and is not a firmware candidate.
- Final qualification: `outputs/rea/driver5g-ops-20261022-legacy23-qualification-r2/result.json`, SHA256 `6f0d43e4e26d9315f2038726d5e9f1423d3e864035b172572a64a0a18409e488`, status `PASS_OFFLINE_SOURCE23_OP490_SAME_HEADER_SEVEN_MODULE_CLOSURE`; 1,864 input hashes verified before and after qualification.
- Full ABI: `outputs/rea/legacy23-independent-full-prefix-abi-20261022T194700Z-r1/result.json` — 374 parsed IC members, 210 VAP members, 72 enclosing QDRV members. Old IC/VAP offsets and widths are unchanged. IC size 22,644→22,652 and QDRV WLAN 30,556→30,564. Fields after embedded IC shift exactly eight bytes; new host integers are at 22,644/22,648. VAP 3,084 and QDRV VAP 3,540 remain unchanged. Wire widths 32/76/64/20 remain unchanged. Absent conditional fields are recorded explicitly.
- ABI dependency supplement: `outputs/rea/legacy23-independent-full-prefix-abi-pins-20261022T195000Z-r2`. Its 912 hashes describe current actual witness dependencies after compilation; they are not represented as full prebuild hashes.
- Actual transaction fixture: `outputs/rea/legacy23-full-transaction-fixture-20261022T195700Z-r2/result.json` — 3,548,272 operations, every u16 current mode and all 13 admitted rate values; independent expected target/state/profile/order checks, repeated set, any-negative restore, invalid no-mutation exits. Host callbacks are fakes.
- Negative audit: `outputs/rea/driver5g-ops-20261022-legacy23-negative-audit-r1/result.json` — all seven deliberate changes rejected: unrelated body, executable bytes, data, private table, relocation, symbol and diagnostic bytes.

## Scope of the exact residual audit

All 34 QDRV units, the 18 unchanged WLAN units, and five provider units (57 units in total) match the complete same-header control as whole objects. QDRV and all five provider modules also match the same-header control as whole files. The control WLAN matches the qualified NAC22 WLAN exactly.

Only WLAN setter `ieee80211_ioctl_setparam` and the new 328-byte `om_fix_legacy_rate` helper are admitted semantic bodies. The setter's exact additional two-byte alignment NOP is checked. Three exact source diagnostic encodings are checked separately. The remaining 955 function bodies, 9,200 relocation records, existing symbol attributes, 28 sections, anonymous executable bytes and all non-table data are audited. The single new private record is exactly `(490,18433,0,"fix_legacy_rate")`, and the private count changes 588→589. The setter-to-helper link and the helper's exact links to `ieee80211_param_to_qdrv`, `ieee80211_setmode`, and `ieee80211_wireless_reassoc` are explicitly checked.

The source-identical header control's 154 changed QDRV function hashes are retained as a layout delta inventory, not declared instruction-equivalent by masking their bodies. Selected SOURCE23 QDRV bytes must exactly match that full control.

## Selected module identity

| Module | Bytes | SHA256 |
| --- | ---: | --- |
| qdrv.ko | 684740 | ca6fcb24a2c6e9c051d27a7220a70e034fd3479378f7c7d68c951982b6e802f6 |
| wlan.ko | 631120 | 486818be02666694706d1660f49ad3b41a05b197d4c4b5bc42dd6476cd3b2d11 |
| wlan_scan_ap.ko | 15508 | 0345ee6674524126d322a00a10e2a74c4038dd54b18225ffd82cd96e11437093 |
| wlan_scan_sta.ko | 18740 | 8a1825a68082dd2086c6ba496ee70bd1d1663a2c0aa3acbf2f0c5da085b290a5 |
| wlan_ccmp.ko | 9792 | 7c54b944fd3f2d214dd8a4d0c98615a48b6fed1acb0b29d86176dff05c9d700f |
| wlan_tkip.ko | 13472 | 5a625b63d6a86b8f43bc28a2cae90f5a8270cdc6298711029a9aac59f58a4f22 |
| wlan_xauth.ko | 3340 | 0cc908723d5d097f11acad4eb0c4b33d5ee5b183a134657e0291c8540c145989 |

## Behavior and limits

Admitted values 0..12 encode `0x7f000000 | v | (v<<8) | (v<<16)` and forward via existing parameter 62. The first set retains the original current mode until restoration; repeated sets preserve it. Any negative value restores when fixed state is active; inactive restore and positive values above 12 return EINVAL without mutations or callbacks. State/wire/mode/profile/reassociation/HOSTAP channel ordering follows the OEM flow. Existing zero initialization includes both appended host integers. All header users must use the new matching build; old QDRV modules must never be mixed with this host layout.

Void callbacks cannot prove asynchronous PHY/MuC completion, rollback after a runtime failure or rate transition success. Inherited two faithful EOPNOTSUPP records and NAC sampled-prefix/zero-denominator limitations remain. No physical fixed-rate, radio, traffic, aggregation, boot, persistence, rollback or release acceptance is claimed.

The actual private table has 589 records: 23 original gaps recovered (including two faithfully unsupported records), three existing port additions and 21 original gaps still absent. The remaining exact original tuple names are recorded separately in `remaining-records.json`. Do not add separate beacon/TQE prototype counts to this lane.

## Immutable failed attempts

The first enlarged fixture output `legacy23-full-transaction-fixture-20261022T195700Z-r1` completed behavioral assertions and failed only its mistaken expected total; r2 corrects the total. Qualification r1 failed on Windows absolute path normalization before the residual audit. Both old output directories are retained; r2 is the selected qualification. Independent final review is managed by `/root/independent_review`.
