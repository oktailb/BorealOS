#include <ch.h>

void	goto_last_ch(t_ch_int *list)
{
  for (; list->next != 0; list = list->next);
}
