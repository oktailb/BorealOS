#include <my.h>

char	*bistro_add(char *a, char *b, char *base)
{
  char	*res;
  int	size;
  int	i;
  int	tmp;
  int	ret;
  int	len;

  a = clean(a, base, "()/*-+");
  b = clean(b, base, "()/*-+");
  len = my_strlen(b);
  ret = 0;
  if (b[0] == '-')
    return (bistro_sub(a, b+1, base));
  if (a[0] == '-')
    return (bistro_sub(b, a+1, base));
  if (my_strlen(a) < len)return (bistro_add(b, a, base));
  size = my_strlen(a) + 1;
  res = xmalloc(size * sizeof(*res));
  for (i = len; i >= 0; i--)res[size - len + i] = b[i];
  for (i = 0; i < (size - len); i++)res[i] = base[0];
  len = my_strlen(base);
  for (i = (size - 1); i > 0; i--)
    {
      if ((base2nbr(a[i-1], base) < 0) || (base2nbr(res[i], base) < 0))
	{
	  my_printf("*** %s (%c) *** %s (%c) *** %s *** (%d)\n", a, a[i-1], res, res[i], base, i);
	  return("Base ou operande incorrects\n");
	}
      tmp = base2nbr(a[i - 1], base) + base2nbr(res[i], base) + ret;
      ret = ( tmp  >= len ) ? (tmp / len) : 0;
      tmp = ( tmp  >= len ) ? (tmp % len) : tmp;
      res[i] = nbr2base(tmp , base);
    }
  res[i] = nbr2base(ret, base);
  for (i = 0; res[i] == base[0]; res++);
  return (res);
}

