# Current handoff — 22 October 2026

The independent repository is private on`apelsito/openmitrastar-hgw500-quantenna-5ghz-driver`.
Original available GPL firmware source is preserved as a pinned Git submodule;
radio/port files, comments, copyright and line endings are retained exactly.

## Delivered

* Original source reference:46770files,598076127bytes,commit116a8b0.
* Full C/C++ comment record:1331535comments,104725712exact bytes,35018files,
  every compressed raw record replayed by hash. Document and HTTPS client page.
* Independent builds of QDRV,WLAN,TQE each completed twice. TQE whole module
  is byte-identical; QDRV/WLAN operational code/data/RELA/symbols match with
  declared source identity `srcversion` differences.
* Qualified20record QDRV/WLAN/ancillary closure:18functional entries+2faithful
 unsupported,586WEXTrecords,24OEMtablegaps.116Ccompiles,7modulesx2,full main and
 ancillary deltas,negativechecks and independent review.
* AMSDU20 OEMVAPdefault/peerADDBA/MuC evidence and actualARC VAP layout preserve
 all209oldfields and use oldtailpadding; no aggregateperformance proof.
* Separate unqualified BSS/neighbor source candidates and TQE IRQ prototype.
* HTTPS-only client desktop shell10168bytes tested in actual browser. It is
  not installed on the HGW. Games run in the client; router serves assets.

## Immediate blocker and next action

The executor is recovered after the owner's explicit request to persist through
errors. REA-Linux was stopped and the WSL virtual machine restarted; the disk,
user files and pinned ARCcompiler were then verified. The18pair completed106
Ccompiles,x2 modules and global qualification. No router operation occurred.
AMSDU20 complete seven-module closure now passes x2 with its exhaustive
ABI/code/data/relocation qualification and negative checks. The global beacon
descriptor-lifetime candidate has also built twice and resolved its exports;
its separate global comparison is still required before promotion.

Windows REA/Ghidra now works in an isolated local ARC lab without modifying
installed REA/Codex. Completed MuC analyses include NAC,SIFS,RTS,SETdispatch
and11nAMSDU consumer; OEMsend_mgmt is also complete static evidence.
These do not substitute for source/callback/ABI/radio runtime acceptance.

AMSDU484 is now reconstructed in source with its distinct typedVAPflag, default1
and exact peerADDBA predicate. It leaves`iv_tx_amsdu` unchanged. ARCwitnesses
confirm all209previous field offsets/widths and VAP/enclosingQDRV sizes are
unchanged because the appended flag uses old tail padding. Full module closure
and global code/data proof remain separate from that source/layout evidence.

TQE publication prototype still has unlimited READY/completion waits with IRQ
disabled and a foreign-writer counterexample. It remainsNO_GO_MODULE.
Neighbor capability source has a fixed-arena global budget gate; source fixture
PASS is not a feature, driver or physical PASS.

The full Linux5.10/driver goal is unfinished. Native radar, hardware/firmware
behavior,association,traffic,soak,persistence and preserved rollback are separate
gates. No module/image from this work was installed, flashed or booted on the HGW.
