#!/usr/bin/env bash
# Private offline AELITA kernel/QDRV/EMAC orchestration. Never loads hardware.
set -euo pipefail

MODE="${1:---plan-only}"
if [[ "$MODE" != "--plan-only" && "$MODE" != "--build" ]]; then
  printf 'Usage: %s [--plan-only|--build]\n' "$0" >&2
  exit 2
fi
REPO=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
DEPS="${AELITA_DEPS:-$HOME/openmitrastar-deps}"
RUN="${AELITA_RUN:-aelita73-a}"
BUILD_ROOT="${AELITA_BUILD_ROOT:-$HOME/builds}"
BASE="$BUILD_ROOT/$RUN-base"
OUT="$BUILD_ROOT/$RUN-qdrv"
EMAC="$BUILD_ROOT/$RUN-emac"
TC="$DEPS/arc-toolchain/bin/arc-snps-linux-uclibc-"
DRIVER="$REPO/src/$(cat "$REPO/src/CURRENT")"

# Download/extract the pinned private toolchain first, following AELITA_TOOLCHAIN.
# This script only uses present files and performs no network acquisition.
test -f "${TC}gcc"
printf '%s  %s\n' \
  3918a0da3c9f2cfde359f6fed1d0df19e737438dfd2cc60edae63f2dcbf14263 \
  "${TC}gcc" | sha256sum -c -

if [[ "$MODE" == "--build" && ! -e "$BASE" ]]; then
  python3 "$REPO/components/ethernet-linux73/prepare_kernel.py" \
    --upstream "$REPO/vendor/linux-7.3-upstream" \
    --realtek "$REPO/components/realtek-hgw500" \
    --driver-source "$DRIVER" --current-topaz-header \
    --toolchain-prefix "$TC" --output "$BASE" --build-kernel-only
fi
test -f "$BASE/result.json"
test -f "$BASE/kernel-context/Module.symvers"
test -f "$BASE/kernel-source-pins.json"
EXPECTED_SOURCE=$(python3 - "$BASE/result.json" <<'PY'
import json,sys
r=json.load(open(sys.argv[1]))
assert r['compiled_kernel'] and r['new_Module_symvers'], 'Build the base kernel first; do not fabricate symvers'
print(r['kernel_source_pins_sha256'])
PY
)
printf '%s  %s\n' "$EXPECTED_SOURCE" "$BASE/kernel-source-pins.json" | sha256sum -c -
REF_HASH=$(sha256sum "$BASE/kernel-context/Module.symvers" | cut -d' ' -f1)
CFG_HASH=$(sha256sum "$BASE/kernel-context/.config" | cut -d' ' -f1)
MK_HASH=$(sha256sum "$BASE/kernel-source/Makefile" | cut -d' ' -f1)

python3 "$REPO/tools/linux73_build_aelita_verified.py" \
  --kernel-source-pins "$BASE/kernel-source-pins.json" \
  --expected-kernel-source-pins-sha256 "$EXPECTED_SOURCE" \
  --expected-reference-symvers-sha256 "$REF_HASH" \
  --driver-source "$DRIVER" --kernel-source "$BASE/kernel-source" \
  --kernel-context "$BASE/kernel-context" \
  --reference-symvers "$BASE/kernel-context/Module.symvers" \
  --toolchain-prefix "$TC" --gpl-root "$REPO/vendor/legacy-firmware-source" \
  --expected-config-sha256 "$CFG_HASH" --expected-kernel-makefile-sha256 "$MK_HASH" \
  --output "$OUT" "$MODE"

if [[ "$MODE" == "--build" ]]; then
  EMAC_CFG=$(sha256sum "$OUT/kernel-context/.config" | cut -d' ' -f1)
  EMAC_SYM=$(sha256sum "$OUT/kernel-context/Module.symvers" | cut -d' ' -f1)
  python3 "$REPO/components/ethernet-linux73/build.py" \
    --kernel-source "$OUT/kernel-source" --kernel-context "$OUT/kernel-context" \
    --toolchain-prefix "$TC" --expected-config-sha256 "$EMAC_CFG" \
    --expected-symvers-sha256 "$EMAC_SYM" --output "$EMAC" --build
fi
printf 'Completed %s. No module loading, image installation or device access.\n' "$MODE"
