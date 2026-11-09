# Twenty-entry driver closure — 22 October 2026

Status: PASS_OFFLINE_COMPLETE_TWENTY_TYPED_HEADER_CLOSURE.
No physical target, UART, radio, flash, boot environment, reboot or firmware
packaging action occurred. The complete original driver goal remains unfinished.

Actual58 C units per lane were rebuilt:34QDRV,19WLAN and five scan/crypto/auth
provider units. All seven modules were linked twice, with identical bytes in
both lanes; no baseline objects were reused. Original source files remained
unchanged. Inputs were pinned before compilation; qualification verified1511
build/source/tool/fixture/ABI/baseline/module input hashes before and after.

| Module | Bytes | SHA256 |
| --- | --- | --- |
| qdrv.ko |684772|f79e2836e3a18c64216be197ea371d4a4c314056451c30394404ecb0581ea913|
| wlan.ko |630312|aa00ffd4fa3fba0f7871fdea14c76f2a6816c7add9fd3ff0d9a7b0c9fa6c2c02|
| wlan_scan_ap.ko |15508|0345ee6674524126d322a00a10e2a74c4038dd54b18225ffd82cd96e11437093|
| wlan_scan_sta.ko |18740|8a1825a68082dd2086c6ba496ee70bd1d1663a2c0aa3acbf2f0c5da085b290a5|
| wlan_ccmp.ko |9792|7c54b944fd3f2d214dd8a4d0c98615a48b6fed1acb0b29d86176dff05c9d700f|
| wlan_tkip.ko |13472|5a625b63d6a86b8f43bc28a2cae90f5a8270cdc6298711029a9aac59f58a4f22|
| wlan_xauth.ko |3340|0cc908723d5d097f11acad4eb0c4b33d5ee5b183a134657e0291c8540c145989|

Modules and executed command logs are under
outputs/rea/driver5g-ops-20261022-twenty-full-closure-r1/a and /b.

The QDRV module is byte-identical to the complete eighteen-entry parent.
WLAN's586 private records preserve the original584-record eighteen-entry table
exactly and append only TXAMSDU11n484 set/get, matching the exact OEM ABI.
There are583 OEM record matches,24 missing original OEM records and three
existing port additions. Twenty recovered records include two faithful
EOPNOTSUPP responses; these are not implemented capabilities.

The strict WLAN audit accounts for all959 baseline functions,2250 symbols,
all28 sections and8969 relocations outside four intentionally changed bodies.
The changed bodies are getparam,setparam,vap_setup andsend_mgmt, backed by
actual-C fixtures and OEM REA provenance. Every residual executable byte,
including anonymous padding, and all allocated data match after the exact
48-byte table append, count584→586 and seven explicit diagnostic immediate
changes. Two diagnostics change compact/wide encoding; their exact instruction
pairs and relocation offset mapping are checked rather than broadly masked.

Five ancillary modules match every original executable/data byte, every
symbol and every relocation, with only a declared35-byte srcversion metadata
addition and resulting build-id change. Their exact original artifact identities
were verified against the retained F152 module-manifest.tsv before auditing.
No original metadata was copied into rebuilt module ELF files.

The actual ARC typed VAP witness checks209 previous fields' offsets and widths.
The new distinct11n scalar occupies old tail padding at3081; sizeof VAP remains
3084 and sizeof qdrv_vap remains3540. All following measured QDRV VAP anchors
remain unchanged. Host ioctl32bytes, SETPARAM payload76bytes and MuC beacon
descriptor20bytes remain unchanged. This is actualcompiler evidence, not an
assumption that append-only always preserves enclosing layout.

Actual host fixtures pass262144 peer flag/base/11n combinations,28 setter cases
and three explicit counterexamples. Seven parsed ELF mutation cases reject
unrelated code, section bytes, allocated data, table prefix, external relocations,
symbol size and extra diagnostic instruction changes. These are offline scope
tests; they do not establish physical aggregation, latency or packet delivery.

Remaining channel-set operation461 is a concrete subsystem gap. Fresh Windows
REA proves1814-byte chanset-selection and4942-byte chanset-pick functions using
80-byte heap entries, metric history, bandwidth policies and scan callbacks.
Neither subsystem function exists in the retained port source/module inventory.
Appending/reporting its flag alone would not restore its channel-selection
behavior. Exact new evidence is under windows-oem-chanset-start/pick-
20261022T165100Z-r1. Both runs finished exit0 with unchanged targets.

Next gates are full reconstruction of the remaining24 OEM operations and their
consumers, whole callback/hostlink/firmware ownership coverage, queue arbitration,
and eventually separately authorized physical client, traffic, cold boot,
rollback and release acceptance. The capability17/19 prototypes and NAC prototype
remain distinct source-only work and are not silently included in this closure.
