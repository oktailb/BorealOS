#include <graf.h>

int		coloriage(t_graphe* graphe, int** colors)
{
  int		size;
  int		i;
  int		j;
  int		*color_marker;
  int		*res;
  int		*names;
  t_ch_int	*tmp;
  t_ch_ch_int	*ttmp;

  size = taille(graphe);
  color_marker = xmalloc(sizeof(*color_marker)*size);
  names = xmalloc(sizeof(*names)*size);
  res = xmalloc(sizeof(*res)*size);
  for (i = 0, ttmp = graphe->graphe; ttmp != NULL; i++, ttmp = ttmp->next)
    names[i] = ttmp->number;
  for (i = 0; i < size; i++)
    {
      color_marker[i] = 0;
      res[i] = 0;
    }
  for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next) 
    {
      for (j = 0; j < size; j++)
	color_marker[j] = 0;
      for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	color_marker[res[get_index(tmp->number, names, size)]]++;
      for (j = 1; j < size+1; j++)
	if (color_marker[j] == 0)
	{
	  res[get_index(ttmp->number, names, size)] = j;
	  break;
	}
    }
  for (i = 0, j = 0; i < size; i++)
    if (res[i]>j)
      j = res[i];
  free(color_marker);
  free(names);
  *colors = res;
  return (j);
}

void		affiche_coloriage(int* colors, int size)
{
  int		i;

  for (i = 0; i < size; i++)
    my_printf("sommet %d: couleur %d\n",i,colors[i]);
}
