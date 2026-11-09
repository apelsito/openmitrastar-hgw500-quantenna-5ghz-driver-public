R15 scanner checkpoint for the owning root agent. No hardware, firmware, Git or publication action was performed.

Selected source is `outputs/rea/chanset461-kernel-adapter-20261022T193500Z-r15`.
Only new R14/R15 candidate directories were edited. The R13 parent is immutable.
R14 removes the duplicate `ss_next++` after the channel fetch already increments it.
R15 gates HostAP beacon-stop on local `om_start_owned` admission. Legacy return behavior stays intact.

Source scan SHA256: `539ba2dc423ea36feaf4151411bda59c2ba8c07503a1241dfaa38c29fad7f266`.
R14→R15 patch SHA256: `aab5c4c97a08385bc389689b58a83ecf833b26407e1a0412591a1826f67484c0`.
The current patch is explicitly incremental. Parent incremental R13→R14 patch and the retained initial R13 patch have distinct filenames in R15. Do not present the initial parent patch as a complete current cumulative diff.

ARC selected scan unit passed with 1,138 pins unchanged: 51,232 bytes, EM_ARC93,
78 functions, SHA256 `8c9d5afab9f9c11ea3a0584418dbef530f03938a401b91a206c4e92ed6602e31`.
Receipt `outputs/rea/chanset461-kernel-adapter-syntax-20261022T193600Z-r15/result.json`
SHA256 `0bad552d383364a05d2b3a32cc0409211e74907181dfc8e810151511f2c74964`.
This is one unit, not full module or header closure acceptance.

Actual complete scan-start function plus typed adapter passed five source cases:
interposed other-VAP request is rejected before beacon-stop, admitted owner starts
once, typed busy matching/other owner calls do not stop beacons, ordinary legacy
success stops once, and legacy busy preserves original return without another stop.
Receipt `outputs/rea/chanset461-r15-actual-start-beacon-20261022T225800Z-r2/result.json`
SHA256 `cd4468c369e738ba089e9fbd11a9084e1d3eb25b334751b129a64c7abf1110f8`.
Fixture SHA256 `64e1876e596341c84019c1b3db13c357c52bce75826bf59d67b03d80585a67e5`.

Actual complete `scan_next` + `ap_end` plus actual typed picker/pure helpers passed
seven foreground source cases: chosen channel, missing shared SCS, exhausted legal
set, cancellation, ABOVE rejected first/following allowed, ABOVE rejected last with
queue20 restart, and BELOW rejected first/following allowed. The old R13 source
failed the same following-allowed assertion; negative evidence remains immutable.
Receipt `outputs/rea/chanset461-r15-actual-next-end-20261022T225900Z-r2/result.json`
SHA256 `c332705c824e6842a8c1bd0f97e1e9d8ff233ed327367717b06534cf5b9e426b`.
Fixture SHA256 `5af6a316f2b50260d75f42019221c0140f2b060ad083155a06054b86d4f70530`.
The 40MHz queue3/4 states were explicitly installed as valid protocol states;
current BAND24 `prepare` defaults use queue20 because instantaneous CCI/ACI factors
are zero. These fixtures do not establish normal automatic 40MHz scan behavior.
Optional QTN_BG_SCAN/QSCS/QBMPS compile hooks and hardware hooks were fake/omitted.
An empty-list diagnostic did not reproduce a fault and has no reachability claim:
actual `scan_restart` refuses `ss_next==ss_last` before publishing a scan timer.

Earlier review findings and fixes:
- R8 callback return was inverted: pending returned1 and finished0. R9 fixes it.
- R10 missing SCS/exhausted candidate and rank failure could retain busy owner;
  source R13 fixed and same whole-picker cases passed with terminal errors.
- R10 final rejection mutated selected channel direction first; R13 restores
  prior flags/center/hostis_set and returns EINVAL. Same source fixture passed.
- R10 snapshot limited accepted observations only. R13 caps visited nodes; a
  rejected2048-node fixture now visits255 instead of2048.
- R12 joined timers before running powersave tasklet, which could rearm after
  joins. Actual complete tasklet/VAP detach schedule reproduced pending1.
  R13 tasklet-first joins pass the identical schedule with pending0.
- R10 actual legacy start could steal a typed reservation; R12 guard and local
  admission return passed the same actual complete-start interleaving.

Reachable reserve/start/teardown scope is narrow. Exact generated F363 config
SHA256 `ee4c4d98622b2c5caf94bebd4e4e3afa7888f1eb4630b4e86958dbbffdb7a61a`
has UP/PREEMPT_NONE, no SMP, no DEBUG_SPINLOCK/DEBUG_LOCK_ALLOC. Current typed start
call site is proto `__ieee80211_newstate`→`om_chanset_kernel_start` at1867.
`ieee80211_new_state` at1635 holds VAPS `spin_lock_bh` around the state callback;
scanner-get uses tryload0, scanner attach is NOWAIT, and reserve→start has no
sleeping operation. Process AP open follows netdev/RTNL; the STA-join tasklet is
joined by `ieee80211_vap_detach` before scanner teardown. QDRV VAP exit wraps VAP
detach in RTNL. Whole `ieee80211_ifdetach` deletes all VAPs under RTNL before
`ieee80211_scan_detach` releases scanner state. No additional reachable pending
starter/teardown race was demonstrated under this exact UP configuration. This
is not approval of external arbitrary callers, SMP/PREEMPT variants, or hardware.

New tools are reversible local source generators/fixtures only:
`prepare_chanset461_scan_advance_fix.py`, `prepare_chanset461_scan_beacon_gate.py`,
`test_chanset461_kernel_critical_review.py`, `test_chanset461_actual_scan_start.py`,
`test_chanset461_actual_detach_rearm.py`, `test_chanset461_actual_scan_end_timer.py`.

All candidate/receipt fields remain firmware_eligible=false. Full paired build,
delta/module dependency qualification, operational caller validation, shared tuple
coherence, physical radio/client/traffic/persistence/recovery/release gates remain
with the owning root task. No F315–F319 artifact or physical result changed here.
