/*
  CPU: QuadCore AMD A10-9600P, 2400 MHz
  RAM: Samsung 4+2 GB DDR4 1866 MHz 13-13-13-31
  SSD: WD Blue WDS500G2B0A-00SM50 (500 GB, SATA-III)
  OS: Microsoft Windows 10 Pro 10.0.19043.1288 (Win10 21H1 May 2021 Update)

  Test results:
    n = 20000
    3.907225 sec
    1537.7 MB
*/
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int *adj = NULL, *check = NULL, *result = NULL, step = 0;

int memalloc(int n)
{
  adj = (int*)malloc(n * n * sizeof(int));
  check = (int*)malloc(n * sizeof(int));
  result = (int*)malloc(n * sizeof(int));
  if (!adj || !check || !result)
    return 1;
  return 0;
}

void memfree(void)
{
  free(adj);
  free(check);
  free(result);
}

void search(int v, int n)
{
  result[step++] = v;
  check[v] = 1;
  for (int i = 0; i < n; i++)
    if (adj[v * n + i] == 1 && check[i] != 1)
      search(i, n);
}

void dfs(int n)
{
  for (int i = 0; i < n; i++)
    if (check[i] != 1)
      search(i, n);
}

double test(int n)
{
  LARGE_INTEGER freq, c1, c2;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      adj[i * n + j] = rand() % 2;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&c1);
  dfs(n);
  QueryPerformanceCounter(&c2);
  return (c2.QuadPart - c1.QuadPart) / (double)freq.QuadPart;
}

int main(void)
{
  int a = 0, b = 0, n = 0;
  while (scanf("%d", &n) != 1 || n == 0);
  if (n > 0)
  {
    if (memalloc(n))
    {
      memfree();
      return 1;
    }
    while (scanf("%d %d", &a, &b) != EOF)
    {
      adj[a * n + b] = 1;
      adj[b * n + a] = 1;
    }
    dfs(n);
    for (int i = 0; i < n; i++)
      printf("%d ", result[i]);
    memfree();
  }
  else
  {
    if (memalloc(-n))
    {
      memfree();
      return 1;
    }
    printf("%lf", test(-n));
    memfree();
  }
  return 0;
}