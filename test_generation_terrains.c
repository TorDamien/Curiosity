#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nb_cases_occupees(Terrain T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(&T); x++) {
    for (y = 0; y < hauteur(&T); y++) {
      if (T.tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat

int main(int argc, char **argv) {
  srand(time(NULL)); //initialisation du rand
  int N, i, l, h;
  float dObst;
  FILE *resFile;
  Terrain T;

  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);

  // test de l et h
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "%d\n", N);

  int posx = l/2;
  int posy = h/2;
  int nb_tv = 0;
  int nb_tl = 0;
  int nb_cases_occ_total = 0;
  float densite,tmp,densite_total;
  // Initialisation de la fonction random
  // A compléter
  i = 0;
  while (i<N){
    generation_aleatoire(&T,l,h,dObst);
    if(existe_chemin_vers_sortie(T)){
      ecrire_terrain(resFile,&T,posx,posy);
      nb_cases_occ_total += nb_cases_occupees(T);
      tmp = nb_cases_occupees(T);
      densite = (float)tmp/(float)(l*h);
      fprintf(resFile, "Densité : %.5f\n",densite);
      fprintf(resFile, "Densité attendue : %f\n",dObst);
      fprintf(resFile, "\n\n-----------------\n\n\n");
      nb_tv++;
      i++;
    } else {
      nb_tl++;
    }
  }

  densite_total = (float)nb_cases_occ_total/(float)(l*h*N);
  fprintf(resFile,"\nTerrains : -Echec : %d -Total : %d\n",nb_tl,nb_tl+nb_tv);
  fprintf(resFile,"Densité totale obtenue sur %d terrains valides : %.5f\n",N,densite_total);

  // A compléter
  // Génération aléatoire des terrains
  // Écriture des terrains générés dans le fichier resFile
  // Écriture/Affichage des statistiques

  // fermeture des fichiers
  fclose(resFile);
  return 0;
}
