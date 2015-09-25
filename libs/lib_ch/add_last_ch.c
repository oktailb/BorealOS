#include <ch.h>

void		add_last_ch(t_ch_int *list, t_ch_int *new)
{
  t_ch_int	*last;
  
  for (last = list; last->next != 0; last = last->next);
  last->next = new;
  new->prev = last;
  new->next = 0;
  list = new;
}
