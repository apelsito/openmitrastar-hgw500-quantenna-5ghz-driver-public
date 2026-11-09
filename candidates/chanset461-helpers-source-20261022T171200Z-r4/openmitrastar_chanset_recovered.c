/* Exact first-match semantics from OEM FUN_ram_00012f9c, using typed
 * fields rather than OEM raw offsets. Invalid bounds refuse before dereference.
 * Template selection is OEM FUN_ram_00012f18 for bands 0 and 1; out-of-range
 * band values fail explicitly instead of treating any nonzero as band5.
 */
#include "openmitrastar_chanset_recovered.h"
#include "openmitrastar_chanset_tables.h"
typedef char om_cs_size_is_80[(sizeof(struct om_chanset_entry) == 80) ? 1 : -1];
int om_chanset_template(enum om_chanset_band band, unsigned int bandwidth,
                        struct om_chanset_view *output)
{
    struct om_chanset_view selected = {0,0,0};
    if (!output) return -22;
    if (band == OM_CHANSET_BAND24) {
        if (bandwidth == 20) {
            selected.entries = om_cs_band24_bw20; selected.count = 14;
        } else if (bandwidth == 40) {
            selected.entries = om_cs_band24_bw40; selected.count = 18;
        }
    } else if (band == OM_CHANSET_BAND5) {
        if (bandwidth == 20) {
            selected.entries = om_cs_band5_bw20; selected.count = 30;
        } else if (bandwidth == 40) {
            selected.entries = om_cs_band5_bw40; selected.count = 28;
        } else if (bandwidth == 80) {
            selected.entries = om_cs_band5_bw80; selected.count = 24;
        } else if (bandwidth == 160) {
            selected.entries = om_cs_band5_bw160; selected.count = 16;
        }
    }
    if (!selected.entries) return -22;
    selected.capacity = selected.count;
    *output = selected;
    return 0;
}
int om_chanset_find(const struct om_chanset_view *view, unsigned int channel,
                    unsigned int bandwidth, unsigned int secondary_offset,
                    const struct om_chanset_entry **output)
{
    unsigned int index;
    if (!view || !output || !view->entries || view->count > view->capacity ||
        view->capacity > 98U) return -22;
    for (index = 0; index < view->count; index++) {
        const struct om_chanset_entry *entry = &view->entries[index];
        if (entry->primary == channel && entry->bandwidth == bandwidth &&
            entry->secondary_offset == secondary_offset) {
            *output = entry;
            return 1;
        }
    }
    *output = 0;
    return 0;
}

/* Recovered OEM 0x131a8: queue7, BW40 band24 uses ABOVE3/BELOW4.
 * OEM full queue restarts at0; this helper refuses instead of overwriting.
 * Invalid bandwidth leaves both queue and current_bandwidth unchanged.
 */
int om_chanset_add_scan_type(om_chanset_scan_queue *queue,
                            enum om_chanset_band band, unsigned bandwidth)
{
    unsigned index = 0, first, second = 0;
    if (!queue || (band != OM_CHANSET_BAND24 && band != OM_CHANSET_BAND5))
        return -22;
    if (bandwidth == 20) first = 1;
    else if (bandwidth == 40) {
        first = band == OM_CHANSET_BAND24 ? 3 : 2;
        if (band == OM_CHANSET_BAND24) second = 4;
    } else if (bandwidth == 80 && band == OM_CHANSET_BAND5) first = 5;
    else if (bandwidth == 160 && band == OM_CHANSET_BAND5) first = 6;
    else return -22;
    while (index < 7 && queue->cells[index]) index++;
    if (index == 7 || (second && index >= 6)) return -28;
    {
        unsigned tail;
        /* Require one populated prefix followed by unused zero slots. */
        for (tail=index;tail<7;tail++) if (queue->cells[tail]) return -22;
    }
    queue->cells[index] = first;
    if (second) queue->cells[index+1] = second;
    queue->current_bandwidth = bandwidth;
    return 0;
}

/* Exact nine-word defaults selected by OEM 0x13314. Channel-policy permissions
 * stay with the caller; constant template availability does not allow TX. */
int om_chanset_rank_defaults(enum om_chanset_band band, int alternate,
                             struct om_chanset_rank_params *output)
{
    struct om_chanset_rank_params value;
    if (!output) return -22;
    if (band == OM_CHANSET_BAND24) {
        value.cci_instant_factor=0; value.aci_instant_factor=0;
        value.cci_longterm_factor=100; value.aci_longterm_factor=0;
    } else if (band == OM_CHANSET_BAND5) {
        value.cci_instant_factor=alternate?100:10;
        value.aci_instant_factor=alternate?20:5;
        value.cci_longterm_factor=alternate?10:100;
        value.aci_longterm_factor=alternate?5:20;
    } else return -22;
    value.range_factor=10; value.dfs_factor=band==OM_CHANSET_BAND24?0:(om_cs_u32)-30;
    value.min_cochannel_rssi=-80; value.max_bandwidth_benefit=2;
    value.dense_cci_span=5; *output=value; return 0;
}

/* OEM helpers 0x13514/0x13598. Use signed 64-bit intermediates and refuse an
 * unrepresentable edge; the recovered static templates remain within bounds. */
int om_chanset_edges(const struct om_chanset_entry *entry,
                     enum om_chanset_band band, unsigned neighborhood,
                     unsigned dense_span, int *low, int *high)
{
    long long base, span, center, lo, hi;
    if (!entry || !low || !high ||
        (band != OM_CHANSET_BAND24 && band != OM_CHANSET_BAND5)) return -22;
    base=entry->primary<36?2407:(entry->primary<183?5000:4000);
    span=(band==OM_CHANSET_BAND24 && neighborhood==2)?dense_span:entry->bandwidth/2;
    center=(long long)entry->center*5+base;lo=center-span;hi=center+span;
    if (lo < (-2147483647LL-1) || lo>2147483647LL ||
        hi < (-2147483647LL-1) || hi>2147483647LL) return -34;
    *low=(int)lo;*high=(int)hi;return 0;
}

/* Exact 88-byte ARC hardware-loop body0x1488e..0x148e6. Ghidra omits this
 * non-procedure loop; original text bytes and ARC objdump supply the evidence.
 * ARC mpy/add/sub use low32 bits; preserve that wrap using unsigned arithmetic.
 * The final subtract is unconditional, even when entry->dfs is zero.
 */
int om_chanset_cost(const struct om_chanset_entry *entry,
                    const struct om_chanset_rank_params *params,
                    om_cs_u32 *output)
{
    om_cs_u32 cost;
    if (!entry || !params || !output) return -22;
    cost=entry->aci_instant*params->aci_instant_factor;
    cost+=entry->cci_instant*params->cci_instant_factor;
    cost+=entry->aci_longterm*params->aci_longterm_factor;
    cost+=entry->cci_longterm*params->cci_longterm_factor;
    cost+=entry->dfs*params->dfs_factor;
    cost+=entry->range_cost*params->range_factor;
    cost-=params->dfs_factor;
    *output=cost;return 0;
}
