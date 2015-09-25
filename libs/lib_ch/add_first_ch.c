#include <ch.h>

void		add_first_ch(t_ch_int *list, t_ch_int *new)
{
  t_ch_int	*first;

  for (first = list; first->prev != 0; first = first->prev);
  new->next = first;
  first->prev = new;
  new->prev = 0;
  list = new;
}
