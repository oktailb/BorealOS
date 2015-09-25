#include <ch.h>

void		insert_ch(t_ch_int *prev, t_ch_int* new)
{
  new->prev = prev;
  new->next = prev->next;
  prev->next->prev = new;
  prev->next = new;
}
