#include <my.h>

char	*bistro_sub(char *a, char *b, char *base)
{
  int	len;
  int	i;
  char	*res;
  int	tmp;
  int	ret;

  ret = 0;
  len = my_strlen(a);
  my_printf("soustraction de %s et %s\n", a, b);
  if (len < my_strlen(b))
    return (bistro_sub(b, a, base));
  res = my_strdup(a);
  for (i = len-1; i >= len - my_strlen(b); i--)
    {
      tmp = base2nbr(res[i], base);
      if (tmp <  (base2nbr(b[my_strlen(b) - len + i], base) + ret))
	{
	  tmp += my_strlen(base);
	  ret = 1;
	}
      else
	ret = 0;
      tmp -= (base2nbr(b[my_strlen(b) - len + i], base));
      res[i] = nbr2base(tmp, base);
    }
  res[i] -= ret;
  for (; res[i] == base[0]; res++);
  return (res);
}
