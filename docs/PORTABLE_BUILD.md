# Portable offline baseline build

Run `tools/build_driver.py` inside Linux or WSL with Python 3. It compiles the
selected module from this repository twice, retaining the executed ARC flags
from `manifests/build-*.json`. The compiler, recovered Linux 5.10.260 source,
F363 generated ABI context and F364 dependency exports are explicit external
inputs. Their used files and repository sources are pinned before and after
compilation; the compiler dependency closure must be pinned.

```sh
python3 tools/build_driver.py --module switch_tqe \
  --kernel /absolute/linux-5.10.260 \
  --context /absolute/f363-generated-kernel-context \
  --toolchain-root /absolute/arc-uclibc \
  --peer-symvers /absolute/f364/dependencies.symvers \
  --parent /absolute/qualified-switch_tqe.ko \
  --output "$PWD/build/new-immutable-run"
```

The toolchain root contains `arc-arc700-linux-uclibc/bin/arc-linux-gcc` and the
ARC GCC 15.2.0 build 10504 runtime. The context supplies `.config`,
`Module.symvers`, generated includes, `scripts/mod/modpost` and
`scripts/module.lds`. Parent modules remain in ignored `local-only/modules`
unless passed explicitly. Proprietary binaries and toolchains are not in Git.

Select `--module qdrv` or `--module wlan` for their retained complete build
manifests. Each output is new and remains under ignored `build/`. Reusing a
previous output directory is rejected. GPL comments and source bytes remain
unaltered. Compile prefix maps preserve the original `__FILE__` strings,
including the original nested include path spelling. Compatibility headers
precede the preserved GPL headers; the current port ABI overlay precedes its
GPL baseline. Source path maps do not alter source files.

Verified here on 22 October 2026: independent repository TQE build produced two
35,552-byte complete modules, SHA-256
`bd5ab1018d293dc6bed655ec2bbc6da64a3969f28a3de578c04fa9c3f4af635f`,
byte-identical to the original. All 52 functions, 464 RELA records, 275 symbols,
11,788 allocated code bytes, data and metadata compare exactly. See
`evidence/portable-tqe-20261022-r5/result.json`. This does not promote the
bounded-admission prototype or establish universal CPUIF arbitration.

WLAN/QDRV portable verification status is recorded separately as those builds
complete. The previous workspace WLAN rebuild established full operational
static equivalence, with a declared source identity `srcversion` difference;
it did not reproduce the original whole-file hash. The portable verifier
requires complete executable/data/symbol/relocation equivalence and rejects
metadata differences beyond the module manifest's declared set.

No builder accesses a router, network, UART, flash, boot environment or runtime.
Successful compilation remains distinct from RF/client traffic, persistence,
recovery and release acceptance.
## Exact executed builder and host limit

The verified TQE run used the exact executed builder retained at
`evidence/portable-tqe-20261022-r5/executed-builder-r5.py`, SHA-256
`b373123401fb20486f2bd4b1c7c864ae59d1ada8742194681bd77a1b8014a5c4`.
It matches the input pin and the Git object at commit `88b12c0`. The current
builder subsequently scopes vendor input discovery to the two driver header
trees and uses lexical path normalization; that update has not been rerun
through a complete portable build.

Portable WLAN preparation encountered an unrelated vendor firmware/buildroot
traversal and `OSError 12: Cannot allocate memory`. The scoped discovery fix
is implemented. Subsequent trivial read-only WSL calls failed with
`Wsl/Service/0x8007274c`, so portable WLAN remains uncompiled. See
`evidence/portable-wlan-host-failure-20261022-r1/result.json`. No WSL reset,
restart, device reboot or other hardware action was performed.
## WLAN portable verification completed

The later scoped builder completed both full WLAN compilations on22October.
629180bytes, SHA2567b0e504e6aee9b763757ea59268b21ebf2f18cdaf6a3c30f52ca43970a6f8dc0.
All959functions/9543RELA/2250symbols/319940codebytes match the baseline;
only declared srcversion differs.6685inputs verified unchanged.
See evidence/portable-wlan-20261022-r3, with exact executed builder retained.
Earlier host blockers remain historical evidence and were not reinterpreted.
