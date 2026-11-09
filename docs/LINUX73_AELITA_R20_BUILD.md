# Construcción privada actual de AELITA: R20

`src/CURRENT` y el manifiesto seleccionan `src/linux-7.3-r24`. La receta R20
se conserva como base: R24 añade el pool SRAM tipado, preflight de firmware,
hashes exactos y un lease de escritura. Es obligatorio CRYPTO_LIB_SHA256=y,
ya incluido en el template AELITA y en el contexto compilado. La pareja actual
pasa con74 unidades C y22 módulos nativos en dos builds idénticos. Usar
`tools/linux73_build_aelita_verified.py` con el árbol sanitizado generado desde
los repositorios y sus hashes completos. Seguir la
[guía actual de AELITA](AELITA_SANITIZED_KERNEL_BUILD_20261022.md), que mantiene
vacíos initramfs y nombre DT. La herramienta anterior y la
[guía R18](LINUX73_AELITA_R18_BUILD.md) son referencias históricas. R13, R16, R18
y R19 se conservan intactas, junto a R20/R22. Ver
[SRAM R22](LINUX73_SRAM_TYPED_R22.md) y [cargador R23](LINUX73_LOADER_BOUNDS_R23.md).
El [contrato R24](LINUX73_CARRIER_WRITER_R24.md) añade límites actuales y pruebas.

R20 añade owner y desmontaje de GPIO y valida tamaño, alineación, overflow,
asignación y propiedad del backend DMA. La
[revisión del ELF y pruebas](../evidence/linux73-r20-elf44-comment-review-20261022T220000Z-r1/result.json)
confirma 610 registros reales de 24 bytes y las 44 entradas OEM con cmd,
set/get y nombres exactos. El alias getter468 está corregido. Dos retornos
OEM no soportados se conservan; no son nuevas funciones operativas.

El [recibo del primer build](../evidence/linux73-full44-native-stack-v8-receipt-20261023T012000Z-r20/result.json)
registra 72 unidades C,21 módulos nativos y26 módulos base contra el kernel
7.3 reconstruido. La pareja reproducible, el ABI de todos los peers y el
módulo Ethernet externo se verifican por separado. Ya pasan la
[pareja completa](../evidence/linux73-full44-complete-pair-receipt-20261023T020500Z-r20/result.json)
y [auditoría ARC](../evidence/linux73-r20-pair-abi-audit-20261023T020000Z-r1/result.json):
dos kernels nuevos con26 módulos base,21 nativos y Ethernet por build,72
unidades C idénticas, imports resueltos y ausencia de SWAPE/LLOCK/SCOND en
kernel y48 módulos. `sk_buff`208B,`net_device`1408B y descriptor DMA20B coinciden.
Enlace, registros y fixtures
del código real no acreditan tráfico RF ni una imagen instalable.

Ethernet se reconstruye con `components/ethernet-linux73/build.py` contra el
nuevo kernel/contexto. Mantener IPv6, bridge, GPIO, I²C, WEXT, NET_SCHED, libaes,
CPU750D, MMUv2/8KiB, UP y regulatory.db firmado. El overlay de paquetes obliga
a reconstruir todos los módulos con el mismo ABI.

RBS466/468 sigue sin proveedor de permiso para la trama exacta durante CAC,
por lo que rechaza esa transmisión. La consulta467, VOPT y los consumidores
MDIE mantienen sus límites. No hay aceptación de carga normal, asociación,
tráfico, soak, persistencia ni rollback físico de R20.

La [revisión de carga SRAM](../evidence/linux73-r20-sram-load-limit-20261022T220300Z-r1/AELITA_LOAD_LIMITS.md)
identifica dos negativas deliberadas en los valores por defecto: el gate TQE
está deshabilitado y su ring requiere SRAM cuyo allocator OEM aún no está
portado. Habilitar el gate por sí solo no resuelve el segundo requisito. El
fallback DRAM manual es de laboratorio, no prueba una solución de SRAM. No
promover este build como driver listo para cargar hasta portar y validar el
backend con reserva y límites de banco reales. La siguiente pieza está recuperada
en `candidates/linux73-sram-core-r1`: first-fit/split/coalescencia OEM, validación
de reserva y ledger de propiedad. Su
[checkpoint](../evidence/linux73-sram-core-checkpoint-20261022T221800Z-r1/result.json)
compila para ARC7.3 y pasa4120 casos de32 bits. No añade exports ni habilita el
heap de R20: falta ocupación SRAM real, mapa de todos sus usuarios y reserva
exclusiva. El mapa GPL por sí solo no acredita memoria disponible. El
[ledger de carriers seleccionados](../evidence/linux73-selected5g-sram-occupancy-20261022T223000Z-r1/SRAM_OCCUPANCY_AND_RESERVATION.md)
encuentra0 bytes de solape estático en el intervalo propuesto, pero no acota
escritores indirectos y reset. AuCnomu además solapa el qmatrix del perfil GPL
fuera de ese intervalo. La propuesta de reserva sigue deshabilitada y NO_GO.

Los [comentarios de R20](COMMENTS.md) tienen catálogo propio:174 archivos,
7140 comentarios y721223 bytes exactos. Código y evidencia siguen privados;
[toolchain](AELITA_TOOLCHAIN.md) se descarga como asset privado y los datos de
la unidad se provisionan fuera de Git con sus contratos. Verificar por separado
acceso y reconstrucción desde AELITA.
