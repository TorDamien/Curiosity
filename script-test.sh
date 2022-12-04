#!/bin/bash

# Script pour lancer des tests de curiosity-test depuis le fichier fichiers-tests-tp7
# Et ainsi ne pas surchager le fichier principal

make curiosity-test

mv curiosity-test ./fichiers-tests-tp7/

cd ./fichiers-tests-tp7

# Paramètre par défault à changer
./curiosity-test A1.1_chemin_crash_r.test

rm curiosity-test

cd ..

# A décocher si on ne veut garder tous les rapports précédent à chaque utilisation
# du script 
mv ./fichiers-tests-tp7/rapport_test.err .