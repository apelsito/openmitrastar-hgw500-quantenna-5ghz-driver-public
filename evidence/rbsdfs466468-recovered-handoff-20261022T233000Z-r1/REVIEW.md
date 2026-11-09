OEM466/468 RBS DFS source recovery, offline only. No router, UART, radio, firmware,
Git or publication action. No selected driver/module source was changed.

Selected standalone source:
`outputs/rea/rbsdfs466468-recovered-core-20261022T232000Z-r2`.
C SHA256 `8cc8c6673fe388f88c02971a3f362e8ae9ade5431f311fb2d0803f47e0f738c9`;
header SHA256 `d0671a3f71149a58a9628b8919fd98334b1783f4a8d566a4949ceb090359c57d`.
Receipt SHA256 `69eddd3b97464d79184c2dc9e256e5504d6ebdc814981f89dfaf9c8d4759a859`.

Operations recovered from exact OEM WLAN switch table/body assembly:
-466 `rbs_dfs_msr_cac`: store `value != 0` as u8 at OEM IC+0x5d88, return0.
  Getter `g_rbsdfs_msrcac` loads that u8. Setter has no STA/EU validation.
-468 `rbs_dfs_end_tx`: unsigned `(value-1) >999` refuses-EINVAL; accepted range
  is1..1000 inclusive, storing u32 at OEM IC+0x5da8. Getter loads u32.
  QDRV initialization at0x46d78/0x46d9c/0x46dc8 writes500; default proven directly.
  The standalone state initializes466 false; whole OEM zero-init provenance is
  not claimed solely from this recovered switch fragment.

Do not alias to `sta_dfs_strict_msr_cac` or `sta_dfs_tx_chan_close_time`. Current
GPL/ported STA setter requires STA+EU for its measurement option; its state and
producer routes differ. Shared source fields have no demonstrated RBS equivalence.

Fresh REA used the already configured isolated Windows ARC700 lab. Installed MCP
and ordinary CLI reject ARC ELF at format admission; no setup/configuration/restart
was attempted. The lab accepts the same ARC bytes with the task-owned runtime.
Symbol-only derivatives retained every allocated section byte/layout unchanged.

Fresh dossiers:
- measurement send gate,0x4ac48..0x4ae67 inclusive,544bytes:
  `outputs/rea/rbsdfs466-measurement-rea-20261022T231000Z-r1/rea.json`
  SHA256 `1b2886ec288501eb91da3fdaec8fdb73b1a6ef97d9096cf43a6d828be261518e`.
- RBS radar producer,0x688c4 named `qdrv_radar_detected`:
  `outputs/rea/rbsdfs466468-radar-rea-20261022T231100Z-r1/rea.json`
  SHA256 `e18a57ca4c125b090351ac2cbab8eb09914d21c4725f530f23634bed892dac40`.
- expiry,0x68598..0x6862f inclusive,152bytes:
  `outputs/rea/rbsdfs468-expiry-rea-20261022T231600Z-r2/rea.json`
  SHA256 `c0a8ba7449c0ac10096683c1f4e217bbcc86ed96635e14c81495063d5816c93c`.
- MBS channel-switch producer,0x686ec:
  `outputs/rea/rbsdfs466-mbs-rea-20261022T232500Z-r1/rea.json`
  SHA256 `641a1d447026494006df7f88e832c8c433b16a140877b6d6dfffd6c1f002d88f`.
Only their bounded semantic blocks were reconstructed; full functions, unrelated
STAstrict paths, packet metadata and every indirect call are not recovered here.

Observed contracts:
1. RBS role2, radar during CAC,466 true: set one-shot IC+0x5d89, call
   `sys_enable_xmit`, copy current channel to timer data, then arm IC+0x5d8c at
   jiffies+floor(end_tx_ms*200/1000). HZ200 is observed in this OEM module.
2. Channel-switch producer with usable selected channel, not WDS-RBS, running
   HostAP, radar during CAC and466 true: set IC+0x5d8a then enable TX before CSA.
   Standalone helper represents this per-VAP branch; full list/CSA delivery is not
   represented by the helper's return alone.
3. Measurement/data send gate: role2 uses5d89, role1 uses5d8a. If466 is enabled and
   allowance exists, consume exactly one allowance and pass. Otherwise query
   channel status:4 (radar detected) or8 (CAC required) refuses. OEM makes a second
   status call only after first is not4. It does not validate frame identity here.
4. Expiry finds stored channel, compares it with current, and disables TX only if
   equal. It does not clear radar flags, complete CAC, mark availability or consume
   the allowance itself. Standalone helper has an explicit null-channel input
   guard; this is adapter validation, not proof OEM handled null safely.

The OEM producer's temporary TX-enable request remains in the isolated recovered
body as a mandatory callback. It was not replaced by a policy refusal or tied to
the active port's raw hostlink. The separate exact-frame boundary requires all
validate/grant/send/revoke callbacks. Missing provider refuses before mutation;
uncertain publication retains frame/state and forbids another grant/retry.
Provider must restrict grant to the validated single measurement/CSA frame and
must never permit general traffic, clear radar or complete CAC.

Boundary limitations are material: this source does not bind a real Linux grant
provider, pin modules/frame allocation, enforce a scheduler deadline, stop a
running callback or prove hostlink/MMIO completion. `deadline` is recorded only.
It is not a runnable production Linux adapter. OEM send gate permits any next
packet once its allowance is set; an exact-frame Linux adapter is a deliberately
narrower safety boundary and needs its own provenance/qualification.

Source fixture:2,705cases PASS including all1000 accepted delay values, unsigned
edge values, role/CAC admission, one-shot/status4/8 short-circuit, expiry comparison,
missing callback refusal, grant/send/revoke unknown retention and no retry.
`outputs/rea/rbsdfs466468-core-fixture-20261022T232200Z-r1/result.json`
SHA256 `0b36714b0f78cce6897e802609eb1646f7cbd271d18e3528ccaddd5cbc8a7d2a`.
Fixture SHA256 `b88513fab74513b3b450ceaa7bea86ecebd5ca67926393a64e9af5b5d2d4eb4d`.
Callbacks are fakes; this establishes source behavior, not kernel/runtime grants.

ARC standalone object PASS:2,220bytes SHA256
`7b3bc42a7e6b2aabeba7a7031b1993cb2ebb446d2959229a18d83b39ce7cf416`;
1,040 input pins unchanged, ten core functions plus three retained header helpers.
`outputs/rea/rbsdfs466468-core-arc-20261022T232400Z-r2/result.json`
SHA256 `79134afba85d62525f6e29e90ece3cb175567859bca0bc0067d9475a152c9d3f`.
The earlier ARC r1 used an incompatible chanset-specific compiler entrypoint and
failed to find its expected filename; no candidate was changed to satisfy it.

No candidate was integrated atop consolidated30r2. Available runtime TX control
is global hostlink enable/disable, which cannot yet prove a single-frame grant.
Implementing the recovered exception there now would relax current DFS failclosed
behavior. A real exact-frame provider, complete caller/frame ownership and closure
must precede integration. This is a concrete missing provider contract, not an
additional hardware permission requirement.

Status: source recovery+host fixture+ARC object PASS; integrated module, operational
producer, physical DFS/report/client/traffic/persistence/release UNRUN. Four table
entries466set/get468set/get remain unqualified and contribute0 completed ops.
All artifacts retain firmware_eligible=false.
