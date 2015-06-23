/*
  INF3105 - Structures de données et algorithmes
  Session Hiver 2013.
  TP2
  Auteur : Mathieu Pronovost PROM18118300
  Version : 21 mars 2013
*/

#if !defined(__INVENTAIRE_H__)
#define __INVENTAIRE_H__
#include <iostream>
#include "arbremap.h"


class ListeIngredients {
public:
    ListeIngredients& operator+=(const ListeIngredients& liste);
    ListeIngredients& operator*=(int quantite);
    ArbreMap <std::string, int> ingredients;

private:
    friend std::istream& operator >> (std::istream&, ListeIngredients&);
    friend class Inventaire;


};

class Inventaire {
public:
    ArbreMap<std::string,  ArbreMap<int, int> > items;
    Inventaire& operator+=(const Inventaire&);
    Inventaire& operator-=(const ListeIngredients&);

private:

    friend std::istream& operator >> (std::istream&, Inventaire&);

};

#endif

