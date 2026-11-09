# RBS exact-frame provider: bounded full MuC payload audit

Offline evidence only. No router, SSH, UART, COM, RF, calibration, protected partitions, firmware installation or Git mutation occurred in this lane. R24 source remains untouched. Eight fresh REA function queries completed with exit0 and exact target unchanged. All process handles were collected; no background job remains.

Original MuC SHA256: `71dcd32849f122c09397490914227af6f77e51c34d15615028faa3d0fabb0fe4`.
Analysis-only full payload ELF SHA256: `f5878c74735e08516b4345e36a30e9a5c9f610c7439b471bf8cb2b9264722906`.
Result SHA256: `eac58a1d5cebd5abe92f67c8d3808fbe224327c5d4e666af15b78573a336bc4a`.
Input pins SHA256: `e878219d9f719ddc2d4e6d73b8ad4b2c87e6462be010d0f7d29b2c1039b7123d`.

## What the bytes establish

Host `qdrv_tx_muc_post_hostdesc` publishes management physical descriptors to mailbox1, chaining `hd_nextpa` under the corresponding semaphore. Firmware `0xf0bdc` allocates130 mailbox words (520 bytes), initializes ioctl mailbox0 type6 and management mailbox1/node mailboxes2..129 type1. Record initialization is `0xf0ba4`;32-byte records reside at netdev+0x44, mailbox pointer at+0x2c.

The registered callback `0xf11c8` consumes **record0/ioctl**, not mailbox1. It walks ioctl+28 and dispatches netdev callback+124. The callback+104 set during radio setup points to `0x10862c`, whose exact1024-byte body initializes/reset/channel/radio resources. It is not a TX packet consumer. Response/control messages use `0xefe3c` into shared6003a3c8/cc. HTXD completion `0xe7b84` invokes callback+48 and examines flag word+448 (ARC `ld.as112`) for NO_RETURN/IMM_RETURN.

Actual tasklet scheduler is `0x80023900` (216 bytes), which calls registered pointers in28-byte tasklet records. A TX admission contract cannot be inferred from the scheduler loop.

Original name metadata is useful only as a hint. Several independently checked bodies disagree with its label: `uc_scheduler`→800239d8 is RX soft-ring/TQE; `qtn_tx_restrict_process`→12dbb4 is a20-byte global*10 helper; get/release semaphore labels are one body displaced; `qtn_auc_tqe_mgmt_xmit`→1563ac is actually the420-byte **synchronous AuC IPC command/wait** routine. Its own diagnostic string says `qtn_mproc_sync_auc_ipc_wait_done`; it writes through18c260, rings e0000178 and waits with loopout/halt handling. Calling this grant/revoke would invent its command-specific contract.

## Physical mapping correction

Segment3 declares VA80020000 but loader destination/paddr46e004. The analysis map places these1732 exact bytes at46e004 and preserves segment2 at80020000. Superposing them at80020000 corrupts the analysis. Segment5 has a declared6003a3c0 virtual alias of exact8003a3c0 bytes; this is not independent proof of cache alias behaviour. Large BSS/runtime initialized pointers remain outside this static payload representation.

## Result and continuation

**Provider unresolved;0 qualified ops added;0 adapter added.** This bounded search does not prove universal absence. Known ioctl26 remains global TX control and must not be used as CAC permission. R20 providerNULL boundary remains preserved; no R24 edits occurred.

Next acquisition should bind actual mailbox1 descriptor ingress/admission to frame identity, owner/node lifetime, role, channel and deadline; then recover command-specific AuC acceptance/revoke/completion if that path owns TX. Keep AuC default `e1fd397e...` distinct from nomu `c10018f4...`; neither has instruction qualification in this lane. Preserve all earlier negative receipts and the eight new full-map REA outputs. `result.json` enumerates exact directories, ranges, call edges and timestamps; `input-pins.json` binds the source, analysis target and receipts.

Directory labels such as `real-netdev-tx-callback` or `mgmt-xmit` record the queried hypothesis, not a claim about its final semantics. The new ledger supplies the qualified interpretation without rewriting old evidence.
