# Cargador de firmware: límites R23

R23 añade preflight de todo el carrier antes del primer mapeo: ELF32 little
endian ARC, tabla packed, tamaños disponibles, filesz≤memsz, FWINFO compatible,
traducción de direcciones y rangos SRAM/DRAM/CCM. Los segmentos BSS no necesitan
leer datos y conservan su offset anterior; el trailer DSP existente se acepta.
El entry se publica tras instalar todos los segmentos. Los fallos de mapa se
propagan; si ocurre después del primero puede haber escrituras anteriores.

La [prueba del código real](../evidence/linux73-bounded-loader-fixture-20261023T043700Z-r23/result.json)
pasa1321 casos con los tres carriers exactos y sus mutaciones. Una revisión
adicional pasa24 buffers desalineados: Ehdr y Phdr se copian a objetos locales
y FWINFO usa la estructura packed. Los errores de preflight no abren mapas.

La [compilación limpia](../evidence/linux73-bounded-loader-r23-single-receipt-20261023T044000Z-r1/result.json)
comprueba74 unidades C y22 módulos con imports e instrucciones ARC; el
[testigo de constantes/helpers](../evidence/linux73-r23-actual-map-witness-20261023T044500Z-r1/result.json)
usa las cabeceras reales. Esto aún no prueba la traducción en hardware.
La [pareja de módulos](../evidence/linux73-bounded-loader-r23-pair-receipt-20261023T052000Z-r1/result.json)
también pasa y el [testigo ABI](../evidence/linux73-r23-actual-abi-witness-20261023T054500Z-r1/result.json)
compilado confirma DMA20B,sk_buff208B ynet_device1408B. El delta con R22 cambia
sólo qdrv_fw.o/qdrv.ko; los otros21 módulos conservan sus bytes.

La [revisión del contrato](../evidence/linux73-r23-independent-loader-contract-20261023T045000Z-r1/result.json)
califica este cambio como validación estructural. Sus ventanas amplias de
memoria no verifican autenticidad ni evitan por sí solas escribir en memoria
de otro propietario. La admisión de carriers fijados y reserva SRAM, la
exclusión de todos los escritores y el control de resets siguen pendientes.
El proveedor SRAM de R22 permanece cerrado sin una reserva kernel aceptada.

Código, herramientas y evidencia permanecen privados. No hubo firmware nuevo,
operaciones físicas, carga de módulos, tráfico RF ni incremento de operaciones
funcionales aceptadas. Todos los originales y sus comentarios permanecen en
R20/R22; el código modificado se guarda en una revisión separada.
