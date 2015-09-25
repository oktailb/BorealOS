#include <ch.h>

void		add_data_ch(t_ch_int *dest, void *data, int size)
{
  dest->data = xmalloc(size);
  my_memcpy(dest->data, data, size);
}
