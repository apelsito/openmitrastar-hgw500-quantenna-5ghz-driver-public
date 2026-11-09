# Stateful legacy rate recovery — 22 October 2026

OEM490 fix_legacy_rate now has a stateful C implementation on the qualified
NAC22 candidate. It preserves the saved mode across repeated settings and the
wire62 encoding for rates0..12 or255 on restore, with the recovered mode,
profile, CSA, reassociation and channel-callback order. Invalid requests leave
state unchanged. Void callbacks do not prove hardware completion or rollback.

The seven modules build twice from116 fresh C compilations. A same-header
parent control compiles58 fresh units:57 objects and six modules match the
candidate exactly. Control WLAN matches NAC22 exactly. The global WLAN audit
checks955 unchanged functions,9200 residual relocations,2250 old symbols and
28 sections; only the reconstructed helper/dispatch, exact diagnostics, one
private record and its count change.

QDRV684740bytes SHA256
`ca6fcb24a2c6e9c051d27a7220a70e034fd3479378f7c7d68c951982b6e802f6`.
WLAN631120bytes SHA256
`486818be02666694706d1660f49ad3b41a05b197d4c4b5bc42dd6476cd3b2d11`.

Actual ARC witnesses check374 IC fields,210 VAP fields and72 QDRV members.
Old IC/VAP offsets and widths remain unchanged. The new two radio-owned ints
grow IC by8bytes, shifting fields after the embedded IC in QDRV by8. Wire
32/76/64/20 remains unchanged. The dependency supplement explicitly records
postcompile hashes rather than claiming they were all pinned before compilation.

The [qualification](../evidence/driver5g-ops-20261022-legacy23-qualification-r2/result.json)
checks1864 hashes,3548272 transaction cases and seven negative auditor cases.
The [independent final review](../evidence/driver5g-ops-20261022-legacy23-independent-final-review-r1/result.json)
verifies all14 module hashes/sizes and repeats the global audit.
See the [full handoff](../evidence/legacy23-handoff-20261022T172500Z-r1/HANDOFF.md).

The table has589 records:586 OEM matches,21 OEM gaps and3 Linux-port additions.
The23 recovered records include two faithful unsupported responses. This count
does not establish23 complete physical features. NAC scalar sampling, RF,
traffic, persistence, rollback and release gates remain separate.

The channel-set461 prototype now adds scaled13-word scan normalization and
pure final candidate selection to the earlier six helpers. Boundary, overflow,
signed-cost and tie cases pass; nine public helpers and their private scaler
compile for ARC. It still has no461 registration, shared-scan lifecycle or real
channel callbacks. APP IE and scan-lock source changes remain separately versioned.

No router access, UART, reboot, radio activation or firmware write occurred.
