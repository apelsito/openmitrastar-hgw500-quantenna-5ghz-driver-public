
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

#define IEEE80211_LOCK(ic) ((void)(ic))
#define IEEE80211_UNLOCK(ic) ((void)(ic))
static void ieee80211_beacon_associate_ie(struct beacon_shared_ie_t *ie, uint8_t *frm, uint16_t size)
{
#ifdef OPENMITRASTAR_QDRV_V89_PAIR
	BUILD_BUG_ON(sizeof(struct beacon_shared_ie_t) != 20);
	BUILD_BUG_ON(offsetof(struct beacon_shared_ie_t, buf) != 0);
	BUILD_BUG_ON(offsetof(struct beacon_shared_ie_t, lhost_buf) != 4);
	BUILD_BUG_ON(offsetof(struct beacon_shared_ie_t, size) != 8);
	BUILD_BUG_ON(offsetof(struct beacon_shared_ie_t, next_muc_addr) != 12);
	BUILD_BUG_ON(offsetof(struct beacon_shared_ie_t, next) != 16);
#endif
	if (ie == NULL || frm == NULL)
		return;
	ie->size = size;
	/* convert to bus address for MuC access only */
	ie->buf = plat_kernel_addr_to_dma(NULL, frm);

	/* to avoid memory remap, keep lhost original buffer address for debug purpose */
	ie->lhost_buf = frm;
	ie->next = NULL;
	ie->next_muc_addr = 0;
}
static struct beacon_shared_ie_t *ieee80211_beacon_alloc_ie(struct ieee80211_beacon_param_t *param)
{
    uintptr_t start, curr;
    struct beacon_shared_ie_t *ie;

    if (!param || param->om_build_error)
        return NULL;
    start = (uintptr_t)param->buf;
    curr = (uintptr_t)param->curr;
    if (param->size != BEACON_PARAM_SIZE || curr < start ||
        curr - start > param->size || (curr & 3) ||
        (curr - start) % sizeof(*ie) ||
        param->size - (curr - start) < sizeof(*ie)) {
        param->om_build_error = -ENOSPC;
        return NULL;
    }
    ie = (struct beacon_shared_ie_t *)curr;
    memset(ie, 0, sizeof(*ie));
    param->curr += sizeof(*ie);
    return ie;
}
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
}
static int om_beacon_allocation_ready(struct ieee80211_beacon_param_t *param)
{
    uintptr_t start, curr;
    if (!param || param->om_build_error)
        return 0;
    if (om_beacon_chain_valid(param))
        return 0;
    start = (uintptr_t)param->buf;
    curr = (uintptr_t)param->curr;
    if (param->size != BEACON_PARAM_SIZE || curr < start ||
        curr - start > param->size || (start & 3) || (curr & 3) ||
        (curr - start) % sizeof(struct beacon_shared_ie_t) ||
        param->size - (curr - start) < sizeof(struct beacon_shared_ie_t)) {
        param->om_build_error = -ENOSPC;
        return 0;
    }
    return 1;
}
int om_beacon_reserved_remove(struct ieee80211_beacon_param_t *param,
        struct beacon_shared_ie_t *slot)
{
    struct beacon_shared_ie_t *ie, *previous = NULL;
    unsigned int count = 0;

    if (!param || param->om_build_error)
        return -EINVAL;
    if (!om_beacon_descriptor_valid(param, slot) ||
        (slot != param->om_cca_ie && slot != param->om_app_ie))
        return om_beacon_slot_error(param);
    if (om_beacon_chain_valid(param))
        return -EINVAL;
    if (!slot->size)
        return 0;
    for (ie = param->head; ie;
         previous = ie, ie = ie->next) {
        if (count++ >= BEACON_PARAM_SIZE / sizeof(*ie) ||
            !om_beacon_descriptor_valid(param, ie) || !ie->size)
            return om_beacon_slot_error(param);
        if (ie != slot)
            continue;
        if (previous) {
            previous->next = ie->next;
            previous->next_muc_addr = ie->next_muc_addr;
        } else {
            param->head = ie->next;
        }
        if (param->tail == ie)
            param->tail = previous;
        memset(slot, 0, sizeof(*slot));
        return 0;
    }
    return om_beacon_slot_error(param);
}
int om_beacon_reserved_update(struct ieee80211_beacon_param_t *param,
        struct beacon_shared_ie_t *ie, uint8_t *frm, uint16_t size)
{
    struct beacon_shared_ie_t *cursor, *previous = NULL;
    unsigned int count = 0;

