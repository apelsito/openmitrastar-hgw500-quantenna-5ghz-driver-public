
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#ifndef ESHUTDOWN
#define ESHUTDOWN 108
#endif
#define QNET_HLRING_ENTRIES 4
#define QTN_HLINK_STATUS_AVAIL 1U
#define spin_lock_irqsave(x,f) ((void)(x),(f)=0)
#define spin_unlock_irqrestore(x,f) ((void)(x),(void)(f))
#define DBGPRINTF_E(...) ((void)0)
#define DBG_LOG_FUNC_TEST(x) 0
#define QDRV_LF_HLINK 1
struct host_ioctl{unsigned ioctl_status,ioctl_dev;};
struct qdrv_wlan{struct{int hl_flowlock;struct host_ioctl hl_ring[4];}tx_if;unsigned unit;};
struct qdrv_vap{struct qdrv_wlan*parent;unsigned devid;};
static void dump_hring(struct qdrv_wlan*x){(void)x;}
static unsigned args_freed,param_freed,skb_freed,ioctl_freed,publication;
struct qtn_beacon_args{int x;};struct vap{int x;};struct skb{int x;};
static void qdrv_hostlink_free_coherent(void*d,unsigned n,void*p,int h){(void)d;(void)n;(void)p;(void)h;args_freed++;}
static void ieee80211_beacon_destroy_param(struct vap*v){(void)v;param_freed++;}
static void dev_kfree_skb_any(struct skb*s){(void)s;skb_freed++;}
static struct host_ioctl *qdrv_alloc_ioctl(struct qdrv_wlan *qw)
{
	int indx;
	struct host_ioctl *ioctl;
	unsigned long flags;

#ifdef QDRV_FEATURE_KILL_MUC
	if (qw->flags_ext & QDRV_WLAN_MUC_KILLED) {
		return NULL;
	}
#endif
	spin_lock_irqsave(&qw->tx_if.hl_flowlock, flags);

	/* Search for an empty IOCTL */
	for (indx=0; indx < QNET_HLRING_ENTRIES; indx++) {
		ioctl = &qw->tx_if.hl_ring[indx];
		if (ioctl->ioctl_status == QTN_HLINK_STATUS_AVAIL)
			break;
	}

	if (indx == QNET_HLRING_ENTRIES) {
		DBGPRINTF_E("Hostlink buffer not available\n");

		if (DBG_LOG_FUNC_TEST(QDRV_LF_HLINK)) {
			dump_hring(qw);
		}

		spin_unlock_irqrestore(&qw->tx_if.hl_flowlock, flags);
		return (NULL);
	}

	ioctl->ioctl_status = 0;

	spin_unlock_irqrestore(&qw->tx_if.hl_flowlock, flags);

	memset(ioctl, 0, sizeof(*ioctl));
	ioctl->ioctl_dev = qw->unit;

	return (ioctl);
}
static void qdrv_free_ioctl(struct host_ioctl *ioctl)
{
	if (ioctl == NULL) {
		return;
	}

	ioctl->ioctl_status = QTN_HLINK_STATUS_AVAIL;
}
struct host_ioctl *vnet_alloc_ioctl(struct qdrv_vap *qv)
{
	struct qdrv_wlan *qw = (struct qdrv_wlan*)qv->parent;
	struct host_ioctl *ioctl;

	ioctl = qdrv_alloc_ioctl(qw);
	if (ioctl != NULL) {
		ioctl->ioctl_dev = qv->devid;
	}

	return (ioctl);
}
static void vnet_free_ioctl(struct host_ioctl*ioctl){if(ioctl){assert(ioctl->ioctl_status!=QTN_HLINK_STATUS_AVAIL);ioctl_freed++;qdrv_free_ioctl(ioctl);}}
static int actual_frame(struct host_ioctl*ioctl,int error){struct qtn_beacon_args a,*bc_args=&a;struct vap v,*vap=&v;struct skb s,*beacon_skb=&s;int args_dma=1;			qdrv_hostlink_free_coherent(NULL, sizeof(*bc_args), bc_args, args_dma);
			vnet_free_ioctl(ioctl);
			ieee80211_beacon_destroy_param(vap);
			dev_kfree_skb_any(beacon_skb);
			return error;}
static int actual_preflight(struct host_ioctl*ioctl,int error){struct qtn_beacon_args a,*bc_args=&a;struct vap v,*vap=&v;struct skb s,*beacon_skb=&s;int args_dma=1;			vnet_free_ioctl(ioctl);
			qdrv_hostlink_free_coherent(NULL, sizeof(*bc_args),
				bc_args, args_dma);
			ieee80211_beacon_destroy_param(vap);
			dev_kfree_skb_any(beacon_skb);
			return error;}
static int actual_encode(struct host_ioctl*ioctl,int error){struct qtn_beacon_args a,*bc_args=&a;struct vap v,*vap=&v;struct skb s,*beacon_skb=&s;int args_dma=1;			vnet_free_ioctl(ioctl);
			qdrv_hostlink_free_coherent(NULL, sizeof(*bc_args),
				bc_args, args_dma);
			ieee80211_beacon_destroy_param(vap);
			dev_kfree_skb_any(beacon_skb);
			return error;}
static int actual_shutdown(struct host_ioctl*ioctl,int error){struct qtn_beacon_args a,*bc_args=&a;struct vap v,*vap=&v;struct skb s,*beacon_skb=&s;int args_dma=1;		qdrv_hostlink_free_coherent(NULL, sizeof(*bc_args), bc_args, args_dma);
		ieee80211_beacon_destroy_param(vap);
		dev_kfree_skb_any(beacon_skb);
		vnet_free_ioctl(ioctl);
		return error;}

int main(void){struct qdrv_wlan w={0};struct qdrv_vap v={&w,7};for(unsigned n=0;n<4;n++)w.tx_if.hl_ring[n].ioctl_status=QTN_HLINK_STATUS_AVAIL;
struct host_ioctl*p=vnet_alloc_ioctl(&v);assert(p&&p->ioctl_status==0);assert(actual_frame(p,-EINVAL)==-EINVAL);
p=vnet_alloc_ioctl(&v);assert(actual_preflight(p,-EINVAL)==-EINVAL);
p=vnet_alloc_ioctl(&v);assert(actual_encode(p,-EIO)==-EIO);
p=vnet_alloc_ioctl(&v);assert(actual_shutdown(p,-ESHUTDOWN)==-ESHUTDOWN);
assert(args_freed==4&&param_freed==4&&skb_freed==4&&ioctl_freed==4&&publication==0);
for(unsigned n=0;n<4;n++)assert(w.tx_if.hl_ring[n].ioctl_status==QTN_HLINK_STATUS_AVAIL);
puts("{\"paths\":4,\"owned_resources_freed_once_each\":true,\"all_hostlink_slots_available\":true,\"publication_zero\":true}");return 0;}
