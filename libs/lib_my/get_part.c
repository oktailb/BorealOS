#include <math.h>
#include <my.h>

int		get_ipart(float n)
{
  return ((int)n);
}

int		get_fpart(float n)
{
  float		tmp;

  n = (n < 0)?-n:n;
  tmp = n - (int)n;
  while ((tmp - (int)tmp) != 0)
    tmp *= 10;
  return (tmp);
}
