# SRAM: carriers seleccionados y propuesta de reserva

Estado: ledgeroffline de firmware/kernel; reserva NO_GO, deshabilitada.

Intervalo propuesto offset[12000,20000) (host88012000–88020000, bus80012000–80020000, uncached60012000–60020000). Solape dePT_LOADselectedfirmware/kernel y regioneshostfijas:0bytes. Solapeuniversalruntime:UNKNOWN.

MuC seleccionado71dcd...0fe4 carga SRAMf000–109c0,20000–2fe70,30000–39660,3a3c0–3ede0 y BSS3efc0–3ffc0. AuCnomuc10018...85f6 carga/BSS40000–79c00. DSP87c882...79fd no cargaSRAM por elcamino actual(DSPXYskipped). FWINFOffff0000 es metadata, no memoria de payload.

Kernelv6 sóloPT_LOAD enDRAM; no__sram_*symbols. Las secciones.sram de módulos se colocan conexecmemRAMnormal, no en bancosSRAMfijos. HBMpointerpools0–e000/FWTreservedhastaF000/MuCextrahasta11000 quedanfuera delintervalo.

Conflicto deperfil real:AuCnomu ocupa70000–79c00, solapando la regiónqmatrixGPL70000–80000. El starterreference seleccionafw_no_mu1, pero aúnno demuestra queMuC/DSP carezcan deescriturasdinámicas ahí. Esto no solapa elheapcandidato, pero impide afirmar mapauniversal completo.

Antes de activar: seleccióncarrierhash/flags fijada en7.3, exclusivo recurso/mappingkernel, bounds detodos escritoresindirectos y lifetimes antehal_reset(SRAM_RESET)/clearsram. No convertirceros estáticos en autorización dememoria.