    if (!param || param->om_build_error)
        return -EINVAL;
    if (!om_beacon_descriptor_valid(param, ie) || !frm || !size ||
        (ie != param->om_cca_ie && ie != param->om_app_ie))
        return om_beacon_slot_error(param);
    if (om_beacon_chain_valid(param))
        return -EINVAL;
    if (ie->size && om_beacon_reserved_remove(param, ie))
        return -EINVAL;
    /* Wire traversal must match frame order, including APP-first/CCA-later. */
    for (cursor = param->head; cursor; previous = cursor, cursor = cursor->next) {
        if (count++ >= BEACON_PARAM_SIZE / sizeof(*cursor) ||
            !om_beacon_descriptor_valid(param, cursor) || !cursor->size)
            return om_beacon_slot_error(param);
        if ((uintptr_t)cursor->lhost_buf > (uintptr_t)frm)
            break;
    }
    ieee80211_beacon_associate_ie(ie, frm, size);
    ie->next = cursor;
    ie->next_muc_addr = cursor ? plat_kernel_addr_to_dma(NULL, cursor) : 0;
    if (previous) {
        previous->next = ie;
        previous->next_muc_addr = plat_kernel_addr_to_dma(NULL, ie);
    } else {
        param->head = ie;
    }
    if (!cursor)
        param->tail = ie;
    return 0;
}
int om_ieee80211_beacon_build_status(struct ieee80211_beacon_param_t *param)
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
}
static u8*ieee80211_add_beacon_desc_ie(struct ieee80211_node*ni,unsigned id,u8*frm){
 struct ieee80211_beacon_param_t*p=ni->ni_vap->param;unsigned n;
 if(id==IEEE80211_ELEMID_MEASREQ){
#ifdef CONFIG_QTN_80211K_SUPPORT
 n=sizeof(struct ieee80211_ie_measreq)+sizeof(struct ieee80211_ie_measure_comm);
#else
 n=sizeof(struct ieee80211_ie_measreq);
#endif
 memset(frm,0x38,n);CHECK(om_beacon_reserved_update(p,p->om_cca_ie,frm,n)==0);
 }else{n=ni->ni_vap->app_ie[0].length;CHECK(ni->ni_vap->app_ie[0].ie!=NULL);memcpy(frm,ni->ni_vap->app_ie[0].ie,n);CHECK(om_beacon_reserved_update(p,p->om_app_ie,frm,n)==0);}
 return frm+n;
}
static void actual_dynamic(struct ieee80211_node*ni,struct ieee80211vap*vap,struct ieee80211com*ic,struct ieee80211_beacon_offsets*bo,struct sk_buff*skb){
	IEEE80211_LOCK(ic);
	if (om_ieee80211_beacon_frame_status(vap->param, skb))
		goto om_beacon_update_end;
	/* Dynamic extents must match the current unpublished frame before writes. */
	{
		uintptr_t begin = (uintptr_t)skb->data;
		uintptr_t end = begin + skb->len;
		uintptr_t cca = (uintptr_t)bo->bo_cca;
		uintptr_t app = (uintptr_t)bo->bo_appie_buf;
		unsigned int cca_size = vap->param->om_cca_ie->size;
		unsigned int app_size = vap->param->om_app_ie->size;
		if (end < begin || cca < begin || cca > end || app < cca || app > end ||
		    cca_size > end - cca || app != cca + cca_size ||
		    bo->bo_appie_buf_len > end - app ||
		    (app_size && app_size != bo->bo_appie_buf_len)) {
			vap->param->om_build_error = -EINVAL;
			goto om_beacon_update_end;
		}
	}

        /* Change the CCA payload once, then reuse its reserved descriptor.
         * Move a following APP payload before inserting/removing CCA bytes. */
        {
            unsigned int old_len = vap->param->om_cca_ie->size;
            unsigned int old_app_len = vap->param->om_app_ie->size;
            unsigned int new_len = 0;
            int delta;
#if defined(CONFIG_QTN_80211K_SUPPORT)
            if (ic->ic_flags & IEEE80211_F_CCA)
                new_len = sizeof(struct ieee80211_ie_measreq) +
                    sizeof(struct ieee80211_ie_measure_comm);
#else
            if (ic->ic_flags & IEEE80211_F_CCA)
                new_len = sizeof(struct ieee80211_ie_measreq);
#endif
            delta = (int)new_len - (int)old_len;
            if (delta > 0 && skb_tailroom(skb) < delta) {
                vap->param->om_build_error = -ENOSPC;
                goto om_beacon_update_end;
            }
            if (delta) {
                /* Detach both old extents before their payload positions move. */
                if (om_beacon_reserved_remove(vap->param, vap->param->om_cca_ie) ||
                    om_beacon_reserved_remove(vap->param, vap->param->om_app_ie))
                    goto om_beacon_update_end;
                memmove(bo->bo_appie_buf + delta, bo->bo_appie_buf,
                    bo->bo_appie_buf_len);
                bo->bo_xr += delta;
                bo->bo_appie_buf += delta;
                bo->bo_tim_trailerlen += delta;
                if (delta > 0)
                    skb_put(skb, delta);
                else
                    skb_trim(skb, skb->len + delta);
                if (old_app_len &&
                    om_beacon_reserved_update(vap->param,
                        vap->param->om_app_ie, bo->bo_appie_buf,
                        bo->bo_appie_buf_len))
                    goto om_beacon_update_end;
            }
            if (new_len)
                (void)ieee80211_add_beacon_desc_ie(ni,
                    IEEE80211_ELEMID_MEASREQ, bo->bo_cca);
            else if (om_beacon_reserved_remove(vap->param,
                    vap->param->om_cca_ie))
                goto om_beacon_update_end;
            if (vap->param->om_build_error)
                goto om_beacon_update_end;
        }
    /* APP payload replacement uses one reserved slot, including removal. */
    {
        unsigned int new_len = vap->app_ie[IEEE80211_APPIE_FRAME_BEACON].ie ?
            vap->app_ie[IEEE80211_APPIE_FRAME_BEACON].length : 0;
        int delta = (int)new_len - (int)bo->bo_appie_buf_len;

        if (delta > 0 && skb_tailroom(skb) < delta) {
            vap->param->om_build_error = -ENOSPC;
            goto om_beacon_update_end;
        }
        if (delta > 0)
            skb_put(skb, delta);
        else if (delta < 0)
            skb_trim(skb, skb->len + delta);
        bo->bo_appie_buf_len = new_len;
        bo->bo_tim_trailerlen += delta;
        if (new_len)
            (void)ieee80211_add_beacon_desc_ie(ni,
                IEEE80211_ELEMID_VENDOR_APP, bo->bo_appie_buf);
        else if (om_beacon_reserved_remove(vap->param,
                vap->param->om_app_ie))
            goto om_beacon_update_end;
        if (vap->param->om_build_error)
            goto om_beacon_update_end;
        vap->iv_flags_ext &= ~IEEE80211_FEXT_APPIE_UPDATE;
    }

om_beacon_update_end:IEEE80211_UNLOCK(ic);return;
}

