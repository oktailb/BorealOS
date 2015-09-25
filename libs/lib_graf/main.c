/********************************************************
 *	TP Algorithmique - Maitrise d'Informatique	*
 *		LECOQ Vincent - MITAINE Yohan		*
 *	Bibliothèque de manipulation des graphes	*
 *							*
 *		Fichier principal			*
 *							*
 *******************************************************/


#include <graphes_ol.h>
#include <graphes_om.h>
#include <graphes_nol.h>
#include <graphes_nom.h>
#include <cliques.h>
#include <connexes.h>
#include <conversionGraphes.h>
#include <files.h>
#include <listes.h>
#include <parcours.h>
#include <coloriage.h>
#include <gnuplot.h>
#include <parfait.h>

int main(int argc, char* argv[])
{
  int i,j;
  
  GNOL* graphe_nol;
  GNOL* T;
  GNOM* T2;
  GNOL* T3;
  GNOL* T4;
  GNOL* T5;

  CH_CH_INT* cliques;
  CH_CH_INT* cliques_2;
  CH_CH_INT* conx;
  int* colors;

  if(argc == 1)
    {
      printf("Usage : %s nom_de_fichier\n\n",argv[0]);
      exit(-1);
    }
  // initialisations ...
  cliques = (CH_CH_INT*) malloc(sizeof(CH_CH_INT));
  cliques_2 = (CH_CH_INT*) malloc(sizeof(CH_CH_INT));
  conx = (CH_CH_INT*) malloc(sizeof(CH_CH_INT));
  graphe_nol = (GNOL*) malloc(sizeof(GNOL));
  T = (GNOL*) malloc(sizeof(GNOL));
  T2 = (GNOM*) malloc(sizeof(GNOM));
  T3 = (GNOL*) malloc(sizeof(GNOL));
  T4 = (GNOL*) malloc(sizeof(GNOL));
  T5 = (GNOL*) malloc(sizeof(GNOL));

  // lis un graphe depuis un fichier
  lire_nol(argv[1], graphe_nol);
  printf("Graphe final : \n");
  // affiche le dit graphe
  afficher_nol(graphe_nol);

  // recherche les cliques de ce graphe
  printf("\nRecherche des cliques ...\n");
  printf("%d cliques trouvées.\n",find_all_cliques_maxi(graphe_nol, cliques));
  // affiche le résultat
  affiche_liste_cliques(cliques);

  // effectue un parcours en profondeur du graphe
  printf("\nParcours en profondeur ...\n");
  parcours_profondeur_nol(graphe_nol, 0, T);
  // affiche le résultat
  afficher_nol(T);

  // recherche les composantes connexes du graphe
  printf("\nRecherche des zones connexes ...\n");
  calc_connexes(graphe_nol, conx);
  // affiche le résultat
  affiche_liste_connexes(conx);

  // convertit le résultat en représentation matricielle
  nol2nom(T,T2);
  // l'affiche
  afficher_nom(T2);
  printf("\n");
	
  //convertit le graphe d'origine en représentation matricielle
  nol2nom(graphe_nol, T2);
  // l'affiche
  afficher_nom(T2);

  // le reconvertit en représentation par liste de successeurs
  nom2nol(T2,T3);
  // le réaffiche ...
  afficher_nol(T3);

  // convertit la conversion (!) en graphe pour GNUPlot
  sauveTrace_nol("result.plot", T3);
  // Lance gnuplot ...
  gnuplot("result.plot");

  // sauve la version matricielle dans un fichier
  sauve_nom("result.txt",T2);

  colors = (int*) malloc(sizeof(int)*taille_nol(T3));

  // Effectue le calcul du coloriage du graphe
  printf("coloriage : %d\n", coloriage(T3, colors));
  // affiche le résultat
  affiche_coloriage(colors, taille_nol(T3));

  // teste si le graphe est parfait
  if(estParfait(T3))
    {
      printf("Ce graphe est parfait !\n");
    }
  else
    {
      printf("Ce graphe n'est pas parfait !\n");
    }

  initRandK_nol(100,3,T4);
  printf("\nRecherche des cliques ...\n");
  printf("%d cliques trouvées.\n",find_all_cliques_maxi(T4, cliques_2));
  // affiche le résultat
  affiche_liste_cliques(cliques_2);
  //afficher_nol(T4);
  parcours_profondeur_nol(T4, 0, T5);

  // convertit la conversion (!) en graphe pour GNUPlot
  sauveTrace_nol("result_rand.plot", T5);
  // Lance gnuplot ...
  gnuplot("result_rand.plot");
  parcours_profondeur_nol(T4, 0, T5);
  ol2om(T5, T2);
  afficher_nom(T2);
  for (i = 0; i < T2->taille; i++)
  {
    for (j = 0; j < T2->taille; j++)
    {
      if (T2->graphe[i][j])
	printf ("%d-%d ", i, j);
    }
  }
  // Conchita fait le ménage ...
  effacer_nol(graphe_nol);
  effacer_nol(T);
  effacer_nol(T3);
  effacer_nom(T2);
  free(colors);
  free(conx);
  free(cliques);


  return 0;
}
