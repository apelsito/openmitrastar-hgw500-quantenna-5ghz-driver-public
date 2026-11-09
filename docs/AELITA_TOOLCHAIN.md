# Toolchain privado para AELITA

El compilador completo ya está como asset del prerelease privado
`aelita-arc-toolchain-15.2.0-10504-20261022` del repositorio Quantenna.
GitHub confirmó su SHA-256 y tamaño tras la subida. Contiene binutils, sysroot,
librerías, permisos, symlinks y licencias. No se limita al ejecutable GCC.

```bash
mkdir -p ~/openmitrastar-deps
gh release download aelita-arc-toolchain-15.2.0-10504-20261022 \
  --repo apelsito/openmitrastar-hgw500-quantenna-5ghz-driver \
  --dir ~/openmitrastar-deps
cd ~/openmitrastar-deps
printf '%s  %s\n' \
  cb37498e50328183732b84d93cba670076888f66868c3c7385119f1e8e45825a \
  arc-snps-linux-uclibc-gcc15.2.0-10504-aelita.tar.xz | sha256sum -c -
tar -xJf arc-snps-linux-uclibc-gcc15.2.0-10504-aelita.tar.xz
```

El directorio superior del archivo es `arc-toolchain`. Pasar el prefijo
`~/openmitrastar-deps/arc-toolchain/bin/arc-snps-linux-uclibc-` a los builders.
El archivo tiene59864216 bytes y conserva2777 entradas,2430 archivos regulares,
108 symlinks y112 archivos de licencia. El GCC aprobado tiene SHA-256
`3918a0da3c9f2cfde359f6fed1d0df19e737438dfd2cc60edae63f2dcbf14263`.

El manifiesto de descarga verifica contenido, modos y destinos de enlaces.
Las fuentes completas de construcción de GCC/binutils/libc no están embebidas
en este carrier ejecutable; se conservan la configuración ct-ng y las licencias.
No se ha observado extracción o ejecución en AELITA. El prerelease contiene
herramientas de desarrollo, sin firmware de router ni datos privados de unidad.
