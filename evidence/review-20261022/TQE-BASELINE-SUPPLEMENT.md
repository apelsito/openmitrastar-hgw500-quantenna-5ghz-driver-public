# TQE baseline independent review supplement

2026-10-22. This adds new evidence without replacing the earlier REVIEW.md.
Offline read-only artifact review; no router, SSH, UART, REA session or fixture
re-execution in this follow-up.

The new gate is `tools/rebuild_switch_tqe_baseline_gate.py`, SHA256
`4449c8da2a6f914df53f3f09ba8872bb2707218ff7235160ee010e0d24b3863f`.
Its r4 result is `outputs/rea/driver5g-ops-20261022-tqe-baseline-r4/result.json`,
SHA256 `af8420544d0c6de80a12456759fbd7d0ae92c01abec69d2b306f3c1b8f6545fc`.

Independent hashing confirms the exact retained module and both complete new
compilations have identical whole-file SHA256:
`bd5ab1018d293dc6bed655ec2bbc6da64a3969f28a3de578c04fa9c3f4af635f`.
Each module is35552bytes. Exact metadata matches, including name, depends,
vermagic, every parameter and type; neither parent nor rebuilt module contains
srcversion. The full comparison has no errors and records52functions,
464RELA,275symbols and11788executable-section bytes.

**The complete TQE baseline reconstruction gap is now resolved for r4.**
The script reads retained compiler commands, reuses pinned ARC toolchain and
F363 context, creates new immutable output only, compiles source rather than
patching the ELF, and never produces firmware or accesses hardware. Its
file-prefix mapping preserves the retained source path identity. This result
does not solve arbitration, change packet ownership or promote the bounded
admission prototype. Its foreign-writer collision and module-policy NO_GO
remain unchanged.

Three prospective verifier improvements were reported to the rebuild agent:

1. Require the complete candidate hash to equal PARENT_SHA in the pass condition,
   not only semantic sections and metadata subsets. This equality was observed
   independently for r4; it was not an explicit script assertion.
2. Compare srcversion without a generic exception for an exact baseline gate.
   r4 is unaffected because the complete bytes are identical and srcversion
   is absent on both sides.
3. Add the actual `include/generated/autoconf.h` dependency explicitly and fail
   any actual compilation dependency absent from the prebuild pin set. Cross
   checking the actual source `.d` found745dependencies and753pins; the one
   missing dependency is
   `/workspace/OpenMitraStar-build/f363/f363-components-20260921T203041Z/kernel/include/generated/autoconf.h`.
   Current hashes of every existing recorded pin match: zero drift. The missing
   pin does not invalidate the observed r4 module equality, but complete
   prebuild input coverage must not be claimed until that omission is fixed.

Older r1-r3 failures and r4 evidence remain intact. A hardened r5 or later gate
must use a new output and its own evidence; this supplement does not infer its
result or modify the source review's original conclusions at their time.
