static void *qdrv_sch_alloc_fast_data(size_t sz)
{
	void *ret = NULL;

#ifdef CONFIG_ARCH_RUBY_NUMA
	ret = heap_sram_alloc(sz);
#endif
	if (!ret) {
		ret = kmalloc(sz, GFP_KERNEL);
	}
	if (ret) {
		memset(ret, 0, sz);
	}

	return ret;
}
