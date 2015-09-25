#include <graf.h>

int estParfait(t_graphe* graphe)
{
  t_ch_ch_int** cliques;
  int** colors;
  int max=0, i=0;
  t_ch_ch_int* tmp;
  t_ch_int* tmp2;

  cliques = malloc(sizeof(cliques));
  colors = malloc(sizeof(*colors)*taille(graphe));
  calc_cliques(graphe, cliques);
  for (tmp = *cliques; tmp->next != NULL; tmp = tmp->next)
    {
      i = 0;
      for (tmp2 = tmp->liste; tmp2->next != NULL; tmp2 = tmp2->next)
	i++;
      if (i > max)
	max = i;
    }
  if (max == coloriage(graphe, colors))
    return (1);
  return (0);
}
