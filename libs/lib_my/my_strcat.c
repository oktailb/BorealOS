#include <my.h>

char	*my_strcat(char *debut, char *fin)
{
  char	*dest;

  dest = xmalloc((my_strlen(debut) + my_strlen(fin)) * sizeof(*debut) + 1);
  my_strcpy(dest, debut);
  my_strcpy(&dest[my_strlen(debut)], fin);
  return (dest);
}

char	*my_strncat(char *debut, char *fin, int n)
{
  char	*dest;

  dest = xmalloc((my_strlen(debut) + n) * sizeof(*debut) + 1);
  my_strcpy(dest, debut);
  my_strncpy(&dest[my_strlen(debut)], fin, n);
  return (dest);
}
