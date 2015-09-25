/*
** bubble.c for tri a bulles simple in /u/prof/lecoq_v/Work/src/lib_sort
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Mon Feb 21 15:06:57 2005 vincent lecoq
** Last update Mon Feb 21 15:19:00 2005 vincent lecoq
*/

#include <my_sort.h>

void		swap_elem_in_tab(int *tab, int i, int j)
{
  register int	mem;
  
  mem = tab[i];
  tab[i] = tab[j];
  tab[j] = mem;
}

void		bubble_sort(int *tab, int len)
{
  int	i;
  int	inv;
  
  inv = 1;
  while (inv)
    {
      inv = 0;
      for (i = 0; i < len - 1; i++)
	if (tab[i] > tab[i + 1])
	  {
	    swap_elem_in_tab(tab, i, i + 1);
	    inv = 1;
	  }
    }
}
