/*
** quick.c for quick sort in /u/prof/lecoq_v/Work/src/lib_sort
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Mon Feb 21 15:09:31 2005 vincent lecoq
** Last update Mon Feb 21 15:19:52 2005 vincent lecoq
*/

#include <my_sort.h>

int	part(int *tab, int deb, int end)
{
  int	compt = deb;
  int	pivot = tab[deb];
  int	i;

  for (i = deb + 1; i <= end; i++)
    {
      if (tab[i] < pivot)
	{
	  compt++;
	  swap_elem_in_tab(tab, compt, i);
	}
    }
  swap_elem_in_tab(tab, compt, deb);
  return (compt);
}

void	quick_sort_bis(int *tab, int begin, int end)
{
  if (begin < end)
    {
      int pivot = part(tab, begin, end);
      quick_sort_bis(tab, begin, pivot - 1);
      quick_sort_bis(tab, pivot + 1, end);
    }
}

void	quick_sort(int *tab, int len)
{
  quick_sort_bis(tab, 0, len - 1);
}
