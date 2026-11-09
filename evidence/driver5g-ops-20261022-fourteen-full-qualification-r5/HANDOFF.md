# Final14-entry offline pair qualification

2026-10-22. Status: **PASS_OFFLINE_COMPLETE_FOURTEEN_ENTRY_PAIR**.
Complete build remains
`outputs/rea/driver5g-ops-20261022-fourteen-full-module-pair-r4`.
All34QDRV+19WLAN Cunits were compiled in both lanes,106compilations; no old
objects were reused. Both module hashes are reproducible:

-QDRV684756bytes, `3e76821b3d6358682db6a109dc1f1dc33f6a440e36fcb13bf1711dc58c2ecf06`.
-WLAN629984bytes, `46075f1fca1d4b07fac0841798a29bc5fab28c900b2bb9692e8f946d7098cb79`.

14original WEXT records are appended:12functional entries and2faithful
unsupported returns. The table is580entries, preserving all566original records
and exact appended ID/set/get/name descriptors in order.30OEM table gaps remain.
NAC is not included. No full-driver, firmware or physical completion is claimed.

The new private BBDFbool is appended at30552;struct size30552to30556 and every
existing private prefix field offset is preserved. Allocation and zeroing use
the new exact size. BBDF host normalization/SETPARAM11behavior is recovered;
the firmware's real response is a separate physical contract.

The final audit enforces exact section membership and section type,flags,
alignment and entry size;all executable bytes outside declared get/set bodies,
including unsymbolized holes;all original symbol occurrences;all relocations
outside changed get/set;all allocated data and strings;BSS;and exact metadata.
Only the exact privateargs append/count,defined getter/setter size shifts,two
alignmentNOPs,diagnosticline immediates,threeMOVsizewords30552to30556 and six
explicit compiler CSWTCHrenames are normalized. Changes inside get/set bodies
are supported separately by the bound source fixtures, not labelled equivalent.

Provenance is sealed in `qualification-input-pins.json`:17read-once inputs
hashed before parsing and rechecked at completion, including build receipt,
build pin manifest,three candidate-modified sources and proof,parsers/tools,
BBDF/CCA/devmode/maxbw/fault fixture results. Exact executed qualifier is retained
in this directory. Its SHA256 is
`66192fa6be17a03b8faaedfae3ec33f4846448a8b13d3caf4a3829fbe7440e65`.

Bound fixture evidence:

-CCA getters/high16forward327681checks and ARC named-field offsets.
-Device mode256roles×2flags×256high16samples plus65536unsupported inputs.
-Maxbandwidth65536cases/196608checks.
-BBDF131072cases plus private-prefix ARC witness.
-Fault tests7cases:correctmodulePASS;changed text-hole,RELA,rodata,string,
 added section or altered section flags allREJECTED. Final rerun is
 `outputs/rea/driver5g-ops-20261022-fourteen-full-fault-tests-host-r3/result.json`.

The actual ARC complete build and first qualification verified2220pins while
REA-Linux was healthy. The final Windows parser gate refreshes1502C/Dhost pins;
718Linux-only pins are unavailable after the laterWSL/Service0x8007274c failure.
They are reported as previously verified, not newly checked. No WSL reset,
restart,configuration repair or hardware operation occurred. The pure offline
Windows verifier/fault tests completed without changing original modules.

`result.json` SHA256:
`4f5c151e0e06744820a2b2cae4af422a53b8e42d58f8eb1393890525c692e7f7`.
`qualification-input-pins.json` SHA256:
`df9b72cab252de58af894a64074205a46c493765f0e1030c302cbf8c24c8932f`.

All earlier stopped/failed build and qualification evidence remains intact.
Physical visibility,WPA2association,traffic,TQEarbitration,MuCacceptance,
soak,coldboot,persistence,rollback andrelease acceptance remainUNRUN.
