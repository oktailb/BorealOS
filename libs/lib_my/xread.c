#include <my.h>

int	xread(int fd, void *buff, int size)
{
  int	res;

  if ((res = read(fd, buff, size)) < 0)
    {
      my_printf("pb read\n");
      exit(0);
    }
  return (res);
}
