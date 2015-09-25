#include <ch.h>

void	goto_first_ch(t_ch_int *list)
{
  for (; list->prev != 0; list = list->prev);
}
