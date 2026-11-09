void heap_sram_free(void *ptr)
{
	if (dram_sram_fallback_active) {
		pr_info("OMSRAM stage=free backend=dram-lab ptr=%px\n", ptr);
		kfree(ptr);
	}
}
