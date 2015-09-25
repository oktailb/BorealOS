#include <my.h>

void *xmalloc(int size)
{
  void *res;

  if((res = malloc(size)) == 0)
    {
      my_printf("Malloc impossible\n");
      exit (0);
    }
  return (res);
}
