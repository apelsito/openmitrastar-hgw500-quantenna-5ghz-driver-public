# Preserved component notes

Editorial copy of the previous README. Workstation paths are normalized; original source and immutable evidence are not modified. Earlier status claims belong to their named checkpoints, not the live device. Start with the current [README](../README.md).

# OpenMitraStar HGW500 — Quantenna 5 GHz driver

**8 November 2026 — source and firmware reference.** The latest installed
image evidenced by HGW500 receipts is Foundation R3 (6 November), using
kernel R51 and the R54 host-control repair. Core Boot R1 + R39 is retained
as rollback. R3 passed exact-image RAM management, live-flash boot and double
full SPI readback; its specific coldboot, both-port traffic and long-load
gates remain open. A 7 November read matched the R3 kernel/PID1 before access
was lost; current hardware state has not been rechecked here.

The newest combined source is
`src/linux-7.3-integrated-r1` (`src/linux-7.3-integrated-r1/README.md`; maintainer-workspace reference):
security R2 plus radar R1 over R54, with 195 source hashes intact and two
matching ARC builds of 79 C units/22 modules against the R51 ABI. It remains
an **offline candidate, not firmware eligible**. MuC/AuC RX provenance and
TQE offload, the calibrated PHY radar producer and physical DFS/CAC, WPA,
client/traffic and whole-image gates are unqualified. `src/CURRENT` remains
R39; dependency manifests and HGW500 Git pins are separate references.
See [current state](../docs/STATUS.md) and [candidate build](../docs/BUILD.md).

The owner-directed cleanup on 8 November removed the obsolete source trees
R31–R35, R37–R38, R40, R42–R43, R45 and R55. Their manifests and full file/hash
inventories remain locally in `out/retired-candidates-20261108/`; historical
code variants were retired, not claimed byte-identical. R39 rollback, R54,
the integrated candidate, test controls, security/radar parents and REA stay
available. The dated checkpoints below describe their original scope.

The checkpoints below retain their dated scope; they do not select a newer
firmware or transfer physical acceptance between revisions.

**7 November 2026 — security R1 offline candidate.**
The isolated source package (`src/linux-7.3-security-r1/README.md`; maintainer-workspace reference) rejects failed
RX crypto/MIC validation, enforces the slow-path plaintext policy and keeps AP
EAPOL local to the authenticator. Actual-C adversarial/control tests and paired
74-C/21-module ARC builds pass against R51; only `qdrv.ko` changes from R54.
This is a bounded host repair, with OEM fragment/aggregation and physical gates open.

**6 November 2026 — R54 offline candidate, R39 still selected.**
`src/linux-7.3-r54` bounds command token storage and register snapshots, avoids
allocating the unused snapshot buffer, handles allocation/mapping failure and
validates the optional buffer pair/flag in `hwregcmp`. The actual-C regression
suite (`tools/test_quantenna_hwreg_memory.py`) reproduces the previous failures
and passes the corrected cases under ASan/UBSan. Two builds of 74 C units and
21 modules match against kernel R51; only `qdrv.ko` changes from the R52 module
set. OEM coprocessor binaries, integration pins and `src/CURRENT` remain unchanged.
This candidate has no new physical acceptance. Local receipts are kept in
`../HGW500/out/offline-foundation-20261106/quantenna-r54/`.

The component checkout in `${WORKSPACE}/openmitrastar-hgw500-quantenna-5ghz-driver`
was materialized on 3 November 2026 from the existing private repository,
including all 7,241 tracked paths and the preserved Git history. Its selected
delivery is `src/linux-7.3-r39` on `port/linux-7.3` after the 5 November memory,
loader, common-kernel and live-radio repairs. Earlier checkpoints remain preserved.
The HGW500 integration checkout retains its pinned submodule reference.

R39 has passed core startup, factory calibration access, 5 GHz AP creation,
phone association and bounded bidirectional traffic in RAM. The owner requested
an open AP; 20 MHz windows measured 63.23 Mb/s receive and 37.74 Mb/s send.
Native netdev MAC bookkeeping and the PM workqueue API are repaired, and their
previous warnings did not recur in the physical trial. At that RAM checkpoint,
the selected source was local; integration pins and persistent firmware remained unchanged.
See RAM evidence and limits (`docs/LINUX73_RAM_CORE_R35.md`; maintainer-workspace reference).

R30 connects the OEM kernel-bank SRAM allocator to boot/module dependency
initialization before dmautil/TQE. It separates radio preparation (release bits
and RFIC preparation) from a destructive reset, retaining live descriptors.
The initializer/allocator passed a temporary physical Core R1 probe with the
entire bank restored afterwards. That earlier R30 probe did not qualify the
full radio; the subsequent common-kernel/R39 acceptance is recorded above.
See R30 SRAM initialization (`docs/LINUX73_SRAM_BOOT_R30.md`; maintainer-workspace reference).

