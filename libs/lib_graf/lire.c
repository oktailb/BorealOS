#include <graf.h>

void lire_ol(char* fname, t_gol* graphe)
{
  char c;
  int i=0, size = 0, init=1, tete=0, node;
  char* str = (char*) malloc(0);
  FILE* f;

  printf("lecture d'un graphe ol\n");
	
  f = fopen(fname, "r");
  while((c=getc(f)) != '\n')
    {
      if(!((c == ' ')||(c == '\t')))
	{
	  i ++;
	  realloc(str, i);
	  str[i-1] = c;
	}
      else
	{
	  i ++;
	  realloc(str, i);
	  str[i-1] = '\0';
	}
    }

  size = atoi(str);
  printf("\n%d\n", size);

  init_ol(size, graphe);

  while((c=getc(f)) != EOF)
    {
      if(c == '\n'){init=1;c=getc(f);}
      if(init == 1)
	{
	  if(c == EOF)break;
	  sprintf(str,"%c",c);
	  tete = atoi(str);

	  init=0;
	  c=getc(f);
	}
      if((c != '-') && (init != 1))
	{
	  sprintf(str,"%c",c);
	  node = atoi(str);
	  ajouterArc_ol(tete, node, graphe, NULL);
	}
    }
}

void lire_om(char* fname, t_gom* graphe)
{
  char c;
  int i=0, size = 0, init=1, tete=0, node;
  char* str = (char*) malloc(0);
  FILE* f;

  printf("lecture d'un graphe om\n");

  f = fopen(fname, "r");
  while((c=getc(f)) != '\n')
    {
      if(!((c == ' ')||(c == '\t')))
	{
	  i ++;
	  realloc(str, i);
	  str[i-1] = c;
	}
      else
	{
	  i ++;
	  realloc(str, i);
	  str[i-1] = '\0';
	}
    }

  size = atoi(str);
  printf("\n%d\n", size);

  init_om(size, graphe);

  while((c=getc(f)) != EOF)
    {
      if(c == '\n'){init=1;c=getc(f);}
      if(init == 1)
	{
	  if(c == EOF)break;
	  sprintf(str,"%c",c);
	  tete = atoi(str);
	  init=0;
	  c=getc(f);
	}
      if((c != '-') && (init != 1))
	{
	  sprintf(str,"%c",c);
	  node = atoi(str);
	  ajouterArc_om(tete, node, graphe, NULL);
	}
    }
}

void lire_nol(char* fname, t_gnol* graphe)
{
  char c;
  int i=0, size = 0, init=1, tete=-1, node;
  char str[10];
  FILE* f;

  printf("lecture d'un graphe nol\n");

  f = fopen(fname, "r");
  while((c=getc(f)) != '\n')
    {
      if(!((c == ' ')||(c == '\t')))
	{
	  i ++;
	  str[i-1] = c;
	}
      else
	{
	  i ++;
	  str[i-1] = '\0';
	  break;
	}
    }
  while((c=getc(f)) != '\n');

  size = atoi(str);
  printf("\nTaille du graphe : %d\n", size);

  init_nol(size, graphe);

  while(c != EOF)
    {
      if(c == '\n'){init=1;c=getc(f);}
      c=getc(f);
      if(init == 1)
	{
	  if(c == EOF)break;
	  i=0;
	  while(c != '-')
	    {
	      i ++;
	      str[i-1] = c;
	      c=getc(f);
	    }
	  i ++;
	  str[i-1] = '\0';
	  printf(str);
	  tete++;
	  init=0;
	}
      if(((c != '-') && (c != '\n')) && (init != 1))
	{
	  i=0;
	  while(((c != '-')) && (c != '\n'))
	    {
	      i ++;
	      str[i-1] = c;	
	      c=getc(f);
	    }
	  i ++;
	  str[i-1] = '\0';
	  node = atoi(str);
	  ajouterArc_nol(tete, node, graphe, NULL);
	}
    }
}

void		lire_nom(char* fname, t_gnom* graphe)
{
  char		c;
  int		i;
  int		size;
  int		init;
  int		tete;
  int		node;
  char		*str = (char*) malloc(0);
  FILE		*f;

  printf("lecture d'un graphe nom\n");
  init = 1;
  tete = 0;
  i = 0;
  size = 0;
  f = fopen(fname, "r");
  while((c=getc(f)) != '\n')
    {
      if(!((c == ' ')||(c == '\t')))
	{
	  i ++;
	  realloc(str, i);
	  str[i-1] = c;
	}
      else
	{
	  i ++;
	  realloc(str, i);
	  str[i-1] = '\0';
	}
    }

  size = atoi(str);
  printf("\n%d\n", size);

  init_nom(size, graphe);

  while((c=getc(f)) != EOF)
    {
      if(c == '\n'){init=1;c=getc(f);}
      if(init == 1)
	{
	  if(c == EOF)break;
	  sprintf(str,"%c",c);
	  tete = atoi(str);
	  init=0;
	  c=getc(f);
	}
      if((c != '-') && (init != 1))
	{
	  sprintf(str,"%c",c);
	  node = atoi(str);
	  ajouterArc_nom(tete, node, graphe, NULL);
	}
    }

}

