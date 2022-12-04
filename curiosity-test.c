#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "terrain.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TMAX_FICHIER 100

/* Convertie le caractère, récupéré dans le fichier, indiquant l'évènement final en un type
résultat_inter */
void conversion_char_resinter(char c, resultat_inter *res) {
	switch (c){
	case 'N':
		*res = OK_ROBOT;
		break;
	case 'F':
		*res = ARRET_ROBOT;
		break;
	case 'S':
		*res = SORTIE_ROBOT;
		break;
	case 'O':
		*res = CRASH_ROBOT;
		break;
	case 'P':
		*res = PLOUF_ROBOT;
		break;
	default:
		printf("Commande incorrecte\n");
	}
}

/* Fonction d'affichage du résultat de l'interprétation */
void gestion_erreur_interprete(resultat_inter err) {
	switch (err) {
	case OK_ROBOT:
		printf("Robot sur une case libre, programme non terminé\n");
		break;
	case SORTIE_ROBOT:
		printf("Le robot est sorti :-)\n");
		break;
	case ARRET_ROBOT:
		printf("Robot sur une case libre, programme terminé\n");
		break;
	case PLOUF_ROBOT:
		printf("Le robot est tombé dans l'eau\n");
		break;
	case CRASH_ROBOT:
		printf("Le robot s'est écrasé sur un rocher\n");
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

/* Convertie le caractère,récupéré dans le fichier, indiquant l'orientation final du robot 
dans le cas ou l'énènement final est 'N' ou 'F' en type oriention */
void convertion_char_orientation(char o, Orientation *orient) {
  switch (o) {
  case 'N':
    *orient = Nord;
    break;
  case 'E':
    *orient = Est;
    break;
  case 'S':
    *orient = Sud;
    break;
  case 'O':
    *orient = Ouest;
    break;
  default:
  	printf("Commande incorrecte\n");
  }
}


int main(int argc, char **argv) {

	Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;

  /* Décoché pour pouvoir passer tous les tests d'un coup 
	if (argc < 2) {
    	printf("Usage: %s <fichier_de_test>\n", argv[0]);
    	return 1;
  	}
  */

  /* On silence l'observateur ici tant qu'on ajoute pas une récupération en ligne de
  commande d'une valeur pour activer ou non l'observateur */
  envt.interrupteur = 0;

  for (int i = 1; i < argc; i++) {

	  FILE *f;

	  f = fopen(argv[i], "r");

	  if (f == NULL) {
	  	printf("Erreur lors de l'ouverture du fichier : %s", argv[1]);
	  	return 2;
	  }

		char fichier_terrain[TMAX_FICHIER];
		char fichier_program[TMAX_FICHIER];
		int nb_pas;
		char e;
		resultat_inter ev_fin;
		int x,y;
		char o;
		Orientation ori;

		fscanf(f,"%s",fichier_terrain);  	
		errt = initialise_environnement(&envt, fichier_terrain);
		gestion_erreur_terrain(errt);

		fscanf(f,"%s",fichier_program);
		errp = lire_programme(&prog, fichier_program);
		gestion_erreur_programme(errp);

		fscanf(f,"%d", &nb_pas);
		fscanf(f," %c", &e);

		if (e == 'N' || e == 'F') {

			fscanf(f," %d",&x);
			fscanf(f," %d",&y);
			fscanf(f," %c", &o);
			convertion_char_orientation(o, &ori);
		}
		conversion_char_resinter(e, &ev_fin);

		init_etat(&etat);
		
		res = OK_ROBOT;

		while (res == OK_ROBOT && nb_pas > 0 ) {
	  	res = exec_pas(&prog, &envt, &etat);
	  	//if (nb_pas % 3 == 0){afficher_envt(&envt);} //Sinon affiche trop de fois le même terrain
	  	afficher_envt(&envt);
	  	//printf("nb_pas =%d \n",nb_pas);
	  	/*
	  	if (res == OK_ROBOT){
	  		printf("OK_ROBOT\n");
	  	}
	  	if (res == ARRET_ROBOT){
	  		printf("ARRET_ROBOT\n");
	  	}*/
	  	nb_pas--;
	  }

	  printf("Nombre de pas restant : %d \n",nb_pas);
	  printf("\n**Résultat final de l'interprétation**\n");
	  gestion_erreur_interprete(res);
	  printf("\n**Résultat attendu**\n");
	  gestion_erreur_interprete(ev_fin);

	  /* Gestion de la sortie d'erreur */
	  FILE *g; 

	  g = fopen("rapport_test.err", "a+w");
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

  	/* Permet de s'assurer qu'on ne print pas deux fois sur la sortie d'erreur
  	   ou qu'on affiche pas 'OK' vu qu'on a enlevé les return pour que tous les
  	   fichiers tests s'exécutent */
	  int flag = 0;

	  if (res != ev_fin) {
	  	printf("\nLe programme ne s'est pas arrêté sur l'évènement attendu\n");
	  	fprintf(g,"[%02d/%02d][%02d:%02d:%02d] ",day,mois,h, min,s);
	  	fputs("Le test de ",g);
	  	fputs(argv[0],g);
	  	fputs(" sur le fichier ",g);
	  	fputs(argv[i],g);
	  	fputs(" a échoué\n\n",g);
	  	fclose(f);
	  	fclose(g);
	  	flag = 1;
	  	//return 3;
	  }
	  
	  else if ((res == OK_ROBOT || res == ARRET_ROBOT) && (flag == 0)) {
	  	if (envt.r.x != x || envt.r.y != y) {
	  		printf("\nLe robot n'est pas à la bonne position\n");
	  		fprintf(g,"[%02d/%02d][%02d:%02d:%02d] ",day,mois,h, min,s);
	  		fputs("Le test de ",g);
	  		fputs(argv[0],g);
	  		fputs(" sur le fichier ",g);
	  		fputs(argv[i],g);
	  		fputs(" a échoué\n\n",g);
	  		fclose(f);
	  		fclose(g);
	  		flag = 1;
	  		//return 4;
	  	}
	  	if (envt.r.o != ori) {
	  		printf("\nLe robot n'a pas la bonne oriention\n");
	  		fprintf(g,"[%02d/%02d][%02d:%02d:%02d] ",day,mois,h, min,s);
	  		fputs("Le test de ",g);
	  		fputs(argv[0],g);
	  		fputs(" sur le fichier ",g);
	  		fputs(argv[i],g);
	  		fputs(" a échoué\n\n",g);
	  		fclose(f);
	  		fclose(g);
	  		flag = 1;
	  		//return 5;
	  	}
	  }

	  if (flag == 0) {

			printf("\nOk\n");

		  fclose(f);
		  fclose(g);
		}

	}
	
	return 0;
}