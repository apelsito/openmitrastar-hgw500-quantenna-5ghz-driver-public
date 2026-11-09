# Bounded TQE provider candidate R2

This candidate resolves the local wait/publication race for the pinned UP,
PREEMPT_NONE, non-PCIe build. It is an offline candidate, not a release.

READY and completion polling use1024 reads each. Before TXSTART, refusal retains
caller ownership; after uncertain publication, the provider records ownership,
quarantines the bank and prevents retry, dequeue or HBM free on the covered
paths. The provider owns tokens/ledger and is pinned with its consumer until
reboot. No CGQ-to-TQE import remains; TQE imports seven provider exports.

R2 places the multicast quarantine guard before VLAN/HBM effects. The earlier
R1 guard was late and is not accepted as a functional correction. Original
sources and every failed/previous result remain preserved.

Two complete builds match per module:

* CGQ17000bytes SHA256a0face8555ed07df340c0808a60e7caccf02b775903ac1aa63cce77eac375568.
* TQE36236bytes SHA2567cb21e6a87284b3b29bfa285d6e51ffa7fac7f1bd4419574c48965e6af84b58f.

Both original baselines were rebuilt byte-identically.171bound fixture cases,
global code/data/RELA/symbol scope,12negative checks and independent R2 review
pass. Changed/new function bodies are supported by source fixtures and exact
artifact hashes; they are not declared instruction-equivalent to the old code.

Open gates: all retained references must be catalogued when the256record ledger
saturates; HBM calls outside this core can still wait indefinitely; foreign
processors may access LHOSTbank; real retirement/recovery and physical traffic
remain unproved. Retention until reboot is explicit. No release or hot-unload
support is claimed, and no module from this work was loaded on the router.

See`evidence/driver5g-ops-20261022-tqe-provider-r2-delivery-r1/` and
`evidence/driver5g-ops-20261022-tqe-provider-independent-final-review-r2/`.
