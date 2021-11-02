#include <stdio.h>
#include "memallocator.h"

typedef struct handle 
{
	int size;
	struct handle *next;
}handle;
handle *head = NULL, *last = NULL;

int memgetminimumsize() 
{
	return sizeof(handle);
}

int memgetblocksize() 
{
	return sizeof(handle);
}

int meminit(void *pMemory, int size)
{
	if (!pMemory || size <= memgetminimumsize())
		return 1;
	head = (handle*)pMemory;
	head->size = memgetminimumsize() - size;
	head->next = NULL;
	last = head;
	return 0;
}

handle* find(int size)
{
	handle *cur = last;
	if (!head || size <= 0)
		return NULL;
	while (cur)
	{
		if (-cur->size >= size)
		{
			last = cur;
			return cur;
		}
		cur = cur->next;
	}
	cur = head;
	while (cur != last)
	{
		if (-cur->size >= size)
		{
			last = cur;
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

void* memalloc(int size)
{
	handle *fit = find(size);
	if (!fit)
		return NULL;
	void *ptr = (void*)(fit + 1);
	fit->size = -fit->size;
	if (fit->size - size > memgetblocksize()) 
	{
		handle *buf = (handle*)((char*)ptr + size);
		buf->next = fit->next;
		buf->size = -(fit->size - size - memgetblocksize());
		fit->next = buf;
		fit->size = size;
	}
	return ptr;
}

void memfree(void *p)
{
	handle *cur = (handle*)p - 1, *prev = NULL;
	if (!p || !cur || cur->size < 0)
		return;
	if (last == cur)
		last = head;
	if (cur != head)
	{
		prev = head;
		while (prev->next)
		{
			if (prev->next == cur)
				break;
			prev = prev->next;
		}
	}
	if (prev && prev->size < 0) 
	{
		prev->size = -(-prev->size + cur->size + memgetblocksize());
		prev->next = cur->next;
	}
	cur->size = -cur->size;
	if (cur->next && cur->next->size < 0)
	{
		if (prev && prev->size < 0)
			cur = prev;
		cur->size = -(-cur->size - cur->next->size + memgetblocksize());
		cur->next = cur->next->next;
	}
}

void memdone() 
{
	while (head) 
	{
		if (head->size > 0)
			fprintf(stderr, "Leak: %d bytes at %p\n", head->size, head);
		head = head->next;
	}
	last = NULL;
}