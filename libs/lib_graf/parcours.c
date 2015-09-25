/*
** parcours.c for parcours de graphes in /u/prof/lecoq_v/Work/src/lib_graf
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Wed Feb 16 17:06:54 2005 vincent lecoq
** Last update Wed Mar  9 13:30:46 2005 vincent lecoq
*/

#include <graf.h>

int	tout_vu(int *visite, int size)
{
  while (size != 0)
    {
      if (visite[size] == 0)
	return (0);
      size--;
    }
  return (1);
}

int	vue(int *visite, int size)
{
  int	i;

  i = 0;
  my_printf("{");
  while (i < size)
    {
      my_printf("%d,", visite[i]);
      i++;
    }
  my_printf("\b}\n");
  return (1);
}

int	choisir(t_ch_int* pere, int* visite)
{
  t_ch_int* tmp;

  for (tmp = pere; tmp->next != NULL; tmp = tmp->next)
    if ((!visite[tmp->number]) && (tmp->number != pere->number))
      return(tmp->number);
  return (-1);
}

int	get_index(int val, int* tab, int size)
{
  int	i;

  for (i = 0; i < size; i++)
    if (tab[i] == val)
      return (i);
  return (-1);
}

t_graphe	*parcours_profondeur(t_graphe* graphe, char *n_s)
{
  int			*visite;
  int			*names;
  t_pile		*pile;
  int			s;
  int			u;
  int			v;
  int			i;
  int			succ;
  int			size;
  t_ch_int		*tmp;
  t_ch_ch_int		*ttmp;
  t_graphe		*T;
  void			*data;

  u = 0;
  v = 0;
  data = 0;
  succ = 0;
  size = taille(graphe);
  visite = xmalloc(sizeof(*visite)*size);
  names = xmalloc(sizeof(*names)*size);
  pile = xmalloc(sizeof(*pile));
  pile->pile = xmalloc(sizeof(*pile->pile)*size);
  pile->taille_pile = 0;
  T = init(GO);
  empiler(name_to_number(graphe, n_s), pile);
  for (i = 0, ttmp = graphe->graphe; i < size; i++, ttmp = ttmp->next)
    names[i] = ttmp->number;
  inserer(get_index(s, names, size), visite);
  while (pile->taille_pile != 0)
    {
      u = sommet(pile);
      succ = 0;
      for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
	if (ttmp->number == u)
	  break;
      for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	if (!visite[get_index(tmp->number, names, size)])
	  succ++;
      if (succ != 0)
	{
	  for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
	    if (ttmp->number == u)
	      break;
	  for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	    if (!visite[get_index(tmp->number, names, size)])
	      {
		v = tmp->number;
		data = tmp->data;
		break;
	      }
	  ajouterArc(T, number_to_name(graphe, u), number_to_name(graphe, v), data);
	  empiler(v,pile);
	  inserer(get_index(v, names, size), visite);
	}
      else
	depiler(pile);
    }
  return (T);
}

int			*get_visite(int size)
{
  int			*visite;
  int			i;

  visite = xmalloc(sizeof(*visite) * size);
  for (i = 0; i < size; i++)
    visite[i] = 0;
  return (visite);
}

int			*get_numbers(t_graphe *graphe)
{
  int			*names;
  int			i;
  t_ch_ch_int		*ttmp;

  names = xmalloc(sizeof(*names) * graphe->taille);
  for (i = 0, ttmp = graphe->graphe; ttmp != NULL; i++, ttmp = ttmp->next)
    names[i] = ttmp->number;
  return (names);
}

char			**get_names(t_graphe *graphe)
{
  char			**names;
  int			i;
  t_ch_ch_int		*ttmp;

  names = xmalloc(sizeof(*names) * graphe->taille);
  for (i = 0, ttmp = graphe->graphe; ttmp != NULL; i++, ttmp = ttmp->next)
    names[i] = my_strdup(ttmp->name);
  return (names);
}

t_graphe		*parcours_largeur(t_graphe* graphe, int s)
{
  int			*visite;
  int			*visite2;
  int			*names;
  t_graphe		*T;
  t_ch_ch_int		*ttmp;
  t_ch_int		*tmp;

  T = init(GO);
  visite = get_visite(graphe->taille);
  visite2 = get_visite(graphe->taille);
  names = get_numbers(graphe);
  visite[get_index(s, names, graphe->taille)] = 2;
  for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
    {
      if (visite[get_index(ttmp->number, names, graphe->taille)] == 2)
	{
	  visite[get_index(ttmp->number, names, graphe->taille)] = 1;
	  for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	    if (visite[get_index(tmp->number, names, graphe->taille)] == 0)
	    {
	      ajouterArc(T, ttmp->name, tmp->name, tmp->data);
	      visite[get_index(tmp->number, names, graphe->taille)] = 2;
	    }
	  ttmp = graphe->graphe;
	}
    }
  return (T);
}
