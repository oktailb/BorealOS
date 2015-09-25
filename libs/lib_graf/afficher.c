#include <graf.h>

void		afficher(t_graphe* graphe)
{
  t_ch_int	*tmp;
  t_ch_ch_int	*ttmp;
  
  my_printf("Graphe de %d arcs entre %d noeuds\n", graphe->arcs, graphe->taille);
  for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
    {
      my_printf("%s -> { ", ttmp->name);
      for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	my_printf("%s,", tmp->name);
      my_printf("\b}\n");
    }
}

