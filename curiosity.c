#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "terrain.h"
#include <stdio.h>
#include <stdlib.h>


/* J'ai déplacé la fonction gestion_erreur_terrain dans le fichier terrain.c */

/* J'ai déplaceé les fonctions affichage_position_programme et gestion_erreur_programme dans le
fichier programme.c */


int main(int argc, char **argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;

  if (argc < 3) {
    printf("Usage: %s <terrain> <programme>\n", argv[0]);
    return 1;
  }

  /* On silence l'observateur ici tant qu'on ajoute pas une récupération en ligne de
  commande d'une valeur pour activer ou non l'observateur */
  envt.interrupteur = 0;

  /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
  errt = initialise_environnement(&envt, argv[1]);
  gestion_erreur_terrain(errt);

  /* Lecture du programme */
  errp = lire_programme(&prog, argv[2]);
  gestion_erreur_programme(errp);

  /* Initialisation de l'état */
  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);
    /* Affichage du terrain et du robot */
    afficher_envt(&envt);
  } while (res == OK_ROBOT);

  /* Affichage du résultat */
  switch (res) {
  case OK_ROBOT:
    printf("Robot sur une case libre, programme non terminé (ne devrait pas "
           "arriver)\n");
    break;
  case SORTIE_ROBOT:
    printf("Le robot est sorti :-)\n");
    break;
  case ARRET_ROBOT:
    printf("Robot sur une case libre, programme terminé :-/\n");
    break;
  case PLOUF_ROBOT:
    printf("Le robot est tombé dans l'eau :-(\n");
    break;
  case CRASH_ROBOT:
    printf("Le robot s'est écrasé sur un rocher X-(\n");
    break;
  case ERREUR_PILE_VIDE:
    printf("ERREUR : pile vide\n");
    break;
  case ERREUR_ADRESSAGE:
    printf("ERREUR : erreur d'adressage\n");
    break;
  case ERREUR_DIVISION_PAR_ZERO:
    printf("ERREUR : division par 0\n");
    break;
  }
}