R29 adds board identity/Wi-Fi/RFIC admission before the first SoC reset and
passes validated values into shared publication. It also repairs the disabled
RSSI/KILL diagnostic path: idle samples cannot trigger a kill, prepublication
failures do not claim kill admission, and published timeouts retain the command.
The diagnostic remains compiled out. See R29 validation (`docs/LINUX73_CONTROL_R29.md`; maintainer-workspace reference).

R27 rejects invalid AuC statistics ranges and indices, clears stale mapping
pointers, keeps statistics snapshots idempotent, and uses the I/O accessors for
firmware writes. Its aligned AuC CCM transfers avoid byte-wise read/modify/write.
The actual-C regression checks and paired GCC 15.2/GCC 16.2 builds pass; physical
5 GHz remains unqualified. See R27 source and build (`docs/LINUX73_MEMORY_R27.md`; maintainer-workspace reference).

The sibling `hgw500-5ghz-driver-reverse-engineer` repository collects the
recovered Linux 5.10 reference source and readable OEM REA evidence.
`hgw500-5ghz-firmwares` contains separate MuC, AuC and DSP analysis folders.
Pinned external kernel/legacy/Realtek submodules can be initialized explicitly
when their source is required. The root component checkout currently contains
the complete driver repository tree and its direct GPL radio snapshot.

Verify the distributed source and preservation manifests with
`python tools/verify_distributed_checkout.py`. This check reports the eleven
unprovisioned private baseline inputs declared by historical manifests.
Full historical comparison uses `tools/verify_repository.py` after those
local inputs are provisioned. Build inputs and current acceptance limits
remain documented below.

Private working repository for the owner-authorized HGW-500BNA-QCv2 A2 port
from OEM Linux 2.6.35.12 to **Linux7.3**. Linux5.10.260 remains a reconstruction
reference. The 5 GHz hardware uses Quantenna
Topaz/QMAC on ARC700, with net80211 and Wireless Extensions.

The first step is to preserve the available old code and comments exactly,
then recover missing OEM operations and port them to Linux7.3. This is
an engineering repository, not a completed release or a flashable image.

## Structure

| Path | Purpose |
| --- | --- |
| `vendor/legacy-gpl/` | Original radio GPL Git blobs, byte for byte; do not edit |
| `vendor/legacy-firmware-source/` | Full original GPL firmware tree, pinned Git submodule: kernel, bootloader, buildroot, host and radio |
| `vendor/port-context/` | Current shared headers, separate from original GPL |
| `vendor/linux-7.3-upstream/` | Linux7.3 pinned commit submodule |
| `components/realtek-hgw500/` | Private Realtek and HGW board-port submodule |
| `src/linux-7.3/` | First preserved7.3 source package:62 real C units compiled; full closure pending |
| `src/linux-7.3-r16/` | Preserved intermediate native-provider source |
| `src/linux-7.3-r18/`, `src/linux-7.3-r19/` | Preserved intermediate source/build checkpoints |
| `src/linux-7.3-r20/` | Preserved paired kernel/48-module baseline |
| `src/linux-7.3-r22/`, `src/linux-7.3-r23/` | Preserved typed SRAM DMA and bounded-loader checkpoints |
| `src/linux-7.3-r24/` | Preserved exact carrier identity and writer checkpoint |
| `src/linux-7.3-r26/`, `src/CURRENT` | Delivered compiled port: clear/load/reset ownership, paired74 C/22 native modules |
| `components/ethernet-linux73/` | Current MAC-init/TXdiag source and new ABI external build recipe |
| `src/qdrv/`, `src/wlan/`, `src/platform/` | Recovered Linux 5.10 source baselines |
| `src/compat/` | Linux 5.10/ARC compatibility headers |
| `candidates/` and `patches/` | Independent changes with explicit provenance |
| `tools/` | Portable analysis, comments and build tools |
| `tools/integration/` | Exact historical workspace tools; require their recorded context |
| `manifests/` | Source hashes, upstream identity and build contracts |
| `evidence/` | Immutable offline results and REA evidence |
| `desktop/` | Static client-side desktop prototype; HTTPS only |
| `local-only/` | Ignored owner-local firmware/modules; never sent to GitHub |

Clone with `git clone --recurse-submodules` to retrieve the complete pinned
legacy firmware source. The radio snapshot also remains directly in this repo.
Use a case-sensitive Linux filesystem for the full kernel checkout: original
upper/lowercase filename collisions are recorded in its manifest and all
original bytes remain in the pinned Git objects.

See [current gates](../docs/STATUS.md), [provenance](../docs/PROVENANCE.md),
[comment catalog](../docs/COMMENTS.md), [build instructions](../docs/BUILD.md),
[HTTPS desktop architecture](../docs/DESKTOP.md) and [publication preparation](../docs/PUBLICATION.md).

## Historical evidence — 22 October 2026

