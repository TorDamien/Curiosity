#!/bin/bash

# Script pour lancer des tests de performance depuis le fichier fichiers-tests-tp8
# Et ainsi ne pas surchager le fichier principal

make curiosity-perf

mv curiosity-perf ./fichiers-tests-tp8/

cd ./fichiers-tests-tp8

# Rappel arguments ligne de commande :
# <fichier_programme> <N> <largeur> <hauteur> <densite_obstacle> <graine> <nb_step_max> <fichier_res>
# fichier_res attendu avec extension .perf

# Paramètre par défault à changer
./curiosity-perf programme_soutenance1.prg 100 13 15 0.4 42 25000 fichier_res_soutenance1.perf  
./curiosity-perf programme_soutenance2.prg 100 13 15 0.4 42 25000 fichier_res_soutenance2.perf


rm curiosity-perf

cd ..

# A décocher si on ne veut garder tous les rapports précédent à chaque utilisation
# du script 
#mv ./fichiers-tests-tp8/*.perf .