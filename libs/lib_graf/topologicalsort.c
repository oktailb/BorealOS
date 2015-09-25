/*
** topologicalsort.c for tri topologique in /u/prof/lecoq_v/Work/src/lib_graf
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Thu Mar 10 16:11:24 2005 vincent lecoq
** Last update Thu Mar 10 16:35:02 2005 vincent lecoq
*/


t_ch_int	*topological_sort(t_graphe *graphe)
{
  t_ch_int	*res;
  t_graphe	*parcours;
  int		sommet;
  int		*visite;
  
  if (graphe->taille <= graphe->arcs)
    return (NULL);
  res = xmalloc(sizeof(*res));
  visite = get_visite(graphe->taille);
  sommet = find_tree_head(graphe->graphe)->sommet;
  parcours = parcours_largeur(graphe, sommet);
  
  return (res);
}
