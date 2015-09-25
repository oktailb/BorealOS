#include <graf.h>

t_graphe	*ajouterSommet(t_graphe* graphe, char *name)
{
  t_ch_ch_int	*ttmp;
  t_ch_ch_int	*back;

  back = 0;
  ttmp = graphe->graphe;
  while (ttmp != NULL)
    {
      back = ttmp;
      ttmp = ttmp->next;
    }
  ttmp = xmalloc(sizeof(*ttmp));
  if(back != 0)
    back->next = ttmp;
  else
    graphe->graphe = ttmp;
  ttmp->next = 0;
  ttmp->liste = 0;
  ttmp->number = graphe->taille;
  ttmp->name = my_strdup(name);
  graphe->taille++;
  return (graphe);
}
