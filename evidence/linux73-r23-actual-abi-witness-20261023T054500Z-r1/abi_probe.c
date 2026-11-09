#include <openmitrastar_qdrv_compat.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/stddef.h>
#include <qtn/dmautil.h>
static_assert(offsetof(struct sk_buff, qtn_cb) >= offsetof(struct sk_buff, headers));
static_assert(offsetof(struct sk_buff, recycle_list) + sizeof(((struct sk_buff *)0)->recycle_list) <= offsetof(struct sk_buff, headers) + sizeof(((struct sk_buff *)0)->headers));
static_assert(offsetof(struct net_device, qtn_flags) < offsetof(struct net_device, priv));
static_assert(sizeof(struct qtn_skb_cb) == 12);
static_assert(sizeof(aligned_dma_descs) == 20);
#define ABI_SIZE(name, value) unsigned char name[(value) + 1]
ABI_SIZE(om_abi_skb_size, sizeof(struct sk_buff));
ABI_SIZE(om_abi_skb_qtn_cb, offsetof(struct sk_buff, qtn_cb));
ABI_SIZE(om_abi_skb_orig_dev, offsetof(struct sk_buff, orig_dev));
ABI_SIZE(om_abi_skb_recycle, offsetof(struct sk_buff, recycle_list));
ABI_SIZE(om_abi_ndev_size, sizeof(struct net_device));
ABI_SIZE(om_abi_ndev_qtn_flags, offsetof(struct net_device, qtn_flags));
ABI_SIZE(om_abi_ndev_priv, offsetof(struct net_device, priv));
ABI_SIZE(om_abi_ndevstats_size, sizeof(struct net_device_stats));
ABI_SIZE(om_abi_qtn_cb_size, sizeof(struct qtn_skb_cb));
ABI_SIZE(om_abi_dma_desc_size, sizeof(aligned_dma_descs));
MODULE_LICENSE("GPL");
