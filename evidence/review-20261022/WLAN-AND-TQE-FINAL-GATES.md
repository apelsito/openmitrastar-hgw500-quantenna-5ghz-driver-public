# Independent follow-up: WLAN and strengthened TQE gates

2026-10-22, offline local evidence. Existing review and failure artifacts are
not replaced. No hardware, runtime or fixture replay occurred in this review.

TQE r5 at `outputs/rea/driver5g-ops-20261022-tqe-baseline-r5/result.json` remains
whole-file exact: both35552-byte builds match the retained SHA256
`bd5ab1018d293dc6bed655ec2bbc6da64a3969f28a3de578c04fa9c3f4af635f`.
The revised builder now includes generated/config headers, requires every
actual source dependency from `.d` in the prebuild pin set, compares all module
metadata and requires whole_file_exact before PASS. The autoconf omission and
prospective metadata/hash review issues identified for r4 are addressed.
This remains reconstruction of unchanged TQE, not an arbitration correction.

WLAN baseline r2 at
`outputs/rea/driver5g-ops-20261022-wlan-baseline-r2/result.json` records19units,
959functions,9543RELA,2250symbols and319940executable-section bytes. Its two
629180-byte complete builds match each other at SHA256
`7b0e504e6aee9b763757ea59268b21ebf2f18cdaf6a3c30f52ca43970a6f8dc0`.
The retained original F365 hash is
`f4f6099b43dd675dd44e7021374f1ea2f4380a4d9d9fd81b1d35ea411984897f`.
**These are not byte-identical to the retained original.** Complete semantic
section/relocation/symbol comparison passes with the explicitly declared
srcversion delta `C90B1F07BF94C284D7647F7` to `9A233B0BD384175810C9191`.
The result correctly uses
`PASS_OFFLINE_FULL_BASELINE_EQUIVALENT_DECLARED_SRCVERSION` and states that
exact original source identity is not reproduced. No original metadata was
copied to mask the difference.

WLAN alias candidate r2 at
`outputs/rea/driver5g-ops-20261022-wlan-alias-module-r2` builds twice to629228bytes,
SHA256 `0d86d01d2d89bedd54ce618b8a075cc922a6d55b547bc6ab30927bfc865e89b1`.
Its complete qualifier r2 explicitly binds baseline and candidate build proof
and hashes, checks protected inputs, and requires metadata exactly equal to
the reconstructed baseline, including the declared srcversion.

The only allowed module changes are two exact24-byte WEXT entries for command
246: setter `(18433,0,"tx_maxmpdu")` and getter
`(0,18433,"get_tx_maxmpdu")`; private table count566to568; subsequent .rodata
symbol/relocation offsets shifted48bytes; and two exact `mov r2,line` diagnostic
instructions whose existing assertion line numbers increase by5. The complete
module comparison after those precise normalizations passes. All other
executable/data sections, symbols and relocations are compared globally.

The qualifier's separate fault result at
`outputs/rea/driver5g-ops-20261022-alias-qualifier-fault-tests-r1/result.json`
has5cases: correct complete module PASS; wrong ioctl ID, unrelated text byte,
unrelated relocation addend and unrelated rodata byte are all rejected. This
supports that the normalization is limited rather than swallowing unrelated
differences. Original modules were not modified.

**Independent decision:** the TQE exact baseline gate, WLAN semantic baseline
gate with declared srcversion and complete two-alias module delta gate are
supported by the inspected evidence. This recovers two names only. It neither
recovers the42remaining OEM private entries nor repairs the TQE foreign-writer
collision, queue policy, firmware completion or radio retirement. No candidate
is installed; client/traffic/persistence/recovery/release gates remain UNRUN.
