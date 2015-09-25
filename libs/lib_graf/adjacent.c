#include <graf.h>

int		adjacent(t_graphe *graphe, int x, int y)
{
  t_ch_int	*tmp;
  t_ch_ch_int	*ttmp;

  if (x == y)
    return (1);
  ttmp = graphe->graphe;
  while ((ttmp) != NULL)
    {
      if(ttmp->number == y)
	break;
      ttmp = ttmp->next;
    }
  tmp = ttmp->liste;
  while ((tmp) != NULL)
    {
      if(tmp->number == x)
	return (1);
      tmp = tmp->next;
    }
  return (0);
}

