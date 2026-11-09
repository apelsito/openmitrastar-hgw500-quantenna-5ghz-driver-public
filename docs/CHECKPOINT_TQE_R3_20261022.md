# TQE finite multicast ownership — 22 October 2026

Selected source-r4 reserves one catalog record per shared multicast payload
before modifying the reference count. Its ledger remains256 records. Typed
callback tokens distinguish the batch owner from normal writers. Three
reproduced failures were corrected: generation overflow, callbacks after the
expected count and finishing an incomplete batch. Each uncertain outcome keeps
the owner and prevents further publication.

CGQ19316bytes SHA256
`aefc674cabb331fef82e4cca9789ed98525b7bc3b7af19ea25621c685a459f5f`.
TQE36704bytes SHA256
`e842f63c6b62333026f152b4f5d1354803dcefd0235342df77edffc7886afef9`.
Both full modules build twice identically. CGQ is the provider and does not
import TQE. The original baselines were checked again before construction.

Source fixtures pass35212 capacity/token cases and142 typed callback cases.
The new259-case fixture executes the actual multicast caller and four OEM
enumeration bodies, covering255 port maps with no nodes, the maximum134
outputs for inputEMAC0, refusal before reference mutation and retention after
timeout. It does not establish every node arrangement or execute the outer
IP/FWT decoder. The historical146-case fixture includes a removed legacy
callback and is not represented as proof of every active writer.

The [independent module review](../evidence/tqe-r3-source-r4-independent-final-review-20261022T201400Z-r1/result.json)
checks current module bytes,1140 build inputs,527 global inputs, old symbol
values, complete new membership and four negative cases. The later
[caller supplement](../evidence/tqe-r3-source-r4-independent-fullcaller-review-20261022T201700Z-r1/result.json)
repeats the259 real-caller cases and closes that bounded fixture gap.

The diagnostic policy deliberately replaces multicast congestion retry with
quarantine and retained ownership. Its packet performance is unaccepted; this
is not a release or universal arbitration result. HBM waits outside the core,
foreign processor writes, retained-buffer recovery and physical acceptance
remain open. Module references are retained until reboot. No device access,
radio operation, reboot, flash or firmware installation occurred.
