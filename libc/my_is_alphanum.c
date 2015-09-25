#include <my.h>

int	my_is_alphanum(char *str)
{
  int	i;

  i = 0;
  while (str[i] != '\0')
    {
      if ( ! ((str[i] >= 'a') && (str[i] <= 'z')))
	if ( ! ((str[i] >= 'A') && (str[i] <= 'Z')))
	  if ( ! ((str[i] >= '0') && (str[i] <= '9')))
	    return (0);
      i++;
    }
  return (1);
}

int	my_is_alpha(char *str)
{
  int	i;

  i = 0;
  while (str[i] != '\0')
    {
      if ( ! ((str[i] >= 'a') && (str[i] <= 'z')))
	if ( ! ((str[i] >= 'A') && (str[i] <= 'Z')))
	  return (0);
      i++;
    }
  return (1);
}

int	my_is_num(char *str)
{
  int	i;

  i = 0;
  while (str[i] != '\0')
    {
      if ( ! ((str[i] >= '0') && (str[i] <= '9')))
	return (0);
      i++;
    }
  return (1);
}
