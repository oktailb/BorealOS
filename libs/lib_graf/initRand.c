#include <graf.h>

t_graphe		*initRand(int type, int n)
{
  int		i;
  int		j;
  int		new_arc;
  t_graphe	*graphe;

  new_arc = 0;
  graphe = init(type);
  srand(time(0)*getpid());
  new_arc = random();
  for (i = 0; i < n; i++)
      for (j = 0; j < (n-1); j++)
	{
	  new_arc = 1 + rand();
	  if (new_arc < n)
	    ajouterArc(graphe, i, j, NULL);
	}
  return (graphe);
}

