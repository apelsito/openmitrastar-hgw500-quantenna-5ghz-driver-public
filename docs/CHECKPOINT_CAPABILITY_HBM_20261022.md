# Capability records and HBM baseline — 22 October 2026

The [R12 capability audit](../evidence/consolidated23-beacon-r12-capability-audit-20261022T212200Z-r1/result.json)
checks the four OEM492/493 records and their actual beacon/response consumers.
1148576 setter/getter transactions,309 frame blocks and131329 TLV cases pass.
It permits counting27 reconstructed private records and17 remaining table gaps
for R12, with explicit differences: disabled beacon IE70 remains present, short
TLVs are rejected, and association uses the length actually copied. Full action
handlers, RF and physically functional operation counts remain unknown.

CoC25 is separately qualified for25 recovered records and19 gaps; these counts
must not be added until the combined source/module closure is verified. The new
combined29 source is still undergoing its own build and audit.

The [HBM baseline](../evidence/driver5g-ops-20261022-hbm-full-baseline-r1/result.json)
reproduces the original full module twice:21284bytes SHA256
`f3b8b78ffda138c5c8287c923086c51cd896e51539ff7a17caa394c0bc6cebb8`.
28 functions,259 relocations,138 symbols and1006 input hashes pass.

The [combined source fixture](../evidence/driver5g-ops-20261022-hbm-combined-finalref-void-bypass-r3/result.json)
passes147692 cases with seven OEM bodies. It also reproduces a concrete gap:
after the typed release becomes uncertain, an original weak void put function
can publish that release again when DONE arrives late. Eight header-consuming
units and five modules have their own bodies. A change limited to hbm.c cannot
close this gap. The provider/ownership/header-user integration remains pending.

Original ec1/fc0 recycling stays intact; no global fence or recovery acceptance
is claimed. No hardware, radio, reboot, flash or firmware operation occurred.
