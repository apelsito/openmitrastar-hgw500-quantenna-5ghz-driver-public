# Beacon and channel-set reconstruction — 22 October 2026

Beacon R9 lifetime changes now pass their complete offline audit against a
freshly compiled parent with matching headers. Candidate builds compiled116 C
units across two waves; the independent control compiled58. All seven candidate
modules match between builds. Full residual executable bytes include anonymous
code; data, symbol and relocation contracts are checked. The qualifier verifies
2342 hashes before and after and rejects11 deliberate invalid cases.

The original1000-byte descriptor arena and20-byte descriptors remain unchanged;
host ioctl32, SETPARAM76 and beacon arguments64 bytes remain unchanged. Host
structures grow: ieee80211com22644 to22652, beacon offsets64 to68, parameters
1016 to1028, qdrv_wlan30556 to30564 and qdrv_vap3540 to3544. Matching rebuilt
modules are required. This audit qualifies the lifetime change and does not
accept all inherited19 capability operations or management action handlers.

See the [R9 delivery](../evidence/beacon-r9-qualified-delivery-20261022T192000Z-r1/result.json)
and [independent review](../evidence/beacon-r9-independent-final-review-20261022T193500Z-r1/result.json).

R10 separately adds two missing vnet_free_ioctl calls on unpublished preflight
and encode failures. QDRV builds twice identically:684896bytes, SHA256
`637ce4985a391d43ad350dae130554135276f49ae0361887148d7a10c9985002`.
Six other modules are exact R9 bytes. Complete residual checks, four cleanup
paths, seven negative tests and1671 build input hashes pass. The
[R10 delivery](../evidence/beacon-r10-sealed-delivery-20261022T193300Z-r1/result.json)
keeps the scope and physical limitations explicit.

R11c APP IE synchronization is a separate source candidate: pointer/length
replacement and readers share the existing BH lock; frame reservations use the
existing1024-byte maximum and frees happen after releasing the lock. Four ARC
units and host fixtures pass, including two actual thread interleavings. Complete
module and caller/lock-context qualification remain pending.

Channel-set461 now has130 exact OEM constant records and six typed C helpers:
template selection, first-match lookup, bounded scan-type queue, rank defaults,
frequency edges and low32 cost arithmetic. All10400 table bytes match the OEM
module and contain no relocations or live metrics. The cost loop missing from
Ghidra's procedure body was recovered using exact original bytes and pinned ARC
objdump, including its register setup. The helper object compiles for ARC with
1042 input pins. Lookup and arithmetic fixtures pass, while malformed/saturated
queues refuse before mutation.

See [helper evidence and limits](../evidence/chanset461-helper-delivery-20261022T174100Z-r1/HANDOFF.md).
These helpers are not yet registered as461. Scan collection, final candidate
choice, callback/lifetime integration and full module closure remain open. The
qualified NAC22 module still has22 OEM table gaps. Helper comments are retained
in a [document](../evidence/chanset461-helper-comments-20261022T174000Z-r1/COMMENTS.md)
and local searchable page, with exact raw bytes and hashes alongside them.

No device, radio, UART, reboot, boot-environment or firmware write occurred.
Physical RF, client traffic, soak, persistent boot, rollback and release gates
remain separate from these offline results.
