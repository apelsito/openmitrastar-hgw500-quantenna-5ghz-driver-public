# Gates — 22 October 2026

Active target is **Linux7.3**. The repository and `port/linux-7.3` stay private.
R20 compiles72 real C units and links21 native modules against a fresh kernel
and26 base modules. Two fresh complete builds, including external Ethernet,
match byte for byte. The ARC ABI/import/instruction audit passes for48 modules
per build and vmlinux. Normal loading and physical5GHz acceptance remain pending.
Current source is preserved in `src/linux-7.3-r26` and selected
by `src/CURRENT`. See [R24](LINUX73_CARRIER_WRITER_R24.md) and the
[AELITA build recipe](AELITA_END_TO_END_SANITIZED_20261022.md).
R26 completes the compiled port handoff requested by the owner. Two fresh74 C/
22-module builds match; ARC ABI/import/ISA and source pins pass. AuC clear/load
share one writer lease, and reset admission precedes F355 promotion. Physical
radio functionality, producer quiescence and complete firmware size are
transferred to the firmware integration chat. See [handoff](HANDOFF_LINUX73_R26.md).
R24 pins exact firmware hashes and checks all loader destinations while holding
a task-owned SRAM writer lease. The
[current qualification](../evidence/linux73-r24-final-independent-qualification-20261023T060500Z-r1/result.json)
binds74 C units/22 paired modules, unchanged ARC ABI,273 loader cases and410
provider cases. CRYPTO_LIB_SHA256=y is a mandatory external config requirement.
This lease covers the loader, not resets, AuC clears or every indirect firmware
writer. The pool remains unregistered/closed and physical gates remain unrun.
R22 adds the typed SRAM resource/DMA backend; R23 adds complete packed-carrier
preflight and delayed entry publication. Two fresh builds of74 C units and22
native modules match, with imports/instructions and input/copy pins checked.
The R22→R23 delta affects qdrv_fw.o and qdrv.ko; the other21 modules match exactly.
Kernel packet ABI is unchanged and uses the pinned rebuilt AELITA context.
See [typed SRAM](LINUX73_SRAM_TYPED_R22.md) and [loader bounds](LINUX73_LOADER_BOUNDS_R23.md).
Reservation registration, indirect writers and normal radio loading remain unproved.
The [complete MuC payload search](../evidence/rbs-muc-mailbox-fullmap-bounded-audit-20261023T012000Z-r1/HANDOFF.md)
recovers mailbox structure and corrects stale symbol labels using actual code.
Mailbox1 TX admission and per-frame CAC grant/revoke remain unresolved; ioctl26
is global TX control. Eight new REA functions are retained without adding ops.
R19 subsequently corrects the original468 getter alias and builds a fresh7.3
kernel,26 base modules and21 native driver modules. Its
[single-build receipt](../evidence/linux73-full44-native-stack-v7-receipt-20261023T005000Z-r19/result.json)
is offline evidence. R20 fixes GPIO module lifetime and DMA allocation admission
and passes actual controller fixtures. Its real ELF table has610 records and
all44 original missing shapes/names. Load/unload, pair/ABI, client traffic and
firmware acceptance remain separate gates. The
[paired build](../evidence/linux73-full44-complete-pair-receipt-20261023T020500Z-r20/result.json)
and [ARC ABI audit](../evidence/linux73-r20-pair-abi-audit-20261023T020000Z-r1/result.json)
qualify this offline source/package. Kernel/input tree pins are separate evidence;
this pair itself binds the candidate, config, compiler and complete C-object hashes.
All44 missing OEM private records appear in aggregate source, but source table
presence does not establish their operation. RBS exact-frame grant is unavailable;
DFS467 query retains bounded5GHz20MHz/fallback scope.
Default module loading is also refused by the TQE opt-in gate, then by the
required SRAM ring allocator if that gate is enabled. The OEM SRAM backend
is not ported/validated in7.3. See
[the source-bound load limits](../evidence/linux73-r20-sram-load-limit-20261022T220300Z-r1/AELITA_LOAD_LIMITS.md).
The laboratory DRAM fallback is not accepted as an SRAM port or physical proof.
The [sanitized AELITA recipe](AELITA_BUILD_ACCEPTANCE_20261022.md) also completed
a fresh kernel/base/native/EMAC build and48-module ARC ABI/import/ISA audit.
It is a single build of the newly composed source, separate from the v6 pair;
execution on the AELITA computer and physical router gates remain unrun.

