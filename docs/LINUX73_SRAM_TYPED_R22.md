# SRAM y DMA: integración R22

R22 incorpora un proveedor Linux7.3 que mantiene una reserva exclusiva por
páginas, un único mapeo uncached y asignaciones por propietario. DMA utiliza
campos separados para dirección física, bus y mapeo. No convierte la dirección
devuelta por ioremap mediante virt_to_bus/virt_to_phys ni crea un segundo mapeo.
La alineación corresponde a la dirección física y del bus. El descriptor
compartido conserva20 bytes.

La [pareja de módulos](../evidence/linux73-typed-sram-dma-r22-pair-receipt-20261023T042500Z-r1/result.json)
compila74 unidades C y22 módulos por lane desde fuentes limpias. Coinciden
bytes, imports e instrucciones ARC; las entradas y copias se comprueban antes
y después. El kernel utilizado es el contexto AELITA ya construido, con ABI
de paquetes sin cambios; este recibo no es una nueva pareja de kernels.

La [revisión independiente](../evidence/linux73-r22-independent-dma-table-review-20261023T044700Z-r3/result.json)
confirma tabla ELF610 y44 tuplas OEM exactas. Pasan61 escenarios de DMA tipado
en32 bits y362 escenarios de recurso/pool con ASAN/UBSAN. Los mocks separan
las direcciones virtual/física/bus; no prueban el bus del HGW.

La clave del pool es el registro privado de cada intento de asignación, no el
descriptor público: si otro intento gana la publicación, se libera el perdedor
sin tocar el ganador. Un free rechazado conserva el registro en cuarentena;
no existe reintento público de limpieza y no se presenta como recuperación
completa. El módulo permanece fijado mientras existe su reserva.

No hay registro automático ni sustitución DRAM. La admisión necesita un caller
kernel con el contrato inmutable de banco, ocupación, carriers y escritores.
La presencia de hashes o campos afirmativos no demuestra ese contrato. La
carga normal, la reserva real y el lifetime ante reset siguen pendientes.

El [ledger de lifetime](../evidence/linux73-sram-lifetime-ledger-20261022T224800Z-r1/SRAM_RESET_LIFETIME.md)
corrobora que hal_reset ARC escribe0x4180 y no incluye SRAM. El ring TQE ya
existe antes de ese reset: reinicializar el heap después perdería su propiedad.
El efecto de NETSS sobre los consumidores/registros TQE sigue sin demostrar.
Los clears fijos no solapan el intervalo propuesto; los escritores indirectos
y las llamadas vacías de detención AuC/DSP no prueban quiesce completo.

El catálogo R22 conserva178 archivos,7165 comentarios y724025 bytes exactos.
Los originales R20 y los prototipos anteriores permanecen en sus directorios
históricos; no se reescriben ni se promociona una función por su nombre.
