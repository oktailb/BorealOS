#include <graf.h>

t_graphe		*init(int type)
{
  t_graphe	*graphe;

  graphe = xmalloc(sizeof(*graphe));
  graphe->graphe = 0;
  graphe->taille = 0;
  graphe->type = type;
  return (graphe);
}
