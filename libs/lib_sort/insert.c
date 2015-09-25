/*
** insert.c for tri par insertion in /u/prof/lecoq_v/Work/src/lib_sort
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Mon Feb 21 15:15:32 2005 vincent lecoq
** Last update Mon Feb 21 15:18:32 2005 vincent lecoq
*/

#define TRUE    1
#define FALSE  2

#include <my_sort.h>

void	insert_sort(int *tab, int longueur)
{
  int	i;
  int	memory;
  int	compt;
  int	flag;

  for (i = 1; i < longueur; i++)
    {
      memory = tab[i];
      compt = i - 1;
      flag = TRUE;
      while (flag)
        {
          flag = FALSE;
          if (tab[compt] > memory)
	    {
	      tab[compt + 1] = tab[compt];
	      compt--;
	      flag = TRUE;
	    }
          if (compt < 0)
	    flag = FALSE;
	}
      tab[compt + 1] = memory;
    }
}
