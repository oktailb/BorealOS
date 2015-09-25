/*
** bidirbubble.c for bidirbubble in /u/prof/lecoq_v/Work/src/lib_sort
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Wed Mar  2 15:47:50 2005 vincent lecoq
** Last update Wed Mar  2 16:25:27 2005 vincent lecoq
*/


void		bidirbubble_sort(int *a, int limit)
{
  int		j;
  int		st;
  int		flipped;

  st = -1;
  while (st < limit) 
    {
      flipped = 0;
      st++;
      limit--;
      for (j = st; j < limit; j++) 
	{
	  if (a[j] > a[j + 1]) 
	    {
	      int T = a[j];
	      a[j] = a[j + 1];
	      a[j + 1] = T;
	      flipped = 1;
	    }
	}
      if (!flipped) 
	return;
      for (j = limit; --j >= st;) 
	{
	  if (a[j] > a[j + 1]) 
	    {
	      int T = a[j];
	      a[j] = a[j + 1];
	      a[j + 1] = T;
	      flipped = 1;
	    }
	}
      if (!flipped) 
	return;
    }
}
