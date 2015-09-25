#include <my.h>

int	main(int argc, char *argv[])
{
  if (argc >= 3)
    my_printf("%s\n", bistro_mult(argv[1], argv[2], argv[3]));
  return (0);
}
