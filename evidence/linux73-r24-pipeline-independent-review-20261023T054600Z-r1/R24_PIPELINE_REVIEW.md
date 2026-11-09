# R24: prueba del pipeline de firmware y lease de escritor

Estado offline. No se cargó firmware ni se registró/reservó SRAM en hardware.

El pipeline real `qdrv_fw_load` pasa273casos con los3carriers seleccionados.
SHA256 se calcula con OpenSSL3.0.13(realcryptohost); request/release, pool,
direcciones, mapas yCCM son fakes explícitos. ASAN/UBSAN pasan. Las mutaciones
de1bit enheader/metadata/payload y encolaDSP, CPU/tamaño/perfil incorrectos y
solape delúltimoBSSMuC se rechazan antes delprimer mapa; entrypermaneceintacto.
Los fallos debegin/end y mapas tampoco publican elentry.

La fixture independiente delprovider ejecuta core/backend reales con locks
pthread ytaskTLS:410casos(incluidos362delcore), tokenscopiados/stale/foreign
rechazados y3waiterswriter/acquire/close. Losrecursos físicos son simulados.
Esto demuestra elcontrol de loscaminos de código, no concurrencia delkernel
o aceptación delbus enla placa.

El lease se mantiene durante hash/preflight/checks/maps/install y elentry se
publica después deend. Siunmapa posteriorfalla puedenquedar escriturasprevias;
no existe rollback atómico. Unend rechazadoretienelease según elcontrato;
no se inventa ununlock automático desdeuna tarea/token inválidos.

Ellease R24cubre elloader, NOhal_reset, clearsram, AuCclear_mem previo al
loader ni todoslosescritores indirectosMuC/DSP/AuC. Lareserva siguecerrada y
elmapacompleto/Qmatrixnomu permanecependiente. Identidad SHA256noequivale a
firmaOEM o autorización deescritores. No aumentanopsfuncionales aceptadas.
