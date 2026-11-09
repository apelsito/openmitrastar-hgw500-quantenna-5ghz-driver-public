# Ethernet actual para el kernel 7.3 de AELITA

> Public source snapshot. [Scope, provenance and verification](docs/PUBLIC_SNAPSHOT.md).

Este paquete conserva las fuentes exactas del candidato MAC-init-v3 con
diagnóstico TX pasivo. Los cinco archivos de `driver/` y el INTC actual están
fijados en `SOURCE_MANIFEST.json`. No contiene módulos, DT, firmware,
calibración, MAC de la unidad o credenciales. No acredita una prueba física.

La fuente Ethernet actual no estaba incluida en el gitlink Realtek
`a924ed874ba925c1b9fb2e42cbb2ef2e59d74e5d`, que conserva Ethernet-v1.
La implementación actual inicializa los doce registros MAC y mantiene las
comprobaciones de DMA stopped y cuarentena. No reutilizar su módulo anterior
después de cambiar el ABI Quantenna de `sk_buff` o `net_device`.

Orden de preparación:

1. Usar Linux upstream `c03c1a14507dfaf8b856c2608f9ac216021fc222` y el
   `basic73.patch` del submódulo Realtek.
2. Aplicar el overlay Realtek/PCIe/Topaz de ese submódulo y después el INTC
   de `board-overlay/arch/arc/kernel/intc-compact.c` de este paquete. Conserva
   simultáneamente las IRQ Ethernet 19/20 y PCIe 28. La variante sin PCIe queda
   sólo como comparación en `reference/ethernet-only-intc-compact.c`.
3. Aplicar `board-overlay/include/linux/soc/qtn/topaz-ethernet.h` al kernel,
   y activar el contrato de placa
   `CONFIG_QTN_TOPAZ_ETH73=y` definido por el port Ethernet. El DT y cualquier
   nueva secuencia reset deben revisarse por separado: no están incluidos aquí.
4. Aplicar el overlay Quantenna elegido y reconstruir kernel, DT sanitizado,
   módulos base, QDRV/WLAN y proveedores. La receta Quantenna necesita ese
   kernel preparado como entrada; un clon upstream no lo sustituye.
5. Compilar este módulo externo contra el **nuevo** contexto generado.

```bash
python3 components/ethernet-linux73/build.py \
  --kernel-source ~/builds/qdrv73-a/kernel-source \
  --kernel-context ~/builds/qdrv73-a/kernel-context \
  --toolchain-prefix ~/arc-toolchain/bin/arc-snps-linux-uclibc- \
  --expected-config-sha256 <sha256-config-nuevo> \
  --expected-symvers-sha256 <sha256-symvers-nuevo> \
  --output ~/builds/emac73-a \
  --plan-only
```

`--plan-only` no crea salidas. `--build` usa una carpeta nueva, copia únicamente
las fuentes del módulo y comprueba imports contra el `Module.symvers` nuevo y
ausencia de SWAPE/LLOCK/SCOND. Repetir en una segunda salida y comparar hashes;
el plan y una build no prueban ABI de todos los peers ni Ethernet físico.

El GCC aprobado se provisiona fuera de Git. El carrier local reutilizable está
en `work/toolchains/arc-uclibc/arc-arc700-linux-uclibc/`; transferir el árbol
completo a filesystem Linux preservando symlinks y permisos. Copiar únicamente
`gcc` no aporta `cc1`, binutils, sysroot ni librerías del compilador. GCC SHA-256:
`3918a0da3c9f2cfde359f6fed1d0df19e737438dfd2cc60edae63f2dcbf14263`.
`TOOLCHAIN_EXTERNAL.json` conserva los hashes verificados del carrier local y
la transferencia anterior a ext4. No hace falta sustituir esa adquisición por
el compilador del host. La disponibilidad en AELITA aún debe comprobarse allí.
