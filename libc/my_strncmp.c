#include <my.h>

int	my_strncmp(char *str1, char *str2, int n)
{
  int	i;

  i = 0;
  while ((n != 1) && (str1[i] != '\0') && (str2[i] != '\0'))
    {
      if (str1[i] != str2[i])
	{
	  return (str1[i] - str2[i]);
	}
      i++;
      n--;
    }
  return (str1[i] - str2[i]);
}
