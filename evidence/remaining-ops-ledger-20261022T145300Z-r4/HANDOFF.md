# Continuation evidence, 22 October 2026

All work was offline. No hardware, runtime, flash, UART or radio action occurred.
The latest whole-pair qualification found elsewhere in this workspace is
`outputs/rea/driver5g-ops-20261022-seven-pair-qualification-r5/result.json`:
`PASS_OFFLINE_COMPLETE_SEVEN_ENTRY_PAIR`. It is separate from the subsequent
source candidates below. No claim that all OEM operations are complete is made.

New exact REA evidence:

- OEM QDRV setparam body9.996bytes in four ranges:
  `outputs/rea/oem-qdrv-missingops-20261022T142000Z-setparam/rea.json`, SHA256
  `113eaabe10f7ecadf298000ab1c5ca0d796b1afd3f71a36f28a5473c59ba5ed8`.
  This proves generic SETPARAM transport command11, a76-byte payload
  `{id,value,len,data[64]}`, and local BBDF483 bool update before forwarding.
- OEM WLAN exported maximum-bandwidth getter body106bytes:
  `outputs/rea/oem-wlan-maxbw-20261022T143000Z-r1/rea.json`, SHA256
  `cd52b83f8eb4ec4c1a8abb851b0e593c75c45b8b2daf246879cd28535828b3ba`.
  It reads the same `ic+0x4df1` byte as set/get460. The corresponding retained
  source names it `ic_max_system_bw`; candidate code uses that name.
- Exact OEM exported `qdrv_wlan_cleanup_before_reload` at `.text+0x22a0c`
  clears `ic+0x4e2c`, the same field written by setter464. The corresponding
  retained source clears `ic_chan_is_set`; candidate code uses that name.

Stable source branch, preserving earlier evidence:

1. `devmode-unsupported-candidate-20261022T141000Z-r1`: seven earlier entries
   plus getter451 device mode and two faithful unsupported entries465set/495get.
   Source SHA256 `7a128781902eb6cc58f71e2b4a30ae005de7ebf168a10a9ae4139557b93a9be8`.
   Full ARC WLAN unit and actual case fixtures passed. These two unsupported
   records return OEM `-EOPNOTSUPP`; they are not implemented features.
2. `maxbw-aliases-candidate-20261022T143500Z-r1`: adds two460 aliases to that
   branch, using existing `ic_max_system_bw` low-byte store/read. Source SHA256
   `9e31def106b6d73ce0d350ce292521b814bb62dd6fc86ab7e5ca24ceac91d71a`.
   Full ARC WLAN unit and196.608 fake checks passed. No RF command is added.
3. `bbdf-recovery-candidate-20261022T144500Z-r1`: adds483 set/get. A bool is
   appended to private `struct qdrv_wlan`; every pre-existing member is preserved.
   Old/new ARC witness size30552/30556, extension30552; ic/mac/TX/RX/sp offsets
   identical. Actual complete changed QDRV and WLAN units compiled and131.072
   fake bool cases passed. Full-module rebuilding of all header users is required.
   QDRV source SHA256
   `7c163a0d0fa57197e2f0ebc138f79caaf927c6a037f63b1fcd8b36809a48603f`;
   header `154f0088d94f73a2a8bd708cbd543d80029d2f1cc7067cf7cf992582be083727`;
   WLAN `022dc5388da1dddc161673c13faaca5de468054ee9ed071bf0286a1d3ae22f6b`.
4. `check-cur-chan-candidate-20261022T145000Z-r1`: adds464 setter to that
   WLAN source, preserving raw signedint assignment through `ic_chan_is_set`.
   Source SHA256
   `538da528ed69173af54bb5f7c8001e75254fadc883b6464e6674cff2d83e9cb2`.
  131.074 Windows LLVM-MinGW fake checks passed. ARC unit compilation is UNRUN:
   the REA-Linux WSL invocation failed before Python with
   `Wsl/Service/0x8007274c`. No WSL restart or system repair was attempted here.

The fourth source branch has581 private records expected (566+15),578 exact
OEM matches expected (563+15),29 missing original records, three retained port
additions. It has13 recovered functional entries and two faithful unsupported
entries. It is SOURCE_ONLY, not a qualified full module pair. No new binary has
been installed or packaged.

Separate NAC getter prototype — do not merge silently:

`outputs/rea/nac-getter-prototype-20261022T141500Z-r1` reads only the8-byte
published NAC prefix and reproduces the OEM457 output packing. It refuses an
absent pointer or zero denominator with `-EAGAIN`, leaving output unchanged.
This is an explicit safety difference from OEM's unguarded division, which
would fault on the port's zero-initialized prefix. The actual helper fixture
and full ARC WLAN unit passed approximately16.7million test cases. It does not
claim a coherent multi-field MuC snapshot or qualified pointer lifetime, and
does not expose NAC client entries. It is not included in the stable15-record
branch; full-module/pointer-lifetime qualification remains required.

`LEDGER.md` and `result.json` enumerate every original44 gap with exact case
addresses, operand offsets/types, control semantics and remaining consumers.
Most remaining OEM additions require new host ABI fields plus their consumers,
or exact firmware handlers. Setting a stored field alone does not reproduce a
feature. No raw OEM offsets should be used in port C.

Important counterexample: OEM getter467 uses callback `ic+0x54a0`, which calls
`ieee80211_scan_refresh_scan_module_chan_list` and then `scan_pickchannel` with
flags0x130000, returning1 or-95. The current port's similarly named
`ic_dfs_chans_available_for_cac` callback has a different bool contract and scan
logic; rewiring467 to it is not a valid recovery.

Next meaningful gates: recover WSL execution without hardware operations; build
the complete15-record QDRV/WLAN pair with every changed-header user rebuilt;
qualify imports, exports, metadata and complete code/data/relocation changes;
then resume remaining29 operation consumers and ABI reconstruction separately.
