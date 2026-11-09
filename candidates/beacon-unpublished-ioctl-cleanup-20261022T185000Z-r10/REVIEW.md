# R10 unpublished ioctl cleanup correction

2026-10-22. Source-only correction on R9; no headers, descriptor arena,
wire fields, firmware, radio or installed modules changed.

The real worker is `qdrv_wlan_80211_beacon_update_run`. It creates param/skb,
then `vnet_alloc_ioctl(qv)` calls `qdrv_alloc_ioctl(qw)`: a free hostlink-ring
slot with AVAIL1 is reserved by setting status0. The following coherent
argument allocation succeeds before either affected branch is reached.

The optional preflight-error and v89-encoding-error branches free args,param,
skb and return before `vnet_send_ioctl_owned`. They originally omitted
`vnet_free_ioctl(ioctl)`, so the unpublished ring slot stays reserved. The
inherited leak is reproduced by actual cleanup spans at
`outputs/rea/beacon-r9-independent-unpublished-cleanup-counterexample-20261022T184300Z-r1`.

R10 inserts exactly one `vnet_free_ioctl(ioctl)` into each of those two
branches. Full source replacement proof strips those two inserted calls and
matches R9 exactly. Comments and line endings are retained in the copied
source; all original evidence remains unchanged. Candidate qdrv sourceSHA:
`35c3b912c1d4e0a0fda0db2300627deef11cb7b042ed5fba56e577feb23d6445`.

Actual32bit source fixture:
`outputs/rea/beacon-r10-cleanup-fixture-20261022T185300Z-r2/result.json`.
It extracts the real slot allocator and all four cleanup spans reached with
an acquired ioctl: mandatory frameerror, optional preflight, v89encode and
shutdown before transaction transfer. Every case preserves its error,
returns the ioctl slot toAVAIL, frees args/param/skb once and publishes zero
messages. Resource callbacks are synthetic; the code spans and slotstate
are actual source. This is not a complete kernel or MuC timing fixture.

The other early failure paths occur before ioctl acquisition or already
return an acquired slot on coherent-argument allocation failure. Published
and quarantined resources transfer to txn and remain outside this R10fix.
That lifecycle is not treated as a new release or hardware-retirement proof.

Minimal binary step: compile qdrv_wlan twice from R10 using the pinned R9
joint context, relink each complete module with unchanged R9objects, and
qualify the whole delta. Parent R9global source/ABI audit remains distinct.
R10 must remain NO_GO_MODULE until those steps pass.

## Separate inherited APP publication synchronization concern

`add_app_ie` allocates/copies a new APP buffer, frees the prior buffer and
updates the VAP pointer/length without `IEEE80211_LOCK(ic)`.
`remove_app_ie` similarly frees/nulls the APP source. The queued process
context beaconworker copies that source while holding `ic_comlock`.
Without an external common lock around both paths, a replacing ioctl can
free the APPsource while the worker copies it. This is an observed source
ordering concern; no live use-after-free is claimed, and R10does not change it.

Existing primitive is `IEEE80211_LOCK/UNLOCK`: it guards `ic_comlock` and is
already used by the beacon update. A separately versioned correction can
allocate/copy outside the spinlock, acquire the same lock only for swapping
pointer/length (or clearing), release it, then free the old pointer. A reader
already holding the lock completes before the swap, and later readers see
the new stable pair. It must not call sleeping allocation or recursive
beaconupdate while holding that lock. Verify ioctl callcontexts/other APP
readers, lockordering and deterministic worker/replace/remove interleavings
before proposing this as complete synchronization proof.

No WSL reset, build, device access or runtime mutation occurred in preparing
R10source and its hostfixture.
