#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "terrain.h"
#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STEP_MAX 32767
#define GRAINE_MAX 32767

int main(int argc, char **argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;
  Terrain T;

  if (argc < 9) {
    printf(
        "Usage: %s <fichier_programme> <N> <largeur> <hauteur> <densite_obstacle> <graine> <nb_step_max> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  /* On silence l'observateur ici tant qu'on ajoute pas une récupération en ligne de
  commande d'une valeur pour activer ou non l'observateur */
  envt.interrupteur = 0;

  int N,L,H,graine,nb_step_max,i;
  float d;
  FILE *resFile;

  N = strtol(argv[2], NULL, 10);
  L = strtol(argv[3], NULL, 10);
  H = strtol(argv[4], NULL, 10);
  d = strtof(argv[5], NULL);
  graine = strtol(argv[6], NULL, 10);
  nb_step_max = strtol(argv[7], NULL, 10);

  /* On s'assure que les valeurs données en ligne de commande sont valides */
  if (L > DIM_MAX || L % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (H > DIM_MAX || H % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((d > 1) || (d < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }
  if ((graine > GRAINE_MAX)) {
    printf("Graine incorrecte : doit être <= %d\n", GRAINE_MAX);
    return 1;
  }
  if ((nb_step_max > STEP_MAX)) {
    printf("Nombre incorrecte : doit être <= %d\n", STEP_MAX);
    return 1;
  }

  srand(graine); // srand en fonction de la graine

  FILE *ter_alea;
  FILE *ter_alea2;
  
  int step;
  int posx = L/2;
  int posy = H/2;
  int nb_sorties = 0;  //compte quand le robot arrive a sortir
  int nb_bloque = 0;  //et quand il se coince
  int nb_crash = 0;
  int nb_plouf = 0;
  unsigned int nb_pas_total = 0;

  resFile = fopen(argv[8], "w");
  fprintf(resFile, "Nombre de terrains testés : %d\n",N);
  ter_alea2 = fopen("Tout_les_terrains.perf", "a+");

  int h, min, s, day, mois;
  time_t now;
  time(&now);
  
  /* Récupération de la date et heure à l'aide du module time */
  struct tm *local = localtime(&now);
  h = local->tm_hour;        
  min = local->tm_min;       
  s = local->tm_sec;       
  day = local->tm_mday;          
  mois = local->tm_mon + 1;     
  fprintf(ter_alea2,"\n-----Test du [%02d/%02d][%02d:%02d:%02d]-----\n\n",day, mois,h, min, s);
  
  i = 0;
  while (i<N){
    step = 0;
    generation_aleatoire(&T,L,H,d);
    /*
    printf("\n");
    afficher_terrain(&T);
    printf("\n");
    */
    if(existe_chemin_vers_sortie(T)){
      ter_alea = fopen("Terrain_alea.perf", "w");
      ecrire_terrain(ter_alea,&T,posx,posy);
      ecrire_terrain(ter_alea2,&T,posx,posy);
      fclose(ter_alea);
      errt = initialise_environnement(&envt, "Terrain_alea.perf");
      gestion_erreur_terrain(errt);
      errp = lire_programme(&prog, argv[1]);
      gestion_erreur_programme(errp);

      init_etat(&etat);
      do {
        res = exec_pas(&prog, &envt, &etat);
        step++;
      } while (res == OK_ROBOT && step < nb_step_max);
      //printf("%d\n",step);
      //nb_sorties++;
      i++;
      if (res == SORTIE_ROBOT) {
        fprintf(resFile, "%d\n",step);
        nb_pas_total = nb_pas_total + step;
        nb_sorties++;
      } else if (res == PLOUF_ROBOT) {  //Pas d'eau sur les terrains généré
        fprintf(resFile, "-2\n");
        nb_plouf++;
      } else if (res == CRASH_ROBOT) {
        fprintf(resFile, "-3\n");
        nb_crash++;
      } else {
        fprintf(resFile, "-1\n");
        nb_bloque++;
      }

    } else {//Création de terrain raté
      //nb_bloque++;
    }
  } 

  fclose(ter_alea2);
  FILE *f = fopen("Rapport_performance.perf","a+"); 
  
  /* Récupération de la date et heure à l'aide du module time */
  //struct tm *local = localtime(&now);
  h = local->tm_hour;        
  min = local->tm_min;       
  s = local->tm_sec;       
  day = local->tm_mday;          
  mois = local->tm_mon + 1;     

  fprintf(f,"[%02d/%02d][%02d:%02d:%02d] STATISTIQUE\n",day, mois,h, min, s);
  //printf("N = %d | nb_sorties = %d | nb_bloque = %d |nb_crash = %d | nb_sorties/N = %.2f \n",N,nb_sorties,nb_bloque,nb_crash,(float)nb_sorties/N*100);
  fprintf(f,"Pourcentage de terrains où le robot sort du terrain : %.1f \n",(float)nb_sorties/N*100);
  fprintf(f,"Pourcentage de terrains où le robot se bloque : %.1f \n",(float)nb_bloque/N*100);
  fprintf(f,"Pourcentage de terrains où le robot se crash : %.1f \n",(float)nb_crash/N*100);
  fprintf(f,"Pourcentage de terrains où le robot tombe dans l'eau : %.1f \n",(float)nb_plouf/N*100);
  fprintf(f,"Nombre de pas moyen = %.2f\n",(float)nb_pas_total/N);
  fprintf(f,"\n-------------------------\n\n");

  fclose(resFile);
  fclose(f);

}