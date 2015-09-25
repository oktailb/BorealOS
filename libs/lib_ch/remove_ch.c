#include <ch.h>

void		remove_ch(t_ch_int *to_die)
{
  to_die->next->prev = to_die->prev;
  to_die->prev->next = to_die->next;
  free(to_die->data);
  free(to_die);
}
