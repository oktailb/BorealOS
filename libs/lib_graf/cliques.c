/*
** cliques.c for recherche de cliques maximales in /u/prof/lecoq_v/Work/src/lib_graf
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Tue Feb 22 16:01:14 2005 vincent lecoq
** Last update Wed Mar  9 13:31:09 2005 vincent lecoq
*/

#include <graf.h>

t_ch_int	*clik_of(t_graphe *graphe, int sommet, int decal, int *names)
{
  t_ch_int	*clik;
  int		j;
  int		k;
  int		*visite;
  t_ch_int	*tmp;
  t_ch_ch_int	*ttmp;

  clik = xmalloc(sizeof(*clik));
  visite = get_visite(graphe->taille);
  for (ttmp = graphe->graphe; ttmp->number != sommet; ttmp = ttmp->next);
  for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
    visite[get_index(tmp->number, names, graphe->taille)] = 1;
  visite[get_index(ttmp->number, names, graphe->taille)] = 1;
  for (j = decal; j < (graphe->taille)+decal; j++)
    if (visite[j%(graphe->taille)])
      for (k = 0; k < (graphe->taille); k++)
	if (visite[k])
	  visite[k] = adjacent(graphe, names[j%(graphe->taille)],names[k]);
  clik = tab2ch(visite, names, graphe->taille);
  return (clik);
}

int		calc_cliques(t_graphe* graphe, t_ch_ch_int** result)
{
  t_ch_ch_int	*ttmp;
  int		*visite;
  int		*names;
  int		i;
  int		count;

  count = 0;
  visite = get_visite(graphe->taille);
  names = get_numbers(graphe);
  for (i = 0; i < graphe->taille; i++)
    for (ttmp = graphe->graphe; ttmp->next != NULL; ttmp = ttmp->next)
      count += add_row(clik_of(graphe, ttmp->number, names[i], names), result, is_included);
  free(names);
  free(visite);
  return (count);
}
