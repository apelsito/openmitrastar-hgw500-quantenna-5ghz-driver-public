# R22/R23: revisión privada del driver Linux7.3 para AELITA

R22 es una base offline reproducible. Su tabla ELF real contiene610 entradas
(14640bytes), con iw_handler_def/count/RELA correctos. Las44 tuplas OEM
cmd/set/get/nombre son exactas y únicas. Esto no aumenta operaciones funcionales
aceptadas: los adaptadores466/468 siguen sin proveedor real de grants/tramas.

La rama DMA SRAM utiliza el pool tipado: alinea la posición física y publica su
bus/map correctos. El key privado de cada asignación evita liberar el winner en
una carrera de preparación. Una negativa al free conserva pool/key en cuarentena;
no hay recuperación automática ni reintento mediante el descriptor público.
61casos de controlDMA32bits y362casos de proveedor con recursos/mapas falsos son
evidencia de código. La reserva real no está registrada y los defaults cierran
la admisión. Layout público20bytes; no prueba de bus/hardware.

R23 añade un preflight del formato empaquetado de los tres carriers seleccionados.
Valida header/phdr/payload, filesz<=memsz y direcciones antes del primer mapa,
y sólo publica el entry tras completar todos los segmentos. 1321casos con
mutaciones y24buffersdesalineados pasan con las funciones reales y helpers de
mapa/CCM falsos. El DSP contiene28bytes finales de etiqueta del linker válidos;
los offsets BSS pueden referirse a payloadanterior. No aplicar las reglas de
congruencia de ELFgenérico a estos carriers empaquetados.

El preflight R23 es estructural. Sus rangos generales DDR/SRAM/CCM no garantizan
autenticidad ni exclusión de escritores en el heap. Fijar hash/CPU/segmentos y
contrato de vida de los carriers es un gate distinto. Fallar el segundo mapa
puede dejar escritura del primero; F355 mantiene la vida parcial y el entry no
se publica. No hay atomicidad del bus ni rollback por esta comprobación.

El catálogo nuevo R22 contiene7165comentarios/724025bytes exactos/178ficheros.
Las fuentes se preservaron. La entrega privada incluye el ledger y la propuesta
SRAM deshabilitada; ningún permiso de hardware o dato privado se deriva de ellos.

Esta revisión no ejecutó MMIO, loader, reset, RF, TX, SSH ni firmware en la placa.
La viabilidad de carga normal, registro exclusivoSRAM, funcionamiento5GHz y
aceptación de release siguen pendientes de sus contratos y gates físicos.
