# AELITA: cierre de fuentes y dependencias del 22 de octubre de 2026

Este documento actualiza la revisión inicial `AELITA_COMPLETENESS_20261022.md`.
La rama privada y default branch es `port/linux-7.3`. Seguir siempre `src/CURRENT`
y el manifiesto de dependencias; R18 y R16 son históricos. El paquete actual R20
tiene pruebas de compilación/enlace separadas y límites de carga/SRAM que deben
leerse en su recibo. El árbol de fuente no prueba radio funcional ni instalación.

La falta de Ethernet queda cubierta por `components/ethernet-linux73`: fuentes
MAC-init/TXdiag, INTC Ethernet+PCIe, cabecera Topaz y receta del módulo externo
contra el kernel/ABI nuevo. El módulo previo no puede reutilizarse después de
cambiar `sk_buff` o `net_device`.

La falta de kernel preparado queda cubierta por `prepare_kernel.py` y
`KERNEL_PREPARATION.md`. La composición nueva ya se materializó: **95.986 archivos
fuente regulares fijados y verificados**, 34 de los 35 cambios/adiciones del v6
exactos y sólo la cabecera Topaz vigente elegida explícitamente diferente.
`olddefconfig` se ejecutó sin compilar y mantuvo `QTN_TOPAZ_ETH73=y`, nombre DT
vacío e initramfs vacío. ARC activa `GENERIC_BUILTIN_DTB` por su Kconfig; su lista
DTB depende del nombre vacío y no introduce el DT personalizado. La compilación
de ese contexto sanitizado nuevo y su generación real de `Module.symvers` siguen
sin ejecutarse. No se fabrica ni entrega un symvers vacío para fingir cierre.

El toolchain exacto ya está empaquetado para la release privada, sin binarios en
Git. Archivo `arc-snps-linux-uclibc-gcc15.2.0-10504-aelita.tar.xz`, **59.864.216 bytes**,
SHA-256 `cb37498e50328183732b84d93cba670076888f66868c3c7385119f1e8e45825a`.
Se verificaron 2.777 entradas, 2.430 archivos, 108 symlinks y 112 archivos de
licencias, con permisos/contenidos exactos. `TOOLCHAIN_DELIVERY_MANIFEST.json`
conserva el inventario. Seguir `AELITA_TOOLCHAIN.md` para la adquisición privada;
el acceso y extracción en AELITA deben verificarse allí.

Las adquisiciones reutilizables están presentes: upstream Linux7.3 y tag fijado,
port Realtek/placa privado, GPL original público con comentarios, paquete Quantenna
actual, source overlays de placa, Ethernet vigente y toolchain exacto. No hace
falta reconstruir estas fuentes desde memoria ni descargar otro compilador.

Pendientes técnicos reales: kernel sanitizado/base modules nuevos, DT genérico
revisado para una futura imagen, reset Topaz SoC respecto a v6, límites SRAM/carga,
imports/exports y ABI completos, segundo build donde corresponda, proveedor CAC
por trama RBS, alcance acotado 467, aceptación cliente/tráfico/soak/persistencia y
rollback. Firmware interno, calibración y credenciales permanecen fuera de Git.
No hubo acceso a AELITA o al router en esta revisión.
