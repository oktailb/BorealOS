#include <my.h>

int     match(char *s1, char *s2)
{
  int   i;

  if (*s1 == *s2)
    return ((!(*s1)) ? 1 : match(++s1, ++s2));
  if (*s2 == '*')
    {
      if (!(*(s2++)))
        return (1);
      i = match(s1, s2);
      while (*(s1++))
        i += match(s1, s2);
      i = (i > 0) ? 1 : i;
      return (i);
    }
  return (0);
}
