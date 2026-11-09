
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
typedef uint32_t u32;typedef uint8_t u8;typedef uint32_t dma_addr_t;
#define EXPORT_SYMBOL(x)
#define BEACON_PARAM_SIZE 1000
#define IEEE80211_F_CCA 1U
#define IEEE80211_ELEMID_MEASREQ 38
#define IEEE80211_ELEMID_VENDOR_APP 0xff09
#define IEEE80211_APPIE_FRAME_BEACON 0
#define IEEE80211_FEXT_APPIE_UPDATE 1U
struct beacon_shared_ie_t {dma_addr_t buf;uint8_t*lhost_buf;uint32_t size;uint32_t next_muc_addr;struct beacon_shared_ie_t*next;};
struct ieee80211_beacon_param_t {struct beacon_shared_ie_t*head,*tail;uint8_t buf[1000];uint32_t curr;uint16_t size;struct beacon_shared_ie_t*om_cca_ie,*om_app_ie;int om_build_error;};
struct ieee80211_ie_measreq {u8 data[18];};
struct ieee80211_ie_measure_comm {u8 data[5];};
struct sk_buff {u8 *data;unsigned len,capacity;};
struct app_ie {u8*ie;unsigned length;};
struct ieee80211vap {struct ieee80211_beacon_param_t*param;struct app_ie app_ie[1];u32 iv_flags_ext;};
struct ieee80211com {u32 ic_flags;};
struct ieee80211_node {struct ieee80211vap*ni_vap;};
struct ieee80211_beacon_offsets {u8 *bo_cca,*bo_appie_buf,*bo_xr;unsigned bo_appie_buf_len,bo_tim_trailerlen;};
static unsigned checks,cases;
#define CHECK(x) do{checks++;assert(x);}while(0)
static uint32_t plat_kernel_addr_to_dma(void*x,void*y){(void)x;return(uint32_t)(uintptr_t)y;}
static int skb_tailroom(struct sk_buff*s){CHECK(s->len<=s->capacity);return s->capacity-s->len;}
static u8*skb_put(struct sk_buff*s,unsigned n){CHECK(n<=s->capacity-s->len);u8*p=s->data+s->len;s->len+=n;return p;}
static void skb_trim(struct sk_buff*s,unsigned n){CHECK(n<=s->len);s->len=n;}

