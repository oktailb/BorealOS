#include <my.h>

int	my_putchar(char c)
{
  write(1, &c, 1);
  return (1);
}
