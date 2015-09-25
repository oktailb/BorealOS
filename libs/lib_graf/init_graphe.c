#include <graf.h>

t_graphe	*init_graphe(int type, int argc, char** argv)
{
  int		i;
  char		**wordtab;
  t_graphe	*g;

  g = init(type);
  for(i = 1; i < argc; i++)
    {
      wordtab = my_str_to_wordtab_misc(argv[i], "-");
      ajouterArc(g, wordtab[0], wordtab[1], argv[i]);
    }
  return (g);
}
