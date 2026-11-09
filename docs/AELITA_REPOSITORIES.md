# Repositorios privados para AELITA

La rama activa de Quantenna es `port/linux-7.3`. El repositorio sigue privado,
por decisión explícita del propietario. No se ha publicado el árbol ni se han
cambiado avisos de licencia. El port Realtek también permanece privado.

Los commits del kernel, del port Realtek/plataforma y del firmware GPL original
quedan fijados en [el manifiesto de dependencias](../manifests/linux73-dependencies.json).
Los tres se incorporan como submódulos. AELITA necesita autenticarse con una
cuenta que pueda leer ambos repositorios privados.

```bash
git clone --branch port/linux-7.3 --recurse-submodules \
  git@github.com:apelsito/openmitrastar-hgw500-quantenna-5ghz-driver.git \
  ~/openmitrastar-hgw500
git -C ~/openmitrastar-hgw500 submodule status --recursive
```

Usar el disco Linux/WSL sensible a mayúsculas. El original GPL contiene nombres
que colisionan en Windows; no cambiar sus bytes para corregir esa limitación.

`src/linux-7.3` conserva la primera revisión completa del port a7.3: QDRV,
net80211, crypto/escáneres, proveedores, overlay de ABI de paquetes y Kbuild.
Sus62unidades C compilan contra el kernel preparado; el modpost de esa revisión
todavía identifica61imports pendientes. Las siguientes revisiones incorporan
los proveedores reales en `src/linux-7.3-r16` y `src/linux-7.3-r18`. R18 queda
preservada como entrega intermedia. R26 queda seleccionada por `src/CURRENT` y
el manifiesto; sus74 unidades C y22 módulos nativos compilan/enlazan en dos
pasadas idénticas. El template/kernel deben conservar CRYPTO_LIB_SHA256=y.
El propietario transfiere la integración al chat de firmware; seguir el
[handoff R26](HANDOFF_LINUX73_R26.md) y mantener separados sus gates físicos.
Esta entrega no es una imagen lista para instalar.

La [guía de construcción actual](AELITA_END_TO_END_SANITIZED_20261022.md) usa argumentos explícitos y
crea salidas nuevas. La
[receta sanitizada](AELITA_SANITIZED_KERNEL_BUILD_20261022.md) y
`components/ethernet-linux73/prepare_kernel.py` permiten componer el kernel
desde el clon y fijar su árbol completo. El port de placa disponible en
`components/realtek-hgw500/kernel/basic-linux-7.3` y
`components/realtek-hgw500/kernel/radio2g-linux-7.3-v1` debe aplicarse al kernel
upstream fijado; un kernel upstream sin esos cambios no reemplaza al contexto
Topaz verificado. Después hay que aplicar el overlay Quantenna y recompilar el
kernel y todos sus módulos, incluido Ethernet, por el cambio de ABI de paquetes.

El [compilador ARC completo](AELITA_TOOLCHAIN.md) se descarga del prerelease
privado con verificación SHA256, symlinks y permisos conservados.
El firmware interno de radio, calibración, identidad del equipo, claves y PSK
se provisionan fuera de Git. El repositorio conserva sus contratos y referencias,
y no copia secretos ni imágenes personalizadas.

Las pruebas de compilación, ABI, imports y reproducción no establecen asociación
5GHz, tráfico bidireccional, persistencia ni rollback físico. La finalidad es una
actualización fiable y mantenible; esas pruebas siguen en la aceptación final.
