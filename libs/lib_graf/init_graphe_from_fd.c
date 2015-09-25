#include <graf.h>

t_graphe	*init_graphe_from_fd(int type, int fd, char *comment)
{
  t_graphe	*g;
  char		*input;
  char		**wordtab;

  g = init(type);
  input = get_next_line(fd);
  while((input != NULL) && (my_strlen(input) != 0))
    {
      if (!match(input, comment))
	{
	  wordtab = my_str_to_wordtab_misc(input, "-");
	  ajouterArc(g, wordtab[0], wordtab[1] , input);
	  my_free_wordtab(wordtab);
	}
      free(input);
      input = get_next_line(fd);
    }
  return (g);
}
