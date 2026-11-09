# AELITA: clon privado, toolchain y build sanitizado

Este flujo usa las fuentes de `src/CURRENT`, el kernel preparado en carpeta
nueva y el toolchain completo del prerelease privado. No carga módulos ni
produce una imagen de la unidad. Usar filesystem Linux sensible a mayúsculas,
rutas sin espacios, Python 3, Git, make, GCC host/binutils, bc, bison, flex,
libssl-dev y libelf-dev disponibles para Kbuild.

```bash
git clone --branch port/linux-7.3 --recurse-submodules \
  git@github.com:apelsito/openmitrastar-hgw500-quantenna-5ghz-driver.git \
  ~/openmitrastar
cd ~/openmitrastar
git submodule status --recursive

mkdir -p ~/openmitrastar-deps
gh release download aelita-arc-toolchain-15.2.0-10504-20261022 \
  --repo apelsito/openmitrastar-hgw500-quantenna-5ghz-driver \
  --dir ~/openmitrastar-deps
printf '%s  %s\n' \
  cb37498e50328183732b84d93cba670076888f66868c3c7385119f1e8e45825a \
  ~/openmitrastar-deps/arc-snps-linux-uclibc-gcc15.2.0-10504-aelita.tar.xz \
  | sha256sum -c -
tar -xJf ~/openmitrastar-deps/arc-snps-linux-uclibc-gcc15.2.0-10504-aelita.tar.xz \
  -C ~/openmitrastar-deps
```

Los gitlinks fijan Linux `c03c1a14507dfaf8b856c2608f9ac216021fc222`,
Realtek/placa `a924ed874ba925c1b9fb2e42cbb2ef2e59d74e5d` y GPL Quantenna
`116a8b0cd80d75e6bc3b8d21646f9c7fa4ce1cf9`. La cuenta de AELITA necesita
leer ambos repositorios privados; no guardar sus credenciales en el árbol.

Preparar y construir el contexto kernel/base nuevo. Los paths de salida no
pueden existir previamente:

```bash
TC="$HOME/openmitrastar-deps/arc-toolchain/bin/arc-snps-linux-uclibc-"
DRIVER="$PWD/src/$(cat src/CURRENT)"
BASE="$HOME/builds/aelita73-base-a"
OUT="$HOME/builds/aelita73-kernel-qdrv-a"

python3 components/ethernet-linux73/prepare_kernel.py \
  --upstream "$PWD/vendor/linux-7.3-upstream" \
  --realtek "$PWD/components/realtek-hgw500" \
  --driver-source "$DRIVER" \
  --current-topaz-header \
  --toolchain-prefix "$TC" \
  --output "$BASE" \
  --build-kernel-only
```

Esta receta aplica los deltas públicos de placa/MMUv2/cache/IRQ/PCIe/Realtek y
Quantenna, conserva Ethernet, excluye selección DT/initramfs privados y genera
el `Module.symvers` real. No se piden `dtbs`. La propia receta comprueba que
`olddefconfig` deje vacíos el nombre DT y el initramfs antes de compilar.

Los hashes siguientes se calculan sobre esos archivos efectivos; no requieren
un valor pendiente escrito a mano ni un `Module.symvers` inventado:

```bash
SRC_HASH=$(sha256sum "$BASE/kernel-source-pins.json" | cut -d' ' -f1)
REF_HASH=$(sha256sum "$BASE/kernel-context/Module.symvers" | cut -d' ' -f1)
CFG_HASH=$(sha256sum "$BASE/kernel-context/.config" | cut -d' ' -f1)
MK_HASH=$(sha256sum "$BASE/kernel-source/Makefile" | cut -d' ' -f1)

python3 tools/linux73_build_aelita_verified.py \
  --kernel-source-pins "$BASE/kernel-source-pins.json" \
  --expected-kernel-source-pins-sha256 "$SRC_HASH" \
  --expected-reference-symvers-sha256 "$REF_HASH" \
  --driver-source "$DRIVER" \
  --kernel-source "$BASE/kernel-source" \
  --kernel-context "$BASE/kernel-context" \
  --reference-symvers "$BASE/kernel-context/Module.symvers" \
  --toolchain-prefix "$TC" \
  --gpl-root "$PWD/vendor/legacy-firmware-source" \
  --expected-config-sha256 "$CFG_HASH" \
  --expected-kernel-makefile-sha256 "$MK_HASH" \
  --output "$OUT" \
  --build

EMAC_CFG=$(sha256sum "$OUT/kernel-context/.config" | cut -d' ' -f1)
EMAC_SYM=$(sha256sum "$OUT/kernel-context/Module.symvers" | cut -d' ' -f1)
python3 components/ethernet-linux73/build.py \
  --kernel-source "$OUT/kernel-source" \
  --kernel-context "$OUT/kernel-context" \
  --toolchain-prefix "$TC" \
  --expected-config-sha256 "$EMAC_CFG" \
  --expected-symvers-sha256 "$EMAC_SYM" \
  --output "$HOME/builds/aelita73-emac-a" \
  --build
```

La secuencia está también en `scripts/aelita_rebuild.sh`, sin autoejecución.
Tras clonar y extraer el toolchain privado, `bash scripts/aelita_rebuild.sh --build`
prepara la base si no existe, fija el hash de fuentes registrado por su propio
`result.json`, construye kernel/QDRV y después EMAC. `--plan-only` requiere que
la base y su `Module.symvers` real ya existan; no inicia una compilación para
conseguirlos ni crea una salida. `AELITA_RUN` permite una segunda salida distinta:

```bash
AELITA_RUN=aelita73-b bash scripts/aelita_rebuild.sh --build
```

El kernel/base se reconstruye otra vez dentro de la salida Quantenna; esa
duplicación explícita garantiza que los peers se generen con el ABI seleccionado.
Ningún `.ko` antiguo se reutiliza. El wrapper verifica todos los archivos fuente
antes y después y fija el symvers real y el builder. Para comprobar entradas sin
compilar, cambiar `--build` por `--plan-only` y usar un output todavía inexistente.

Repetir en salidas `-b` independientes y comparar módulos/kernel. Después revisar
exports/imports, ABI, instrucciones ARC, límites de SRAM/carga y regresiones.
Los planes ya probados validaron 174 archivos, 72 C units, 21 módulos y todos los
requisitos DT/initramfs/config, pero no acreditan el resultado del build de esta
guía. RBS conserva el rechazo sin proveedor CAC por trama y la consulta 467 su
alcance acotado. Cliente, tráfico, persistencia, rollback y aceptación del release
siguen siendo gates físicos separados.
