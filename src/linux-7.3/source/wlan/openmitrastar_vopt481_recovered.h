/* Derived from pinned OEM ARC481 cases and explicit consumer entries.
 * Original module bytes are unchanged. No kernel ABI offsets are reused. */
#ifndef OM_VOPT481_RECOVERED_H
#define OM_VOPT481_RECOVERED_H
typedef unsigned char om_vopt_u8;
typedef unsigned int om_vopt_u32;
struct om_vopt_state {
    om_vopt_u8 policy, effective, saved_txbf, saved_probe;
    om_vopt_u8 saved_pppc, saved_airfair, live_pppc, live_airfair;
    om_vopt_u8 live_duplicate_mu;
    om_vopt_u32 saved_scs, saved_ocac, saved_bgscan;
};
struct om_vopt_environment {
    om_vopt_u32 flags, flags_ext;
    unsigned ocac_channel;
    int mode_value;
};
struct om_vopt_provider {
    void *context;
    int (*parameter)(void *, unsigned, int);
    int (*scs)(void *, om_vopt_u32);
    int (*ocac)(void *, om_vopt_u32);
    int (*background_get)(void *, unsigned, om_vopt_u32 *);
    int (*background_set)(void *, unsigned, unsigned);
    void (*reassociate)(void *, unsigned, unsigned);
};
struct om_vopt_peer { unsigned self_bss, self_address; om_vopt_u32 flags; };
void om_vopt_initialize(struct om_vopt_state *, unsigned, unsigned);
int om_vopt_set(struct om_vopt_state *, unsigned, om_vopt_u32);
int om_vopt_get(const struct om_vopt_state *, unsigned, int *);
int om_vopt_apply(struct om_vopt_state *, unsigned,
                  const struct om_vopt_environment *, const struct om_vopt_provider *);
void om_vopt_count_peer(const struct om_vopt_peer *, om_vopt_u32 *, om_vopt_u32 *);
int om_vopt_tick(struct om_vopt_state *, om_vopt_u32 *, om_vopt_u32, om_vopt_u32,
                 const struct om_vopt_environment *, const struct om_vopt_provider *);
#endif
