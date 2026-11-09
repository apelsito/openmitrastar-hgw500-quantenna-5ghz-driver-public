
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#define IEEE80211_APPIE_MAX 1024
#define M_DEVBUF 1
#define M_WAITOK 1
struct ieee80211_ie{uint8_t v;};struct ieee80211com{int x;};struct ieee80211_app_ie_t{struct ieee80211_ie*ie;unsigned length;};struct ieee80211vap{struct ieee80211com*iv_ic;struct ieee80211_app_ie_t app_ie[6];};struct ieee80211req_getset_appiebuf{unsigned app_buflen;uint8_t app_buf[1024];};
static unsigned locked,allocs,frees;static int fail_alloc;
#define IEEE80211_LOCK_BH(ic) do{(void)(ic);assert(!locked);locked=1;}while(0)
#define IEEE80211_UNLOCK_BH(ic) do{(void)(ic);assert(locked);locked=0;}while(0)
static int is_valid_ie_list(unsigned n,void*p,int x){(void)p;(void)x;return n<=1024;}
static void*allocation(unsigned n){assert(!locked);allocs++;return fail_alloc?NULL:malloc(n?n:1);}
static void release(void*p){assert(!locked);frees++;free(p);}
#define MALLOC(p,type,n,t,f) ((p)=(type)allocation(n))
#define FREE(p,t) release(p)
static int
add_app_ie(unsigned int frame_type_index, struct ieee80211vap *vap,
	struct ieee80211req_getset_appiebuf *iebuf)
{
	struct ieee80211_ie *ie;

	if (! is_valid_ie_list(iebuf->app_buflen, iebuf->app_buf, 1))
		return -EINVAL;
	/* NB: data.length is validated by the wireless extensions code */
	MALLOC(ie, struct ieee80211_ie *, iebuf->app_buflen, M_DEVBUF, M_WAITOK);
	if (ie == NULL)
		return -ENOMEM;

	memcpy(ie, iebuf->app_buf, iebuf->app_buflen);
	{
		struct ieee80211_ie *old_ie;
		struct ieee80211com *ic = vap->iv_ic;
		IEEE80211_LOCK_BH(ic);
		old_ie = vap->app_ie[frame_type_index].ie;
		vap->app_ie[frame_type_index].ie = ie;
		vap->app_ie[frame_type_index].length = iebuf->app_buflen;
		IEEE80211_UNLOCK_BH(ic);
		if (old_ie)
			FREE(old_ie, M_DEVBUF);
	}

	return 0;
}
static int
remove_app_ie(unsigned int frame_type_index, struct ieee80211vap *vap)
{
	struct ieee80211_app_ie_t *app_ie = &vap->app_ie[frame_type_index];
	struct ieee80211_ie *old_ie;
	struct ieee80211com *ic = vap->iv_ic;
	IEEE80211_LOCK_BH(ic);
	old_ie = app_ie->ie;
	app_ie->ie = NULL;
	app_ie->length = 0;
	IEEE80211_UNLOCK_BH(ic);
	if (old_ie)
		FREE(old_ie, M_DEVBUF);
	return 0;
}
static int
get_app_ie(unsigned int frame_type_index, struct ieee80211vap *vap,
	struct ieee80211req_getset_appiebuf *iebuf)
{
	struct ieee80211_app_ie_t *app_ie = &vap->app_ie[frame_type_index];
	struct ieee80211com *ic = vap->iv_ic;
	IEEE80211_LOCK_BH(ic);
	if (iebuf->app_buflen < app_ie->length) {
		IEEE80211_UNLOCK_BH(ic);
		return -EINVAL;
	}

	iebuf->app_buflen = app_ie->length;
	if (app_ie->length)
		memcpy(iebuf->app_buf, app_ie->ie, app_ie->length);
	IEEE80211_UNLOCK_BH(ic);
	return 0;
}static unsigned int om_app_ie_copy(struct ieee80211vap *vap,
        unsigned int frame_type, uint8_t *destination)
{
    struct ieee80211com *ic = vap->iv_ic;
    unsigned int length = 0;
    IEEE80211_LOCK_BH(ic);
    if (vap->app_ie[frame_type].ie &&
        vap->app_ie[frame_type].length <= IEEE80211_APPIE_MAX) {
        length = vap->app_ie[frame_type].length;
        memcpy(destination, vap->app_ie[frame_type].ie, length);
    }
    IEEE80211_UNLOCK_BH(ic);
    return length;
}int main(void){struct ieee80211com ic;struct ieee80211vap v={.iv_ic=&ic};struct ieee80211req_getset_appiebuf request={0},answer={0};unsigned char copy[1024];for(unsigned n=0;n<=1024;n++){request.app_buflen=n;memset(request.app_buf,(unsigned char)n,n);assert(add_app_ie(0,&v,&request)==0);answer.app_buflen=1024;assert(get_app_ie(0,&v,&answer)==0&&answer.app_buflen==n);assert(om_app_ie_copy(&v,0,copy)==n);assert(!memcmp(copy,request.app_buf,n));assert(!memcmp(answer.app_buf,request.app_buf,n));}fail_alloc=1;request.app_buflen=4;unsigned oldlen=v.app_ie[0].length;struct ieee80211_ie*old=v.app_ie[0].ie;assert(add_app_ie(0,&v,&request)==-ENOMEM&&v.app_ie[0].ie==old&&v.app_ie[0].length==oldlen);fail_alloc=0;assert(remove_app_ie(0,&v)==0&&!v.app_ie[0].ie&&!v.app_ie[0].length);assert(om_app_ie_copy(&v,0,copy)==0);assert(!locked&&frees==1025);puts("{\"length_cases\":1025,\"writer_allocation_outside_lock\":true,\"old_free_outside_lock\":true,\"copy_length_data_coherent\":true,\"allocation_failure_preserves_old\":true}");return 0;}
