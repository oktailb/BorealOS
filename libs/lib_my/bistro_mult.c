#include <my.h>

/*
** Multiplication a la Russe
*/

char	*bistro_mult(char *a, char *b, char *base)
{
  char	*res;
  char	stop[2];

  res = xmalloc(2);
  res[0] = base[0];
  res[1] = '\0';
  stop[0] = base[1];
  stop[1] = '\0';
  while (my_strcmp(a, stop) != 0)
    {
      if ((base2nbr(a[my_strlen(a) - 1], base) & 0x01) == 0x01)
	res = bistro_add(my_strdup(b), my_strdup(res), base);
      a = div2(my_strdup(a), base);
      b = mul2(my_strdup(b), base);
    }
  if ((base2nbr(a[my_strlen(a) - 1], base) & 0x01) == 1)
    res = bistro_add(my_strdup(b), my_strdup(res), base);
  return (res);
}