The active delivery branch is **`port/linux-7.3`** and remains private. R26 is
selected by `src/CURRENT`:74 C units and22 native modules match in two fresh
builds, with real ARC ABI/import/ISA checks. Exact firmware identities and
loader writer ownership pass independent fixtures; normal loading and RF remain
unproved. The owner accepts the compiled port and transfers firmware integration
and physical acceptance to the other chat. See [R26 handoff](../docs/HANDOFF_LINUX73_R26.md). The
[AELITA repository guide](../docs/AELITA_REPOSITORIES.md) pins all source repositories.
Quantenna7.3 R20 has72 real source units and21 native modules built with its
actual Kbuild against a fresh kernel and26 base modules. Two complete builds
including Ethernet are byte-identical; ARC ABI/import/instruction audit passes
for48 modules per build and kernel. Normal load/physical gates remain pending.
No5GHz physical acceptance or complete security certification is claimed.
Default loading remains refused by the TQE opt-in gate and its required,
unported SRAM allocator. See [load limits](../evidence/linux73-r20-sram-load-limit-20261022T220300Z-r1/AELITA_LOAD_LIMITS.md).
All44 previously missing table entries are represented in the aggregate source;
RBS exact-frame grant and the bounded467 query remain explicit incomplete contracts.
Firmware, credentials and calibration are provisioned separately outside Git.
The priorities are reliability, useful radio capabilities and maintained security.

The following candidates retain their **Linux5.10 reference scope**.

The latest combined offline-qualified candidate is **Weather478 source34**:
34 of the original44 missing private records reconstructed,10 remaining.
This includes two faithful OEM unsupported returns. Seven complete modules
match across two builds; the checkpoint binds5544 inputs and the actual
source fixtures. See [the sealed combined result](../evidence/weather478-qualification-20261023T000100Z-r1/result.json).
Full radio operation, client traffic, persistence and release remain unproved.
Coexistence449 is integrated with its actual timer/RX/BSS consumers.
Channel-set461 is integrated with actual scan-start/scan-next/AP-end fixtures
and complete paired-module/ABI checks. Its exact UP foreground scope and
explicit40MHz test states remain documented. DFS467 has separate bounded5GHz
source/module qualification integrated separately on32; VOPT481 and RBSDFS466/468
have recovered C cores and adapters pending full qualification. They do not add
to the qualified34 count. MDID494/FT495 and source37 consolidation remain pending.
Earlier results below are historical.

* QDRV: complete baseline reconstruction, 831 functions and 14,034 RELA records.
  Independent checkout rebuild also passes x2;only declared srcversion differs.
* TQE: two complete builds are byte-identical to the retained 35,552-byte module.
* WLAN: two complete builds match all operational code/data, 959 functions and
  9,543 RELA records. Only the source identity metadata `srcversion` differs.
* Seven OEM records are recovered in an offline-qualified complete QDRV/WLAN
  pair: two parameter246 aliases, three constant getters and two named CCA
  getters. The private table grows from566 to573 records;37 OEM gaps remain.
* The20-record candidate passes full offline qualification:18functional entries
  and two faithful unsupported returns, leaving24OEM table gaps. AllQDRV/WLAN
  and five ancillary modules were rebuilt twice:116Ccompiles,7modules per lane,
  with ARCtyped layout, global deltas,negative checks and independent review.
  Capability/beacon candidates retain separate full-closure gates.
* NAC457 set/get now extends that qualified closure to22 recovered records:
  WLAN630628bytes, two identical builds, complete residual audit and independent
  review. The table has588 records,585 OEM matches and22 OEM gaps.
  Its getter samples three scalars; coherent monitor state and physical NAC
  behavior remain unproved. See the [exact remaining ledger](../evidence/qualified-nac22-remaining-ops-20261022T163900Z-r1/LEDGER.md).
* Legacy490 now has a qualified stateful implementation on NAC22. Seven modules
  compile twice identically, with full source control, ABI and independent review.
  The table has589 records:586 OEM matches,21 missing and3 port additions.
  The [latest ledger](../evidence/qualified-legacy23-remaining-ops-20261022T183000Z-r1/LEDGER.md)
  separates record recovery from PHY, traffic and release acceptance.

The OEM table has 607 private records: 563 match the original port, 44 were
missing and three are additions. Table presence does not establish behavior.
TQE multiwriter arbitration, native radar, physical client traffic, persistent
boot, rollback and final release remain separate unfinished gates.

Original comments, copyright and line endings remain in the imported sources.
Original driver binaries and the firmware carrier are preserved locally; a
decompilation is explicitly derived evidence, never labelled original source.

No flash/reboot/UART/radio/runtime action is performed by these tools.

## Arquitectura de procesamiento del HGW500

El [esquema y sus evidencias](../docs/HARDWARE_ARCHITECTURE.md), conservados el
3 de noviembre de 2026, identifican una CPU LHost ARC750 a 500 MHz y cuatro
coprocesadores: MuC, AuC, DSP y la MCU 8051 del RTL8192EE. Incluyen el inventario
SPI NOR de 16 MiB y el perfil DDR3 OEM de 128 MiB, con fuentes y límites.

![Arquitectura de procesamiento del HGW500](../docs/hardware/hgw500-processing-architecture.png)

El diagrama es una vista conceptual. La etiqueta qdrv describe el papel del
host; los gates de 5 GHz corresponden a la revisión de firmware indicada en
el estado HGW500, no al diagrama.