The table below records **historical Linux5.10 qualification** unless a row
explicitly names Linux7.3.

| Component | Observed gate | Remaining work |
| --- | --- | --- |
| QDRV F365/F370 | Full baseline static equivalence; F370 complete module built twice | OEM extension semantics, datapath behavior, physical acceptance |
| switch_tqe baseline | Byte-identical full module x2, 52 functions / 464 RELA | Universal CPUIF arbitration; no bounded-admission promotion |
| Standalone TQE repository build | Byte-identical full module x2 from this independent checkout; 6685 input pins | Other standalone component gates tracked separately |
| Standalone WLAN repository build | Full operational baseline equivalent x2 from this checkout; 6685 pins | Only declared srcversion differs; physical gates UNRUN |
| Standalone QDRV repository build | All831functions/14034RELA/1932symbols equivalent x2;6685pins | Only declared srcversion differs;physical gates UNRUN |
| WLAN baseline | Full operational equivalence x2, 959 functions / 9543 RELA | Original srcversion differs; further OEM extensions |
| TX_MAXMPDU aliases | Two aliases, full module x2; narrow global delta audit and 5 verifier fault checks | Physical gate UNRUN; no image installation |
| Seven-entry candidate | Complete QDRV/WLAN pair built twice; global delta audit, 2208 input pins and actual function fixtures 327681 checks PASS | 37 OEM table gaps; physical gates UNRUN |
| Fourteen-entry candidate | All53Cunits per lane compiled twice; whole-module audit,7faultchecks and independent review PASS | 12operations+2faithful unsupported returns;30OEM gaps; physical gates UNRUN |
| Fifteen-entry candidate |19WLAN units compiled twice; unchanged qualifiedQDRV; actualARCfield store,whole-module audit and7faultchecks PASS |13operations+2faithful unsupported returns;29OEM gaps;physical gates UNRUN |
| Eighteen-entry candidate |106Ccompiles without reuse,x2 identical pair,ARC private ABI,whole-module audit,7fault checks and independent review PASS |16functional entries+2unsupported;26OEM gaps;physical gates UNRUN |
| AMSDU20 candidate |116freshCcompiles,7modulesx2 identical,209fieldARCABI,global main/ancillary deltas,7negative checks and independent review PASS |18functional entries+2unsupported;24OEM gaps;aggregation/physical gates UNRUN |
| NAC22 candidate | WLAN built twice identically; 36 unchanged objects and header/context identity bound to qualified20; complete residual audit, 7 negative checks and independent review PASS | 588 private records, 585 OEM matches, 22 OEM gaps; sampled NAC scalars are not a coherent tuple or physical monitor proof |
| Legacy23 candidate | Seven modules built twice from116 C compilations; same-header control58 fresh units, global audit, full ABI, 3548272 transaction cases, seven negative tests and independent review PASS | 589 private records, 586 OEM matches, 21 OEM gaps; fixed-rate PHY behavior, NAC coherence and physical acceptance remain unrun |
| CoC25 candidate | Seven modules built twice from116 fresh units; exact-header control54 reused/4 new units; complete audit,2378 pins,11 negatives and independent review PASS | 591 records,588 OEM matches,19 gaps; initialized retained IC scope only; PM/CSA/RF/physical acceptance remains unrun |
| Beacon lifetime R9 | Seven source-built modules identical across two builds; complete same-header control build; global code/data/symbol/RELA audit, 2342 pins and 11 negative checks PASS | Lifetime change qualified offline; inherited19 capabilities, action handlers and physical acceptance remain separate |
| Beacon unpublished ioctl cleanup R10 | Two real changed-unit compilations, full QDRV relinks identical, six other modules exactR9; global audit, four cleanup paths and seven negative checks PASS | Two inherited slot leaks corrected offline; RF, full worker runtime, persistence and release remain UNRUN |
| APP IE synchronization R11c | Four changed WLAN units compile twice; complete seven-module pair, residual audit, 3063 pins and five ELF negative checks PASS | Bounded APP synchronization qualified offline; full kernel reentrancy, physical acceptance and consolidation withLegacy23 remain separate |
| ConsolidatedLegacy23/beacon R12 | All58 C units x2, seven identical modules, complete residual/metadata/ABI checks; four492/493 records audited with explicit advertisement deviations | 27 reconstructed records with17 table gaps; full80211k/v action handlers and physical acceptance unrun; CoC25 separate until consolidation |
| Consolidated29 | CoC25 plus492/493/beacon/APP in one verified source, seven modules x2, complete residual/metadata/ABI checks,3251 pins and source fixtures PASS | 29 reconstructed records,15 remaining;449/461 not included; fullfunctional/RF/client/traffic/persistence/release unrun |
| Consolidated30 r2 | Qualified29 plus449; seven modules identical x2,8 fresh changed-unit compiles/108 exact parent objects, complete old ABI/metadata/residual audit,3933 pins, actual consumer fixtures and11 negative cases PASS | 30 reconstructed records,14 remaining; fullfunctional/RF/client/traffic/persistence/release unrun; rejected r1 timer omission preserved |
| Consolidated32 r2 | Qualified30 plus461set/get; seven modules identical x2,8 changed-unit compilations/108 exact parent objects, public/private scanner ABI, complete residual/metadata audit,4520 pins and11 negative cases PASS | 32 reconstructed records,12 remaining; bounded UP/PREEMPT_NONE foreground scope and sampled metrics; RF/client/traffic/persistence/release unrun |
| Weather478 source34 | Qualified32 plus478set/get and actual AP weather-selection consumer;7 paired modules,6 fresh changed-unit compilations/110 exact parent objects, former padding22605 ABI,5544 pins and16 negative cases PASS; root independent review PASS | 34 recovered records,10 remaining;600-second weatherCAC/radar source intact; physical CAC/RF/client/traffic/persistence/release unrun |
| Coexistence449 r3 | Separate CoC25-derived candidate passes actual BSS geometry/parser/consumer fixtures, ARC ABI, paired modules and independent review; integrated into qualified30 r2 | Physical BSS/MuC/bandwidth and traffic acceptance unrun |
| Channel-set461 adapter r15 | 130 original table records preserve10400bytes; integrated into qualified32 with source/caller/scanner ABI/module checks and5 actual-start/7 complete scan_next/AP_end cases | 40MHz queue states explicitly supplied; full automatic40MHz, coherent shared metrics and physical qualification unrun |
| DFS467 bounded r2 | Independent stable5GHz20MHz AP scope;7 paired modules,386IC/210VAP/72QDRV ABI,4262 pins,28672 refresh/pick cases,131072 getter cases,10 negatives and root independent review PASS | Original all-mode467 remains pending; metric/alternate/fallback,40/80MHz and hotdetach/overlap unproved; not counted in32 |
| Consolidated32 plus bounded467 |598 previous records exact plus1 bounded467;7 paired modules,387IC/210VAP/72QDRV ABI,6750 pins,actual461/query/getter fixtures and10 negative cases PASS; root review PASS |32 reconstructed records plus1 bounded extra/table599; full467 not recovered, separate fromweather34 |
| VOPT481 core r2 | Fresh OEM REA apply/timer/classifier; actual C states and callback sequences pass1110083 cases; independent unsigned-byte correction replay; ARC object2104bytes | Kernel timer/work/provider/ABI integration pending; no new qualified record |
| RBSDFS466/468 core r2 | Fresh OEM REA producers/send/expiry;2705 source cases and ARC object2220bytes PASS | Exact-frame grant/revoke provider and Linux integration missing; no new qualified records |
| TQE LHOST serialization prototype | Four real writers/header primitives,164IRQ interleaving cases | UnboundedIRQ waits and foreign-writer counterexample;NO_GO_MODULE |
| TQE bounded provider R2 |Real baseline+pair builds x2,171bound source cases,global residual audit,12negative checks and independent review PASS |Ledger saturation,HBMwaits,foreign writers and recovery remain;not release/physical PASS |
| TQE typed multicast R3 source-r4 | Pair built twice, fixed256-record ledger, actual caller259 cases, callback142, capacity35212, global residual and independent symbol/value review PASS | Diagnostic congestion retry becomes quarantine; outer IP/FWT entry, HBM waits, foreign writers, recovery and traffic acceptance remain open |
| TQE queue retry R4 source-r8 | Complete pair x2, typed confirmed-CN retry, finite135 markers, atomic owner/credit/head commit, current build/global pins and independent review PASS | BoundedUP/nonPCIe retry scope; allqueue/foreignHBM/recovery/performance/physical acceptance remain open |
| HBM baseline | Complete module reproduced twice byte-identical21284bytes/28functions/259RELA/1006pins | Typed fence not integrated; original weak void APIs in other modules can bypass quarantine |
| OEM ops inventory | Exact tables, relocations, source assignments and REA cases | Source assignments are lexical; unknowns tracked individually |
| Comments | Original source preserved; exact extraction manifest | Regenerate a new catalog when candidate source changes |
| Browser desktop | 10,168-byte shell passes real loopback HTTPS/browser tests; exact comments viewer passes HTTPS decompression/search | Router TLS/certificate trust, measured Doom packaging and actual HGW deployment remain pending |

