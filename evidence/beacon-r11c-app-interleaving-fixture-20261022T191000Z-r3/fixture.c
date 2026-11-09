
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

#include <windows.h>
static CRITICAL_SECTION lock;static HANDLE inside_copy,release_copy,writer_started;static DWORD reader_thread;static int hold_copy;static LONG freed;static int fail_alloc;
#define IEEE80211_LOCK_BH(ic) do{(void)(ic);EnterCriticalSection(&lock);}while(0)
#define IEEE80211_UNLOCK_BH(ic) do{(void)(ic);LeaveCriticalSection(&lock);}while(0)
static int is_valid_ie_list(unsigned n,void*p,int x){(void)p;(void)x;return n<=1024;}
static void*allocation(unsigned n){return fail_alloc?NULL:malloc(n?n:1);}
static void release(void*p){InterlockedIncrement(&freed);free(p);}
#define MALLOC(p,type,n,t,f) ((p)=(type)allocation(n))
#define FREE(p,t) release(p)
static void*controlled_copy(void*d,const void*p,size_t n){if(hold_copy&&GetCurrentThreadId()==reader_thread){SetEvent(inside_copy);assert(WaitForSingleObject(release_copy,5000)==WAIT_OBJECT_0);}return memcpy(d,p,n);}
#define memcpy(d,p,n) controlled_copy(d,p,n)
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
}
static struct ieee80211com ic;static struct ieee80211vap v={.iv_ic=&ic};static struct ieee80211req_getset_appiebuf request;static unsigned char output[1024];
static DWORD WINAPI reader(void*x){(void)x;reader_thread=GetCurrentThreadId();assert(om_app_ie_copy(&v,0,output)==32);return 0;}
static DWORD WINAPI writer(void*x){SetEvent(writer_started);if((uintptr_t)x)assert(remove_app_ie(0,&v)==0);else assert(add_app_ie(0,&v,&request)==0);return 0;}
int main(void){InitializeCriticalSection(&lock);inside_copy=CreateEvent(NULL,TRUE,FALSE,NULL);release_copy=CreateEvent(NULL,TRUE,FALSE,NULL);writer_started=CreateEvent(NULL,TRUE,FALSE,NULL);
for(unsigned remove=0;remove<2;remove++){hold_copy=0;request.app_buflen=32;memset(request.app_buf,0x41,32);assert(add_app_ie(0,&v,&request)==0);LONG before=freed;request.app_buflen=64;memset(request.app_buf,0x42,64);ResetEvent(inside_copy);ResetEvent(release_copy);ResetEvent(writer_started);hold_copy=1;HANDLE r=CreateThread(NULL,0,reader,NULL,0,NULL);assert(WaitForSingleObject(inside_copy,5000)==WAIT_OBJECT_0);HANDLE w=CreateThread(NULL,0,writer,(void*)(uintptr_t)remove,0,NULL);assert(WaitForSingleObject(writer_started,5000)==WAIT_OBJECT_0);assert(WaitForSingleObject(w,100)==WAIT_TIMEOUT);assert(freed==before);SetEvent(release_copy);assert(WaitForSingleObject(r,5000)==WAIT_OBJECT_0);assert(WaitForSingleObject(w,5000)==WAIT_OBJECT_0);for(unsigned i=0;i<32;i++)assert(output[i]==0x41);assert(freed==before+1);CloseHandle(r);CloseHandle(w);}puts("{\"reader_vs_replace_cases\":1,\"reader_vs_remove_cases\":1,\"old_buffer_not_freed_while_copying\":true,\"whole_old_copy_observed\":true}");return 0;}
