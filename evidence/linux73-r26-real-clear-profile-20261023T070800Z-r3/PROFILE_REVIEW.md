# Perfil real de clear/reset R26

El witness seleccionado es r3. Preprocesa el `qdrv_control.c` R26 con el
comando, includes y defines del Kbuild ARC que compiló R24. Los 20 valores
también se compilan como datos con GCC ARC700 y se contrastan con `.rodata`.
No hay acceso al router ni recompilación completa del driver.

| Región | Inicio | Bytes |
| --- | --- | --- |
| MuC stack | 0x8803e7c0 | 4096 |
| MuC SRAM B1 | 0x88020000 | 65536 |
| MuC SRAM B2 | 0x88030000 | 65472 |
| Crumbs | 0x8803ffc0 | 64 |
| MuC DRAM | 0x800e0000 | 3743744 |
| DSP DRAM | 0x80020000 | 786432 |

`HEADER_CORE_DUMP` es 0xdead. El stack termina en 0x8803f7c0 y admite
como máximo 4092 bytes de log tras los dos campos u16. `TOPAZ_RX_ACCELERATE`
vale 1; `TOPAZ_128_NODE_MODE` está definido y selecciona MuC DRAM 0x392000.
El fixture histórico de 36 casos fija un perfil sintético: demuestra las
ramas de control, pero sus límites stack 2048 y magic 0xa5c3 no son estos.

En el kernel real Linux 7.3, `include/linux/slab.h` documenta y define
`ZERO_SIZE_PTR` como `(void *)16` (264-274), y `mm/slub.c` lo devuelve si
size es cero (5408-5409). El controlador R26 acepta ese puntero no nulo,
publica longitud cero y llama `memcpy_fromio` con count cero. La
implementación real `lib/iomem_copy.c` condiciona todos los accesos en count;
por tanto no dereferencia origen o destino para esa longitud. `kfree`
reconoce el sentinel. Esto conserva el comportamiento de longitud cero,
sin probar ejecución física. La fixture debe simular el sentinel sin usar
`free(16)` o una llamada host memcpy que declare inválido el puntero.

La ruta real de memoria mantiene los cinco clear spans del OEM, aunque MuC
DRAM seleccionado es mayor que el perfil sintético histórico. Los checks
del writer deben recibir los valores del witness antes del primer map.
El control de reset por software no acredita pausa de NETSS, TQE o firmware.

r1 queda descartado: el evaluador Python aplicó división flotante a
ROUNDUP y sus valores no representan C. r2 corrigió división entera; r3
añade corroboración con datos producidos por el compilador ARC.
