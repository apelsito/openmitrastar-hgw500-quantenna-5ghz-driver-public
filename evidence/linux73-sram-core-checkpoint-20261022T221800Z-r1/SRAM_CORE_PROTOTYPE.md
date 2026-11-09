# SRAM OEM: prototipo aislado para Linux 7.3

Estado: núcleo offline, no proveedor del driver. R20 y sus valores por defecto permanecen intactos.

El allocator procede del first-fit/split/coalesce de `arch/arc/kernel/module.c` OEM2.6.35. Los cuerpos originales y el patchde adaptación se conservan. Añade admisión de reserva/mapa y un ledger de128 asignaciones; los callers deben serializar operaciones y empezar con un corea0.

Compilación ARC7.3:1872bytes, import único`memset`, sinSWAPE/LLOCK/SCOND. Prueba32bits:4120casos y417144checks (rechazo por defecto, solapes, alineación, overflow, asignaciónliberación, fragmentación, coalescencia, capacidad y frees desconocidos/repetidos/corruptos).

Mapa del perfil GPL compilado:CPU88000000/bus80000000/uncached60000000,8bancos64KiB. KernelOEMempiezaenoffset12000 y su banco2 terminaen20000; elheap real empiezaenalign8(__sram_end). MuC empiezaen20000, AuC40000–70000 y qmatrix70000–80000. Estos valores no demuestran espacio libre del firmwareactual.

Falta para promover: un offsetSRAMoccupied-end real deLinux7.3 con provenance de linkermap/runtime, un intervaloexclusivamente reservado y una lista completa de ocupación de todos losfirmwares ybuffers. Elkernel7.3noexporta__sram_*; su ausencia no autoriza usar ese espacio.

No se añadieron heap_sramexports ni módulo init, mapaMMIO, parámetro automático o fallbackDRAM. La integración física y la aceptación delbus quedan pendientes.
