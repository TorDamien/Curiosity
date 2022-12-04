#include <stdio.h>
#include <string.h>
#include "observateur.h"
#include <stdlib.h>
#include <time.h>

void init_par_defaut(automate *a) {
	int i, j;
	a->nb_etats = NB_MAX_ETATS;
	a->etat_actuel = INIT;
	for (i = 0; i < NB_MAX_ETATS; i++)
		for (j = 0; j < NB_MAX_ENTREES; j++)
			a->transitions[i][j] = i;
}

void init_automate(automate *a) {
	init_par_defaut(a);
	a->nb_etats = NB_ETATS;

	/* Transition depuis l'état INIT*/
	a->transitions[INIT][AVANCER] = SORTIE_FAIL;
	a->transitions[INIT][DROITE] = INIT;
	a->transitions[INIT][GAUCHE] = INIT;
	a->transitions[INIT][M_DEVANT_LIBRE] = M_DEVANT;
	a->transitions[INIT][M_GAUCHE_LIBRE] = M_GAUCHE;
	a->transitions[INIT][M_DROIT_LIBRE] = M_DROIT;
	a->transitions[INIT][M_DERRIERE_LIBRE] = M_DERRIERE;
	a->transitions[INIT][M_AUTRE] = INIT;

	/* Transition depuis l'état M_DEVANT*/
	a->transitions[M_DEVANT][AVANCER] = INIT;
	a->transitions[M_DEVANT][DROITE] = INIT;
	a->transitions[M_DEVANT][GAUCHE] = INIT;
	a->transitions[M_DEVANT][M_DEVANT_LIBRE] = M_DEVANT;
	a->transitions[M_DEVANT][M_GAUCHE_LIBRE] = M_GAUCHE;
	a->transitions[M_DEVANT][M_DROIT_LIBRE] = M_DROIT;
	a->transitions[M_DEVANT][M_DERRIERE_LIBRE] = M_DERRIERE;
	a->transitions[M_DEVANT][M_AUTRE] = INIT;

	/* Transition depuis l'état M_GAUCHE*/
	a->transitions[M_GAUCHE][AVANCER] = SORTIE_FAIL;
	a->transitions[M_GAUCHE][DROITE] = INIT;
	a->transitions[M_GAUCHE][GAUCHE] = M_DEVANT;
	a->transitions[M_GAUCHE][M_DEVANT_LIBRE] = M_DEVANT;
	a->transitions[M_GAUCHE][M_GAUCHE_LIBRE] = M_GAUCHE;
	a->transitions[M_GAUCHE][M_DROIT_LIBRE] = M_DROIT;
	a->transitions[M_GAUCHE][M_DERRIERE_LIBRE] = M_DERRIERE;
	a->transitions[M_GAUCHE][M_AUTRE] = INIT;

	/* Transition depuis l'état M_DROIT*/
	a->transitions[M_DROIT][AVANCER] = SORTIE_FAIL;
	a->transitions[M_DROIT][DROITE] = M_DEVANT;
	a->transitions[M_DROIT][GAUCHE] = INIT;
	a->transitions[M_DROIT][M_DEVANT_LIBRE] = M_DEVANT;
	a->transitions[M_DROIT][M_GAUCHE_LIBRE] = M_GAUCHE;
	a->transitions[M_DROIT][M_DROIT_LIBRE] = M_DROIT;
	a->transitions[M_DROIT][M_DERRIERE_LIBRE] = M_DERRIERE;
	a->transitions[M_DROIT][M_AUTRE] = INIT;

	/* Transition depuis l'état M_DERRIERE*/
	a->transitions[M_DERRIERE][AVANCER] = SORTIE_FAIL;
	a->transitions[M_DERRIERE][DROITE] = M_DERRIERE_D;
	a->transitions[M_DERRIERE][GAUCHE] = M_DERRIERE_G;
	a->transitions[M_DERRIERE][M_DEVANT_LIBRE] = M_DEVANT;
	a->transitions[M_DERRIERE][M_GAUCHE_LIBRE] = M_GAUCHE;
	a->transitions[M_DERRIERE][M_DROIT_LIBRE] = M_DROIT;
	a->transitions[M_DERRIERE][M_DERRIERE_LIBRE] = M_DERRIERE;
	a->transitions[M_DERRIERE][M_AUTRE] = INIT;

	/* Transition depuis l'état M_DERRIERE_D*/
	a->transitions[M_DERRIERE_D][AVANCER] = SORTIE_FAIL;
	a->transitions[M_DERRIERE_D][DROITE] = M_DEVANT;
	a->transitions[M_DERRIERE_D][GAUCHE] = INIT;
	a->transitions[M_DERRIERE_D][M_DEVANT_LIBRE] = M_DEVANT;
	a->transitions[M_DERRIERE_D][M_GAUCHE_LIBRE] = M_GAUCHE;
	a->transitions[M_DERRIERE_D][M_DROIT_LIBRE] = M_DROIT;
	a->transitions[M_DERRIERE_D][M_DERRIERE_LIBRE] = M_DERRIERE;
	a->transitions[M_DERRIERE_D][M_AUTRE] = INIT;

	/* Transition depuis l'état M_DERRIERE_G*/
	a->transitions[M_DERRIERE_G][AVANCER] = SORTIE_FAIL;
	a->transitions[M_DERRIERE_G][DROITE] = INIT;
	a->transitions[M_DERRIERE_G][GAUCHE] = M_DEVANT;
	a->transitions[M_DERRIERE_G][M_DEVANT_LIBRE] = M_DEVANT;
	a->transitions[M_DERRIERE_G][M_GAUCHE_LIBRE] = M_GAUCHE;
	a->transitions[M_DERRIERE_G][M_DROIT_LIBRE] = M_DROIT;
	a->transitions[M_DERRIERE_G][M_DERRIERE_LIBRE] = M_DERRIERE;
	a->transitions[M_DERRIERE_G][M_AUTRE] = INIT;

	/* Transition depuis l'état SORTIE_FAIL*/
	a->transitions[SORTIE_FAIL][AVANCER] = SORTIE_FAIL;
	a->transitions[SORTIE_FAIL][DROITE] = INIT;
	a->transitions[SORTIE_FAIL][GAUCHE] = INIT;
	a->transitions[SORTIE_FAIL][M_DEVANT_LIBRE] = M_DEVANT;
	a->transitions[SORTIE_FAIL][M_GAUCHE_LIBRE] = M_GAUCHE;
	a->transitions[SORTIE_FAIL][M_DROIT_LIBRE] = M_DROIT;
	a->transitions[SORTIE_FAIL][M_DERRIERE_LIBRE] = M_DERRIERE;
	a->transitions[SORTIE_FAIL][M_AUTRE] = INIT;

}

