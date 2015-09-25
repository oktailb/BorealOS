#include <my.h>

int	my_putnbr_base(int nbr, char *base)
{
  int		res;
  int		len;
  static int	total = 0;

  res = 0;
  len = my_strlen(base);
  if (nbr < 0)
    {
      my_putchar('-');
      nbr = -nbr;
    }
  if (nbr >= len)
    my_putnbr_base((nbr - (nbr % len))/len, base);
  total += my_putchar(base[nbr % len]);
  return (total);
}

int	my_putnbr_base_pad(int nbr, char *base, int pad, char pad_char)
{
  int		res;
  int		len;
  static int	total = 0;

  pad--;
  res = 0;
  len = my_strlen(base);
  if (nbr < 0)
    {
      my_putchar('-');
      nbr = -nbr;
    }
  if (nbr >= len)
    my_putnbr_base_pad((nbr - (nbr % len))/len, base, pad, pad_char);
  else
    while ((pad--)>0)
      my_putchar(pad_char);
  total += my_putchar(base[nbr % len]);
  return (total);
}

int	my_putnbr(int nbr)
{
  char	*base = "0123456789";
  
  return (my_putnbr_base(nbr, base));
}

int	my_putnbr_HEXA(int nbr)
{
  char	*base = "0123456789ABCDEF";
  
  return (my_putnbr_base(nbr, base));
}

int	my_putnbr_hexa(int nbr)
{
  char	*base = "0123456789abcdef";
  
  return (my_putnbr_base(nbr, base));
}
