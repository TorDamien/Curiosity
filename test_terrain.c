#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  int x, y;
  erreur_terrain error;
  char nom_f[DIM_MAX];

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");

  error = lire_terrain(f, &t, &x, &y);
  gestion_erreur_terrain(error);
  
  if ( error == ERREUR_FICHIER ) {

    while ( f == NULL ) {
      printf("Donnez le nom d'un fichier valide pour initialiser le terrain : \n");
      scanf("%s", nom_f);
      f = fopen(nom_f, "r");
      error = lire_terrain(f, &t, &x, &y);
      gestion_erreur_terrain(error);
    }
  }

  fclose(f);

  if ( error != OK ) {
    printf("Le terrain ne peut être affiché car une erreur est survenue\n");
  } else {
    afficher_terrain(&t);
    printf("Position initiale du robot : (%d, %d)\n", x, y);
  }
}


