#include <my.h>

int	my_memcpy(register char *dest, register char *src, register int size)
{
  register char		*bkp;

  bkp = src;
  while ((src-bkp) < size)
    *(dest++) = *(src++);
  return (src-bkp);
}
