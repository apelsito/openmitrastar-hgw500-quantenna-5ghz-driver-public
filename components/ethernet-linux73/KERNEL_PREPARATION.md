# Kernel completo de fuentes para AELITA

`prepare_kernel.py` crea un árbol nuevo desde el commit upstream fijado,
el port Realtek/Topaz y el overlay Quantenna elegido. No copia kernel compilado,
objetos, initramfs o DT personalizado. También acepta el archivo upstream
Linux 7.3 ya adquirido, SHA-256
`87363c24f10e06321eb362765521094e94471f6bff8bce2d6b3e40f39f004953`.

La comparación del contexto v6 con ese upstream comprobó 91.941 archivos fuente
regulares, 91.912 intactos, 29 modificados y seis añadidos de placa. Los 18
archivos públicos de placa están conservados en `kernel-preparation-v1/board-overlay`:
ARC Kconfig/Makefile, MMUv2/cache/tlb, IRQ, UART, Topaz BASIC y PCIe. El resto se
obtiene del submódulo Realtek y del paquete Quantenna seleccionado. La fuente
v6 utilizaba la cabecera Topaz anterior; `--current-topaz-header` selecciona
explícitamente la cabecera vigente MAC-init/TXdiag sin ocultar esa diferencia.

Preparación privada desde el clon Linux:

```bash
python3 components/ethernet-linux73/prepare_kernel.py \
  --upstream "$PWD/vendor/linux-7.3-upstream" \
  --realtek "$PWD/components/realtek-hgw500" \
  --driver-source "$PWD/src/$(cat src/CURRENT)" \
  --current-topaz-header \
  --output ~/builds/aelita73-source-a \
  --plan-only
```

Después usar `--prepare-only`: crea `kernel-source`, `kernel-context/.config`
y `kernel-source-pins.json`. La configuración está sanitizada, incluye
`CONFIG_QTN_TOPAZ_ETH73=y` y carece de initramfs, claves de firma y selección
del DT incorporado privado. `--build-kernel-only`, junto con
`--toolchain-prefix ~/arc-toolchain/bin/arc-snps-linux-uclibc-`, ejecuta
`olddefconfig`, `modules_prepare` y kernel/módulos base en el contexto nuevo.
Comprueba antes de compilar que `olddefconfig` mantiene vacío el nombre del DT
incorporado y el initramfs. ARC selecciona siempre el framework
`GENERIC_BUILTIN_DTB`; eso no elige un DT: su Makefile deriva la lista únicamente
del nombre, que debe quedar vacío. No genera `Module.symvers` inventado: sólo el build
real lo produce.

El modo de composición ya se ejecutó sin compilación: 95.986 archivos regulares
fijados y verificados, 92 destinos de overlay. Las 35 modificaciones/adiciones
del contexto v6 conservan sus bytes excepto la cabecera Topaz vigente elegida
explícitamente. Véanse `kernel-preparation-v2/PREPARE_RESULT.json` y
`kernel-preparation-v2/current-source-verification/result.json`. El resultado
es fuente preparada, **no un build ni una imagen aceptada**.

Para evitar depender de las rutas históricas, usar los paths de salida nuevos
y sus hashes reales con la herramienta Quantenna actual:

```bash
sha256sum ~/builds/aelita73-base-a/kernel-context/.config \
  ~/builds/aelita73-base-a/kernel-context/Module.symvers \
  ~/builds/aelita73-base-a/kernel-source/Makefile
```

Pasar el kernel y contexto generados a `tools/linux73_build_current_ipv6.py`,
con esos hashes. Esa herramienta vuelve a producir un build nuevo; la
duplicación es explícita y evita mezclar peers anteriores. Después compilar
`components/ethernet-linux73/build.py` contra el último contexto kernel/QDRV,
repetir el proceso en una segunda salida y comparar ABI, imports/exports y
hashes de todos los módulos.

No ejecutar `dtbs` ni preparar una imagen de la unidad con un DT implícito.
Un DT genérico sanitizado y cualquier reset Topaz futuro tienen sus propios
gates. En el contexto v6 `machine_restart` seguía siendo el upstream que llama
a `machine_halt`; no aparece una implementación de reset SoC recuperada entre
sus deltas. Los originales genéricos upstream permanecen en el árbol de fuente;
este paquete nunca copia el DT personalizado usado en una prueba física.

El toolchain completo se entregará como asset de una release privada, separado
de Git, con su manifiesto de archivos/licencias/permisos y symlinks. No descargar
ni instalar un compilador alternativo para compensar una dependencia ausente.
