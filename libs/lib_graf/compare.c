/*
** compare.c for comparaison de graphes in /u/prof/lecoq_v/Work/src/lib_graf
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Mon Feb 21 14:54:33 2005 vincent lecoq
** Last update Mon Feb 28 14:49:28 2005 vincent lecoq
*/
#include <my_sort.h>
#include <graf.h>

int		compare(t_graphe *g1, t_graphe *g2)
{
  t_ch_ch_int	*ttmp;
  t_ch_ch_int	*ttmp2;
  t_ch_int	*tmp;
  t_ch_int	*tmp2;
  t_graphe	*t1;
  t_graphe	*t2;
  int		i;

  i = 0;
  t1 = grafdup(g1);
  t2 = grafdup(g2);
  for (i = 0,ttmp2 = t1->graphe; ttmp2 != NULL; ttmp2 = ttmp2->next, i++)
    for (ttmp = t1->graphe; ttmp != NULL; ttmp = ttmp->next)
      {
	for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	  if (tmp->number == ttmp2->number)
	    tmp->number = i;
	if (ttmp->number == ttmp2->number)
	  tmp->number = i;
      }
  for (i = 0,ttmp2 = t2->graphe; ttmp2 != NULL; ttmp2 = ttmp2->next, i++)
    for (ttmp = t2->graphe; ttmp != NULL; ttmp = ttmp->next)
      {
	for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	  if (tmp->number == ttmp2->number)
	    tmp->number = i;
	if (ttmp->number == ttmp2->number)
	  tmp->number = i;
      }
  for (ttmp = t1->graphe, ttmp2 = t2->graphe; ((ttmp != NULL) || (ttmp2 != NULL)); ttmp = ttmp->next, ttmp2 = ttmp2->next)
    {
      for (tmp = ttmp->liste, tmp2 = ttmp2->liste; ((tmp != NULL) || (tmp2 != NULL)); tmp = tmp->next, tmp2 = tmp2->next)
	{
	  if ((tmp == NULL) || (tmp2 == NULL))
	    return (0);
	  if (tmp->number != tmp2->number)
	    return (0);
	}
      if ((ttmp == NULL) || (ttmp2 == NULL))
	return (0);
      if (ttmp->number != ttmp2->number)
	return (0);
    }
  return (1);
}

int		compare_ens(t_ch_int *g1, t_ch_int *g2)
{
  t_ch_int	*tmp;
  int		i;
  int		*names1;
  int		*names2;
  int		taille1;
  int		taille2;

  for (taille1 = 0, tmp = g1; tmp != NULL; tmp = tmp->next)
    taille1++;
  for (taille2 = 0, tmp = g2; tmp != NULL; tmp = tmp->next)
    taille2++;
  if (taille1 != taille2)
    return (0);
  names1 = xmalloc(sizeof(*names1) * taille1);
  for (i = 0, tmp = g1; tmp != NULL; tmp = tmp->next, i++)
    names1[i] = tmp->number;
  names2 = xmalloc(sizeof(*names2) * taille2);
  for (i = 0, tmp = g2; tmp != NULL; tmp = tmp->next, i++)
    names2[i] = tmp->number;
  quick_sort(names1, taille1);
  quick_sort(names2, taille2);
  for (i = 0; i < taille1; i++)
    if (names1[i] != names2[i])
      return (0);
  return (1);
}

int		is_included(t_ch_int *g1, t_ch_int *g2)
{
  t_ch_int	*tmp;
  int		i;
  int		j;
  int		*names1;
  int		*names2;
  int		taille1;
  int		taille2;
  int		flag;

  for (taille1 = 0, tmp = g1; tmp != NULL; tmp = tmp->next)
    taille1++;
  for (taille2 = 0, tmp = g2; tmp != NULL; tmp = tmp->next)
    taille2++;
  if (taille1 < taille2)
    {
      tmp = g1;
      g1  = g2;
      g2  = tmp;
      i       = taille1;
      taille1 = taille2;
      taille2 = i;
    }
  names1 = xmalloc(sizeof(*names1) * taille1);
  for (i = 0, tmp = g1; tmp != NULL; tmp = tmp->next, i++)
    names1[i] = tmp->number;
  names2 = xmalloc(sizeof(*names2) * taille2);
  for (i = 0, tmp = g2; tmp != NULL; tmp = tmp->next, i++)
    names2[i] = tmp->number;
  quick_sort(names1, taille1);
  quick_sort(names2, taille2);
  for (i = 0; i < taille2; i++)
    {
      flag = 0;
      for (j = 0; j < taille1; j++)
	if (names2[i] == names1[j])
	  flag = 1;
      if (flag == 0)
	return (0);
    }
  return (1);
}
