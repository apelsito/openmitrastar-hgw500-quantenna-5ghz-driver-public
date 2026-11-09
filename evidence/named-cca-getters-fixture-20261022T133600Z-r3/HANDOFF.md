# Offline 5 GHz operation recovery, 22 October 2026

This work accessed files and REA-Linux only. No target SSH, UART, RF, module
loading, firmware packing, flash, boot environment change or reboot occurred.

The current complete inventory is
`outputs/rea/ops-inventory-20261022T132200Z-r7/result.json`. It binds all symbols
and all allocated-data relocation records of seven exact OEM/F365/F370 modules,
plus 203 selected source files. Source assignments are lexical evidence,
include inactive preprocessor branches and are not a compiled CFG guarantee.

The exact OEM WLAN has 54 standard WEXT slots (42 non-null), 32 private WEXT
slots (31 non-null), and 607 private argument records. F365 preserves those
slot counts, but its private argument table has 566 records: 563 exact matches,
44 missing OEM records and three additional port records. The older search
starting at `setparam` observed only a suffix and cannot prove table completion.

The three port additions are `maccmd`17, `probe_rt`170 and `auc_tx`234.
The complete 44-item list is in the inventory `comparisons.json`.

REA executed against derived analysis-only ELF images of the actual OEM WLAN
and QDRV. OEM e_flags0x303 was admitted in a new isolated local runtime.
The previous lab and installed REA were unchanged. OEM local symbols were
stripped: QDRV init establishes `ic+0x5424 -> .text+0x30730`. A new symbol-only
analysis derivative labels this callback; every allocated section and its
layout remains byte-identical to its parent. That analysis derivative must
never be installed, booted, packaged or flashed.

OEM WLAN set/get REA:

- `outputs/rea/oem-wlan-missingops-20261022T131900Z-getparam-r2/rea.json`.
- `outputs/rea/oem-wlan-missingops-20261022T131900Z-setparam-r2/rea.json`.
- The exact 44 switch-case targets and bounded ARC assembly are under
  `outputs/rea/oem-wlan-case-map-20261022T132100Z-r1`.
- Classified REA excerpts are under
  `outputs/rea/oem-wlan-44-recovered-evidence-20261022T132500Z-r1`.

OEM QDRV init and getparam REA:

- `outputs/rea/oem-qdrv-missingops-20261022T132800Z-init/rea.json`.
- `outputs/rea/oem-qdrv-missingops-20261022T132700Z-getparam-r2/rea.json`.
- QDRV getparam has 4,592 decoded body bytes in two inclusive ranges.
- Its exact get331/get275 are byte reads from existing CCA fields.
- Its get496/get497 both return the constant8. This is the observed OEM
  compatibility policy and is not measured retry behavior or a universal
  hardware limit.
- WLAN get450 returns the existing MIN_CAC_PERIOD70, without changing CAC/RF.
- OEM setter465 `ignr_icac_sel` and getter495 `get_ft_over_ds` already reach
  explicit EOPNOTSUPP. Their advertised records do not prove implementation.

The qualified full WLAN two-alias candidate is a separate completed module
gate owned by the rebuild lane. The subsequent seven-entry candidate below is
source/object/fixture evidence only; its full QDRV/WLAN module pair is unbuilt.

Select only the corrected revision2:
`outputs/rea/named-cca-getters-candidate-20261022T133300Z-r2`.

- QDRV source SHA256
  `bd5d98aef3e1411fe93b30d195df3ea40827c403818d7fd5c629328431a6ae2f`.
- WLAN source SHA256
  `20eb551bac555ecc1df370c3c33dc0509eee82fd45bb2ad43d447c3ce429f955`.
- Two previous TX_MAXMPDU246 aliases reuse existing set/get source dispatch.
- Three read-only constant getters450/496/497 report70/8/8.
- Two read-only CCA getters331/275 use `ic->cca_fix_disable` and
  `ic->auto_cca_enable`; no OEM numerical offset is used in the port code.
- WLAN forwarding masks the low16 parameter bits, exactly like OEM.
- The paired new QDRV is mandatory; original F365 QDRV lacks these getter
  cases and returns success without writing output for unknown parameters.
- Remaining original table gaps after this source candidate:37. Two have
  explicit unsupported OEM behavior;35 still need full implementation/ABI
  qualification. No "all operations reconstructed" claim is warranted.

The ARC ABI witness and actual-source fake fixtures in this directory pass:

- Complete changed `qdrv_wlan.c` ARC700 unit compiled with retained flags.
- Actual port offsets: CCA20743, autoCCA20744, callback20924, struct size22644.
  These differ from OEM21344/21345/21540. Named fields avoid that mismatch.
- 65,536 pairs of byte values,196,609 checks: exact reads and no writes.
- 65,536 high16 variants for each WLAN getter,131,072 checks: correct masking
  and callback forwarding. Address/undefined-behavior sanitizers enabled.

Next defensible engineering gate: compile the complete seven-entry QDRV/WLAN
pair twice from the isolated source, qualify imports/exports/metadata plus all
code/data/relocations and the573-entry private table, then update the isolated
repository's source-manifest and build manifest. Physical gates remain separate.
