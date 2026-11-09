# Completitud para AELITA: revisión del 22 de octubre de 2026

La rama privada `port/linux-7.3` está disponible en el remoto, verificada en
`deb823fc2be94fc320b71290bef8743459ce912e`. La cuenta de GitHub de esta sesión
puede leer los dos repositorios privados; el acceso de la cuenta usada en AELITA
no se ha probado. Esta revisión no abre AELITA ni el router.

Los tres gitlinks existen y sus commits remotos son accesibles:

| Gitlink | Commit | Acceso |
| --- | --- | --- |
| `vendor/linux-7.3-upstream` | `c03c1a14507dfaf8b856c2608f9ac216021fc222` | `torvalds/linux`, público; tag v7.3 verificado |
| `components/realtek-hgw500` | `a924ed874ba925c1b9fb2e42cbb2ef2e59d74e5d` | `apelsito/Realtek-2.4GHz-HGW500`, privado; pull de la sesión verificado |
| `vendor/legacy-firmware-source` | `116a8b0cd80d75e6bc3b8d21646f9c7fa4ce1cf9` | `christianheyl/Quantenna-GPL`, público |

El indicador `-` en el estado local de los submódulos kernel/Realtek significa
que este checkout Windows aún no los tiene inicializados. Los gitlinks sí
están en el commit remoto. Un clon Linux con `--recurse-submodules` debe
materializarlos con la cuenta autorizada, sobre filesystem sensible a mayúsculas.

`src/CURRENT` y `manifests/linux73-dependencies.json` seleccionan R18. La
revisión local comprobó sus **174 archivos de fuente/header/provider/overlay**,
todos sus hashes y **72 unidades C** de Kbuild, sin archivos faltantes. Esto
acredita cobertura de fuente. El recibo R18 llega a modpost con imports
pendientes; no acredita módulos completos enlazados ni ops funcionales.

Faltas para una construcción completa desde el clon:

1. **Toolchain fuera de Git.** Hay un carrier local aprobado reutilizable en
   `work/toolchains/arc-uclibc/arc-arc700-linux-uclibc/` y una transferencia
   anterior a ext4. `components/ethernet-linux73/TOOLCHAIN_EXTERNAL.json` fija
   hashes de GCC, ld, nm, objdump, readelf y cc1. Transferir todo el árbol con
   symlinks, permisos, binutils y sysroot; GCC aislado no basta. No se ha observado
   ese carrier en AELITA.
2. **Kernel Topaz preparado.** El submódulo upstream es la base, no el port de
   placa. El submódulo Realtek aporta BASIC/MMUv2/cache/IRQ, PCIe, Realtek y una
   versión anterior de Ethernet. Hay que componer ese port antes del overlay
   Quantenna y registrar el manifiesto completo del kernel preparado. El ejemplo
   R18 fija config `2990fb628241fa6714dd41af73bd77f4e4df44f9b8eec37feda31c749818ef27`
   y Makefile `2eb7b5d5c2ac15a28136607e086f82ebd13f0d3753acc89e463fed49109bc794`;
   no convierten cualquier árbol upstream en el contexto correcto.
3. **Ethernet vigente.** El paquete nuevo `components/ethernet-linux73` conserva
   los cinco archivos exactos MAC-init-v3/TXdiag, el INTC combinado Ethernet
   19/20 y PCIe 28, y la cabecera Topaz. Su `build.py` compila el EMAC externo
   después del kernel/QDRV nuevo. La receta Quantenna por sí sola no construye
   ese módulo externo. Su compilación con el nuevo ABI sigue UNRUN.
4. **Reset y DT.** El paquete Ethernet excluye DT e identidad de la unidad y no
   contiene la secuencia de reset Topaz más reciente. Revisar y fijar esa fuente
   por separado antes de un firmware. No usar DT personalizado como artefacto
   genérico del clon ni confundir la presencia de reset Ethernet con reset SoC.
5. **Contexto generado.** `.config` y `Module.symvers` son entradas de referencia;
   generar autoconf, utsrelease, modpost, linker script, exports y todos los
   módulos base otra vez. El ABI extendido de `sk_buff`/`net_device` obliga a
   reconstruir kernel, Realtek/mac80211/cfg80211, Quantenna/proveedores y Ethernet
   con el mismo contexto. No copiar módulos previos para cerrar imports.
6. **Cierre del driver.** R18 conserva adapters NAPI/I²C/GPIO, timer/proc/string,
   CCMP/libaes, PM QoS, bridge y proveedores nativos HBM/TQE/FWT. Falta enlazar
   y auditar el conjunto completo, repetir el build y verificar ABI/exports y
   regresiones. RBS conserva proveedor CAC por trama ausente y rechazo; la 467
   mantiene su alcance acotado. Las 44 entradas representadas no son 44 ops
   operativas ni aceptación RF.

Originales, comentarios exactos y herramientas de reconstrucción están en
el repositorio y en el gitlink GPL fijado. Repetir REA contra binarios OEM
requiere los carriers locales con sus hashes; no son necesarios para compilar
la fuente recuperada ni deben incluirse en el clon. Firmware interno de radio,
calibración, claves, PSK y datos de la unidad se provisionan después mediante
gates privados; se excluyen del paquete genérico.

No se ha ejecutado un clon/build en AELITA durante esta revisión. Acceso local,
compilación, enlace reproducible, cliente/traffic, persistencia, rollback y
aceptación de release permanecen gates distintos.
