# CoC25 offline recovery — 22 October 2026

The CoC491 setter/getter and actual PM/CSA consumer are reconstructed on the
qualifiedLegacy23 source. The timer callback defers mutex-based QoS and logging
to the existing worker. Producer requests retain their trigger across completion
work, and teardown gates producers before cancel/join. This is qualified for
initialized retained IC objects; inherited initialization failures, force unload
and the entire external CSA subsystem remain outside this bounded proof.

QDRV684776bytes SHA256
`7c53487f459328d12eb7952b9672a7410d621302da1215a539d600c5de75f1c5`;
WLAN633096bytes SHA256
`06680ba7a6a67ef2003e61f84378aa16d6e4697b78c98fcd300b6c64c2731f29`.
All seven modules match between two116-unit source build waves. The matching
control reuses54 whole objects after source/header identity and recompiles four
changed units; it is not described as another fresh58-unit build. Joint modpost
resolves all strong imports and has empty diagnostics.

The [qualification](../evidence/driver5g-ops-20261022-coc25-qualification-r6/result.json)
and [independent final review](../evidence/driver5g-ops-20261022-coc25-independent-final-review-r6/result.json)
check2378 current hashes, complete residual code/data/symbols/relocations,
376 IC members,210 VAP and72 QDRV members. Host layout grows20 bytes relative
to the original IC; matching rebuilt modules are mandatory and wire widths
remain unchanged.165888 worker cases,262144 ioctl cases, nine event interleavings,
cancel/join contract modeling and11 parsed negative auditor cases pass.

The [remaining ledger](../evidence/qualified-coc25-remaining-ops-20261022T183000Z-r1/LEDGER.md)
measures591 records:588 OEM matches,19 original gaps and three port additions.
25 recovered records include two faithful unsupported responses. This does
not establish25 physically accepted features. See the
[full handoff](../evidence/coc25-handoff-20261022T182800Z-r6/HANDOFF.md).

CoC25 and the consolidatedLegacy23/beacon R12 remain separate versioned
candidates. CoC is not silently merged with the four partial capability records
in R12. Native PM/CSA completion, RF, clients, traffic, soak, persistent boot,
rollback and release remain unrun. No router access or firmware write occurred.
