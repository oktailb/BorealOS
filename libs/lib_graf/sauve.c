#include <graf.h>

void		sauve_ol(char* fname, t_gol* graphe)
{
  int		fd;
  int		size;
  char		buf[16];
  t_ch_int	*tmp;
  t_ch_ch_int	*ttmp;

  fd = open(fname, O_WRONLY|O_CREAT, 0666);
  if(fd == -1)
    {
      printf("Erreur de fichier\n");
      exit (-1);
    }

  size = taille_ol(graphe);
  sprintf(buf, "%d ol\n", size);
  write(fd, buf, my_strlen(buf));
  for (ttmp = graphe->graphe; ttmp != NULL; ttmp = ttmp->next)
    {
      sprintf(buf, "%d", ttmp->sommet);
      write(fd, buf, my_strlen(buf));
      for(tmp = ttmp->liste ;tmp->next != NULL; tmp = tmp->next)
	{
	  sprintf(buf, "-%d", tmp->sommet);
	  write(fd, buf, my_strlen(buf));
	}
      sprintf(buf, "\n");
      write(fd, buf, my_strlen(buf));
    }


}

void		sauve_om(char* fname, t_gom* graphe)
{
  int		fd;
  int		size;
  int		i;
  int		j;
  char buf[16];

  fd = open(fname, O_WRONLY|O_CREAT, 0666);
  if(fd == -1)
    {
      printf("Erreur de fichier\n");
      exit (-1);
    }

  size = taille_om(graphe);
  sprintf(buf, "%d om\n", size);
  write(fd, buf, my_strlen(buf));
  for (i = 0; i < size; i++)
    {
      sprintf(buf, "%d", i);
      write(fd, buf, my_strlen(buf));
      for(j = 0; j < size; j++)
	{
	  if(graphe->graphe[i][j])
	    {
	      sprintf(buf, "-%d", j);
	      write(fd, buf, my_strlen(buf));
	    }
	}
      sprintf(buf, "\n");
      write(fd, buf, my_strlen(buf));
    }


}

void		sauve_nol(char* fname, t_gnol* graphe)
{
  sauve_ol(fname, (t_gol*) graphe);
}

void		sauve_nom(char* fname, t_gnom* graphe)
{
  sauve_om(fname, (t_gom*) graphe);
}

