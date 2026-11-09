# AELITA: kernel y módulos sanitizados sin target dtbs

Para el árbol sanitizado generado por `components/ethernet-linux73/prepare_kernel.py`,
usar **`tools/linux73_build_aelita.py`**, o preferiblemente el nuevo wrapper
`tools/linux73_build_aelita_verified.py` que valida todos los hashes del árbol.
La herramienta histórica `linux73_build_current_ipv6.py` conserva su comando
`dtbs`; no es la entrada de este flujo sanitizado. Ambos precedentes permanecen
intactos para reproducir sus recibos.

El builder nuevo ejecuta `olddefconfig`, `modules_prepare`, `vmlinux modules` y
la construcción de módulos QDRV/proveedores. Antes de `vmlinux` verifica todos
los requisitos de la configuración —incluido Ethernet— y que
`CONFIG_BUILTIN_DTB_NAME=""` y `CONFIG_INITRAMFS_SOURCE=""` permanezcan vacíos.
ARC selecciona el framework `GENERIC_BUILTIN_DTB`; eso no elige un DT. El flujo
sanitizado no pide `dtbs` ni instala una imagen.

El source-only preparado no tiene aún `Module.symvers`. Se puede pasar con
`--reference-symvers` un archivo real de un contexto kernel existente para el
plan; el builder no lo copia ni usa como sustituto del nuevo `Module.symvers`.
El kernel/módulos se reconstruyen antes de los módulos Quantenna. Para una
reproducción independiente en AELITA generar el contexto real con
`prepare_kernel.py --build-kernel-only`, obtener sus hashes y usar ese symvers
como referencia. No fabricar un archivo vacío para superar el plan.

```bash
python3 tools/linux73_build_aelita_verified.py \
  --kernel-source-pins ~/builds/aelita73-prepared/kernel-source-pins.json \
  --expected-kernel-source-pins-sha256 <sha256-manifiesto-fuentes> \
  --expected-reference-symvers-sha256 <sha256-symvers-real> \
  --driver-source "$PWD/src/$(cat src/CURRENT)" \
  --kernel-source ~/builds/aelita73-prepared/kernel-source \
  --kernel-context ~/builds/aelita73-prepared/kernel-context \
  --reference-symvers ~/builds/aelita73-base/kernel-context/Module.symvers \
  --toolchain-prefix ~/arc-toolchain/bin/arc-snps-linux-uclibc- \
  --gpl-root "$PWD/vendor/legacy-firmware-source" \
  --expected-config-sha256 <sha256-config-preparada> \
  --expected-kernel-makefile-sha256 <sha256-Makefile-preparado> \
  --output ~/builds/aelita73-kernel-qdrv-a \
  --plan-only
```

Sustituir `--plan-only` por `--build` crea una salida nueva. El wrapper fija
95.986 archivos de la entrada y verifica sus hashes antes y después de llamar
al builder. También fija el hash del symvers real y del builder. La fuente se
copia para compilar; la entrada no se modifica. Después compilar el EMAC externo
con `components/ethernet-linux73/build.py` contra el contexto kernel de esa última
salida, sin reutilizar un `.ko` previo. Repetir el conjunto en otra salida y
verificar ABI/imports/exports/regresiones y los límites de carga/SRAM.

El plan del builder se ejecutó sobre fuentes, config, compilador y symvers reales:
174 archivos del driver, 72 unidades C, 21 módulos; pasó los hashes y rechazó
DT/initramfs no vacíos y la ausencia de cada requisito antes de `vmlinux`.
Los 95.986 archivos de la fuente preparada usada por el build activo también
coinciden con el manifiesto validado. Estos son checks de entrada y planificación,
no un resultado de compilación, cliente, persistencia o release.

Este documento corrige el conflicto de la guía histórica de preparación sin
editar paquetes o recibos ya sellados. Firmware, DT personalizado, calibración,
claves y PSK siguen fuera del repositorio. Ningún comando de esta guía autoriza
cargar módulos en el router o cambiar hardware.
