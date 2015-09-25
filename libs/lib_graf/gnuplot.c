#include <graf.h>
#include <stdio.h>


t_ch_ch_int	*find_tree_head(t_ch_ch_int *zones)
{
  t_ch_ch_int	*head;
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp;
  int		flag;

  for (flag = 0, head = zones; head != NULL; head = head->next)
    {
      for (ttmp = zones; ttmp != NULL; ttmp = ttmp->next)
	for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	  if (tmp->number == head->number)
	    flag = 1;
      if (flag == 0)
	return (head);
    }
  return (NULL);
}

void		genere_coord_steps(t_ch_ch_int **zones, int *taille_zone)
{
  t_ch_ch_int	*head;
  t_ch_ch_int	*ttmp;
  
  for (ttmp = *zones; ttmp != NULL; ttmp = ttmp->next)
    {
      head = find_tree_head(ttmp);
      if (head != NULL)
	my_printf("%d %d\n", head->number, taille_zone[ttmp->number]);
    }
}

void		genere_coord_circle(t_ch_ch_int **zones, int *taille_zone)
{
  int		i;
  int		alpha;
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp;
  t_point2D	*coord;

  coord = xmalloc(sizeof(*coord));
  for (ttmp = *zones, i = 0; ttmp != NULL; ttmp = ttmp->next, i++)
    for (alpha = 0, tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
      {
	coord->x = taille_zone[i]*sin((float)((2*PI*alpha) / taille_zone[i]));
	coord->y = taille_zone[i]*cos((float)((2*PI*alpha) / taille_zone[i]));
	tmp->data = xmalloc(sizeof(*coord));
	my_memcpy((char*)tmp->data, (char*)coord, sizeof(*coord));
	alpha++;
      }
  free(coord);
}

void		genere_coord_spiral(t_ch_ch_int **zones, int *taille_zone)
{
  int		i;
  int		alpha;
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp;
  t_point2D	*coord;

  coord = xmalloc(sizeof(*coord));
  i = 0;
  for (ttmp = *zones; ttmp != NULL; ttmp = ttmp->next, i++)
    for (alpha = 0, tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
      {
	coord->x = log((double)alpha+1)*taille_zone[i]*sin((float)((2*PI*alpha) / taille_zone[i]));
	coord->y = log((double)alpha+1)*taille_zone[i]*cos((float)((2*PI*alpha) / taille_zone[i]));
	tmp->data = xmalloc(sizeof(*coord));
	my_memcpy((char*)tmp->data, (char*)coord, sizeof(*coord));
	alpha++;
      }
  free(coord);
}

float		get_x(t_point2D* p)
{
  return(p->x);
}

float		get_y(t_point2D* p)
{
  return(p->y);
}

void		gen_gnuplot(t_graphe *graphe, t_ch_ch_int *zones)
{
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp[2];
  float		decal;

  my_printf("unset border\nunset zeroaxis\nunset key\n");
  my_printf("unset xtics\nunset ytics\n");
  for (decal = 0, ttmp = zones; ttmp != NULL; ttmp = ttmp->next, decal+=(graphe->taille+1))
    for (tmp[0] = ttmp->liste; tmp[0] != NULL; tmp[0] = tmp[0]->next)
      my_printf("set label \"%d\" at %f, %f\n", tmp[0]->number, 0.95*get_x(tmp[0]->data)+decal, 0.75*get_y(tmp[0]->data));
  for (decal = 0, ttmp = zones; ttmp != NULL; ttmp = ttmp->next, decal+=(graphe->taille+1))
    for (tmp[0] = ttmp->liste; tmp[0] != NULL; tmp[0] = tmp[0]->next)
      for (tmp[1] = ttmp->liste; tmp[1] != NULL; tmp[1] = tmp[1]->next)
	if (adjacent(graphe, tmp[0]->number, tmp[1]->number))
	    my_printf("set arrow from %f,%f to %f,%f %s\n",\
		   0.9*get_x(tmp[1]->data) + decal, 0.8*get_y(tmp[1]->data),\
		   0.9*get_x(tmp[0]->data) + decal, 0.8*get_y(tmp[0]->data),\
		   (graphe->type == GNO)?"nohead":"" \
		   );
  my_printf("plot '-' with points pointtype 6 pointsize 2\n");
  for (decal = 0, ttmp = zones; ttmp != NULL; ttmp = ttmp->next, decal+=(graphe->taille+1))
    for (tmp[0] = ttmp->liste; tmp[0] != NULL; tmp[0] = tmp[0]->next)
      my_printf("%f %f label \"%d\"\n", 0.9*get_x(tmp[0]->data)+decal, 0.8*get_y(tmp[0]->data), tmp[0]->number);
}

void		SetOutputTrace(char *fname, char *type, char *style)
{
  my_printf("set terminal %s %s\n", type, style);
  my_printf("set output \"%s\"\n", fname);
}

void		ExecTrace(t_graphe* graphe, int (*handler)(t_graphe*, t_ch_ch_int**), char *fname, char *type, char *style)
{
  int		p[2];
  int		pid;

  pipe(p);
  dup2(p[0], 0);
  pid = fork();
  if (pid < 0)
    my_printf("Echec du fork !\n");
  if (pid == 0)
    {
      close(p[1]);
      execl("/usr/bin", "gnuplot", "-persist", NULL);
    }
  else
    {
      close(p[0]);
      dup2(p[1], 1);
      SetOutputTrace(fname, type, style);
      Trace(graphe, handler, genere_coord_spiral);
    }
}

void		Trace(t_graphe* graphe, int (*handler)(t_graphe*, t_ch_ch_int**), void(*drawer)(t_ch_ch_int **, int *))
{
  t_ch_int	*tmp;
  t_ch_ch_int	*ttmp;
  t_ch_ch_int	**zones;
  int		nb_zones;
  int		i;
  int		size;
  int		*taille_zone;

  zones = xmalloc(sizeof(*zones));
  nb_zones = handler(graphe, zones);
  taille_zone = xmalloc(sizeof(*taille_zone) * nb_zones);
  i = 0;
  for (ttmp = *zones; ttmp != NULL; ttmp = ttmp->next)
    {
      size = 0;
      for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	size++;
      taille_zone[i] = size;
      i++;
    }
  drawer(zones, taille_zone);
  gen_gnuplot(graphe, *zones);
}
