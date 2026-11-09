void *heap_sram_alloc(unsigned long size)
{
	void *ptr;

	pr_info("OMSRAM stage=request bytes=%lu requested=%u active=%u\n",
		size, allow_dram_sram_fallback, dram_sram_fallback_active);
	if (!dram_sram_fallback_active) {
		pr_info("OMSRAM stage=denied reason=no-topaz-sram-allocator\n");
		return NULL;
	}

	ptr = kmalloc(size, GFP_KERNEL);
	pr_info("OMSRAM stage=allocated backend=dram-lab bytes=%lu ptr=%px\n",
		size, ptr);
	return ptr;
}