static unsigned free_args,free_ioctl,free_param,free_skb,publication;
struct qtn_beacon_args{int x;};
static void qdrv_hostlink_free_coherent(void*d,unsigned n,void*p,int h){(void)d;(void)n;(void)p;(void)h;free_args++;}
static void vnet_free_ioctl(void*p){(void)p;free_ioctl++;}
static void ieee80211_beacon_destroy_param(struct ieee80211vap*v){(void)v;free_param++;}
static void dev_kfree_skb_any(struct sk_buff*s){(void)s;free_skb++;}
static int om_beacon_descriptor_valid(struct ieee80211_beacon_param_t *param,
        struct beacon_shared_ie_t *ie)
{
    uintptr_t start, curr, address;
    if (!param || !ie || param->size != BEACON_PARAM_SIZE)
        return 0;
    start = (uintptr_t)param->buf;
    curr = (uintptr_t)param->curr;
    address = (uintptr_t)ie;
    return curr >= start && curr - start <= param->size &&
        !(start & 3) && (curr - start) % sizeof(*ie) == 0 &&
        address >= start && address - start <= curr - start &&
        curr - address >= sizeof(*ie) && !(address & 3) &&
        (address - start) % sizeof(*ie) == 0;
}
static int om_beacon_slot_error(struct ieee80211_beacon_param_t *param)
{
    if (param && !param->om_build_error)
        param->om_build_error = -EINVAL;
    return -EINVAL;
}
static int om_beacon_chain_valid(struct ieee80211_beacon_param_t *param)
{
    struct beacon_shared_ie_t *ie, *last = NULL;
    struct beacon_shared_ie_t *seen[BEACON_PARAM_SIZE / sizeof(*ie)];
    unsigned int count = 0, cca_seen = 0, app_seen = 0, i;
    uintptr_t previous = 0;

    if (!param || param->om_build_error)
        return -EINVAL;
    if (!om_beacon_descriptor_valid(param, param->om_cca_ie) ||
        !om_beacon_descriptor_valid(param, param->om_app_ie) ||
        param->om_cca_ie == param->om_app_ie)
        return om_beacon_slot_error(param);
    for (ie = param->head; ie; ie = ie->next) {
        if (count >= BEACON_PARAM_SIZE / sizeof(*ie) ||
            !om_beacon_descriptor_valid(param, ie))
            return om_beacon_slot_error(param);
        for (i = 0; i < count; i++)
            if (seen[i] == ie)
                return om_beacon_slot_error(param);
        if (!ie->size || !ie->lhost_buf ||
            (count && (uintptr_t)ie->lhost_buf < previous) ||
            (uintptr_t)ie->lhost_buf + ie->size < (uintptr_t)ie->lhost_buf ||
            (u32)ie->buf != (u32)plat_kernel_addr_to_dma(NULL, ie->lhost_buf) ||
            ie->next_muc_addr != (ie->next ?
                (u32)plat_kernel_addr_to_dma(NULL, ie->next) : 0))
            return om_beacon_slot_error(param);
        seen[count++] = ie;
        previous = (uintptr_t)ie->lhost_buf + ie->size;
        cca_seen += ie == param->om_cca_ie;
        app_seen += ie == param->om_app_ie;
        last = ie;
    }
    if (last != param->tail ||
        cca_seen != !!param->om_cca_ie->size ||
        app_seen != !!param->om_app_ie->size)
        return om_beacon_slot_error(param);
    return 0;
}int om_ieee80211_beacon_build_status(struct ieee80211_beacon_param_t *param)
{
    if (!param)
        return -EINVAL;
    if (param->om_build_error)
        return param->om_build_error;
    return om_beacon_chain_valid(param);
}
int om_ieee80211_beacon_frame_status(struct ieee80211_beacon_param_t *param,
        struct sk_buff *skb)
{
    struct beacon_shared_ie_t *ie;
    uintptr_t cursor, end;
    int status = om_ieee80211_beacon_build_status(param);
    if (status)
        return status;
    if (!skb || !skb->data)
        return om_beacon_slot_error(param);
    cursor = (uintptr_t)skb->data;
    end = cursor + skb->len;
    if (end < cursor || !param->head)
        return om_beacon_slot_error(param);
    for (ie = param->head; ie; ie = ie->next) {
        if ((uintptr_t)ie->lhost_buf != cursor || ie->size > end - cursor)
            return om_beacon_slot_error(param);
        cursor += ie->size;
    }
    if (cursor != end)
        return om_beacon_slot_error(param);
    return 0;
}static int actual_guard(struct ieee80211vap*vap){struct qtn_beacon_args a,*bc_args=&a;int args_dma=1;void*ioctl=&a;struct sk_buff s,*beacon_skb=&s;
	{
		int build_error = om_ieee80211_beacon_frame_status(vap->param, beacon_skb);
		if (build_error) {
			qdrv_hostlink_free_coherent(NULL, sizeof(*bc_args), bc_args, args_dma);
			vnet_free_ioctl(ioctl);
			ieee80211_beacon_destroy_param(vap);
			dev_kfree_skb_any(beacon_skb);
			return build_error;
		}
	}

publication++;return 0;}
int main(void){struct ieee80211_beacon_param_t p={0};struct ieee80211vap v={0};v.param=&p; p.om_build_error=-ENOSPC;assert(actual_guard(&v)==-ENOSPC);assert(free_args==1&&free_ioctl==1&&free_param==1&&free_skb==1&&publication==0);p.om_build_error=-EINVAL;assert(actual_guard(&v)==-EINVAL);assert(free_args==2&&free_ioctl==2&&free_param==2&&free_skb==2&&publication==0);puts("{\"checks\":10,\"latched_error_cleanup_once\":true,\"publication_zero\":true}");return 0;}