#include <ch.h>

void	close_ch(t_ch_int *first)
{
  t_ch_int	*last;

  if (first->prev == 0)
    {
      for (last = first; last->next != 0; last = last->next);
      last->next = first;
      first->prev = last;
    }
}
