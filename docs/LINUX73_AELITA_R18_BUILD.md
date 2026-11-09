# Entrega privada actual de AELITA: Linux 7.3 R18

`src/CURRENT` selecciona `linux-7.3-r18`. R13 y R16 se conservan como entregas
históricas con sus manifiestos y comentarios intactos. La herramienta actual
es `tools/linux73_build_current_ipv6.py`; los planes R16 demuestran su validación
de entradas y controles negativos, mientras que cada nuevo paquete se valida
contra su propio `result.json`.

R18 adapta las interfaces reales de NAPI, I²C y GPIO a Linux 7.3. El
[recibo de compilación](../evidence/linux73-full44-72c-compile-receipt-20261023T002500Z-r18/result.json)
observó el 22 de octubre de 2026 a las 21:22 UTC la compilación de 72 unidades C.
Ese build llegó a modpost con exportaciones pendientes del kernel. No produjo
una entrega de módulos enlazados ni una imagen instalable. Los nombres fechados
de las carpetas son identificadores históricos; manda la fecha de observación
del recibo.

El [manifiesto de dependencias](../manifests/linux73-dependencies.json) fija los
repositorios y el compilador. Usar un clon con submódulos en Linux/WSL sobre un
filesystem sensible a mayúsculas. Las rutas Kbuild deben carecer de espacios.
El kernel de entrada requiere el port Topaz y Ethernet/Realtek seleccionado;
el submódulo upstream por sí solo no constituye ese port.

```bash
python3 tools/linux73_build_current_ipv6.py \
  --driver-source ~/openmitrastar/src/linux-7.3-r18 \
  --kernel-source ~/kernel73-port-preparado \
  --kernel-context ~/kernel73-config-base \
  --toolchain-prefix ~/arc-toolchain/bin/arc-snps-linux-uclibc- \
  --gpl-root ~/openmitrastar/vendor/legacy-firmware-source \
  --expected-config-sha256 2990fb628241fa6714dd41af73bd77f4e4df44f9b8eec37feda31c749818ef27 \
  --expected-kernel-makefile-sha256 2eb7b5d5c2ac15a28136607e086f82ebd13f0d3753acc89e463fed49109bc794 \
  --output ~/builds/linux73-r18-a \
  --plan-only
```

Los hashes del ejemplo corresponden al contexto base registrado por R18;
no son hashes universales para cualquier kernel preparado. Conservar el
manifiesto completo del port de placa seleccionado y verificarlo. El viejo
`Module.symvers` sólo sirve como referencia. Sustituir `--plan-only` por `--build`
crea un directorio nuevo y reconstruye kernel, DT y módulos base antes del driver.
El overlay cambia `sk_buff` y `net_device`: Ethernet y todos los módulos deben
compartir el nuevo ABI. La configuración exige IPv6, bridge, WEXT, GPIOLIB, I²C,
NET_SCHED, libaes y verificación de la base regulatoria firmada.

El compilador exacto se provisiona por separado y se comprueba por SHA-256.
Los originales, reconstrucciones, comentarios, herramientas y pruebas están
versionados; firmware de radio, calibración, identidad, claves y PSK permanecen
en el almacén local privado de la unidad. El acceso de la cuenta de AELITA y
la reproducción de un clon nuevo siguen siendo verificaciones separadas.

Antes de una imagen física quedan el enlace completo, segundo build reproducible,
auditoría ABI/imports/exports y regresiones. La asociación de clientes, tráfico,
soak, persistencia y rollback físico son gates posteriores. RBS conserva la
negativa a transmitir sin un proveedor de permiso para la trama exacta durante
CAC; la consulta 467 mantiene su alcance limitado. Ninguna entrada de tabla
establece por sí sola una operación funcional.
