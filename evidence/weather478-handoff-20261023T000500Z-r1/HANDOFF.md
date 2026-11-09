# Weather478 source34 offline handoff

Qualification: `outputs\rea\weather478-qualification-20261023T000100Z-r1\result.json`
SHA-256: `1c6296d7ee0111d757d19d9147971d3635df0d7c4e73c665ee5ad8bdea0187e7`

34 original private records reconstructed offline; 600 private table entries. All 598 qualified32 entries retained exactly, plus the separate set478/get478 records. No bounded467 included.

OEM setter478 stores bool(value != 0) at IC+0xbf4; getter478 reads that unsigned byte; whole OEM AP picker checks zero before rejecting a weather channel. Fresh OEM QDRV init clears the complete 0x8004-byte allocation before ieee80211_ifattach and contains no explicit IC+0xbf4 write. This supports construction default zero; external callees and async paths were not universally replayed.

Port IC+22605 uses one former padding byte. ARC witness preserves 387 IC, 210 VAP and 72 QDRV fields and eight wire/enclosing sizes, with conditional omissions reported. Port kmalloc is followed by memset of the complete QDRV structure before 802.11 initialization; explicit default zero is assigned after ifattach. The fixture executes those exact zero/default fragments for 256 initial-byte patterns using ARC witnessed sizes.

Weather policy affects only the existing AP weather exclusion predicate. All channel validation, radar, CAC and 600-second weather-CAC paths preserve source bytes. Nothing permits bypassing those gates. The retained AP metric, width, alternate and ANYC fallback are not universal OEM ranking proof.

Verification: actual whole AP start/pick 57,344 cases with explicit fake radio/channel helpers; actual setter/getter 655,360 cases; preserved actual461 policy 262,144 cases. Three changed units compiled twice, 110 exact qualified32 objects reused, all seven joint modules identical and modpost empty. Complete audits preserve 829 QDRV, 997 WLAN and 26 AP old functions; residual relocations 13,683 / 9,363 / 141. Sixteen parsed tamper cases rejected.

Current input pins verified: 5544.

## Modules

- qdrv: 684948 bytes, SHA-256 `9fe3fc4001cc43e0dbd3b4beb449e7f3db928f0e71453760e47f201c28aa1e09`
- wlan: 660792 bytes, SHA-256 `89305c00819f74e0607679834b8e2c9c1b93d9c8c880c5e676b55fbe05af1404`
- wlan_scan_ap: 15868 bytes, SHA-256 `ff9d0c3a123a00b6b7dc1a68b4dd61bcb0175d16f0999e8732113d9ecf144148`
- wlan_scan_sta: 18740 bytes, SHA-256 `8a1825a68082dd2086c6ba496ee70bd1d1663a2c0aa3acbf2f0c5da085b290a5`
- wlan_ccmp: 9760 bytes, SHA-256 `1eb355a9b96b2ee2a75382565f6a628d62c583aa21600f9b922d038f657d96d5`
- wlan_tkip: 13440 bytes, SHA-256 `02f83895c1e3343ee3cc0b2162e37e9428c66de36fa0e2b8a5ddd834d20dbf5f`
- wlan_xauth: 3340 bytes, SHA-256 `0cc908723d5d097f11acad4eb0c4b33d5ee5b183a134657e0291c8540c145989`

## Boundaries

Offline closure only. No hardware, RF, client AUTH/ASSOC/AUTHORIZED, traffic, cold boot, persistence, rollback or release acceptance. All qualified32 scope limitations remain. Separate root independent review and combined37/467 qualification are pending.
