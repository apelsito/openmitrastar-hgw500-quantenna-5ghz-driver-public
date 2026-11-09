# NAC22 offline checkpoint — 22 October 2026

NAC457 set/get is now reconstructed on the qualified20 closure. The new WLAN
module is630628bytes, SHA256
`dd4896a969b3033e41f8c63eac27b12b1844bd86dd1a506e69c7be5a485b4621`.
Both builds are identical. Headers and36 unchanged WLAN objects are bound to
the prior complete seven-module closure; the actual wireless unit was compiled
twice. This is an incremental build with explicit parent identity.

The setter preserves OEM HOSTAP forwarding via SETPARAM11/76bytes and the
successful no-op in other modes. The getter reproduces sampled scalar packing,
adding EAGAIN when the shared pointer is absent or the denominator is zero.
READ_ONCE accesses do not create a coherent cross-processor snapshot. No new
monitor/timer or firmware behavior is claimed from the host getter.

Qualification covers959 functions,2250 symbols,9151 residual relocations,
allocated data and exact table/diagnostic changes;16,711,680 prefix samples,
1,048,576 setter cases,7 negative checks and1652 input hashes pass. The
[qualification and limitations](../evidence/driver5g-ops-20261022-nac22-qualification-r2/HANDOFF.md)
and [independent review](../evidence/driver5g-ops-20261022-nac22-independent-review-r1/result.json)
are retained exactly.

The measured table has588 records:585 OEM matches,22 OEM gaps and3 additions
from the Linux port. The22 recovered records include the two faithful OEM
unsupported responses. Table counts are not counts of fully accepted features.
The [remaining ledger](../evidence/qualified-nac22-remaining-ops-20261022T163900Z-r1/LEDGER.md)
is generated from the actual qualified module and original OEM table.

Beacon R9 has two identical full source-built seven-module compositions, but
its global lifetime audit is still pending. The review found two inherited
unpublished ioctl-slot leaks; a separate R10 source change adds the missing
frees and has actual cleanup fixtures. APP IE synchronization remains another
separate candidate. These source changes are not silently merged into NAC22.

No router access, radio activation, UART, reboot, firmware installation, flash
or boot-environment change occurred. RF, traffic, persistence, rollback and
release acceptance remain separate physical gates.
