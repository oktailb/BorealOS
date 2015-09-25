#include <graf.h>

int		ajouterArc(t_graphe *graphe, char *x, char *y, void *data)
{
  t_ch_int	*tmp;
  t_ch_int	*bak;
  t_ch_ch_int	*ttmp;
  t_ch_ch_int	*back;

  ttmp = 0;
  back = 0;
  tmp = 0;
  bak = 0;
  if (!existe_sommet(graphe, x))
    ajouterSommet(graphe, x);
  if (!existe_sommet(graphe, y))
    ajouterSommet(graphe, y);
  ttmp = graphe->graphe;
  while (ttmp != NULL)
    {
      back = ttmp;
      if (!my_strcmp(ttmp->name, x))
	break;
      ttmp = ttmp->next;
    }
  if (back != 0)
    tmp = back->liste;
  else
    my_printf("gros soucis\n");
  while (tmp != NULL)
    {
      bak = tmp;
      tmp = tmp->next;
    }
  tmp = xmalloc(sizeof(*tmp));
  if(bak != 0)
    bak->next = tmp;
  else
    back->liste = tmp;
  tmp->next = NULL;
  tmp->data = data;
  tmp->name = my_strdup(y);
  back = 0;
  bak = 0;
  if (graphe->type == GNO)
    {
      ttmp = graphe->graphe;
      while (ttmp != NULL)
	{
	  back = ttmp;
	  if (!my_strcmp(ttmp->name, y))
	    break;
	  ttmp = ttmp->next;
	}
      if (back != 0)
	tmp = back->liste;
      else
	my_printf("gros soucis\n");
      while (tmp != NULL)
	{
	  bak = tmp;
	  tmp = tmp->next;
	}
      tmp = xmalloc(sizeof(*tmp));
      if(bak != 0)
	bak->next = tmp;
      else
	back->liste = tmp;
      tmp->next = NULL;
      tmp->data = data;
      tmp->name = my_strdup(x);
    }
  graphe->arcs++;
  return (1);
}

