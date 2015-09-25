#include <graf.h>

t_graphe		*initRandK(int type, int n, int k)
{
  int		i;
  int		j;
  int		new_arc;
  t_graphe	*graphe;

  new_arc = 0;
  graphe = init(type);
  srandom(time(0)*getpid());
  for (i = 0; i < n; i++)
    for (j = 0; j < (n-1); j++)
      {
	new_arc = 1+(int) (10.0*k*random()/(RAND_MAX+1.0));
	if (new_arc < k)
	  ajouterArc(graphe, i, j, NULL);
      }
  return (graphe);    
}