The initial source-only TQE prototype is not a qualified new TQE driver. Its
fixture reproduces a collision with a noncooperating writer. A passing fixture
documents the counterexample; it does not make arbitration safe.

The later seven-entry source candidate adds getter450 (observed OEM constant70),
getters496/497 (observed constant8), and getters331/275 reading existing named
CCA fields. It also retains the two parameter246 aliases. Port offsets were
compiled independently and differ from OEM; no raw OEM field offset is used.
This leaves37 OEM records unmatched; two are explicitly unsupported in
the OEM and35 require further implementation. The seven-entry complete pair
passes offline qualification; it is not a physical release. The later14-entry
pair is now also qualified offline:12operations plus2unsupported returns,
with30tablegaps remaining. Its final qualification binds17evidence inputs and
the actual fixtures.1502host-accessible build pins were refreshed;718Linux-only
pins were verified in the prior Linux run, not claimed freshly checked on Windows.
Further source-only
branches recover device-mode/bandwidth fields and retain faithful unsupported
returns; the NAC read prototype adds a documented divide-by-zero guard. Those
branches require their own complete-module and lifetime qualification.

Native radar/DFS operation, RF control behavior, WPA2 association, useful
bidirectional traffic, fatal-free soak, persistent cold boot and preserved
rollback are not established by offline compilation. No device was operated
for the work recorded in this repository.

The current combined table count is34 reconstructed records and10 remaining,
sealed in [Weather478 source34](../evidence/weather478-qualification-20261023T000100Z-r1/result.json).
The [CoC25 measured ledger](../evidence/qualified-coc25-remaining-ops-20261022T183000Z-r1/LEDGER.md)
retains its historical19 gaps;492/493,449,461 and478 account for nine subsequent
reconstructed records. The remaining10 records are467get,
466set/get,468set/get,481set/get,494set/get and495set.
The [NAC22 qualification](../evidence/driver5g-ops-20261022-nac22-qualification-r2/HANDOFF.md)
retains the OEM HOSTAP setter/no-op behavior and scalar getter packing, with an
explicit EAGAIN protection for a missing pointer or zero denominator. This
does not establish monitor timers, coherent snapshots, RF or release acceptance.
