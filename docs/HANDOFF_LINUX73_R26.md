# Handoff de Quantenna 5 GHz a la integración del firmware

El propietario acepta cerrar esta tarea con el port compilado para Linux7.3,
sin esperar a funcionamiento físico de5GHz, y pide transferir la incorporación
y las pruebas al chat «Analiza firmware Movistar con REA», ID
`01a163c8-c200-7d40-93ff-e4148d2b86b3` en host local.

Fuente entregada: `src/linux-7.3-r26`, seleccionada por `src/CURRENT` y el
manifiesto de dependencias. El repositorio sigue privado. R24/R25 y las fuentes
originales permanecen preservadas; no sobrescribir resultados antiguos.

## Qué está comprobado

La [pareja R26](../evidence/linux73-clear-reset-r26-pair-receipt-20261023T066500Z-r1/result.json),
SHA256 `eb6d927c378c6a7bc52c1a2ce24b9359f7c4bb9166d496e9a0f72a1698007e75`,
compila74 unidades C y22 módulos nativos desde cero por pasada. Ambos conjuntos
son idénticos, con imports, ISA ARC, copias y pines antes/después verificados.
El kernel se usa como contexto fijado; no se volvió a construir en esa pareja.

Paquete local de desarrollo para el receptor:

```text
/workspace/user-home/Desktop/MitraStar/outputs/rea/linux73-r26-module-handoff-20261023T072000Z-r1/R26-Linux73-22modules-DEVELOPMENT-NOLOAD.tar.xz
Bytes: 463728
SHA256: 3161111682e875288da9dcdb8670f335534eb91059ad61ad0661ddfe3417e228
WSL módulos: /home/rea/linux73-clear-reset-r26-native-pair-r1/a
```

Contiene22 módulos, Module.symvers nativo, manifiesto y README de desarrollo.
Los hashes fueron comprobados al leer el archivo. Está ligado al kernel/ABI
anteriormente descrito; no cargarlo directamente sobre el kernel RAM existente.

[ABI ARC R26](../evidence/linux73-r26-actual-abi-witness-20261023T067000Z-r1/result.json):
DMA20 bytes, sk_buff208 bytes y net_device1408 bytes. Respecto a R24 cambian
cinco objetos y dos módulos; los otros20 módulos son idénticos.

R26 liga clear de AuC e instalación a un mismo lease, después del hash y
preflight de todo el firmware. Normaliza callbacks positivos inesperados a
EPROTO. El crash log usa ioremap/readw/memcpy_fromio. El lease software de reset
se admite antes de F355 y rechaza propietarios SRAM vivos; no reinicializa el heap.

La prueba AuC real pasa46 escenarios con ASAN/UBSAN y SHA256 host real; el
provider R6 pasa468 escenarios acumulados. La prueba de control de36 escenarios
usaba constantes sintéticas; no presentarla como validación del perfil real.
El [perfil real corroborado con GCC ARC](../evidence/linux73-r26-real-clear-profile-20261023T070800Z-r3/result.json)
fija magic0xdead, stack0x8803e7c0/4096, máximo log4092 y MuC DRAM
0x800e0000/0x392000. No se ejecutó un nuevo fixture con esos valores.

El catálogo R26 conserva179 archivos,7179 comentarios y726274 bytes exactos,
con documento/página/stream de bytes. La tabla610/44 entradas OEM conserva
identidad de WLAN con los checkpoints analizados; no equivale a44 capacidades RF.

## Integración requerida por el chat receptor

Aplicar los overlays de R26 al kernel común de la placa y recompilar juntos
kernel, Realtek/mac80211/cfg80211/crypto/bridge, Quantenna y Ethernet. Los cambios
de sk_buff/net_device impiden mezclar estos módulos con el kernel RAM anterior.
Usar la [receta AELITA](AELITA_END_TO_END_SANITIZED_20261022.md) y sus argumentos
explícitos. No instalar el contexto sanitizado sin DT de placa revisado.

Contexto real de las compilaciones entregadas:

```text
WSL distro: REA-Linux, usuario root
Kernel fuente: /home/rea/openmitrastar-aelita-source-fullbuild-20261022T233000Z-r1/kernel-source
Kernel contexto: /home/rea/openmitrastar-aelita-source-fullbuild-20261022T233000Z-r1/kernel-context
Config SHA256: e709aba8ed2b55f3e6debe1480bbea2f1320c4bcd9dc3e9832c185b8e72e8efe
Symvers SHA256: b3bdc2fecfad7321cd5d03454ea47d2341ef43ffc5e3f091de4c78c0d2726950
Toolchain: /home/rea/openmitrastar-linux73-20261022T135100Z/toolchain-arc700/bin/arc-snps-linux-uclibc-
GCC SHA256: 3918a0da3c9f2cfde359f6fed1d0df19e737438dfd2cc60edae63f2dcbf14263
```

R26 exige CRYPTO_LIB_SHA256=y, además de IPv6, WEXT, I2C/GPIO, bridge,
NET_SCHED, signed regdb y la configuración ARC750D/MMUv2/8KiB/UP seleccionada.
No utilizar módulos5.10 ni binarios de análisis REA.

El otro chat integra Doom XZ/cliente y mide la imagen completa. Su conjunto
jugable aislado se midió en1963925 bytes; eso no demuestra por sí solo que
kernel+rootfs+drivers+carriers+servicios entren en menos de7MB. El objetivo de
tamaño se transfiere al empaquetado, sin eliminar silenciosamente componentes.

## Límites que se transfieren

- Pool SRAM sin registro automático; falta reserva real y mapa de escritores.
- Reset0x4180 excluye SRAM, pero el efecto NETSS/TQE y quiescencia global no está
  probado. Ring TQE anterior al start requiere lifecycle válido, sin borrarlo.
- Módulos compilados no significan radio funcionando. Faltan carga, SSID,
  AUTH/ASSOC/AUTHORIZED, tráfico, soak, persistencia y rollback físico.
- RBS466/468 tiene proveedor CAC por trama ausente. Ioctl26 y OCS21 son controles
  globales y no sustituyen el permiso. La ruta AuCnomu queda documentada en sus
  ledgers; decodificación propia AuC y admisión WMAC/CAC siguen incompletas.
- Calibración, firmware interno e identidad privada se provisionan fuera de Git.
- El handoff no autoriza flash, reboot, UART o radio por sí solo. El receptor
  conserva sus autorizaciones propias y los gates exactos de la imagen.

No hubo acceso al HGW ni escrituras de firmware en esta tarea. El cierre es de
port/compilación/documentación y transferencia, con aceptación física separada.
