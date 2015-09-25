#include <graf.h>

int		main(int argc, char *argv[])
{
  t_graphe	*g;
  t_graphe	*parcours;

  g = init_graphe(GNO, argc, argv);
  //g = initRand(GNO, my_getnbr_base(argv[1], "0123456789"));
  parcours = parcours_profondeur(g, g->graphe->sommet);
  SetOutputTrace("test.x11", x11, x11_style);
  Trace(g, calc_connexes, genere_coord_spiral);
  effacer(g);
  effacer(parcours);
  return (0);
}
