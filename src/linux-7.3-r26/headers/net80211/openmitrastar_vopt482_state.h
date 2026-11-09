/* Caller-owned482 state. Initialize once with zeroed storage; never reset a
 * retained/quarantined transaction or free its IC/radio/provider owner. */
#ifndef OPENMITRASTAR_VOPT482_STATE_H
#define OPENMITRASTAR_VOPT482_STATE_H
struct ieee80211vap;
struct om_vopt482_state {
    void *payload,*pending_ioctl,*owner;
    unsigned dma,bytes,generation,stage,processing,quarantined,module_held;
};
int om_qdrv_vopt482(struct ieee80211vap *,struct om_vopt482_state *,int value);
int om_qdrv_vopt482_pending(const struct om_vopt482_state *);
#endif
