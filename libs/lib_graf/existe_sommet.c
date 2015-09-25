#include <graf.h>

int		existe_sommet(t_graphe *graphe, char *x)
{
  t_ch_ch_int	*ttmp;

  for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
    if (!my_strcmp(ttmp->name, x))
      return (1);
  return (0);
}
