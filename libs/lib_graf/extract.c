#include <graf.h>

void		extract(t_graphe *src, t_graphe** dest, char *start)
{
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp;

  *dest = init(src->type);
  for (ttmp = src->graphe; ttmp != NULL; ttmp = ttmp->next)
    if (!my_strcmp(ttmp->name, start))
      break;
  for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
    ajouterArc(*dest, start, tmp->name, tmp->data);
  
}
