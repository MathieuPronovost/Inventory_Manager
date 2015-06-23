/*
  INF3105 - Structures de données et algorithmes
  Session Hiver 2013.
  TP2
  Auteur : Mathieu Pronovost PROM18118300
  Version : 21 mars 2013
*/

#include "inventaire.h"

using namespace std;

ListeIngredients& ListeIngredients::operator+=(const ListeIngredients& autre) {
    ListeIngredients autre2 = autre;
    ArbreMap<string, int>::Iterateur iter = autre2.ingredients.debut();
    for (; iter; ++iter)
        ingredients[iter.cle()] += autre2.ingredients[iter.cle()];
    return *this;
}

ListeIngredients& ListeIngredients::operator*=(int facteur) {
    ArbreMap<string, int>::Iterateur iter = ingredients.debut();
    for (; iter; ++iter)
        ingredients[iter.cle()] *= facteur;
    return *this;
}

istream& operator >> (istream& is, ListeIngredients& liste) {
    liste.ingredients.vider();

    string chaine;
    is >> chaine;
    while(is && chaine!="---") {
        int quantite;
        is >> quantite;
        liste.ingredients[chaine] = quantite;
        is >> chaine;
    }
    return is;
}

Inventaire& Inventaire::operator+=(const Inventaire& autre) {
    Inventaire autre2 = autre;
    ArbreMap<string, ArbreMap<int, int> >::Iterateur iter = autre2.items.debut();
    for (; iter; ++iter) {
        ArbreMap<int, int>::Iterateur iter2 = iter.valeur().debut();
        for (; iter2; ++iter2) {
            items[iter.cle()][iter2.cle()] += autre2.items[iter.cle()][iter2.cle()];
        }
    }
    return *this;
}

Inventaire& Inventaire::operator-=(const ListeIngredients& liste) {
    ListeIngredients liste2 = liste;
    ArbreMap<string, int >::Iterateur iter = liste2.ingredients.debut();
    for (; iter; ++iter) {
        int quantite = iter.valeur();
        ArbreMap<int, int>::Iterateur iter2 = items[iter.cle()].debut();
        for (; iter2; ++iter2) {
            while ((quantite > 0) && (items[iter.cle()][iter2.cle()] > 0)) {
                items[iter.cle()][iter2.cle()]--;
                quantite--;
            }
        }


    }
    return *this;
}

istream& operator >> (istream& is, Inventaire& inventaire) {
    inventaire.items.vider();

    string chaine;
    is >> chaine;
    while(is && chaine!="---") {
        int quantite, expiration;
        is >> quantite >> expiration;
        inventaire.items[chaine][expiration] = quantite;
        is >> chaine;
    }
    return is;
}

