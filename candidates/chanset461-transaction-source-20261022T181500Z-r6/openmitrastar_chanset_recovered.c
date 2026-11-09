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
    while (index < 7 && queue->cells[index]) {
        if (queue->cells[index]>6U) return -22;
        index++;
    }
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
    if (!entry || !low || !high || low==high ||
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

typedef char om_cs_scan_size_is_52[(sizeof(struct om_chanset_scan_sample)==52)?1:-1];
static om_cs_u32 om_cs_scale(om_cs_u32 value, om_cs_u32 denominator)
{
    /* Exact OEM threshold0x418937: preserve divide-first truncation above it. */
    return value<0x418937U?(value*1000U)/denominator:(value/denominator)*1000U;
}
int om_chanset_normalize_sample(const struct om_chanset_scan_sample *input,
                                struct om_chanset_scan_sample *output)
{
    struct om_chanset_scan_sample value;
    if(!input||!output||!input->attempts)return -11;
    value=*input;
    value.cca_idle=om_cs_scale(input->cca_idle,input->attempts);
    value.cca_busy=om_cs_scale(input->cca_busy,input->attempts);
    value.cca_tx=om_cs_scale(input->cca_tx,input->attempts);
    value.cca_interference=om_cs_scale(input->cca_interference,input->attempts);
    value.cca_primary=om_cs_scale(input->cca_primary,input->attempts);
    value.cca_secondary=om_cs_scale(input->cca_secondary,input->attempts);
    value.cca_secondary40=om_cs_scale(input->cca_secondary40,input->attempts);
    value.beacons=om_cs_scale(input->beacons,input->attempts);
    value.crc_errors=om_cs_scale(input->crc_errors,input->attempts);
    value.long_preamble_errors=om_cs_scale(input->long_preamble_errors,input->attempts);
    value.short_preamble_errors=om_cs_scale(input->short_preamble_errors,input->attempts);
    value.attempts=1000;*output=value;return 0;
}
int om_chanset_update_scan_entry(struct om_chanset_entry *entry,
                                const struct om_chanset_scan_sample *scaled)
{
    if(!entry||!scaled||scaled->attempts!=1000)return -22;
    entry->scan_cca=scaled->cca_interference;
    entry->cca[0]=scaled->cca_primary;
    entry->cca[1]=scaled->cca_secondary;
    entry->cca[2]=scaled->cca_secondary40;
    entry->primary_cca[0]=0;
    entry->cca[3]=scaled->cca_primary;
    return 0;
}
static int om_cs_signed_less(om_cs_u32 first, om_cs_u32 second)
{
    /* Order by the ARC signed branch without implementation-defined casts. */
    return (first^0x80000000U)<(second^0x80000000U);
}
static int om_cs_signed_le(om_cs_u32 first, om_cs_u32 second)
{
    return (first^0x80000000U)<=(second^0x80000000U);
}
int om_chanset_select(const struct om_chanset_view *view,
                      enum om_chanset_band band, unsigned maximum_bandwidth,
                      unsigned current_scan_bandwidth, int automatic_bandwidth,
                      unsigned minimum_benefit,
                      const struct om_chanset_entry **output)
{
    const struct om_chanset_entry *best[4]={0,0,0,0},*picked;
    unsigned i,index;
    if(!view||!output||!view->entries||view->count>view->capacity||view->capacity>98||
       (band!=OM_CHANSET_BAND24&&band!=OM_CHANSET_BAND5))return -22;
    for(i=0;i<view->count;i++) {
        const struct om_chanset_entry *entry=&view->entries[i];
        if(entry->bandwidth==20)index=0;
        else if(entry->bandwidth==40)index=1;
        else if(entry->bandwidth==80)index=2;
        else if(entry->bandwidth==160)index=3;
        else continue;
        if(entry->bandwidth>maximum_bandwidth||current_scan_bandwidth>entry->bandwidth||
           entry->invalid||entry->inactive)continue;
        if(!best[index]||om_cs_signed_less(entry->cost,best[index]->cost))best[index]=entry;
    }
    picked=best[3]?best[3]:(best[2]?best[2]:(best[1]?best[1]:best[0]));
    if(automatic_bandwidth) {
        if(band==OM_CHANSET_BAND24) {
            if(best[1]&&best[0]&&
               om_cs_signed_less(best[0]->cost*minimum_benefit,best[1]->cost))picked=best[0];
        } else if(best[3]&&best[2]) {
            picked=om_cs_signed_le(best[3]->cost,best[2]->cost*minimum_benefit)?best[3]:best[2];
        }
    }
    *output=picked;return picked?1:0;
}

int om_chanset_prepare(struct om_chanset_transaction *transaction,
                       enum om_chanset_band band, unsigned current_bandwidth,
                       int alternate_rank, int automatic_bandwidth)
{
    struct om_chanset_rank_params params;
    om_chanset_scan_queue queue={{0},0};
    struct om_chanset_view templates[4];
    unsigned width=current_bandwidth, groups=0, count=0, group, i;
    if(!transaction||transaction->busy||transaction->generation==~0U||
       (current_bandwidth!=20&&current_bandwidth!=40&&current_bandwidth!=80&&current_bandwidth!=160))return -22;
    if(om_chanset_rank_defaults(band,alternate_rank,&params))return -22;
    /* Validate the entire plan before changing transaction fields. */
    while(width>=20) {
        if(om_chanset_template(band,width,&templates[groups]))return -22;
        count+=templates[groups].count;
        if(count>98)return -28;
        groups++;width/=2;
    }
    width=(params.cci_instant_factor||params.aci_instant_factor)?current_bandwidth:20;
    if(om_chanset_add_scan_type(&queue,band,width))return -22;
    if(automatic_bandwidth&&width>20&&om_chanset_add_scan_type(&queue,band,width/2))return -22;
    /* Caller must serialize this state and keep its owning radio alive. */
    transaction->generation++;
    transaction->busy=1;
    transaction->active=1;
    transaction->band=band;
    transaction->params=params;
    transaction->queue=queue;
    transaction->count=count;
    count=0;
    for(group=0;group<groups;group++)for(i=0;i<templates[group].count;i++)
        transaction->entries[count++]=templates[group].entries[i];
    return 0;
}
int om_chanset_scan_request(const struct om_chanset_transaction *transaction,
                            unsigned initial_flags, unsigned *flags,
                            unsigned *secondary_offset)
{
    unsigned value, offset=0, selected;
    if(!transaction||!flags||!secondary_offset||flags==secondary_offset||
       !transaction->busy||!transaction->active)return -22;
    selected=transaction->queue.cells[0];value=initial_flags;
    if(selected<1||selected>6)return -22;
    if(selected>=2&&selected<=4)value|=0x100000U;
    else if(selected==5)value|=0x200000U;
    if(selected==3)offset=1;
    else if(selected==4)offset=3;
    /* OEM scan0x1361c: requests are returned to the caller. No scan is started
     * until a validated kernel/regulatory/lifetime adapter consumes them. */
    *flags=value;*secondary_offset=offset;return 0;
}
int om_chanset_scan_advance(struct om_chanset_transaction *transaction)
{
    unsigned i;
    if(!transaction||!transaction->busy||!transaction->active)return -22;
    for(i=0;i<6;i++)transaction->queue.cells[i]=transaction->queue.cells[i+1];
    transaction->queue.cells[6]=0;
    return transaction->queue.cells[0]?1:0;
}
int om_chanset_scan_sample(struct om_chanset_transaction *transaction,
                           unsigned channel, unsigned secondary_offset,
                           const struct om_chanset_scan_sample *input)
{
    struct om_chanset_scan_sample sample;
    struct om_chanset_view view;
    const struct om_chanset_entry *found;
    unsigned index;
    int result;
    if(!transaction||!input||!transaction->busy||!transaction->active||
       transaction->count>98||channel>=255)return -22;
    result=om_chanset_normalize_sample(input,&sample);
    if(result)return result;
    view.entries=transaction->entries;view.count=transaction->count;view.capacity=98;
    result=om_chanset_find(&view,channel,sample.bandwidth,secondary_offset,&found);
    if(result!=1)return result<0?result:-2;
    index=(unsigned)(found-transaction->entries);
    return om_chanset_update_scan_entry(&transaction->entries[index],&sample);
}
int om_chanset_end(struct om_chanset_transaction *transaction)
{
    if(!transaction||!transaction->busy)return -22;
    /* Does not release memory or buffers used by actual scan callbacks. The
     * kernel adapter must quiesce those callbacks before ending ownership. */
    transaction->active=0;transaction->busy=0;transaction->count=0;
    return 0;
}
