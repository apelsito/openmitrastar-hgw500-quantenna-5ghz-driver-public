# DFS467 bounded offline recovery — 22 October 2026

Selected source: `outputs/rea/dfs467-bounded-source-20261022T225000Z-r2`, derived independently from qualified Consolidated29. This source and its current module pair implement the exact getter sequence within **initialized AP scanner, 5 GHz scope**. The all-mode OEM467 record remains unqualified; this artifact must not promote the full-driver record count.

No router, SSH, UART, flash, reboot, RF or network access occurred. No Git or publication action occurred in this lane.

## Exact OEM observations

- Getter467 dispatch calls the function at OEM IC callback offset `0x54a0`, writes its result as the WEXT output value and returns 0 from the ioctl. The value is 1 or -95, rather than an ioctl errno.
- The callback's original QDRV `.text` offset `0x2740c` maps to linked analysis address **0x3740c**. It calls `ieee80211_scan_refresh_scan_module_chan_list(ic)` followed by `ieee80211_scan_pickchannel(ic,0x130000)`, then returns 1 for a non-NULL result or EOPNOTSUPP otherwise. The earlier ledger used an offset as if it were a linked address; that distinction is corrected here.
- Refresh invokes the scanner's `scan_start(ss,ss_vap)` list-construction callback. This is separate from the driver RF scan-start callback.
- The actual AP scanner picker is original `.text+0xbe0`, linked `0x10be0`, recovered from the original scanner structure relocation at offset36. Its exact flag `0x130000` filters for DFS after channel validation. It does **not** mean CAC-ready-only or available-only.
- OEM phymodes2/3/8/9 delegate AP picking to the separate chanset461 subsystem. They are not covered by this bounded recovery.

REA evidence is retained under `windows-oem-dfs467-callback-20261022T223300Z-r2`, `windows-oem-dfs467-scanap-pick-20261022T223500Z-r1` and `windows-oem-dfs467-refresh-20261022T223500Z-r1`. Explicit entry symbols were added only to an independent analysis derivative, never to firmware. The earlier scan-AP R2 label at0x11fb8 actually described detach/free; it is preserved and excluded from this recovery evidence.

## Implementation and explicit deviations

The new `om_ieee80211_dfs_channel_available(ic)` helper admits modes1/7/10/11–14 and an initialized AP scanner with non-NULL private state, VAP, scan-start and pick callbacks. It refreshes the list, checks its size and delegates actual picking with0x130000. The existing AP picker gets the exact DFS-only filter for that flag; the other port metric, bandwidth, primary-inactive, weather, fallback and alternate-channel machinery is preserved.

The existing `ic_dfs_chans_available_for_cac` boolean callback is untouched and is not aliased to this getter. Its channel-specific and scan-list semantics differ from the OEM467 transaction.

Unsupported/2.4 GHz modes, STA/missing scanner state and missing callbacks return value -95. A missing scanner-start callback originally could leave a stale old list available; the bounded implementation refuses that incomplete path. An empty or out-of-range refreshed list refuses before picking, preventing the inherited/OEM ANYC fallback from returning stale `ss_chans[0]`. These are declared safety/scope deviations.

The query's READ_ONCE scalar snapshots do not establish stable scanner object lifetime across hot detach or concurrent scans. These operations remain outside the proof. The remaining AP picker is not instruction-equivalent to the complete OEM AP picker: metric calculation, alternate updates and fallback retain port behavior. The fixture covers20MHz with deterministic random bytes and fake validation/availability/secondary-channel callbacks; it does not establish all40/80MHz or full-OEM ranking behavior.

## Current offline evidence

- Whole actual query, AP list-construction and AP picker body fixture: `dfs467-bounded-actual-refresh-pick-20261022T225100Z-r3`, 28672 cases (seven admitted phymodes ×4096 three-channel DFS/radar/validation/weather combinations), plus96 missing-state/mode refusals. The list begins stale and is rebuilt before the real picker runs. Empty-list refusal prevents selection. Actual bodies run; channel validation/availability/secondary callbacks are explicitly fake.
- Getter dispatch fixture: `dfs467-bounded-getter-fixture-20261022T224500Z-r1`, 131072 masked-id/result cases. The output value is1/-95 and ioctl return0.
- Three actual ARC units compile: scan, wireless and scan-AP (`dfs467-bounded-arc-syntax-20261022T224200Z-r1`). R2's list-size guard is subsequently compiled twice in the final scan revision.
- Full ARC ABI: `dfs467-bounded-full-abi-20261022T224300Z-r1`, all386 existing IC fields,210 VAP fields and72 QDRV fields preserve widths, offsets and sizes. R2 headers are byte-identical to the witnessed R1 headers. The only header change is an inline function prototype; prior line positions are preserved.
- Initial change compiled scan, wireless and scan-AP independently in both lanes. R2 recompiles only scan twice, retains four earlier paired changed-unit compilations and reuses110 whole qualified-parent units across both lanes. No116-fresh-compilation claim is made.
- Final joint compositions: `dfs467-bounded-joint-a-20261022T225300Z-r2` and `dfs467-bounded-joint-b-20261022T225300Z-r2`. Both pass strong-import closure and unique retained/candidate export contracts, with empty modpost diagnostics. Every paired module is byte-identical.
- Global residual audit: WLAN admits the getter body and new helper plus four exact diagnostic updates;970 other bodies,9606 residual relocations and old symbol attributes/data/anonymous executable owners match. AP scanner admits only its picker plus one exact diagnostic update;26 other bodies and138 residual relocations match. Exact anonymous NOP changes are+2 bytes in each module's text. The five other modules are byte-identical to qualified29.
- Parsed negative audit: `dfs467-bounded-negative-20261022T225500Z-r1`, all ten intentional unrelated function, anonymous executable, data, relocation, symbol and diagnostic changes rejected by the actual auditors.

## Module pair identity

| Module | Bytes | SHA256 |
| --- | ---: | --- |
| qdrv.ko | 684928 | e456176906d31c34d71b52944e3c8cdaf9ac700b4a7cf5b6ad97d5967c9ea743 |
| wlan.ko | 638712 | db1d386286b7e0208b4abd43ce819cd4976a3681ad6e7474d14cd0dc9a3e9675 |
| wlan_scan_ap.ko | 15524 | 9422f2245defece656cc1d2fcc31b09d1983abec9bc6b572bb05657544f3e595 |
| wlan_scan_sta.ko | 18740 | 8a1825a68082dd2086c6ba496ee70bd1d1663a2c0aa3acbf2f0c5da085b290a5 |
| wlan_ccmp.ko | 9760 | 1eb355a9b96b2ee2a75382565f6a628d62c583aa21600f9b922d038f657d96d5 |
| wlan_tkip.ko | 13440 | 02f83895c1e3343ee3cc0b2162e37e9428c66de36fa0e2b8a5ddd834d20dbf5f |
| wlan_xauth.ko | 3340 | 0cc908723d5d097f11acad4eb0c4b33d5ee5b183a134657e0291c8540c145989 |

## Remaining gate

Independent bounded source/module review and its receipt are pending at the time of this immutable handoff. `tools/qualify_hgw5g_dfs467_bounded_closure.py` is prepared to bind that receipt to current source, tests, ABI, module pair, negative audit and input hashes. Its output will remain explicitly bounded; it must retain `allmode_oem467_recovery=false` and leave the qualified all-mode record count unchanged. No RF/PHY, regulatory, calibration, client traffic, persistence, recovery or release acceptance is established by these offline gates.
