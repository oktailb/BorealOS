#include <my.h>

void	*xmalloc(int size)
{
  void	*res;

  if((res = malloc(size)) == 0)
    {
      perror("Malloc :");
      exit (0);
    }
  return (res);
}
