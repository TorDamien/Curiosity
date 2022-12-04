#!/bin/bash

# Script pour lancer des tests depuis le fichier fichiers-tests-tp9
# Et ainsi ne pas surchager le fichier principal

make curiosity-obs

mv curiosity-obs ./fichiers-tests-tp9/

cd ./fichiers-tests-tp9

# Paramètre par défault à changer par le nom du fichier map, le nom du fichier
# programme suivit de 0 ou 1
./curiosity-obs t_observeur_global_map.txt t_observeur_global.prg 1

rm curiosity-obs

cd ..

# A décocher si on veut garder tous les rapports précédent à chaque utilisation
# du script 
mv ./fichiers-tests-tp9/rapport_observateur.err .