#include <my.h>

int	my_is_alphanum(char *str)
{
  int	i;
  char	*data;

  data = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-";
  i = 0;
  while (str[i] != '\0')
    {
      if ( ! is_in(str[i], data))
	return (0);
      i++;
    }
  return (1);
}

int	my_is_alpha(char *str)
{
  int	i;
  char	*data;

  data = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-";
  i = 0;
  while (str[i] != '\0')
    {
      if ( ! is_in(str[i], data))
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
      if ( ! is_in(str[i], "0123456789+-"))
	return (0);
      i++;
    }
  return (1);
}
