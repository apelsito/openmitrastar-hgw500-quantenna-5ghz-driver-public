# Comentarios originales del firmware HGW500

Fuentes conservados sin cambios. Los offsets son bytes originales, cero-based, con fin exclusivo. `comments.raw.bin` es la representación exacta; esta vista decodificada no sustituye a los bytes.


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.c:1

Bytes `0..305`; SHA256 `85e0c9946132a6889b244f021a6ff4c43b3286d7c7955eae4aaad47d40f2a77d`; decodificación `utf-8`.

```text
/* Exact first-match semantics from OEM FUN_ram_00012f9c, using typed
 * fields rather than OEM raw offsets. Invalid bounds refuse before dereference.
 * Template selection is OEM FUN_ram_00012f18 for bands 0 and 1; out-of-range
 * band values fail explicitly instead of treating any nonzero as band5.
 */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.c:55

Bytes `2261..2478`; SHA256 `6897c979ed351011b9531517730298837eaf942fd1316b71c722645f00dfd65f`; decodificación `utf-8`.

```text
/* Recovered OEM 0x131a8: queue7, BW40 band24 uses ABOVE3/BELOW4.
 * OEM full queue restarts at0; this helper refuses instead of overwriting.
 * Invalid bandwidth leaves both queue and current_bandwidth unchanged.
 */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.c:79

Bytes `3298..3363`; SHA256 `242b9f99ec0f1e13b077b3f3c72a29f9dc250997ba6e840fa6916e84360020d9`; decodificación `utf-8`.

```text
/* Require one populated prefix followed by unused zero slots. */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.c:88

Bytes `3585..3742`; SHA256 `6adc3502dec0984979658b0073a0ba2387e8ab6c73b9872842b978e2d4a1b6f2`; decodificación `utf-8`.

```text
/* Exact nine-word defaults selected by OEM 0x13314. Channel-policy permissions
 * stay with the caller; constant template availability does not allow TX. */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.c:109

Bytes `4598..4756`; SHA256 `82fb499644b7749098354a79c0c48f82f1b4532eb330780253d3a54f10548fef`; decodificación `utf-8`.

```text
/* OEM helpers 0x13514/0x13598. Use signed 64-bit intermediates and refuse an
 * unrepresentable edge; the recovered static templates remain within bounds. */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.c:126

Bytes `5502..5811`; SHA256 `aa330c5245d664edc2c63060d868fa46985aee2eb8c67a22f37ec78bba0f5bca`; decodificación `utf-8`.

```text
/* Exact 88-byte ARC hardware-loop body0x1488e..0x148e6. Ghidra omits this
 * non-procedure loop; original text bytes and ARC objdump supply the evidence.
 * ARC mpy/add/sub use low32 bits; preserve that wrap using unsigned arithmetic.
 * The final subtract is unconditional, even when entry->dfs is zero.
 */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.c:150

Bytes `6589..6666`; SHA256 `421caeaa27c059b431d54c5ea4ad1d85d41a6fc227ca00bf9df953c6a2f013a2`; decodificación `utf-8`.

```text
/* Exact OEM threshold0x418937: preserve divide-first truncation above it. */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.c:186

Bytes `8395..8469`; SHA256 `7b8288dfd2723086c6707529daa80f80f7b6529d88fcd7fccb1726db3562ee5f`; decodificación `utf-8`.

```text
/* Order by the ARC signed branch without implementation-defined casts. */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_recovered.h:1

Bytes `0..223`; SHA256 `f50a045bc25b7159367d4e7df81f48afa7e84ea1c9e1ef21b5c7335f1d836c52`; decodificación `utf-8`.

```text
/* Derived from exact OEM constant tables and REA 0x12f18/0x12f9c.
 * Source reconstruction only: no scan, ranking or regulatory acceptance.
 * Templates preserve OEM oddities byte for byte. No table grants channel use.
 */
```


## chanset461-scan-select-source-20261022T175500Z-r5/openmitrastar_chanset_tables.h:1

Bytes `0..58`; SHA256 `05f349d8975b59754dff8d26af1f4836c96b35c3da49ffbdfb8ad298721e39c5`; decodificación `utf-8`.

```text
/* Exact OEM constant 80-byte records; no runtime data. */
```
