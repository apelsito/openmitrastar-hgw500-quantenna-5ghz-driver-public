# Independent offline 5 GHz reconstruction review

Observed on 2026-10-22. Scope: HGW-500BNA-QCv2 A2 local source and retained
ELF/evidence only. No router, SSH, UART, module load, flash or RF operation.
No REA MCP session was used by this reviewer; the parent owns that session.

## Decision

QDRV already has a complete static baseline reconstruction gate. A new driver
should preserve that recovered implementation and fill measured gaps. A
callback-name inventory, decompilation or successful link alone cannot justify
the claim that every operation is functionally recovered.

The highest-priority current implementation gap is shared TQE transmit
arbitration and packet ownership, followed by complete exact-lineage dependency
reproduction. Current non-DFS physical client, traffic, persistent boot and
recovery gates remain unproved by these offline results.

## Current facts and evidence

1. `docs/OPENMITRASTAR_F370_COMPLETE_MODULE_20261021.md:28` and retained
   `outputs/rea/arc700-20261021-r2/f370-module-20261021T221200Z-r2/qualification-20261021T222300Z/result.json`
   record full QDRV baseline comparison: 831 functions, 312852 executable
   section bytes, 1932 symbols and 14034 RELA records. Imports 369 and exports
   15 are unchanged. This reviewer refreshed the accepted candidate hash:
   `d5e1f58303eaaa02123e0cf6352bd40c40df3027be300d2a1143b07f54a05dae`.
   Full ARC compilation was not repeated by this reviewer.
2. `tools/qualify_f370_qdrv_module.py:67` compares all section membership,
   allocated bytes, BSS size/alignment, canonical relocation destinations and
   symbols; it permits only declared source-path string normalization and
   build identity metadata. F370's change is four observational counters;
   it preserves the existing BUSY-free/success-return policy.
3. `work/linux510-radio-port/quantenna-f364-lifetime/qdrv/qdrv_wlan.c:7690`
   through line8036 contains a large explicit `ieee80211com` callback binding
   set, including association, keys, ADDBA/DELBA, channel, VAP and radar.
   Every build-enabled assignment and every secondary table must be matched
   against source type and ELF relocation. Source presence is not by itself a
   compiled or functional coverage result.
4. `docs/OPENMITRASTAR_TQE_BOUNDED_ADMISSION_PROTOTYPE_20261022.md:1` is
   SOURCE_ONLY_PROTOTYPE / NO_GO_MODULE. The exact `switch_tqe.c` source hash
   remains `929d89801045c2b5f421d4d40ca312c32feaf93612de6a575b9b362ab97d9aef`.
   Its full active `switch_tqe.ko` baseline equivalence is unproved. READY
   admission is bounded before consumption, but HBM loops/MMIO stalls and
   noncooperating foreign CPUIF writers are outside that guarantee.
5. `work/linux510-radio-port/quantenna-f344-peers/platform/topaz/switch_tqe.c:423`
   `topaz_tqe_xmit`, line491 `tqe_push_mcast`, line743
   `topaz_swfwd_tqe_xmit` and line1221 `tqe_tx_buf` are independent CPUIF writers.
   Existing wait-before-lock paths leave a wait-to-publication gap. Callback
   state at lines229-250 and call sites at991-999 also require lifetime joins.
6. Exact F364-lineage `qdrv_auc.c:89` unmaps stats without nulling the pointers;
   line158 registers a TQE callback; line165 exit does not revoke it.
   `qdrv_hal.c:212` retains the disabled ARC DSP reset branch with its bus-hang
   warning; line280 AuC stop is empty. F355 pinning makes hot reload unsupported
   after mutation and should be preserved. A full-image iteration does not
   need a new warm-restart implementation.
7. `qdrv_wlan.c:3258` ADDBA builds a firmware wire payload from TX/RX node state,
   sends it through hostlink, then frees DMA arguments. Restoring high-throughput
   aggregation requires ownership/completion proof along that path, not just
   recovering `ic_htaddba`/`ic_htdelba` pointers at7723-7724.
8. The retained F368r2 fixture explicitly excludes other raw producers and
   physical DMA coherency. STORE38 quarantine cannot be generalized to all
   hostlink commands without testing argument payload lifetime and producer
   return/ACK contracts individually.
9. `docs/LYOKO_CHANNEL_POLICY_ES.md:70` identifies radar compatibility as
   fail-closed, not a native detector. Native radar acquisition/provider,
   continuous detection, CAC/evacuation/NOP and restart deadline retention
   require separate coverage. Existing DFS binding names do not prove it.

## Priority and promotion requirements

P0: pin the exact driver set, kernel config, toolchain and firmware inputs.
For each callback/table slot record the field offset, declared function
prototype, enabled configuration, target symbol, section and relocation,
producer/caller, ownership and failure contract. Record literal NULL,
unsupported, conditional and unresolvable entries explicitly. Include qdrv,
wlan, TQE/FWT/HBM/DMA, hostlink and radar dependencies. Resolve direct source
evidence first and use REA for ambiguous shipped behavior; retain assembly
where ARC pseudocode ordering conflicts with instruction ordering.

