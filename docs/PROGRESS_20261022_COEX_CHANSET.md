# Offline driver progress — 22 October 2026

Current selected checkpoint: **Weather478 source34**,600 private records,
34 original records reconstructed and10 remaining. The [qualification](../evidence/weather478-qualification-20261023T000100Z-r1/result.json)
and [root review](../evidence/weather478-root-independent-review-20261022T212000Z-r1/result.json)
bind paired modules,5544 current pins, preserved ABI and the actual AP policy.
WeatherCAC remains600seconds; radar/CAC source paths are unchanged.

The [32 plus bounded467 checkpoint](../evidence/consolidated32-plus-bounded467-qualification-20261022T232300Z-r1/result.json)
and [independent review](../evidence/consolidated32-plus-bounded467-root-review-20261022T211000Z-r1/result.json)
qualify a separate composition:32 records plus1 bounded extra,6750 pins,
599 private records. The query retains stable5GHz20MHz/fallback limitations,
and does not close the complete467 record. It is not yet combined withweather34.

The paragraphs below retain historical checkpoints.

This page retains the earlier r13 checkpoint below. The selected candidate
has since advanced to **Consolidated30 r2**:596 private records,30 reconstructed,
14 remaining. Its [final checkpoint](../evidence/consolidated30-global-checkpoint-20261022T222200Z-r2/result.json)
binds3933 current inputs, eight fresh changed-unit compilations,108 exact
parent objects and seven identical complete modules across both lanes.
All existing ABI fields/sizes, complete residual/metadata/symbol contracts,
eleven negative controls and actual449 timer/parser/geometry fixtures pass.
The prior combined30 timer omission is corrected in a new immutable revision.

Scanner r15 corrects the double channel advance and gates beacon-stop on actual
start admission. [Five actual-start cases](../evidence/chanset461-r15-actual-start-beacon-20261022T225800Z-r2/result.json)
and [seven complete scan-next/AP-end cases](../evidence/chanset461-r15-actual-next-end-20261022T225900Z-r2/result.json)
pass, with the failing old case retained separately. Its ARC scan unit compiles;
this remains source evidence pending integration into combined32, its full
module/ABI qualification and physical gates. The [r15 handoff](../evidence/chanset461-r15-review-handoff-20261022T230000Z-r1/REVIEW.md)
states the exact UP/PREEMPT_NONE caller scope and explicit40MHz fixture states.

Historical r13 checkpoint:

The selected combined candidate remains Consolidated29:595 private records,
29 reconstructed OEM records,15 missing, with two faithful unsupported returns.
The complete module pair, residual/metadata/ABI audits and source fixtures are
sealed in [its checkpoint](../evidence/consolidated29-global-checkpoint-20261022T215400Z-r1/result.json).
This is not a count of fully working radio features.

Coexistence449 is independently qualified on CoC25 in
[Source26 r3](../candidates/coex449-source26-bss-geometry-20261022T192600Z-r3/result.json).
Its [qualification](../evidence/driver5g-ops-20261022-coex26-qualification-r3/result.json)
and [independent final review](../evidence/coex449-r3-independent-final-modules-20261022T220300Z-r1/result.json)
bind the actual BSS geometry, setter and timer/RX consumers, ABI and complete
paired modules. It contributes one record. The first combined30 prototype
omitted the timer policy gate: an extraction selected a call before the actual
function definition. A consumer fixture caught it. That prototype remains
unqualified; the corrected combined revision needs its own module/checkpoint gates.

Channel-set461 has a [versioned r13 source prototype](../candidates/chanset461-kernel-adapter-20261022T192700Z-r13/result.json).
Its ARC scan unit compiles to51232bytes. Exact helper constants retain130
OEM records/10400 non-relocated bytes; metrics and ranking fixtures are bounded
source evidence. Private-table set/get wiring is present but is not promoted.
The OEM entry predicate uses HOSTAP and2.4GHz modes2/3/8/9; other5GHz scanning
retains the existing algorithm.

The actual adapter fixtures reproduce and verify corrections for missing SCS,
exhausted legal channels, ranking failure, rejected channel-direction mutation,
bounded rejected-neighbor traversal, reserved-owner start admission and a
power-save tasklet rearming a timer after the former teardown join order.
See [pick cases](../evidence/chanset461-r13-independent-kernel-fixed-20261022T224600Z-r1/result.json)
and [the teardown schedule](../evidence/chanset461-r13-actual-vdetach-fixed-20261022T224500Z-r1/result.json).
These use actual C bodies with fake kernel primitives; complete scan/AP flow,
caller context and full-module qualification remain pending. The actual-start
fixture uses r12; its `false_success:1` field is the original owner's positive
return value, not a remaining false-success finding.

Sources, comments, exact bytes and rejected historical results are retained.
No firmware image, module or analysis ELF is included in Git. No hardware
operation was performed by this work. RF, association, useful traffic,
persistent boot, recovery and release remain separate gates.
