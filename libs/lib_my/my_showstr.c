#include <my.h>

int my_showstr(char *str)
{
  int	total;

  total = 0;
  while (*str)
    {
      if (!(((*str) < 32) ^ ((*str) >= 127)))
	{
	  total++;
	  my_putchar(*str);
	}
      else
	{
	  total += 4;
	  my_putchar('\\');
	  my_putnbr_base(*str, "01234567");
	}
      str++;
    }
  return (total+1);
}
