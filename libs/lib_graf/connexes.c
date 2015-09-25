/*
** connexes.c for calcul de zones connexes in /u/prof/lecoq_v/Work/src/lib_graf
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Thu Feb 17 09:46:41 2005 vincent lecoq
** Last update Mon Mar  7 10:16:18 2005 vincent lecoq
*/

#include <graf.h>

t_ch_int	*connex_of(t_graphe *graphe, char *sommet, int **status, int *names)
{
  t_graphe	*res;
  int		*visite;
  t_ch_ch_int	*ttmp;
  t_ch_int	*conx;;

  conx = xmalloc(sizeof(*conx));
  visite = get_visite(graphe->taille);
  res = parcours_profondeur(graphe, sommet);
  for (ttmp = res->graphe; ttmp != NULL; ttmp = ttmp->next)
    {
      (*status)[get_index(ttmp->number, names, graphe->taille)] = 1;
      visite[get_index(ttmp->number, names, graphe->taille)] = 1;
    }
  effacer(res);
  conx = tab2ch(visite, names, graphe->taille);
  return (conx);
}

int		calc_connexes(t_graphe* graphe, t_ch_ch_int** conx)
{
  t_ch_ch_int	*ttmp;
  int		*visite;
  int		*names;
  int		count;

  count = 0;
  visite = get_visite(graphe->taille);
  names = get_numbers(graphe);
  *conx = xmalloc(sizeof(**conx));
  for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
    if (visite[get_index(ttmp->number, names, graphe->taille)] != 1)
      count += add_row(connex_of(graphe, ttmp->name, &visite, names), conx, compare_ens);
  free(names);
  free(visite);
  return (count);
}
