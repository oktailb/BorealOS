#include <my.h>

int	xopen(char *fname, int mode)
{
  int	res;
  if ((res = open(fname, mode)) < 0)
    {
      my_printf("Impossible d'ouvrir %s\n", fname);
      exit(0);
    }
  return (res);
}
