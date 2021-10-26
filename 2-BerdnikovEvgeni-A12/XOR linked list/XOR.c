#include "XOR.h"

list* Init(char *data)
{
  list *first = (list*)malloc(sizeof(list));
  if (!first)
    return NULL;
  first->data = data;
  first->addr = NULL;
  return first;
}

int Add(list **lst, char *data)
{
  list *buf = (list*)malloc(sizeof(list));
  if (!buf)
    return 1;
  buf->data = data;
  buf->addr = *lst;
  (uintptr_t)(*lst)->addr ^= (uintptr_t)buf;
  *lst = buf;
  return 0;
}

void Iter(list **first, list **second)
{
  list *buf = *first; 
  *first = *second;
  *second = (list*)((uintptr_t)buf ^ (uintptr_t)(*first)->addr);
}

void Term(list *lst)
{
  list *xor = lst->addr, *buf = NULL;
  while (xor)
  {
    buf = lst;
    Iter(&lst, &xor);
    free(buf);
  }
  free(lst);
}

list* Find(list **first, list **second, char *data)
{
  while (*first)
    if (!second || !(*second)->data)
      Iter(second, first);
    else if (strcmp((*second)->data, data))
      Iter(second, first);
    else
      return *second;
  if (second && (*second)->data)
    if (!strcmp((*second)->data, data))
      return *second;
  return NULL;
}

void Del(list *first, list *del)
{
  list *second = (list*)((uintptr_t)first ^ (uintptr_t)del->addr);
  if (first)
    first->addr = (list*)(((uintptr_t)first->addr ^ (uintptr_t)del) ^ (uintptr_t)second);
  if (second)
    second->addr = (list*)(((uintptr_t)second->addr ^ (uintptr_t)del) ^ (uintptr_t)first);
  free(del);
}

void KeyDel(list *lst, char *data)
{
  list *first = lst->addr, *del = Find(&first, &lst, data);
  if (del)
    Del(first, del);
}

void AddrDel(list *lst, list *del)
{
  list *first = lst->addr;
  while (first && lst != del)
    Iter(&lst, &first);
  if (lst && lst == del)
    Del(first, lst);
}