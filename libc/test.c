#include <my.h>

int	main(int argc, char *argv[])
{
  int	t;
  my_printf("%S   !%n\n", "abcdefghijkmnopqr\011stuvwxyz", &t);
  my_putnbr(t);
  my_putchar('\n');
  return (0);
}
