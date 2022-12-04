#!/bin/bash

# Script pour lancer tous les tests corrects/incorrects, accepté/nonaccepté


make curiosity-obs

mv curiosity-obs ./fichiers-tests-tp9/

cd ./fichiers-tests-tp9

# Paramètre par défault à changer
for test in *acc.prg;
do
    ./curiosity-obs t_observeur_cor_incor_map.txt $test 1  
done
# Boucle pour tester plusieurs programmes

rm curiosity-obs

cd ..

mv ./fichiers-tests-tp9/rapport_observateur.err .
