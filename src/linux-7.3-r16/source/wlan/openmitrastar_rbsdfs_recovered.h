/* OEM466/468 source reconstruction; source-only, never hardware-qualified.
 * Local fields are semantic state, not copied OEM offsets or STAstrict aliases.
 * The OEM producer requests TX enable during CAC; its callback is mandatory.
 * Linux exact-frame grant boundary is separate and fail-closed when unavailable.
 */
#ifndef OPENMITRASTAR_RBSDFS_RECOVERED_H
#define OPENMITRASTAR_RBSDFS_RECOVERED_H
#ifdef __KERNEL__
#include <linux/types.h>
typedef u32 omrbs_u32;typedef u8 omrbs_u8;
#else
#include <stdint.h>
typedef uint32_t omrbs_u32;typedef uint8_t omrbs_u8;
#endif
struct om_rbsdfs_state { omrbs_u8 measurement_in_cac,allow_rbs_report,allow_mbs_csa;
    omrbs_u32 end_tx_ms; };
struct om_rbsdfs_oem_hooks {
    void *context;
    void (*enable_tx)(void *);
    void (*disable_tx)(void *);
    void (*arm_timer)(void *,omrbs_u32 channel,omrbs_u32 expires);
    void *(*find_channel)(void *,omrbs_u32 channel);
    int (*equal_current)(void *,void *channel);
    unsigned (*channel_status)(void *);
};
void om_rbsdfs_init(struct om_rbsdfs_state *state);
int om_rbsdfs_set466(struct om_rbsdfs_state *state,omrbs_u32 value);
int om_rbsdfs_get466(const struct om_rbsdfs_state *state,omrbs_u32 *value);
int om_rbsdfs_set468(struct om_rbsdfs_state *state,omrbs_u32 value);
int om_rbsdfs_get468(const struct om_rbsdfs_state *state,omrbs_u32 *value);
int om_rbsdfs_oem_radar_window(struct om_rbsdfs_state *,unsigned role,
    int during_cac,omrbs_u32 channel,omrbs_u32 now,omrbs_u32 hz,
    const struct om_rbsdfs_oem_hooks *);
int om_rbsdfs_oem_mbs_window(struct om_rbsdfs_state *,int wds_is_rbs,
    int vap_hostap_run,int during_cac,const struct om_rbsdfs_oem_hooks *);
int om_rbsdfs_oem_send_permitted(struct om_rbsdfs_state *,unsigned role,
    const struct om_rbsdfs_oem_hooks *);
int om_rbsdfs_oem_expire(omrbs_u32 channel,const struct om_rbsdfs_oem_hooks *);

/* Provider-bound exact-frame adapter; no default MMIO/hostlink TX implementation.
 * Validation must prove a single measurement frame / owner / allocation remains
 * live and authorized. Grant and revoke may retain uncertainty; never replay it.
 */
struct om_rbsdfs_frame_hooks {
    void *context;
    int (*validate_measurement)(void *,const void *frame,unsigned role,omrbs_u32 channel);
    int (*grant_frame)(void *,const void *frame);
    int (*send_frame)(void *,const void *frame);
    int (*revoke_frame)(void *,const void *frame);
};
struct om_rbsdfs_frame_state {
    omrbs_u32 generation,channel,deadline;
    const void *frame;
    unsigned active,quarantined;
};
enum om_rbsdfs_frame_result { OMRBS_REFUSED=0,OMRBS_COMPLETE=1,OMRBS_PUBLISHED_UNKNOWN=2 };
enum om_rbsdfs_frame_result om_rbsdfs_exact_frame_window(struct om_rbsdfs_frame_state *,
    const void *frame,unsigned role,omrbs_u32 channel,omrbs_u32 deadline,
    const struct om_rbsdfs_frame_hooks *);
#endif