void gestion_erreur_observateur(int e) {

	FILE *f = fopen("rapport_observateur.err","a+");
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
  	switch(e) {
  		case(7): // Cas état actuel FAIL
  			fprintf(f,"[%02d/%02d][%02d:%02d:%02d] Avance plusieurs fois à la suite\n",day, mois,h, min, s);
  			break;
  		case(0): // Cas état actuel INIT
  			fprintf(f,"[%02d/%02d][%02d:%02d:%02d] Avance avant une mesure valide\n",day, mois,h, min, s);
  			break;
  		case(2): // Cas état actuel M_DROIT
  			fprintf(f,"[%02d/%02d][%02d:%02d:%02d] Avance au lieu de tourner à droite\n",day, mois,h, min, s);
  			break;
  		case(4): // Cas état actuel M_DERRIERE
  			fprintf(f,"[%02d/%02d][%02d:%02d:%02d] Avance au lieu de tourner à droite ou à gauche\n",day, mois,h, min, s);
  			break;
  		case(6): // Cas état actuel M_DERRIERE_D
  			fprintf(f,"[%02d/%02d][%02d:%02d:%02d] Avance au lieu de tourner à droite\n",day, mois,h, min, s);
  			break;
  		default: // Gère les cas M_GAUCHE et M_DERRIERE_G qui attendent la même commande 
  			fprintf(f,"[%02d/%02d][%02d:%02d:%02d] Avance au lieu de tourner à gauche\n",day, mois,h, min, s);
  				
  	}  
  	fclose(f);
}











