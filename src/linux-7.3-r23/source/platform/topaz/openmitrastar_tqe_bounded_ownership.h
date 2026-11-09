#ifndef OPENMITRASTAR_TQE_BOUNDED_OWNERSHIP_H
#define OPENMITRASTAR_TQE_BOUNDED_OWNERSHIP_H
#include <linux/types.h>
struct module;
union topaz_tqe_cpuif_ppctl;
/* Private provider-owned token, never a netdev/XMIT return code. */
struct omcgq_bank_token { u32 generation; struct module *owner; };
#define OMTQE_OWNERSHIP_UNCERTAIN 0x40
#define OMTQE_OWNERSHIP_REFUSED 0x41
int omcgq_bank_acquire(struct module *owner, struct omcgq_bank_token *token);
int omcgq_bank_token_valid(const struct omcgq_bank_token *token);
int omcgq_bank_cancel(const struct omcgq_bank_token *token);
int omcgq_bank_quarantine(const struct omcgq_bank_token *token,
	const union topaz_tqe_cpuif_ppctl *desc, bool published);
int omcgq_bank_retain(const union topaz_tqe_cpuif_ppctl *desc, bool published);
int omcgq_bank_quarantined(void);
int omcgq_bank_retained(void);

struct omcgq_batch_token { u32 generation; struct module *owner; };
int omcgq_batch_admit(struct module *owner, const union topaz_tqe_cpuif_ppctl *payload,
	u32 expected, struct omcgq_batch_token *batch);
int omcgq_batch_valid(const struct omcgq_batch_token *batch);
int omcgq_batch_acquire(const struct omcgq_batch_token *batch,
	struct omcgq_bank_token *token);
int omcgq_batch_complete(const struct omcgq_batch_token *batch, bool published);
int omcgq_batch_finish(const struct omcgq_batch_token *batch);

struct topaz_congest_q_desc;
struct topaz_congest_queue;
typedef int (*omcgq_batch_xmit_fn)(const struct omcgq_batch_token *,
	union topaz_tqe_cpuif_ppctl *);
int omcgq_batch_xmit_register(struct module *owner, omcgq_batch_xmit_fn fn);
int omcgq_batch_queue_admit(const struct omcgq_batch_token *batch,
	struct topaz_congest_q_desc *queue, union topaz_tqe_cpuif_ppctl *ppctl);
int omcgq_batch_has_queued(void);
int omcgq_batch_retry_acquire(const struct omcgq_batch_token *batch,
	struct omcgq_bank_token *token);
int omcgq_batch_queue_reserve(struct topaz_congest_queue *queue, u32 expected);
void omcgq_batch_queue_cancel_reserve(void);
#endif