static unsigned list_count(struct ieee80211_beacon_param_t*p,struct sk_buff*s){
 unsigned n=0;u8*position=s->data;struct beacon_shared_ie_t*last=NULL;
 for(struct beacon_shared_ie_t*i=p->head;i;i=i->next){CHECK(++n<=50);CHECK(om_beacon_descriptor_valid(p,i));CHECK(i->lhost_buf==position);CHECK(i->buf==(u32)(uintptr_t)i->lhost_buf);CHECK(i->next_muc_addr==(u32)(uintptr_t)i->next);position+=i->size;last=i;}
 CHECK(last==p->tail);CHECK(position==s->data+s->len);return n;
}
static void init(struct ieee80211_beacon_param_t*p,u8*bytes,unsigned base){
 memset(p,0,sizeof(*p));p->size=1000;p->curr=(u32)(uintptr_t)p->buf;
 p->om_cca_ie=ieee80211_beacon_alloc_ie(p);p->om_app_ie=ieee80211_beacon_alloc_ie(p);
 struct beacon_shared_ie_t*i=ieee80211_beacon_alloc_ie(p);ieee80211_beacon_associate_ie(i,bytes,base);p->head=p->tail=i;
 CHECK(om_ieee80211_beacon_build_status(p)==0);
}
int main(void){
 _Static_assert(sizeof(void*)==4,"32bit host required");_Static_assert(sizeof(struct beacon_shared_ie_t)==20,"wire descriptor");
 struct ieee80211_beacon_param_t p;u8 buffer[512]={0},app[280];memset(app,0x65,sizeof(app));
 struct sk_buff s={buffer,20,sizeof(buffer)};struct ieee80211vap v={0};struct ieee80211com ic={0};struct ieee80211_node ni={&v};struct ieee80211_beacon_offsets bo={buffer+20,buffer+20,buffer+20,0,20};v.param=&p;
 init(&p,buffer,20);unsigned curr=p.curr;
 /* APP is attached first. Every CCA toggle must preserve serialized frame order. */
 v.app_ie[0].ie=app;v.app_ie[0].length=17;actual_dynamic(&ni,&v,&ic,&bo,&s);CHECK(list_count(&p,&s)==2);cases++;
 for(unsigned n=0;n<10000;n++){ic.ic_flags=n&1;v.app_ie[0].length=(n%3)?17:270;v.app_ie[0].ie=(n%5)?app:NULL;actual_dynamic(&ni,&v,&ic,&bo,&s);CHECK(!p.om_build_error);CHECK(p.curr==curr);list_count(&p,&s);if(v.app_ie[0].ie)CHECK(memcmp(bo.bo_appie_buf,app,bo.bo_appie_buf_len)==0);cases++;}
 ic.ic_flags=0;v.app_ie[0].ie=NULL;actual_dynamic(&ni,&v,&ic,&bo,&s);CHECK(list_count(&p,&s)==1);CHECK(s.len==20);cases++;
 /* Once reserved, dynamic updates never consume the remaining 47 static slots. */
 while(om_beacon_allocation_ready(&p)){CHECK(ieee80211_beacon_alloc_ie(&p)!=NULL);}CHECK(p.curr==(u32)(uintptr_t)p.buf+1000);CHECK(p.om_build_error==-ENOSPC);unsigned before=p.curr;CHECK(ieee80211_beacon_alloc_ie(&p)==NULL);CHECK(p.curr==before);cases++;
 /* Invalid, misaligned, foreign, one-past and corrupt chain pointers are refused. */
 for(unsigned k=0;k<6;k++){init(&p,buffer,20);struct beacon_shared_ie_t*bad=k==0?NULL:k==1?(void*)(p.buf+1):k==2?(void*)(p.buf+1000):k==3?(void*)buffer:k==4?(void*)(p.buf-20):(void*)(p.buf+60);CHECK(om_beacon_reserved_update(&p,bad,buffer+20,8)==-EINVAL);CHECK(p.om_build_error==-EINVAL);cases++;}
 init(&p,buffer,20);p.head->next=(void*)(p.buf+1000);CHECK(om_beacon_reserved_update(&p,p.om_app_ie,buffer+20,4)==-EINVAL);CHECK(p.om_build_error==-EINVAL);cases++;
 init(&p,buffer,20);p.head->next=p.head;CHECK(om_beacon_reserved_update(&p,p.om_app_ie,buffer+20,4)==-EINVAL);CHECK(p.om_build_error==-EINVAL);cases++;
 init(&p,buffer,20);p.om_cca_ie=(void*)(p.buf+1);CHECK(om_ieee80211_beacon_build_status(&p)==-EINVAL);CHECK(p.om_build_error==-EINVAL);cases++;
 /* Validate every suffix and tail before reaching any insertion point. */
 init(&p,buffer,20);p.tail=(void*)(p.buf+1000);CHECK(om_ieee80211_beacon_build_status(&p)==-EINVAL);CHECK(p.om_build_error==-EINVAL);cases++;
 init(&p,buffer,20);p.head->lhost_buf=buffer+40;p.head->buf=(u32)(uintptr_t)(buffer+40);p.head->next=(void*)(p.buf+1000);p.head->next_muc_addr=(u32)(uintptr_t)p.head->next;CHECK(om_beacon_reserved_update(&p,p.om_cca_ie,buffer+20,8)==-EINVAL);CHECK(p.om_cca_ie->size==0);CHECK(p.head->lhost_buf==buffer+40);cases++;
 init(&p,buffer,20);p.head->buf^=1;CHECK(om_ieee80211_beacon_build_status(&p)==-EINVAL);cases++;
 init(&p,buffer,20);p.head->next_muc_addr=1;CHECK(om_ieee80211_beacon_build_status(&p)==-EINVAL);cases++;
 init(&p,buffer,20);p.om_app_ie->size=8;CHECK(om_ieee80211_beacon_build_status(&p)==-EINVAL);cases++;
 init(&p,buffer,20);p.head->size=19;s.len=20;s.capacity=512;CHECK(om_ieee80211_beacon_frame_status(&p,&s)==-EINVAL);cases++;
 init(&p,buffer,20);s.len=20;s.capacity=512;bo=(struct ieee80211_beacon_offsets){buffer+20,buffer+20,buffer+20,0,20};p.tail=(void*)(p.buf+1000);u8 bad_snapshot[512];memcpy(bad_snapshot,buffer,512);actual_dynamic(&ni,&v,&ic,&bo,&s);CHECK(p.om_build_error==-EINVAL);CHECK(memcmp(bad_snapshot,buffer,512)==0);CHECK(s.len==20);cases++;
 init(&p,buffer,20);s.len=20;s.capacity=512;bo=(struct ieee80211_beacon_offsets){buffer+20,buffer+19,buffer+20,0,20};actual_dynamic(&ni,&v,&ic,&bo,&s);CHECK(p.om_build_error==-EINVAL);CHECK(s.len==20);cases++;
 /* Tailroom refusal occurs before changing payload, offsets or descriptor links. */
 init(&p,buffer,20);s.len=20;s.capacity=20;bo=(struct ieee80211_beacon_offsets){buffer+20,buffer+20,buffer+20,0,20};ic.ic_flags=1;u8 snapshot[512];memcpy(snapshot,buffer,512);actual_dynamic(&ni,&v,&ic,&bo,&s);CHECK(p.om_build_error==-ENOSPC);CHECK(memcmp(snapshot,buffer,512)==0);CHECK(s.len==20);CHECK(bo.bo_appie_buf==buffer+20);cases++;
 printf("{\"cases\":%u,\"checks\":%u,\"wire_bytes\":20,\"dynamic_updates\":10000}\n",cases,checks);return 0;
}
