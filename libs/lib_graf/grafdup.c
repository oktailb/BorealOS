#include <graf.h>

t_graphe	*grafdup(t_graphe* src)
{
  t_graphe	*res;
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp;

  res = init(GO);
  for (ttmp = src->graphe; ttmp != NULL; ttmp = ttmp->next)
    for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
      ajouterArc(res, ttmp->name, tmp->name, tmp->data);
  res->type = src->type;
  return (res);
}
