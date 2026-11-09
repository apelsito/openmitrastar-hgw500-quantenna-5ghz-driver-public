# Seven-entry QDRV/WLAN offline module pair

2026-10-22. **PASS_OFFLINE_COMPLETE_SEVEN_ENTRY_PAIR**.
No module was installed, firmware packaged or hardware accessed. The modules
must be considered together: the old QDRV does not implement the CCA getters.
Remaining37OEM private entries, TQE arbitration, physical association/traffic,
persistence, recovery and release acceptance remain unproved.

## Exact immutable modules

Build: `outputs/rea/driver5g-ops-20261022-seven-module-pair-r2`.

- `a/qdrv/qdrv.ko` and `b/qdrv/qdrv.ko`:684108bytes,
  SHA256 `dd8f29a1dccb9a01236eb4577f8909088eb544eac872593d4696bf1585cfad06`.
- `a/wlan/wlan.ko` and `b/wlan/wlan.ko`:629496bytes,
  SHA256 `461c23d2d29833216bb3e7b10afba51531b33cae73e7d0ed6c929482e1faec92`.

Both modified C units were compiled independently for each lane. Every
complete source-built baseline unit was linked;33unchanged QDRV objects and
18unchanged WLAN objects were reused with hashes from qualified baseline builds.
No ELF code or metadata was patched. Source files remain unchanged.

QDRV lineage is the reconstructed F364/F365 baseline retained under the F370
build's `baseline/` lane; it deliberately excludes F370's observational AuC
counters. The original QDRV baseline was statically equivalent to F365 after
declared source-path normalization, not whole-file identical. WLAN baseline
is the reconstructed module with its separately declared original-to-build
srcversion difference. The candidate module metadata is byte-identical to
its corresponding reconstructed baseline.

## Seven recovered WEXT entries

Table566to573; exactly168added bytes:

| ID | Setter | Getter | Name |
|---|---:|---:|---|
|246|18433|0|tx_maxmpdu|
|246|0|18433|get_tx_maxmpdu|
|450|0|18433|get_min_cac|
|496|0|18433|get_sretry_lmt|
|497|0|18433|get_lretry_lmt|
|331|0|18433|get_cca_fixed|
|275|0|18433|get_auto_cca_en|

QDRV returns its existing named fields for331/275; WLAN forwards the low16bits
of those parameter IDs, preserving compatibility with high16bit inputs.
450returns the existing70second minimum;496/497return the exact retained OEM
compatibility constant8. These values do not prove hardware retry behavior,
change setter policy, enable radar, shorten CAC/NOP or mutate radio state.

## Qualification evidence

`result.json` and `whole-module-audit.json` in this directory record:

-2208source/header/build pins verified live and each module bound to its build
 receipt path/hash/size.
-QDRV14034RELA and1932symbols; WLAN9543baseline/9544candidateRELA and2250symbols.
 All symbols and relocations outside the intentionally changed getter body
 match semantically after exact piecewise insertion mapping.
-All executable section bytes, including code without function symbols and
 alignment holes, match after replacing only the changed getter body and exact
 diagnostic line immediates. No broad relocation or instruction byte mask.
-All allocated data outside the exact168byte table insertion, private count,
 and declared string source-path mapping match. All original strings are
 compared, including strings without symbol records; BSS size/alignment match.
-QDRV getter grows2968to3016bytes. WLAN getter grows5576to5712bytes. Their
 bodies are intentionally changed and are explicitly not labelled equivalent.
-The existing `mov r1/r2,line` diagnostics have exact expected line changes:
 QDRV+9; WLAN+6 or+38. One compact ARC instruction is accepted only as the
 exact observed byte pair `8a229807`to`8a221809` (+6 line count), confirmed by
 pinned toolchain disassembly. All other instructions match.
-Separate actual-source/ARC ABI fixture has327681checks PASS. Measured port
 fields:CCA fixed20743,auto CCA20744,callback20924,struct size22644. Raw OEM
 numeric offsets are not used in candidate code.

Fault tests:
`outputs/rea/driver5g-ops-20261022-seven-pair-fault-tests-r1/result.json`.
5cases pass: correct parsed actual module accepted; code byte in unsymbolized
text hole, unrelated relocation addend, rodata byte and string byte rejected.
Mutations exist only in parsed in-memory copies; original modules untouched.

Builder: `tools/build_hgw5g_seven_getter_module_pair.py`.
Qualifier: `tools/qualify_hgw5g_seven_getter_module_pair.py`.
Fault runner: `tools/test_qualify_hgw5g_seven_getter_module_pair.py`.

Earlier buildr1 dependency failure and qualifier r1-r3 failures are preserved
unchanged. Their causes were missing aggregate baseline header pins, an exact
diagnostic immediate encoding and unsymbolized code-address mapping. No
historical failure was rewritten as PASS.
