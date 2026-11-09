/* Derived from exact OEM constant tables and REA 0x12f18/0x12f9c.
 * Source reconstruction only: no scan, ranking or regulatory acceptance.
 * Templates preserve OEM oddities byte for byte. No table grants channel use.
 */
#ifndef OPENMITRASTAR_CHANSET_RECOVERED_H
#define OPENMITRASTAR_CHANSET_RECOVERED_H
#ifdef __KERNEL__
#include <linux/types.h>
typedef u32 om_cs_u32;
#else
#include <stdint.h>
#include <stddef.h>
typedef uint32_t om_cs_u32;
#endif
struct om_chanset_entry {
    om_cs_u32 primary, secondary_offset, bandwidth, center;
    om_cs_u32 invalid, inactive, cca[4], primary_cca[2];
    om_cs_u32 scan_cca, cci_instant, aci_instant, cci_longterm;
    om_cs_u32 aci_longterm, range_cost, dfs, cost;
};
struct om_chanset_view {
    const struct om_chanset_entry *entries;
    unsigned int count, capacity;
};
enum om_chanset_band { OM_CHANSET_BAND24 = 0, OM_CHANSET_BAND5 = 1 };
int om_chanset_template(enum om_chanset_band band, unsigned int bandwidth,
                        struct om_chanset_view *output);
int om_chanset_find(const struct om_chanset_view *view, unsigned int channel,
                    unsigned int bandwidth, unsigned int secondary_offset,
                    const struct om_chanset_entry **output);

typedef struct { om_cs_u32 cells[7], current_bandwidth; } om_chanset_scan_queue;
struct om_chanset_rank_params {
    om_cs_u32 cci_instant_factor, aci_instant_factor;
    om_cs_u32 cci_longterm_factor, aci_longterm_factor;
    om_cs_u32 range_factor, dfs_factor;
    int min_cochannel_rssi;
    om_cs_u32 max_bandwidth_benefit, dense_cci_span;
};
int om_chanset_add_scan_type(om_chanset_scan_queue *queue,
                            enum om_chanset_band band, unsigned bandwidth);
int om_chanset_rank_defaults(enum om_chanset_band band, int alternate,
                             struct om_chanset_rank_params *output);
int om_chanset_edges(const struct om_chanset_entry *entry,
                     enum om_chanset_band band, unsigned neighborhood,
                     unsigned dense_span, int *low, int *high);
int om_chanset_cost(const struct om_chanset_entry *entry,
                    const struct om_chanset_rank_params *params,
                    om_cs_u32 *output);

struct om_chanset_scan_sample {
    om_cs_u32 bandwidth, cca_idle, cca_busy, cca_tx, cca_interference, attempts;
    om_cs_u32 cca_primary, cca_secondary, cca_secondary40;
    om_cs_u32 beacons, crc_errors, long_preamble_errors, short_preamble_errors;
};
int om_chanset_normalize_sample(const struct om_chanset_scan_sample *input,
                                struct om_chanset_scan_sample *output);
int om_chanset_update_scan_entry(struct om_chanset_entry *entry,
                                const struct om_chanset_scan_sample *scaled);
int om_chanset_select(const struct om_chanset_view *view,
                      enum om_chanset_band band, unsigned maximum_bandwidth,
                      unsigned current_scan_bandwidth, int automatic_bandwidth,
                      unsigned minimum_benefit,
                      const struct om_chanset_entry **output);

/* Per-radio transaction storage is caller-owned and bounded by original98
 * widest-band table records. It must not be allocated on a kernel stack. */
struct om_chanset_transaction {
    struct om_chanset_entry entries[98];
    om_chanset_scan_queue queue;
    struct om_chanset_rank_params params;
    unsigned count, generation, busy, active;
    unsigned initial_flags;
    enum om_chanset_band band;
};
int om_chanset_prepare(struct om_chanset_transaction *transaction,
                       enum om_chanset_band band, unsigned current_bandwidth,
                       int alternate_rank, int automatic_bandwidth);
int om_chanset_scan_request(const struct om_chanset_transaction *transaction,
                            unsigned initial_flags, unsigned *flags,
                            unsigned *secondary_offset);
int om_chanset_scan_advance(struct om_chanset_transaction *transaction);
int om_chanset_scan_sample(struct om_chanset_transaction *transaction,
                           unsigned channel, unsigned secondary_offset,
                           const struct om_chanset_scan_sample *input);
int om_chanset_end(struct om_chanset_transaction *transaction);

struct om_chanset_observation {
    unsigned channel, bandwidth, secondary_offset;
    int rssi_dbm;
};
struct om_chanset_channel_state {
    unsigned allowed, inactive, dfs;
    om_cs_u32 power_range_cost;
};
int om_chanset_rank(struct om_chanset_transaction *transaction,
                    const struct om_chanset_channel_state *channels,
                    unsigned channel_count,
                    const struct om_chanset_observation *neighbors,
                    unsigned neighbor_count, unsigned neighborhood);
#endif
