#include <my.h>

int get_file_size(int fd)
{
  struct stat	*st;

  st = xmalloc(sizeof(*st));
  fstat(fd, st);
  return (st->st_size);
}
