/*
** stack.c for gestion de stack in /u/prof/lecoq_v/Work/src/lib_graf
** 
** Made by vincent lecoq
** Login   <lecoq_v@epita.fr>
** 
** Started on  Fri Feb 25 15:59:01 2005 vincent lecoq
** Last update Fri Feb 25 16:00:28 2005 vincent lecoq
*/

#include <stack.h>

void	empiler(int sommet, t_pile* pile)
{
  pile->taille_pile++;
  pile->pile[pile->taille_pile-1] = sommet;
}

void	depiler(t_pile* pile)
{
  pile->taille_pile--;
}

int	sommet(t_pile* pile)
{
  return pile->pile[pile->taille_pile-1];
}

void	inserer(int sommet, int* visite)
{
  visite[sommet] = 1;
}

