static void *qtn_memget(unsigned long nbytes)
{
	struct memblock *memlist = &s_memlist;
	unsigned long flags;
	register struct memblock *prev, *curr, *leftover;

	if(0 == nbytes)
	{
		return NULL;
	}

	/* Round to multiple of memblock size   */
	nbytes = (unsigned long) roundmb(nbytes);

	spin_lock_irqsave(&s_heap_lock, flags);

	prev = memlist;
	curr = memlist->next;

	while(curr != NULL)
	{
		if(curr->length == nbytes)
		{
			prev->next = curr->next;
			memlist->length -= nbytes;

			goto return_curr;
		}
		else if(curr->length > nbytes)
		{
			/* Split block into two */
			leftover = (struct memblock *)((unsigned long) curr + nbytes);
			prev->next = leftover;
			leftover->next = curr->next;
			leftover->length = curr->length - nbytes;
			memlist->length -= nbytes;

			goto return_curr;
		}

		prev = curr;
		curr = curr->next;
	}

	spin_unlock_irqrestore(&s_heap_lock, flags);
	return NULL;

return_curr:
	spin_unlock_irqrestore(&s_heap_lock, flags);
	return (void *)curr;
}
