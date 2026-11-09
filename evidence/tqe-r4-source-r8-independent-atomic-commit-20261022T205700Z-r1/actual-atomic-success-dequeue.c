
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
struct topaz_congest_queue{unsigned total_qlen;};struct omcgq_queue_marker{bool active;};
static struct topaz_congest_queue oldq,newq;static struct topaz_congest_queue*omcgq_credit_queue;
static struct{unsigned completed,expected,published,ledger_slot;bool active;}omcgq_batch;
static unsigned omcgq_queue_pending,omcgq_queue_reserved,omcgq_held_count;static bool omcgq_reserved;static unsigned omcgq_held[256];

static unsigned irq,pending,head_commits;
static void producer(void){assert(!omcgq_batch.active&&!omcgq_queue_reserved&&!omcgq_queue_pending);assert(head_commits==1);omcgq_credit_queue=&newq;omcgq_queue_reserved=2;omcgq_batch.active=true;omcgq_held_count++;}
static void injected_producer(void){if(irq)pending=1;else producer();}
#define local_irq_save(f) do{(f)=irq;irq=1;}while(0)
#define local_irq_restore(f) do{irq=(f);if(!irq&&pending){pending=0;producer();}}while(0)
static void topaz_congest_dequeue(void*q){(void)q;assert(irq);head_commits++;}
static void omcgq_typed_success(struct omcgq_queue_marker *marker)
{
	if (!marker || !marker->active)
		return;
	marker->active = false;
	if (omcgq_queue_pending)
		omcgq_queue_pending--;
	omcgq_batch.published++;
	if (!omcgq_queue_pending && omcgq_batch.completed == omcgq_batch.expected &&
	    omcgq_batch.published == omcgq_batch.expected && !omcgq_reserved) {
		memset(&omcgq_held[omcgq_batch.ledger_slot], 0, sizeof(omcgq_held[0]));
		omcgq_held_count--;
		omcgq_batch.active = false;
		omcgq_queue_reserved = 0;
		injected_producer();
		omcgq_credit_queue = NULL;
	}
}
int main(void){struct omcgq_queue_marker m={true};struct omcgq_queue_marker*typed=&m;void*queue=NULL;unsigned retry_irq_flags;omcgq_credit_queue=&oldq;omcgq_batch.completed=omcgq_batch.expected=1;omcgq_batch.active=true;omcgq_queue_pending=omcgq_held_count=1;
		if (typed) {
			local_irq_save(retry_irq_flags);
			omcgq_typed_success(typed);
			topaz_congest_dequeue(queue);
			local_irq_restore(retry_irq_flags);
		} else {
			topaz_congest_dequeue(queue);
		}

assert(omcgq_batch.active&&omcgq_queue_reserved==2&&omcgq_credit_queue==&newq&&!irq&&!pending&&head_commits==1);puts("{\"newbatch_credit_pointer_preserved\":true}");return 0;}