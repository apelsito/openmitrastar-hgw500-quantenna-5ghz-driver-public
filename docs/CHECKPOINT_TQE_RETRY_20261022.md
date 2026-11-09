# Typed multicast queue retry — 22 October 2026

R4 source-r8 restores retry after confirmed congestion while keeping the
shared payload owner and queue credit in a typed marker. Uncertain publication
is retained and never retried or freed. A processing lease rejects nested queue
consumers; owner, head and credit commit together. Callback registration pins
both modules before storing the function pointer. Earlier failed revisions and
the reproduced producer race remain separate evidence.

The original ownership ledger256 and global queue capacity2048 remain unchanged.
135 finite markers use2700 BSS bytes. Host queue-element and wire layouts remain
unchanged. CGQ22696bytes SHA256
`7bfb3a2759eb6ccdc46b19037a04001f2d9d0b9051aeb2419f2edb0cb0116b3a`;
TQE37508bytes SHA256
`b9cfdb8292f069d564d9d22c6355f40ef7058c8bfdd78fe70669af0004893bb6`.
Both full modules build twice identically and retain the one-way provider link.

The [final delivery](../evidence/driver5g-ops-20261022-tqe-r4-final-reviewed-delivery-r1/result.json)
and [independent review](../evidence/tqe-r4-source-r8-independent-final-review-20261022T210600Z-r1/result.json)
bind current source-r8 and build-r5. The review checks1140 build and1158 global
inputs in Linux, all old named values and complete new membership, with four
additional negative cases. Global residual relocations are450 TQE and220 CGQ.
Ten queue/provider cases and three initial-to-async retry cases pass. These
source fixtures use fake hardware and are a bounded sample, not all queue states.

The earlier105-case outer IP/FWT entry fixture belongs to R3. R4 changes the
callee's credit integration, so it is not counted as a complete R4 outer-path
test. Unmarked queue HBM cleanup remains inherited and unbounded. Foreign
bank writers, real throughput, soak, recovery, RF, client traffic, persistent
boot and release acceptance remain open. Pins are retained until reboot.
No device or firmware operation occurred.
