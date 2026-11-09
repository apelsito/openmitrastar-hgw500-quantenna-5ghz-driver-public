static int __init tqe_module_init(void)
{
#ifdef OPENMITRASTAR_QDRV_V89_PAIR
	bool target_ok = of_machine_is_compatible("mitrastar,hgw-500bna-qcv2");
	pr_info("OMTQE stage=gate-parse enable=%d fwt_reset_probe=%d target_ok=%u ring_entries=%u default=%u\n",
		w3_switch_tqe_only_enable, w3_fwt_reset_probe_enable, target_ok,
		w3_rx_ring_entries, QTN_BUFS_LHOST_TQE_RX_RING);
	if (w3_switch_tqe_only_enable != 1 || !target_ok) {
		pr_err("OMTQE stage=gate-refused enable=%d target_ok=%u\n",
			w3_switch_tqe_only_enable, target_ok);
		return -EPERM;
	}
	if (w3_rx_ring_entries != QTN_BUFS_LHOST_TQE_RX_RING &&
		w3_rx_ring_entries != 256) {
		pr_err("OMTQE stage=ring-profile-reject count=%u allowed=%u,256\n",
			w3_rx_ring_entries, QTN_BUFS_LHOST_TQE_RX_RING);
		return -EINVAL;
	}
	if (!is_power_of_2(w3_rx_ring_entries) ||
		w3_rx_ring_entries > U16_MAX) {
		pr_err("OMTQE stage=ring-profile-reject count=%u reason=bounds\n",
			w3_rx_ring_entries);
		return -EINVAL;
	}
	pr_info("OMTQE stage=ring-profile count=%u default=%u compact=%u bytes=%zu target=mitrastar,hgw-500bna-qcv2\n",
		w3_rx_ring_entries, QTN_BUFS_LHOST_TQE_RX_RING,
		w3_rx_ring_entries != QTN_BUFS_LHOST_TQE_RX_RING,
		(size_t)w3_rx_ring_entries * sizeof(union topaz_tqe_cpuif_descr));
	pr_info("OMTQE stage=gate-pass target=mitrastar,hgw-500bna-qcv2\n");
#endif
	tqe_netdev = tqe_netdev_init();

	return tqe_netdev ? 0 : -EFAULT;
}
