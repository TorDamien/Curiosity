#ifndef __OBSERVATEUR__
#define __OBSERVATEUR__

/* Version actuelle de l'automate */
/* Les commandes de déplacements valides dans l'automate sont :
	- M(devant) A
	- M(droite) D A
	- M(gauche) G A
	- M(derrière) (GG) OU (DD) A
   On part du principe si il y a plusieurs mesures d'affilées, seul la dernière avant
   une commande de mouvement(GDA) compte. 
   Tout enchainement de mesures en vue d'un déplacement multiple à la suite peut être
   réduit à l'un des cas de déplacements valides.
   Notamment, M(devant-droite)M(devant) A D A revient à faire M(devant) A M(droite) D A
*/

/* Valeurs à changées en fonction des modifications possibles de l'automate */
#define NB_MAX_ETATS 8 
/* Entrée actuelles = {A,D,G,Mdev,Mg,Md,Mder,Mautre} */
#define NB_MAX_ENTREES 8 

/* A changé en cas de modification de l'automate */
#define NB_ETATS 8

//ETATS
#define INIT 0
#define M_DEVANT 1
#define M_DROIT 2
#define M_GAUCHE 3
#define M_DERRIERE 4
#define M_DERRIERE_G 5
#define M_DERRIERE_D 6
#define SORTIE_FAIL 7

//TRANSITION
#define AVANCER 0
#define DROITE 1
#define GAUCHE 2
#define M_DEVANT_LIBRE 3
#define M_DERRIERE_LIBRE 4
#define M_DROIT_LIBRE 5
#define M_GAUCHE_LIBRE 6
#define M_AUTRE 7     //Les diagonales et les mesures raté

typedef struct {
  int nb_etats;
  int etat_actuel;
  int transitions[NB_MAX_ETATS][NB_MAX_ENTREES] ;
} automate; 


/* Initialise l'automate à vide */
void init_par_defaut(automate *a);

/* Initialise l'automate et ses transitions */
void init_automate(automate *a);

/* Print dans le fichier Rapport_observateur.err les erreurs programmes détectées
   Ce fichier est ouvert en mode a+ pour ne pas supprimer les anciens messages
   La date et l'heure de chaque erreurs y sont données */
void gestion_erreur_observateur(int e);

#endif

