# AELITA: resultado real de la receta privada

La receta nueva sobre la composición pública de placa y fuente R20 completó
un build limpio de kernel7.3,26 módulos base,21 módulos del driver y Ethernet
externo. El [recibo de build](../evidence/aelita-sanitized-actualbuild-receipt-20261022T235000Z-r1/result.json)
mantiene separada esta composición sanitizada del contexto histórico v6.
Se conservaron initramfs vacío y nombre DT vacío; no se pidió `dtbs`.

La [auditoría ARC](../evidence/aelita-sanitized-actualbuild-receipt-20261022T235000Z-r1/actual-abi-import-audit.json)
verifica48 módulos, imports fuertes resueltos y ausencia de SWAPE/LLOCK/SCOND
en módulos y kernel. Sus layouts coinciden con R20: `sk_buff`208B,
`net_device`1408B, `qtn_skb_cb`12B y descriptor DMA20B.
El [EMAC externo](../evidence/aelita-sanitized-actualbuild-receipt-20261022T235000Z-r1/emac-result.json)
compila contra ese contexto nuevo y resuelve80 imports. La
[copia de fuentes](../evidence/aelita-sanitized-actualbuild-receipt-20261022T235000Z-r1/copied-source-pins.json)
comprueba95986 archivos exactos contra el manifiesto del kernel preparado.

El kernel sanitizado tiene cambios de cabecera Topaz y localversion declarados
respecto a v6. Este build es una ejecución independiente de la receta del
repositorio, sin extracción/ejecución en el equipo AELITA. No se le atribuye la
pareja de hashes del contexto v6; una segunda ejecución de esta composición
se registra como un gate separado.

La receta, fuente seleccionada, overlays y toolchain están guardados en el
repositorio privado y su prerelease. Los datos originales de firmware,
calibración, identidad y claves de unidad se provisionan mediante sus gates
privados. Los comentarios y fuentes originales permanecen preservados.

La carga normal del driver aún está limitada por el gate TQE y el allocator
SRAM no integrado, y RBS carece de permiso por trama durante CAC. El núcleo
SRAM recuperado y el ledger de ocupación son evidencia offline; su reserva
sigue NO_GO. No hubo carga física, asociación, tráfico, persistencia, flash,
rollback ni aceptación de release durante esta tarea.
