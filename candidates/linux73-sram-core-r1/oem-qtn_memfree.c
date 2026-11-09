static int qtn_memfree(void *memptr, unsigned long nbytes)
{
	struct memblock *memlist = &s_memlist;
	unsigned long flags;
	register struct memblock *block, *next, *prev;
	unsigned long top;

	block = (struct memblock *) memptr;
	nbytes = (unsigned long) roundmb(nbytes);

	spin_lock_irqsave(&s_heap_lock, flags);

	prev = memlist;
	next = memlist->next;
	while((next != NULL) && (next < block))
	{
		prev = next;
		next = next->next;
	}

	/* Find top of previous memblock */
	if(prev == memlist)
	{
		top = (unsigned long) NULL;
	}
	else
	{
		top = (unsigned long) prev + prev->length;
	}

	/* Make sure block is not overlapping on prev or next blocks */
	if ((top > (unsigned long) block)
		|| ((next != NULL) && ((unsigned long) block + nbytes) > (unsigned long)next))
	{
		spin_unlock_irqrestore(&s_heap_lock, flags);
		return -1;
	}

	memlist->length += nbytes;

	/* Coalesce with previous block if adjacent */
	if(top == (unsigned long) block)
	{
		prev->length += nbytes;
		block = prev;
	}
	else
	{
		block->next = next;
		block->length = nbytes;
		prev->next = block;
	}

	/* coalesce with next block if adjacent */
	if(((unsigned long) block + block->length) == (unsigned long) next)
	{
		block->length += next->length;
		block->next = next->next;
	}

	spin_unlock_irqrestore(&s_heap_lock, flags);

	return 0;
}
