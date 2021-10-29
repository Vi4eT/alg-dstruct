#include <stdio.h>
#include <math.h>
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
	return 0;
}

void* memalloc(int size)
{
	void *ptr = NULL;
	handle *cur = head, *buf = NULL, *fit = NULL;
	if (!head || size < 0)
		return NULL;
	if (last)
		cur = last->next;
	while (cur)
	{
		if (-cur->size >= size)
		{
			fit = cur;
			break;
		}
		buf = cur;
		cur = cur->next;
	}
	if (!fit)
	{
		if (last)
		{
			cur = head;
			while (cur != last->next)
			{
				if (-cur->size >= size)
				{
					fit = cur;
					break;
				}
				buf = cur;
				cur = cur->next;
			}
			if (!fit)
				return NULL;
		}
		else
			return NULL;
	}
	ptr = (void*)(fit + 1);
	fit->size = -fit->size;
	if (fit->size - size > memgetblocksize())
	{
		buf = (handle*)((char*)ptr + size);
		buf->next = fit->next;
		buf->size = -(fit->size - size - memgetblocksize());
		fit->next = buf;
		fit->size = size;
	}
	last = fit;
	return ptr;
}

void memfree(void *p)
{
	handle *cur = head, *prev = NULL;
	while (cur) 
	{
		if ((char*)p > (char*)(cur + 1)) 
		{
			prev = cur;
			cur = cur->next;
		}
		else if ((char*)p == (char*)(cur + 1))
			break;
		else
			return;
	}
	if (!cur || cur->size < 0)
		return;
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
		cur->size = -(abs(cur->size) + abs(cur->next->size) + memgetblocksize());
		cur->next = cur->next->next;
	}
}

void memdone() 
{
	handle *h = head;
	while (h) 
	{
		if (h->size > 0)
			fprintf(stdout, "Leak: %d bytes at %p\n", h->size, h);
		h = h->next;
	}
}