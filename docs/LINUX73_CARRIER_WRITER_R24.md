# R24: identidad de firmware y exclusión del pool durante la carga

R24 fija tamaño y SHA-256 completos para MuC, AuCnomu y DSP seleccionados.
Después de request_firmware, adquiere un lease por tarea y token antes de
calcular el hash, validar estructura y comprobar todos los destinos memsz/BSS.
El lease impide que acquire/close cambien el pool entre la comprobación y la
última copia. Un destino que solapa el pool activo se rechaza incluso si no
quedan asignaciones, porque su free-list vive allí. No se activa el pool.

El kernel real proporciona sha256 mediante CONFIG_CRYPTO_LIB_SHA256=y. Este
requisito se documenta fuera del manifiesto sellado R24, que aún no lo lista
explícitamente; conservarlo en cualquier preparación o revisión futura. El
template AELITA actual lo habilita y los imports del build se resuelven contra
vmlinux. Un hash conocido identifica el carrier; no es una firma OEM.

La [prueba independiente del pipeline](../evidence/linux73-r24-pipeline-independent-review-20261023T054600Z-r1/result.json)
comprueba273 casos con SHA-256 real de OpenSSL en host y los tres carriers
exactos. Mutaciones de bytes/tamaño/CPU/perfil y solape del último BSS se
rechazan antes del primer mapa. El código del provider pasa410 casos con
ASAN/UBSAN y pthreads, incluidos waiters de acquire/close/otro writer y rechazo
de tokens copiados, antiguos o de otra tarea. Los recursos físicos son mocks.

La [pareja completa de módulos](../evidence/linux73-carrier-writer-r24-pair-receipt-20261023T055500Z-r1/result.json)
compila74 unidades C y22 módulos nuevos por pasada, idénticos en bytes. Los
pines antes/después, copias, imports e instrucciones ARC pasan. El
[delta con R23](../evidence/linux73-r23-r24-module-delta-20261023T055000Z-r1/result.json)
cambia sólo loader y provider: los otros20 módulos permanecen idénticos y los
símbolos públicos existentes se conservan. El
[testigo ABI ARC](../evidence/linux73-r24-actual-abi-witness-20261023T060000Z-r1/result.json)
confirma DMA20B,sk_buff208B ynet_device1408B contra el contexto AELITA fijado.
Este recibo no vuelve a compilar ni cualifica una nueva pareja de kernels.

El entry se publica sólo tras instalación y end del lease correctos. Un fallo
de mapa puede dejar segmentos anteriores copiados; no se presenta como rollback
del bus. La cuarentena de un free rechazado tampoco tiene recuperación automática.

El lease cubre este loader. Todavía no cubre AuCclear_mem, hal_reset, clearsram
ni escrituras indirectas de MuC/AuC/DSP. El solape AuCnomu/qmatrix del perfil
GPL permanece sin resolver. La reserva y el registro del pool siguen cerrados
hasta demostrar ocupación, exclusión de escritores y lifetime real.

La tabla de operaciones sigue sin establecer funcionalidad física; RBS466/468
carece de proveedor CAC por trama. El
[ledger MuC](../evidence/rbs-muc-mailbox-fullmap-bounded-audit-20261023T012000Z-r1/HANDOFF.md)
recupera130 mailboxes y distingue callback de ioctl, completion y scheduler.
Varios nombres de símbolos heredados no corresponden a sus cuerpos; se retienen
direcciones, bytes y límites. El consumidor de admisión TX de mailbox1 sigue
sin contrato demostrado. No hubo hardware, firmware instalado,
transmisión, flash ni incremento de operaciones funcionales aceptadas.

El catálogo R24 tiene179 archivos,7176 comentarios y725675 bytes exactos.
Sus documentos/página/bytes y los originales de todas las revisiones anteriores
se conservan separados y privados para AELITA.
