# OEM operation recovery

Recovery starts with the exact binary tables and dispatch, not an inferred API.
The original private ioctl table has607 entries. F365 has566, of which563
match OEM. Initially44 entries were absent and3 were port additions.

The complete baseline module recovers the two parameter246 aliases without
altering their existing setter/getter implementation. This candidate is
compiled twice and globally qualified. There are42 still missing after it.

REA additionally recovered the OEM WLAN setter/getter cases and QDRV getter
callback from its actual installation instruction. The later source candidate
includes seven entries total. It reads CCA fields by name and preserves the
OEM low16 parameter mask. Actual-function fixtures exercise all256×256 byte
values and all65536 high16 values:327681 checks pass. The complete later
QDRV/WLAN pair now passes full offline qualification with2208 checked input
pins. Both complete modules were linked twice identically. All executable
bytes outside the intentionally changed getter and exact diagnostic-line
immediates match, including code without function symbols; allocated data,
symbol attributes and relocation contracts outside the change also pass.
QDRV is684108bytes, SHA256
`dd8f29a1dccb9a01236eb4577f8909088eb544eac872593d4696bf1585cfad06`;
WLAN is629496bytes, SHA256
`461c23d2d29833216bb3e7b10afba51531b33cae73e7d0ed6c929482e1faec92`.

Later independent source branches preserve `get_dev_mode`, system bandwidth
and OEM unsupported returns. A NAC getter prototype reads only the published
ABI prefix and returnsEAGAIN when the denominator is zero or the pointer absent.
Its guard is an explicit safety change from the OEM behavior, with pointer
lifetime and asynchronous snapshot consistency still outside its fixture.
These branches are not part of the qualified seven-entry pair. The14-entry
BBDF/device-mode/bandwidth pair now passes its own full offline qualification:
106Ccompiles without object reuse, exact two builds per component, all residual
bytes/sections/symbols/relocations checked and7negative verifications. It adds
12operations and2faithful unsupported returns;30OEMrecords remain.

QDRV684756bytes SHA256
`3e76821b3d6358682db6a109dc1f1dc33f6a440e36fcb13bf1711dc58c2ecf06`;
WLAN629984bytes SHA256
`46075f1fca1d4b07fac0841798a29bc5fab28c900b2bb9692e8f946d7098cb79`.
Final receipt:`evidence/driver5g-ops-20261022-fourteen-full-qualification-r5/`.
This remains module-only and offline. MuC effects, queue behavior and physical
radio acceptance are not proved by that receipt.

The15-entry pair subsequently qualifies`check_cur_chan` (464). It reuses the
unchanged qualified14QDRV and rebuilds all19WLAN units twice. The actual case
block stores to`ic_chan_is_set` at the decoded port offset19488, width4bytes;
the case jump-table link and focused Ghidra instructions are preserved. The
separate offset-compiler invocation failed in WSL and is explicitly UNRUN;
the complete Cunit/module compilation and direct ARC decode did complete.
All580prior WEXT records remain, table581;13operations+2unsupported returns,
29OEMgaps. Seven negative checks and131074actual-source assignment cases pass.
WLAN629896bytes SHA256
`eac8c0633dc719d96850cb4cb95ff9cec42e2d3e217bda632089c7b2e922f946`.
Receipt:`evidence/driver5g-ops-20261022-fifteen-pair-sealed-r1/`.

The separate18-entry source candidate adds the existing command11/76byte
transport for dynamicSIFS463 and duplicateRTS498 set/get. Windows REA confirms
the actual MuC dispatch and selector/flag consumers. The low8 reportedRTS state
is kept in a new privateQDRV byte while the completeu32 command value is passed
to MuC; that is an explicit state-ownership change from OEM, not a wire-layout
change. Its whole106Ccompile and private-ABI witness remain blocked by Linux
executor I/O. It is SOURCE_ONLY/NO_GO_MODULE;26tablegaps is a source target,
not an accepted module count. NAC remains an independent prototype. After
executor recovery, the18pair was fully qualified; the later20pair also passes
116freshCcompiles and allsevenmodulesx2. The20WLAN adds typed484VAPstate with
the actualADDBApredicate and OEMdefault1.209priorfields and allancillary code/
data/symbol/RELA remain unchanged; only declared source-version metadata differs.
All seven negatives and independentreview pass.586records=18functional+2
faithfullyunsupported,24OEMgaps. This is offline qualification, not runtime
aggregation or release evidence.

OEM setter465 `ignr_icac_sel` and getter495 `get_ft_over_ds` already return
`EOPNOTSUPP`. Restoring their table entries cannot be described as implementing
features. Other extensions need node/VAP/firmware ABI work before enabling them.

The evidence includes exact original module hashes, full symbol/relocation
inventory, WEXT slot layouts,44 case addresses and REA assembly/pseudocode.
Some local function names are absent in the original. A separate analysis
derivative adds labels only; all allocated bytes are verified unchanged.
Neither the labelled ELF nor the synthetic linked ELF is a runtime module.

Current dispatch names, function pointers and lexical source assignments do
not establish every operation's runtime contract. Further reconstruction must
track callers, arguments, DMA lifetime, result codes, preprocessor conditions
and dependency closure alongside recovered code.
