#include <graf.h>

t_graphe	*retirerArc(t_graphe *graphe, int x, int y)
{
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp;
  t_ch_int	*bak;

  for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
    for (bak = NULL, tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
      {
	if (
	    ((ttmp->number == x) && (tmp->number == y)) 
	    || 
	    ((ttmp->number == y) && (tmp->number == x))
	   )
	{
	  if (bak != NULL)
	    bak->next = tmp->next;
	  else
	    ttmp->liste = tmp->next;
	  tmp->next = NULL;
	  free(tmp);
	}
	bak = tmp;
      }
  return (graphe);
}
