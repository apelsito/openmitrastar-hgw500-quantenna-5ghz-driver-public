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
#endif
