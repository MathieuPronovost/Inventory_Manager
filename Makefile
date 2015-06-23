# Makefile pour TP2.
# Adaptez ce fichier au besoin.

# Options standard.
OPTIONS = -Wall -fexceptions -g -c

# L'option -g permet de générer les infos de débogage.
# Décommentez la ligne suivante si vous désirez utiliser un débogueur.
#OPTIONS = -g -O0 -Wall

# Les options -O, -O1, -O2, -O3 permetent d'optimiser le code binaire produit.
# Décommentez la ligne suivante si avant la remise finale
#OPTIONS = -O0 -Wall


# Syntaxe : cible : dépendance1 dépendance2 ...
# Ensuite, la ou les ligne(s) débutant par une tabulation (\t) donne les commandes pour construire une cible
tp2: tp2.cpp inventaire.o
	g++ $(OPTIONS) -o tp2 tp2.cpp inventaire.o

inventaire.o: inventaire.h inventaire.cpp
	g++ $(OPTIONS) -c -o inventaire.o inventaire.cpp

clean :
	rm -f *.o
	rm -f tp2
	rm -f *~

