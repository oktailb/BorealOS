#include <graf.h>

t_graphe		*retirerSommet(t_graphe *graphe, int x)
{
  t_ch_ch_int	*ttmp;
  t_ch_ch_int	*back;
  t_ch_int	*tmp;
  t_ch_int	*bak;

  for (back = NULL, ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
    {
      if (ttmp->number == x)
	{
	  if (back != NULL)
	    back->next = ttmp->next;
	  else
	    graphe->graphe = ttmp->next;
	  ttmp->next = NULL;
	  free(ttmp);
	}
      else
	for (bak = NULL, tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	  {
	    if (ttmp->number == x)
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
      back = ttmp;
    }
  return (graphe);
}
