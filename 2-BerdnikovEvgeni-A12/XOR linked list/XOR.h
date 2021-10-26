#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <string.h>

typedef struct list
{
  char *data;
  struct list *addr;
}list;

list* Init(char *data);
int Add(list **lst, char *data);
void Iter(list **first, list **second);
void Term(list *lst);
list* Find(list **first, list **second, char *data);
void Del(list *lst, list *del);
void KeyDel(list *lst, char *data);
void AddrDel(list *lst, list *del);

#ifdef __cplusplus
}
#endif