#include <my.h>

int		base2nbr(int n, char *base)
{
  register int	i;
  register int	len;

  len = my_strlen(base);
  for (i = 0; i < len; i++) 
    if (base[i] == n)
      return (i);
  return (-1);
}

char		*clean(char *a, char *base, char *oper)
{
  int		i;

  for (i = 0; i < my_strlen(a); i++)
    if (base2nbr(a[i], base) == -1)
      {
	if (base2nbr(a[i], oper) == -1)
	  {
	    a[i] = '\0';
	    return (a);
	  }
      }
  return(a);
}

char		*div2(char* a, char *base)
{
  register int	i;
  int		ret;
  register int	tmp;
  register int	len;

  len = my_strlen(base);
  ret = 0;
  i = 0;
  while (a[i] != '\0')
    {
      tmp = base2nbr(a[i], base);
      a[i] = nbr2base(tmp >> 1, base) + ret;
      ret = ((tmp % 2) == 1)?(len >> 1):0;
      i++;
    }
  for (i = 0; a[i] == base[0]; a++);
  return (a);
}

char		*mul2(char *a, char *base)
{
  return (bistro_add(my_strdup(a), my_strdup(a), base));
}
