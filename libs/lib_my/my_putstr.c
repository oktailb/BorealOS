#include <my.h>

int	my_putstr(char *str)
{
  if (my_strlen(str) != 0)
    return (write(1, str, my_strlen(str)));
  return (0);
}
