#include <graf.h>

int		effacer(t_graphe* graphe)
{
  t_ch_int	*tmp;
  t_ch_int	*bak;
  t_ch_ch_int	*ttmp;
  t_ch_ch_int	*bbak;
    
  ttmp = graphe->graphe;
  while ((ttmp) != NULL)
    {
      bbak = ttmp->next;
      tmp = ttmp->liste;
      while ((tmp) != NULL)
	{
	  bak = tmp->next;
	  free(tmp);
	  tmp = bak;
	}
      free(ttmp);
      ttmp = bbak;
    }
  free(graphe);
  return (0);
}
