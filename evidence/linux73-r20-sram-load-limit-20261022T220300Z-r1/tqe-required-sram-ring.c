static int tqe_descs_alloc(struct tqe_netdev_priv *priv)
{
	int i;
	unsigned int ring_entries = QTN_BUFS_LHOST_TQE_RX_RING;
	union topaz_tqe_cpuif_descr __iomem *bus_descs;

	#ifdef OPENMITRASTAR_QDRV_V89_PAIR
	ring_entries = w3_rx_ring_entries;
	#endif
	pr_info("OMTQE stage=desc-request count=%u default=%u desc_size=%zu align=%u is_sram=1 bytes=%zu\n",
		ring_entries, QTN_BUFS_LHOST_TQE_RX_RING,
		sizeof(priv->rx.descs[0]),
		TOPAZ_TQE_CPUIF_RXDESC_ALIGN,
		(size_t)ring_entries * sizeof(priv->rx.descs[0]));
	if (ALIGNED_DMA_DESC_ALLOC(&priv->rx, ring_entries,
			TOPAZ_TQE_CPUIF_RXDESC_ALIGN, 1)) {
		pr_err("OMTQE stage=desc-failed count=%u bytes=%zu\n",
			ring_entries,
			(size_t)ring_entries * sizeof(priv->rx.descs[0]));
		return -ENOMEM;
	}
	pr_info("OMTQE stage=desc-allocated virt=%px mapped=%px bus=%#lx count=%u\n",
		(void *)priv->rx.aligned_vdescs, priv->rx.descs,
		priv->rx.descs_dma_addr, priv->rx.desc_count);

	bus_descs = (void *)priv->rx.descs_dma_addr;
	for (i = 0; i < priv->rx.desc_count; i++) {
		priv->rx.descs[i].data.next =
			&bus_descs[(i + 1) % priv->rx.desc_count];
	}

	printk(KERN_INFO "%s: %u tqe_rx_descriptors at kern uncached 0x%p bus 0x%p\n",
			__FUNCTION__, priv->rx.desc_count, priv->rx.descs, bus_descs);

	topaz_tqe_cpuif_setup_ring((void *)priv->rx.descs_dma_addr, priv->rx.desc_count);

	return 0;
}
