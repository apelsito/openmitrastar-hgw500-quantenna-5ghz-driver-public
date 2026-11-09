# R24: cierre offline privado para AELITA

Fuentes179ficheros verificadas; pareja de74unidadesC y22módulos iguales,
sin imports externos pendientes ni SWAPE/LLOCK/SCOND según los recibos de build.
Sólo qdrv.ko y om73_sram.ko cambian respecto deR22.

WLANR24 es byteidénticoalELFR22 yaanalizado: tabla610entradas/14640bytes,
count/RELA correctos y44tuplasOEM únicasexactas. Es una herencia porSHA256,
no una ejecución físicanueva. ABIARC20B DMA/208B skb/1408B net_device verificado.

273casosdelpipeline loader y410provider/corecaseconlease pasan enhost con
OpenSSL/pthread y recursos simulados. Elhash fija identidad completa porCPU;
preflight/rangos ylease se ejecutan antes delprimer mapa. Entry sólotrasend.

La reproducción exige CONFIG_CRYPTO_LIB_SHA256=y(además deIPV6 y los demás
requisitos). Está activo y exportasha256builtin en elkernel fijado porconfig
e709...8efe/symversb3bd...6950. ElmanifestR24 noañadeSHA256 a suarrayrequired;
esta condición es obligatoria en la calificación/herramientaexterna antes de
construir. No se alteró elmanifest o lasfuentes selladas.

Catálogonuevo:179ficheros,7176comentarios/725675bytes exactos. Fuentes intactas.
Doscomentarios de funcionesde loader reemplazadas permanecen en R20/R22históricos.

Elpool permaneceCLOSED/default, sinregistration deboard/reserva real. Ellease
sólocubreloader, noAuCclear_mem/reset/clearsram/escritoresindirectos. La tabla
no incrementaopsfuncionales y466/468 siguen sinprovider de tramas. Viabilidad
de carga normal, bus/cache,5GHzclient/tráfico/persistencia/recovery/release no
aceptadas. Ningún dato privado niacción física deriva deeste cierre.
