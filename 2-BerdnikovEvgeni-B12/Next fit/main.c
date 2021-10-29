#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "memallocator.h"
#define ITERS 2000

int main(void)
{
	FILE *f = NULL;
	LARGE_INTEGER freq, c1, c2;
	int i = 0, j = 0;
	double t1 = 0;
	void *p[ITERS] = { NULL }, *pin = NULL;
	if (fopen_s(&f, "output.csv", "w"))
		return 1;
	pin = malloc((ITERS + 1 + memgetblocksize() + memgetminimumsize()) * ITERS / 2);
	if (!pin)
	{
		fclose(f);
		return 1;
	}
	if (meminit(pin, (ITERS + 1 + memgetblocksize() + memgetminimumsize()) * ITERS / 2))
	{
		free(pin);
		fclose(f);
		return 1;
	}
	QueryPerformanceFrequency(&freq);
	for (i = 1; i <= ITERS; i++)
	{
		QueryPerformanceCounter(&c1);
		for (j = 0; j < i; j++)
			p[j] = malloc(j);
		for (j = 0; j < i; j++)
			free(p[j]);
		QueryPerformanceCounter(&c2);
		t1 = (c2.QuadPart - c1.QuadPart) / (double)freq.QuadPart;
		QueryPerformanceCounter(&c1);
		for (j = 0; j < i; j++)
		{
			p[j] = memalloc(j);
			if (!p[j])
				printf("%d\n", j);
		}
		for (j = 0; j < i; j++)
			memfree(p[j]);
		QueryPerformanceCounter(&c2);
		memdone();
		fprintf(f, "%lf;%lf\n", t1, (c2.QuadPart - c1.QuadPart) / (double)freq.QuadPart);
	}
	free(pin);
	fclose(f);
	return 0;
}