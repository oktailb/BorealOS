#include <my.h>
#include <math.h>

int	my_putnbr_base_float(float nbr, char *base)
{
  int	total;
  int	ipart;
  int	fpart;

  total = 0;
  if (nbr < 0)
    {
      my_putchar('-');
      nbr *= -1;
    }
  ipart = get_ipart(nbr);
  fpart = get_fpart(nbr);
  total += my_putnbr_base(ipart, base);
  my_putchar('.');
  total++;
  total += my_putnbr_base(fpart, base);
  return (total);
}

int	my_putnbr_base_float_pad(float nbr, char *base, int pad, char pad_char)
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
    my_putnbr_base_pad((nbr - ((int)nbr % len))/len, base, pad, pad_char);
  else
    while ((pad--)>0)
      my_putchar(pad_char);
  total += my_putchar(base[(int)nbr % len]);
  return (total);
}

int	my_putnbr_float(float nbr)
{
  char	*base = "0123456789";
  
  return (my_putnbr_base_float(nbr, base));
}

int	my_putnbr_float_HEXA(float nbr)
{
  char	*base = "0123456789ABCDEF";
  
  return (my_putnbr_base_float(nbr, base));
}

int	my_putnbr_float_hexa(float nbr)
{
  char	*base = "0123456789abcdef";
  
  return (my_putnbr_base_float(nbr, base));
}
