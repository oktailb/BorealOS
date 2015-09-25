/*
** data_base.c for data base in /u/prof/lecoq_v/Work/src/lib_graf
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Wed Feb 23 10:32:26 2005 vincent lecoq
** Last update Fri Mar  4 13:31:11 2005 vincent lecoq
*/

#include <ch.h>

t_ch_int	*tab2ch(int *visite, int *names, int size)
{
  t_ch_int	*result;
  t_ch_int	*temp;
  t_ch_int	*tmp;
  int		k;

  tmp = 0;
  result = xmalloc(sizeof(*result));
  temp = result;
  for (k = 0; k < size; k++)
    if(visite[k])
      {
	temp->number = names[k];
	temp->next = xmalloc(sizeof(*temp));
	tmp = temp;
	temp = temp->next;
      }
  free(temp);
  tmp->next = NULL;
  return (result);
}

int		add_row(t_ch_int* new_conx, t_ch_ch_int** result, int (*handler)(t_ch_int *g1, t_ch_int *g2))
{
  t_ch_ch_int	*tmp;
  
  if (*result == NULL)
    {
      *result = xmalloc(sizeof(*result));
      (*result)->liste = new_conx;
      (*result)->next = NULL;
    }
  else
    {
      for (tmp = *result; tmp != NULL; tmp = tmp->next)
	if (handler(tmp->liste, new_conx))
	  return (0);
      for (tmp = *result; tmp->next != NULL;tmp = tmp->next);
      tmp->next = malloc(sizeof(*tmp));
      tmp->next->liste = new_conx;
      tmp->next->next = NULL;
    }
  return (1);
}

void		show_data_base(t_ch_ch_int* list, char* title)
{
  t_ch_ch_int	*ttmp;
  t_ch_int	*tmp;
  int		i;

  i = 0;
  my_printf("%s", title);
  for (ttmp = list; ttmp != NULL; ttmp = ttmp->next)
    {
      i++;
      my_printf("Zone %d : ", i);
      for (tmp = ttmp->liste; tmp != NULL; tmp = tmp->next)
	my_printf("%s (%d) ", tmp->name, tmp->number);
      my_printf("\n");
    }
}
