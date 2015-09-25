#include <my.h>

char	*my_strstr(char* grande, char *petite)
{
  int	size;
  int	i;

  size = my_strlen(petite);
  i = 0;
  while(my_strncmp(&grande[i], petite, size) != 0)
    {
      i++;
    }
  if (i != size)
    return &grande[i];
  return (0);
}
