# NAC457 record recovery checkpoint — 22 October 2026

Status: PASS_OFFLINE_NAC22_RECORD_SEMANTICS_AND_PARENT_CLOSURE.
This is an offline record/semantics/closure gate, not22 fully accepted features
and not physical NAC monitor, packet delivery or radio acceptance.

The exact qualified20 source/7-module closure remains immutable. Only WLAN's
ieee80211_wireless.c changes; every other candidate source/header is checked
against that parent. The modified C unit was compiled twice and each resulting
WLAN module linked all19 source-built objects. The18 unchanged objects in each
lane are byte-verified against the parent (36 checks), preserving its already
qualified QDRV and five ancillary modules. No20 full rebuild was repeated.

WLAN modules both630628bytes, SHA256
dd4896a969b3033e41f8c63eac27b12b1844bd86dd1a506e69c7be5a485b4621.
They are under outputs/rea/driver5g-ops-20261022-nac22-module-r1/a/wlan and/b/wlan.

The588-record private table preserves all586 parent records and appends only
NAC457 setter nac_mode and getter get_nac_mode. OEM matches585, remaining22,
three previous port additions. Counts refer to recovered records; two other
legacy entries faithfully returnEOPNOTSUPP, and this getter samples scalars
without a generation counter/coherent-snapshot guarantee.

Exact OEM source/artifact evidence:

- WLAN setter457 sends through existing ic_setparam only for HOSTAP. Other
  modes return successful no-op, exactly as the retained OEM branch.
- Existing QDRV generic transport sends SETPARAMcommand11,76-byte payload
  {id,value,len,data[64]}, full original integer, no new ownership mechanism.
- MuC457 dispatcher invokes the retained NAC monitor enable/disable function.
  Its low8 byte selects enable/disable; high16 and next8 set timer/percent policy.
  Fresh Windows REA452-byte function and7148-byte dispatch bind the actual code.
- QDRV allocates/zeroes the2596-byte NAC area in its existing sharedallocation,
  publishes the LHOST pointer at shared+388 and the bus pointer atshared+392.
  F355 retains the ownership after core-start; VAP access belongs to that start.
  Sharedallocation/teardown/runtime gates retain their existing limitations.
- OEM getter reads only the first three fields: u16value0,u16value1,u8enabled.
  Its output packs value1<<16, low8(value0*100/value1)<<8 and low8enabled.

The recovered getter usesREAD_ONCE scalar sampling. It reads no client records,
MAC addresses, calibration payload or opaque NAC tail. An absent sharedpointer,
absent NACpointer or zero denominator returns-EAGAIN and leaves output unchanged.
That guard is an explicit safety deviation from OEM's unguarded division.
It does not make the three scalar reads a coherent MuC snapshot, nor prove
monitor readiness or ownership beyond the pinned baseline start/retention path.

Actual host C fixtures pass16711680 prefix combinations and1048576 setter cases.
The prefix fixture checks output packing, zero/missing refusal and no state writes;
the setter fixture checks all16 modes and65536 values for original forwarding/no-op.
Seven in-memory parsedELF mutation cases all reject unrelated function bytes,
section bytes, data, table prefix, external relocation, symbol size and extra
diagnostic instruction mutation. No original ELF file is altered by those tests.

The strict auditor verifies all959 functions,2250 symbols and9151 relocations
outside the intentionally modified get/set bodies. All residual executable
bytes, including anonymous padding, and allocated data match after only the
48-byte table append, count586→588, three exact diagnostic-line encoding changes
and three explicitly bounded GCC switch-table name renames. The getter's new
helper was inlined; no new standalone function/export was added.

Qualification reads pins once and verifies1652 build/source/tool/parent/module/
fixture inputs before and after. The first verifier failure remains retained
asNO_GO; it was the missing explicit GCC-generated name mapping and is not
retrospectively rewritten into successful evidence.

New tools:

- tools/prepare_hgw5g_nac22_source.py
- tools/test_hgw5g_nac22_source.py
- tools/build_hgw5g_wlan_incremental_from_closure.py
- tools/qualify_hgw5g_nac22_closure.py
- tools/test_hgw5g_nac22_delta_negative.py

Current source: outputs/rea/nac22-source-20261022T171000Z-r1;
fixture: nac22-host-fixture-20261022T171100Z-r1;
build: driver5g-ops-20261022-nac22-module-r1;
negative: driver5g-ops-20261022-nac22-negative-r1;
qualification: this directory revision2.

The original goal remains active and incomplete:22 original records plus broad
callbacks/firmware/queue/lifecycle and all relevant physical acceptance gates.
No repo publication, device access, runtime mutation or firmware write occurred.