P0: qualify every CPUIF writer under one admission/publication contract. Test
READY boundary, semaphore preownership/foreign ownership, lost ownership,
IRQ state, local reentrancy/SMP and a foreign writer between admission and
TXSTART. BUSY must retain original ownership where the API demands it; any
qdrv policy of consuming/dropping BUSY must be intentional and measured.
No timeout path may introduce double-free, use-after-free or unbounded cleanup.

P0: independently reproduce the complete active TQE and WLAN modules before
replacing them. Compare all allocatable sections/RELA/BSS and symbol/version
metadata as QDRV already does. Recovered function bodies are insufficient
for unverified module ABI closure. Build the selected candidate twice, audit
every changed instruction/data relocation and pin all dependencies.

P1: fault fixtures must extract actual candidate C bodies and wire constants.
Exercise hostlink late completions, ring exhaustion, DMA allocation/free,
congestion/requeue/wakeup, GSO/VLAN/shared skb references, node teardown and
TX BA state transitions. Host sanitizers do not establish ARC MMIO/cache or
firmware timing; add exact ARC layouts and binary-level evidence separately.

P1: preserve reboot-only retention. Warm restart may remain explicitly
unsupported. If warm restart is included, require processor/DMA retirement,
callback revocation/join, checked subordinate teardown, no stale mappings,
NOP deadline retention and Ethernet ownership isolation before claiming it.

P1: preserve fail-closed DFS. Recovering radar ops must not admit DFS TX until
the native backend, continuous detector and complete policy gates pass.

P2: only after offline promotion, create a new immutable full image with
manifest/hash/size/single-variable description/Bootbase/slot-fit. Physical
execution then needs exact-artifact authority, fresh binding/readback and
recovery. Test SSID visibility, WPA2 AUTH/ASSOC/AUTHORIZED, matched bidirectional
traffic, reconnection, fatal-free soak, cold boot and recovery. Keep each gate
distinct; an offline build is not an install or release pass.

## Replayed tests in this review

- `tools/test_f370_tqe_observation.py`: PASS, 12cases/87checks.
  Output: `f370-replay/result.json`; actual function ownership/return fixture.
- `tools/test_f368r2_txoff_fault.py`: PASS_OFFLINE_QUARANTINE,
  16cases/80checks. Output: `f368r2-replay/result.json`.
- `tools/test_tqe_bounded_admission_prototype.py`: 38cases/245checks PASS
  under ASan/UBSan, local and PCIe configurations.
  Output: `tqe-replay/result.json`. The foreign-writer collision is reproduced
  again; PASS means the counterexample is captured. Module remains NO_GO.

Total replay: 66cases/412checks. All three fixture candidate/harness hashes
match their retained intended inputs; none certifies hardware.

## Existing relevant verification entry points

- QDRV full build/equivalence: `tools/build_f370_qdrv_module.py`,
  `tools/qualify_f370_qdrv_module.py` and pinned dependency closure.
- ARC layout witnesses: `tools/audit_f344_abi.py`,
  `tools/qtn_qdrv_abi_diff.py`, `tools/test_qdrv_abi_contract.py`.
- STORE38 late-ACK: `tools/test_f368r2_txoff_fault.py`.
- AuC result/ownership: `tools/test_f370_tqe_observation.py`.
- TQE admission/collision: `tools/test_tqe_bounded_admission_prototype.py`.
- Radar contract: `tools/test_f316_qdrv_dfs_contract.py`; static contract
  tests are not continuous native-radar or physical CAC/NOP evidence.
- DMA/cache and queue history: `tools/test_qtn_switch_tqe_dma_cache.py`,
  `tools/test_qtn_tqe_compact_ring.py`, `tools/test_qtn_5g_disassoc_atomic.py`,
  `tools/test_qtn_5g_v62_scheduler_contract.py`. Inspect inputs and side effects
  before selecting any historical builder/test.

## Planning estimate, not a completion guarantee

With existing source and the working ARC700 analysis lane, a first reviewable
ops/coverage dossier should take roughly 1-2hours. Exact dependency baseline
reconstruction and one isolated module correction could take another 2-6hours
if retained inputs reproduce; a provenance mismatch changes that estimate.

A coherent non-DFS queue/ownership candidate with independent review is more
realistically 2-5working days than one decompilation pass. Physical acceptance
adds separately authorized installation/client windows and roughly 1-2days
of testing, including a useful soak. Native radar/all-mode and warm-restart
completion cannot be promised from this evidence; unrecovered backend or
processor-retirement contracts can extend the task into weeks.

These ranges are engineering estimates based on the observed dependency and
qualification gaps, not measured execution times or a claim that a physical
failure will occur.
