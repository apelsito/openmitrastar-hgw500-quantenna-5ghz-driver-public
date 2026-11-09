# RBS 466/468: búsqueda offline del proveedor por trama

Resultado: evidencia negativa acotada; **ningún proveedor por trama demostrado**.
No se promocionan ops, se instala proveedor o modifica la fuente R20.

El MuC original seleccionado permanece idéntico:
`71dcd32849f122c09397490914227af6f77e51c34d15615028faa3d0fabb0fe4`.
La ventana de análisis contiene los 963.131 bytes exactos del segmento8 en
0xe0000; ELF derivado `2a9e1e7920d529fe376748be4b06260589d1bfef28ade0850883ae9c423ba27a`.
Es análisis, nunca firmware. AuC seleccionado:
`e1fd397ef56b061af7c91cbd422973dcfcf1382933afa226f8f428dd315f65af`.

La llamada al REA conectado rechazó ARC con `unsupported ELF architecture`.
Se reutilizó el laboratorio ARC aislado ya cualificado, sin readiness, setup,
instalación ni modificación de runtime instalado. Cuatro dossiers nuevos pasan:

- 0x111330..0x11152b, 508 bytes: configuración OCAC de canal, dwell, umbrales,
  estado y offsets de TX-halt. Esto no es un permiso para una trama RBS.
- 0x16fe74..0x16ffd7, 356 bytes: un único argumento booleano actualiza máscaras
  globales 0x250/0x254/0x258/0x25c y estados de cuatro colas. No recibe frame,
  descriptor, generación, propietario o deadline.
- 0x1710d0..0x171127, 88 bytes: wrapper de exclusión para escritura de campos.
- 0x171000..0x1710cf, 208 bytes: read/modify/write de registro en e6040000 y
  metadatos del selector. No tiene concesión ni revocación por descriptor.

La tabla de 66 comandos en0x111a14 se decodifica desde bytes exactos. Comando26
salta a0x111ff8, lee arg1, escribe el global0x18c090 y llama a0x16fe74. Su cero
y no-cero se convierten en apagar/encender global. No se conecta al adaptador.
Los dos dispatchers no estaban creados como procedimientos por Ghidra; se
conservan los NO-GO originales y el ensamblador GNU independiente. No se afirma
equivalencia de los dispatchers completos por esos dossiers fallidos.

La fuente actual distingue mailbox0 ioctl, mailbox1 management y mailbox2+node
para datos. La ruta management publica un `host_txdesc` ordinario; los seis flags
documentados son AMSDU, NAV, retry y return. `IMM_RETURN` describe devolución y
no autoriza TX durante CAC. La configuración OCAC incluye un descriptor QoS-null
para proteger tráfico antes de salir del canal, con un release que rechaza CAC
en curso; no prueba una excepción RBS sobre el canal en CAC.

AuC tiene nueve segmentos packed y ninguna cadena de los términos examinados,
pero su análisis de instrucciones no se ejecutó. No se deduce ausencia de un
contrato oculto de esa falta de cadenas. También siguen sin resolver los aliases
SRAM y los callee fuera de la ventana de código MuC.

Siguiente punto preciso: localizar el consumidor firmware de **mailbox1**, seguir
el decode de descriptor y la puerta del scheduler/estatus antes de publicar a
hardware. El contrato exigido debe vincular identidad de trama, role, canal,
deadline, aceptación/revoke/completion y lifetime frame/node. Hasta obtenerlo,
`openmitrastar_rbsdfs_kernel.inc` conserva `provider=NULL` y rechazo explícito.

No se generó un adaptador nuevo porque el contrato no está claro. Sin hardware,
RF, flasheo, calibración, mtd o modificación de binarios. Todos los jobs finalizaron.
