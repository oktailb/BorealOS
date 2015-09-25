#include <graf.h>

t_gom		*ol2om(t_gol* graphe)
{
  t_ch_int	*tmp;
  t_ch_ch_int	*ttmp;
  t_gom		*new_gom;
  
  new_gom = init_om();
  for (ttmp = graphe->graphe; ttmp->next != NULL; ttmp = ttmp->next)
    for(tmp = ttmp->liste; tmp->next != NULL; tmp = tmp->next)
      ajouterArc_om(ttmp->sommet, tmp->sommet, new_gom, tmp->data);
  return (new_gom);
}

t_gol		*om2ol(t_gom *graphe)
{
  int		i;
  int		j;
  t_gol		*new_gol;

  new_gol = init_ol();
  for (i = 0; i < taille_om(graphe); i++)
    for (j = 0; j < taille_om(graphe); j++)
      if (graphe->graphe[i][j] != NULL)
	ajouterArc_ol(graphe->list[i],graphe->list[j],new_gol, graphe->graphe[i][j]);
  return (new_gol);
}

t_gnom		*nol2nom(t_gnol* graphe)
{
  t_ch_int	*tmp;
  t_ch_ch_int	*ttmp;
  t_gnom	*new_gom;
  
  new_gom = init_nom();
  for (ttmp = graphe->graphe; ttmp->next != NULL; ttmp = ttmp->next)
    for (tmp = ttmp->liste; tmp->next != NULL; tmp = tmp->next)
      ajouterArc_nom(ttmp->sommet, tmp->sommet, new_gom, tmp->data);
  return (new_gom);
}

t_gnol		*nom2nol(t_gnom *graphe)
{

  int		i;
  int		j;
  t_gnol	*new_gnol;

  new_gnol = init_nol();
  for (i = 0; i < graphe->taille; i++)
    for (j = 0; j < graphe->taille; j++)
      if (graphe->graphe[i][j] != NULL)
	ajouterArc_nol(graphe->list[i], graphe->list[j], new_gnol, graphe->graphe[i][j]);
  return (new_gnol);
}

t_gnom		*om2nom(t_gom* graphe)
{
  int		i;
  int		j;
  t_gnom	*new_gnom;

  new_gnom = init_nom();
  for(i = 0; i < taille_om(graphe); i++)
    for(j = 0; j < taille_om(graphe); j++)
      if (graphe->graphe[i][j] != NULL)
	ajouterArc_nom(graphe->list[i], graphe->list[j], new_gnom, graphe->graphe[i][j]);
  return (new_gnom);
}

t_gnol		*ol2nol(t_gol* graphe)
{
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp;
  t_gnol	*new_gnol;

  new_gnol = init_nol();
  for(ttmp = graphe->graphe; ttmp->next != NULL; ttmp = ttmp->next)
    for(tmp = ttmp->liste; tmp->next != NULL;tmp = tmp->next)
      ajouterArc_nol(tmp->sommet, ttmp->sommet, new_gnol, tmp->data);
  return (new_gnol);
}
