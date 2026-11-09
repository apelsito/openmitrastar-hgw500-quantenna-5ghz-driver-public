# R23: perfiles de firmware y admisión de escritores SRAM

Propuesta sin implementar. El pool permanece cerrado por defecto; no hay
registro, reserva, mapa ni escritura física nuevos.

El kernel actual tiene CONFIG_CRYPTO_LIB_SHA256=y y exporta sha256 desde
vmlinux. API real: `<crypto/sha2.h>`, `sha256(data, len, digest)`. Fijar CPU,
longitud y digest del carrier completo, incluida la colaDSP de28bytes; no basta
el nombre o FWINFO. Los perfiles exactos de MuC/AuCnomu/DSP y cada segmento
traducido están en carrier-writer-contract.json. Son hashes de identidad,
no firmas OEM ni demostración de autorización de todos los escritores.

Orden obligatorio propuesto: request_firmware → snapshotbytesinmutables →
lease de vida compartido con elpool → SHA256/rol/tamaño → preflight completo →
plan de destinos con memsz/BSS → exclusión del intervaloreservado → maps/writes.
Ese mismo lease debe impedir acquire/close/reset/clear concurrentes. Hash y
rangos antesdellease dejarían una carrera con acquire. Elpool actual carece de
API writerbegin/end; un getter dearena o boolallowed no sustituye ese contrato.

La admisión bootstrap debe fijar los3carriers y fw_no_mu=1 antesde adquirirelpool.
Incluso con segmentosdisjuntos, sólo un mapacompleto de escritoresindirectos
puede permitir firmwareactivo juntoalheap. ElAuCnomu ocupa70000–79c00, que se
solapa con qmatrixGPL70000–80000; no demuestra queMuC/DSP dejen de usarla.

La exclusión usa direcciones físicasnormalizadas y longitudesmemsz, noaliases
virtuales ni sólofilesz. [12000,20000) corresponde a physical88012000–88020000.
FWINFOffff0000 se excluye como metadata; DSPXY permanece skip acotado. Elentry
sólo se publica despuésde todaslasinstalaciones; fallo conservaF355 parcial.

El catálogo R23 conserva178ficheros,7167comentarios/724512bytes. Doscomentarios
de funcionesreemplazadas noestánliteralmenteenR23: permanecen en los históricos
R20/R22 inalterados. La nuevaextracción eslexicalexacta, no reintroduce texto ni
modifica ninguna fuente. No declarar que todosloscomentariosoriginales siguen
presentes en elarchivoR23.

Gate pendiente: identidad de unkernelactual exacto, mapacompleto deocupación,
reservaexclusiva real y leasekernel que coordinepool/reset/firmware. No se
implementó eladmission ni se tocóhardware, calib, claves, flash, TX o Ioctl26.
