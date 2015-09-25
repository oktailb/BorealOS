/*
** get_next_line.c for getNL in /u/prof/lecoq_v/Work/src/lib_my
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Thu Jun 16 13:56:13 2005 vincent lecoq
** Last update Wed Oct 19 19:47:58 2005 vincent lecoq
*/

#include <my.h>

#define		BUFF_SIZE	4096

static char	buff[BUFF_SIZE];
static int	in_buf = 0;
static int	start;

int	read_from_file(int fd)
{
  in_buf = read(fd, buff, BUFF_SIZE);
  start = 0;
  return (in_buf);
}

char	*add_to_line(char *line, int cur)
{
  char	*new;
  int	old_len;

  new = xmalloc((old_len = (line ? my_strlen(line) : 0)) + cur + 1);
  if (line)
    {
      my_memcpy(new, line, old_len);
      free(line);
    }
  my_memcpy(new + old_len, buff + start, cur);
  new[old_len + cur] = 0;
  start += cur + 1;
  return (new);
}

char	*get_next_line(int fd)
{
  int	cur;
  char	*line;

  for (line = 0, cur = 0; ; cur++)
    {
      if (start >= in_buf)
        {
          if (!read_from_file(fd))
            return (line);
          cur = 0;
        }
      if (buff[start + cur] == '\n')
        {
          line = add_to_line(line, cur);
          return (line);
        }
      if (start + cur == in_buf - 1)
        line = add_to_line(line, cur + 1);
    }
}
