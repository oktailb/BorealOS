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
#include <math.h>

float   my_getnbr_base_float(char *nbr, char *base)
{
  int   i;
  float	res;
  char  *ipart;
  char  *fpart;
  char	*part;

  res = 0;
  part = my_strdup(nbr);
  ipart = part;
  i = 0;
  while(part[i] != '.')
    i++;
  part[i] = '\0';
  fpart = &nbr[i + 1];
  res += my_getnbr_base(ipart, base);
  res += (my_getnbr_base(fpart, base)/(float)my_pow(my_strlen(base), my_strlen(fpart)));
  free(part);
  return (res);
}

