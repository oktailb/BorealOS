/*
** my_getnbr_float_base.c for eval float in /u/prof/lecoq_v/Work/src/lib_my
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Fri Mar 18 10:15:24 2005 vincent lecoq
** Last update Fri Mar 18 10:22:44 2005 vincent lecoq
*/

#include <my.h>

float	my_getnbr_float_base(char *n, char *base)
{
  int	i;
  float	ret;
  float	div;
  float	mul;

  mul = 1;
  div = 1;
  ret = 0;
  i = 0;
  while (n[i] != '\0')
    {
      if (n[i] == '-')
        return (-my_getnbr(&n[i + 1]));
      if (n[i] == '.')
	{
	  mul = my_strlen(base);
	  i++;
	}
      if ((get_fpos_in_str(n[i], base) >= 0) && 
	  (get_fpos_in_str(n[i], base) <= (my_strlen(base) - 1)))
        ret = (ret * my_strlen(base) + get_fpos_in_str(n[i], base));
      else
        return (ret / div);
      div *= mul;
      i++;
    }
  return (ret / div);
}
