#include <my.h>

int	get_fpos_in_str(char c, char *str)
{
  int	i;
  
  i = 0;
  while (str[i] != '\0')
    {
      if (str[i] == c)
	return (i);
      i++;
    }
  return (-1);
}

int	my_getnbr_base(char *nbr, char* base)
{
  int	res;
  int	i;
  int	size;
  int	signe;
  int	pos;
  
  pos = 0;
  size = my_strlen(base);
  signe = 1;
  res = 0;
  i = 0;
  while (nbr[i] != '\0')
    {
      if (nbr[i] == '-')
	signe *= -1;
      else
	{
	  pos = get_fpos_in_str(nbr[i], base);
	  if (pos != -1)
	    res = res * size + pos;  
	  else
	    return (res * signe);
	}
      i++;
    }
  return (res * signe);
}

int	my_getnbr(char *nbr)
{
  return (my_getnbr_base(nbr, "0123456789"));
}
