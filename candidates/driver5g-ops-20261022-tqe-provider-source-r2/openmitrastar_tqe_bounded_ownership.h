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
#endif
