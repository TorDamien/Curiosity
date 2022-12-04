#!/bin/bash

# Script pour lancer tous les tests sur tous les interpreteurs pour le tp7

for i in `seq 0 9`;
do
    mv ./interpretes-tp7/interprete$i.c .
done


for i in `seq 0 9`;
do
    make curiosity-test$i
    mv curiosity-test$i ./fichiers-tests-tp7/
done

cd ./fichiers-tests-tp7

for i in `seq 0 9`;
do
	for test in *.test;
	do
    	./curiosity-test$i $test
    done
done

cd ..

# A décocher si on ne veut garder tous les rapports précédent à chaque utilisation
# du script 
mv ./fichiers-tests-tp7/rapport_test.err .

for i in `seq 0 9`;
do
    rm ./fichiers-tests-tp7/curiosity-test$i
done

for i in `seq 0 9`;
do
    mv ./interprete$i.c ./interpretes-tp7/
done