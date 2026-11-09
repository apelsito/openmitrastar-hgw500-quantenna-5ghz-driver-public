# Offline checkpoint — 22 October 2026

All requested static Windows REA runs have completed with exit0. No background
analysis started by this lane remains outstanding. Hardware, UART, radio,
firmware, reboot, boot environment and installed Codex/REA settings were untouched.

An isolated Windows ARC700 lab now works using already installed Node24,
JDK21 and Ghidra12.1.4. Its local REA copy recognizes real ARC ELF32LE93 and
admits the exact flags0 MuC analysis artifact. The installed package and prior
Linux lab remain unchanged. The compiled ARCompact SLA was copied through
read-only UNC, SHA2563906c522f205a736dbf33c5753e4f7a4c597c9ca74ef710258d3236b68ea3037.
Native Windows addon files are present and copied from the existing package;
the first failed local attempt omitted them and remains retained as failure.

The source MuC remains SHA256
71dcd32849f122c09397490914227af6f77e51c34d15615028faa3d0fabb0fe4.
Its derived analysis-only code window is SHA256
2a9e1e7920d529fe376748be4b06260589d1bfef28ade0850883ae9c423ba27a,
with963131 exact executable payload bytes at0xe0000. Other memory regions,
hardware/register state and aliases are absent. Never install or package it.

Successful Windows REA results:

| Result directory under outputs/rea | Body | Meaning |
| --- | --- | --- |
| windows-muc-nac-rea-20261022T154600Z-r2 |452B|NAC457 monitor enable/disable/timer and percent fields|
| windows-muc-dynsifs-rea-20261022T154800Z-r1 |304B|SIFS463 high8/next8/low16 selector transport|
| windows-muc-duprts-rea-20261022T154800Z-r1 |248B|DupRTS498 bit0/bit4 state and firmware calls|
| windows-muc-setparams-rea-20261022T154800Z-r1 |7148B|Dispatch457/463/483/484/498 confirmed|
| windows-muc-dynsifs-get-20261022T155000Z-r1 |derived|Get selectors0..2/0..3; ARC loop pseudocode limitation|
| windows-muc-dynsifs-set-20261022T155000Z-r1 |derived|Set selectors0..2/0..3 and low16 value0..2|
| windows-muc-11n-amsdu-consumer-20261022T160000Z-r1 |436B|11n/non-VHT plus MuC node+80 controls aggregation size decision|
| windows-oem-send-mgmt-amsdu-20261022T160000Z-r1 |11596B|OEM WLAN ADDBA A-MSDU bit consumers and frame callbacks|

The final two result SHA256 values are respectively
fa85779cf85210038c4d6f389c806014f5f4659492da7d12038c983ea0a77c53
and b66fd0c9ce29eadfbb57afc106ea684cc10c81e5f48a88a865dd317c30be2896.
Their exact source and lab hashes, start/end timestamps, exit status and
target-unchanged proofs are in each result.json.

The current stable source progression:

*15-record parent remains unchanged.
*16-record dynamicSIFS463 alias:
  outputs/rea/dynamic-sifs-source-candidate-20261022T155300Z-r1;
  WLAN45a1e2a496d6e7a203b3625cf450ce8b1e5a8a0bea993be48f963d8cbcaadd5b.
  196608 actual host forwarding cases PASS. Exact existing QDRV command11/76B
  transport passes through fullu32; asynchronous firmware success is not claimed.
*18-record DupRTS498 set/get candidate, this directory:
  QDRV1f528c7fd2fef8d78d292282ef8296f6071d411898a9e0261d9b0a8e60a5994e,
  headerdeaa788ae7b90cfd85d64bd59d5f4daec6b8ef04b71538f06686b5f8e7c45bcd,
  WLAN8969d9885b4c9e3f8cef2b53e59f5cca7ad69955369345dc9600409dee36de8f.
  1048576 actual host cases PASS. The reported low8 state moved from OEM WLAN
  ic storage to the private QDRV tail, an explicit ownership difference; fullu32
  transport is preserved. Full pair/all-header-user rebuild/ABI/delta gates remain.
  Expected private table584, OEM matches581, remaining26, three port additions.

Separate capability prototype is NOT merged into the stable18 branch:
outputs/rea/neighbor-capability-integration-20261022T152300Z-r3 contains493
BSSTM and492 NeighborReport state/copy consumers. It needs full host ABI closure,
ancillary module rebuild and descriptor arena qualification. Its single20B
descriptor admission guard passed1022023 host states; this does not prove the
whole arena remains safe after later baseline/dynamic allocations. The disabled
RRM IE remains present with zero bit, an explicit deviation from OEM optional
presence. No802.11k/v management action support or client behavior is claimed.

First remaining implementation blocker:484 TX A-MSDU11n.
Exact OEM WLAN sets a distinct VAP bytec55 while existing iv_tx_amsdu isc54.
Its ADDBA bit is set only when iv_tx_amsdu is true and either peer flags
0x4008 are not exactly8, or the distinct11n flag is true. MuC command484 writes
node+80; the confirmed consumer additionally requires11n, non-VHT and other
peer conditions before choosing size0xeff/0x1eff. This is a static policy path,
not aggregation throughput proof. A new source candidate must recover a shared
VAP field or a typed callback accessible at the actual peer ADDBA site, initialize
it, preserve full param484 wire behavior and rebuild every field/header user.
The existing iv_tx_amsdu field cannot be reused. No20-entry candidate was invented.

The root's latest fully qualified candidate count and source-only count remain
separate. The long-running goal is unfinished: remaining operations, complete
module closure, queue/firmware behavior and physical acceptance are outstanding.
