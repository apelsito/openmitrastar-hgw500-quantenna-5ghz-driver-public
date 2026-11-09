# Complete15entryofflinepair

2026-10-22: **PASS_OFFLINE_COMPLETE_FIFTEEN_ENTRY_PAIR**.
This preserves the qualified14entryQDRV unchanged and recompiles every19WLAN
Cunit twice:38Ccompilations with retainedARCflags and2224buildpins. BothWLAN
builds are byte-identical. No module was loaded or firmware image produced.

-QDRV684756bytes, SHA256
 `3e76821b3d6358682db6a109dc1f1dc33f6a440e36fcb13bf1711dc58c2ecf06`.
-WLAN629896bytes, SHA256
 `eac8c0633dc719d96850cb4cb95ff9cec42e2d3e217bda632089c7b2e922f946`.

Modules are paired in `a/`and`b/`. QDRV derives from the already-qualified14
pair; it was copied exactly and not recompiled or changed because noQDRVsource
orheaderABI changes. WLANbuild is
`outputs/rea/driver5g-ops-20261022-fifteen-wlan-build-r1`.

The15threcord is command464,setdescriptor18433,getdescriptor0,name
`check_cur_chan`. All580parentrecords are unchanged; table count581.13entries
are functional and2remain faithfully unsupported.29original tablegaps remain.

The actual added setter stores its integer value in existing
`ic->ic_chan_is_set`. The field is declaredint in the existingABI. A separate
sourcefixture exercises131074signed/integercases with noRFoperation. The
existing channel validation/cleanup already consumes/clears this field; this
candidate adds noRFactivation ornewhardwarewrite.

The complete module audit (`audit.json`) checks section membership/attributes,
all residual executable bytes,all2250symbols,all allocated data/metadata and
9197relocations outside the intentionally changedsetter. Baseline9547RELA to
candidate9543RELA is confined to the changedsetterbody. That body shrinks
16294to16232bytes and loses a pre-existing exact2bytealignmentNOP, giving the
exact64bytepiecewise shift of following code. Exactdiagnosticline changes are
recorded and all other instructions are unchanged. The soledata change is the
24byteentry and count580to581. NoELFpatch ormetadata substitution occurred.

ARCfieldstore evidence is `field-write-witness.json` plus exactWindowsREA and
focusedGhidra logs. REA on the originalET_REL succeeds but its auto-discovered
function body is partial because internal jump-table relocations are unapplied;
its pseudocode is not used to claim the fullsetter orcase464.

The REAentrydecodesr14=ic fromdev+0x580 andr13=value fromparam[1]. Independently
reading the exactrelocations and16bittable proves tablebase.text158814,
caseindex464-94=370,entry1006,thereforecaseblock.text159820. A bounded Ghidra
script decodes originalbytes atmappedaddress0x3704c:

`add_s r14,r14,0x5000`
`st.as r13,[r14,-0x3e0]`

Thus the effectivecompiledfieldoffset is0x4c20=19488 with4bytewidth. The
originalmodulehash remains unchanged. The separateARCoffsetofwitness launch
failedWsl/Service0x8007274c after the completebuild; that witness is explicitly
UNRUN. Actual sourceunit/module compile and focused actualmachine decoding
are separate successful evidence. NoWSLreset/restart/configuration change.

Negativefixture7casesPASS:correctmodule accepted; unrelatedcode,RELA,rodata,
string,addedsection andsectionflags rejected. OriginalELFs never modified.
`outputs/rea/driver5g-ops-20261022-fifteen-fault-tests-r1/result.json`.

Inputproofs in`input-pins.json`bindcandidate source538da528...e9cb2,buildreceipt,
parent14qualification,sourcefixture,actualREAoutput,Ghidra log,tools andmodule
hashes. Originalsources andpreviousqualification remain unchanged. Consumer
semantics establish host behavior only; MuC,RF,association,traffic,persistence,
recovery andfinalrelease remain separateUNRUNgates.
