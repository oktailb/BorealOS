#include <my.h>

int	my_pow(int e, int x)
{
  if (x == 1)
    return (e);
  else
    {
      if ((x & 0x01) == 0x00)
	return (my_pow(e, x>>1) * my_pow(e, x>>1));
      else
	return (my_pow(e, x>>1) * my_pow(e, x>>1) * e);
    }
  return (0);
}

char	*bistro_my_pow(char *e, char *x, char *base)
{
  char	term[2];

  term[0] = base[1];
  term[1] = '\0';
  if (my_strcmp(x, term) == 0)
    return (e);
  if ((x[my_strlen(x) - 1] & 0x01) == 0)
    return (bistro_mult(bistro_my_pow(my_strdup(e), div2(my_strdup(x), base), base),\
			bistro_my_pow(my_strdup(e), div2(my_strdup(x), base), base),\
			base));
  else
    return (bistro_mult(bistro_mult(\
				    bistro_my_pow(my_strdup(e), div2(my_strdup(x), base), base),\
				    bistro_my_pow(my_strdup(e), div2(my_strdup(x), base), base),\
				    base),\
			my_strdup(e),\
			base));
  return (NULL);
}
