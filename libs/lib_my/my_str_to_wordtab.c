#include <my.h>

int			is_in(char c, char *tab)
{
  int			i;

  i = 0;
  while (tab[i] != '\0')
    {
      if (tab[i] == c)
	return (1);
      i++;
    }
  return (0);
}

int                     count_wordtab_misc(char *str, char *separator)
{
  int                   i;
  int                   mots;

  mots = 0;
  i = 0;
  if (! is_in(str[0], separator))
    mots++;
  while (str[i] != '\0')
    {
      if ((is_in(str[i], separator)) && ((!is_in(str[i + 1], separator)) || (str[i + 1] != '\0')))
        mots++;
      i++;
    }
  return (mots);
}

int                     count_wordtab(char *str)
{
  return (count_wordtab_misc(str, " \t"));
}

char                    **my_str_to_wordtab_misc(char *str, char *separator)
{
  char                  **ret;
  int                   i;
  int                   debut;
  int                   mots;
  char                  c;

  ret = xmalloc(((mots = count_wordtab_misc(str, separator)) + 1) * sizeof(*ret));
  mots = 0;
  debut = 0;
  i = 0;
  while (is_in(str[debut], separator))
    debut++;
  while (i < my_strlen(str))
    {
      if ((is_in(str[i], separator)) && (!is_in(str[i + 1], separator)))
        debut = i + 1;
      if ((!is_in(str[i], separator)) && ((is_in(str[i + 1], separator)) || (str[i + 1] == '\0')))
        {
          c = str[i + 1];
          str[i + 1] = '\0';
          ret[mots] = my_strdup(&str[debut]);
          mots++;
          str[i + 1] = c;
        }
      i++;
    }
  ret[mots] = NULL;
  return (ret);
}

char                    **my_str_to_wordtab(char *str)
{
  return (my_str_to_wordtab_misc(str, " \t"));
}

void                    my_show_to_wordtab(char **tab)
{
  int                   i;

  i = 0;
  while (tab[i] != 0)
    {
      my_putstr(tab[i]);
      my_putchar('\n');
      i++;
    }
}

void                    my_free_wordtab(char **tab)
{
  int                   i;

  i = 0;
  while (tab[i] != 0)
    {
      free(tab[i]);
      i++;
    }
  free(tab[i]);
  free(tab);
}
