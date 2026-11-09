# Offline build and analysis

The active target is Linux7.3. Use [the AELITA guide](AELITA_REPOSITORIES.md)
and [current portable build instructions](LINUX73_AELITA_R20_BUILD.md). The preserved
`src/linux-7.3-r20` package adds native providers and real bridge source. Its
paired full builds and ARC ABI/import/instruction audits pass; load/physical gates
remain pending. All kernel/base/Quantenna
modules must be rebuilt after the packet ABI overlay; no5.10 objects may be used.

The remainder of this page is historical Linux5.10 build evidence.

Portable scripts run from this repository. `tools/integration` retains exact
workspace scripts as evidence: their recorded absolute paths are historical
integration requirements, not generic build instructions.

The kernel target is ARC700 Linux 5.10.260, built with the pinned GCC 15.2.0
ARC toolchain and generated F363 ABI context. A host x86 compiler is suitable
only for an explicit fake function fixture. It cannot build an HGW module.

Build inputs stay external: kernel headers, generated configuration/symvers,
module linker script/modpost and the ARC compiler. No multi-gigabyte toolchain
or personalized firmware is committed. Manifests pin the source and command
contract; each run must use a new output directory and retain its result.

```sh
python3 tools/recover_hgw5g_driver_ops.py --root . \
  --manifest manifests/ops-local-inputs.json --output build/ops-new-run
python3 tools/catalog_hgw_firmware_comments.py --root . \
  --source-root vendor/legacy-gpl --source-root src \
  --output build/comments-new-run
```

The local ops manifest references ignored module files. A fresh clone needs
owner-supplied matching artifacts or explicit alternative manifest paths;
the tools verify supplied artifact identity and do not fetch device data.

TQE baseline original SHA256:
`bd5ab1018d293dc6bed655ec2bbc6da64a3969f28a3de578c04fa9c3f4af635f`.
WLAN reconstructed SHA256:
`7b0e504e6aee9b763757ea59268b21ebf2f18cdaf6a3c30f52ca43970a6f8dc0`.
The WLAN original source-version record differs as declared in its evidence.

Module compilation and static equivalence do not authorize hardware loading
or firmware installation. All build output is ignored by Git.

The standalone TQE build from this checkout now passes twice with the exact
original module hash and 6685 pinned inputs. Receipt:
`evidence/portable-tqe-20261022-r5/result.json`.

```sh
python3 tools/build_driver.py --module switch_tqe \
  --kernel /path/to/linux-5.10.260 \
  --context /path/to/generated-f363-kernel-context \
  --toolchain-root /path/to/arc-uclibc \
  --peer-symvers /path/to/dependencies.symvers \
  --output build/tqe-new-run
```

The toolchain root contains `arc-arc700-linux-uclibc/`. WLAN and QDRV use their
own build manifests; portable proof is recorded individually after each run.
The old firmware source submodule is preserved for reconstruction, not silently
used as Linux5.10 headers or a source of current generated ABI configuration.

Portable WLAN has now passed two complete builds from this checkout. The old input scan
entered the newly added complete firmware tree and failed allocation. The
builder now scopes only relevant driver/header trees. This later discovery
revision was executed in `build/portable-wlan-r3`, after earlier WSL process
calls returned transport error0x8007274c. All959functions,9543RELA,2250symbols
and319940codebytes match. Only source identity `srcversion` differs as declared.
Receipt and exact executed builder: `evidence/portable-wlan-20261022-r3/`.
The earlier TQE result remains tied to its retained exact executed builder.

Portable QDRV also passes two complete builds from this checkout:831functions,
14034RELA,1932symbols and312852executable bytes match the original baseline.
Candidate size684660bytes, SHA256
`c8b924ccbf8b85616136fef11fb091e8975c67fb7dc499bddcaef38804c29688`.
Only `srcversion` differs as declared;6685pins verified unchanged.
Receipt and exact builder:`evidence/portable-qdrv-20261022-r1/`.
