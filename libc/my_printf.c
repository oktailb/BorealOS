#include <my.h>
#include <stdarg.h>

int	do_aff(char type, va_list *args, int out)
{
  int	n;
  int	*ptrn;
  char	*s;
  char	c;
  
  if (type == 'c'){c = va_arg(*args, int);return (my_putchar(c));}
  if (type == '%'){return (my_putchar('%'));}
  if (type == 's'){s = va_arg(*args, char*);return (my_putstr(s));}
  if (type == 'S'){s = va_arg(*args, char*);return (my_showstr(s));}
  if (type == 'X'){n = va_arg(*args, int);return (my_putnbr_HEXA(n));}
  if (type == 'x'){n = va_arg(*args, int);return (my_putnbr_hexa(n));}
  if (type == 'd'){n = va_arg(*args, int);return (my_putnbr(n));}
  if (type == 'b'){n = va_arg(*args, int);return (my_putnbr_base_pad(n, "01", 8, '0'));}
  if (type == 'n'){ptrn = va_arg(*args, int*);*ptrn = out;return (0);}
  return (0);
}

void		my_printf(char *str, ...)
{
  va_list	args;
  char		*bkp;
  int		out;

  out = 0;
  bkp = str;
  va_start(args, str);
  while ((*str) != '\0')
    {
      if (*str == '%')
	{
	  out += do_aff(*(str+1), &args, out);
	  str++;
	}
      else
	out += my_putchar(*str);
      str++;
    }
  va_end (args);
}
