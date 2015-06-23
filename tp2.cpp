/*
  INF3105 - Structures de données et algorithmes
  Session Hiver 2013.
  TP2
  Auteur : Mathieu Pronovost PROM18118300
  Version : 21 mars 2013
*/

#include <fstream>
#include "inventaire.h"
#include "arbremap.h"

using namespace std;

int main(int argc, const char** argv) {
    // Gestion de l'entrée :
    //  - lecture dans un fichier si un nom est spécifié.
    //  - sinon, lecture dans cin
    istream* entree;
    ifstream* entree_fichier = NULL;
    if(argc>1) {
        entree = entree_fichier = new ifstream(argv[1]);
        if(entree->fail())
            cerr << "Erreur d'ouverture du fichier '" << argv[1] << "'" << endl;
    } else
        entree = &std::cin;

    // Déclaration du dictionnaire de recettes.
    ArbreMap<string, ListeIngredients> recettes;
    ListeIngredients ingredientsRequis;

    Inventaire inventaire;
    int nocommande = 0;
    int date = -1;
    bool reussite = true;

    while(*entree) {
        string typeevenement;
        *entree >> typeevenement;
        if(!(*entree)) break;

        if(typeevenement=="recette") {
            string nomrecette;
            *entree >> nomrecette;
            ListeIngredients ingredients;
            *entree >> ingredients;
            recettes[nomrecette] = ingredients;

            ArbreMap<string, int>::Iterateur iter = recettes[nomrecette].ingredients.debut();
            for (; iter; ++iter) {
                ingredientsRequis.ingredients[iter.cle()] = 0;
            }
        } else if(typeevenement=="livraison") {
            int date_reception;
            *entree >> date_reception;
            if(date_reception <= date)
                cout << "Attention : ce programme supporte uniquement un ordre chronologique!" << endl;
            date = date_reception;
            Inventaire inventairerecu;
            *entree >> inventairerecu;
            inventaire += inventairerecu;
        } else if(typeevenement=="commande") {
            int date_preparation;
            *entree >> date_preparation;
            if(date_preparation <= date)
                std::cout << "Attention : ce programme supporte uniquement un ordre chronologique!" << std::endl;
            date = date_preparation;

            string nomrecette;
            *entree >> nomrecette;
            while(*entree && nomrecette!="---") {
                int quantite=0;
                *entree >> quantite;

                // Calculer ingredientsRequis pour commande
                ArbreMap<string, int>::Iterateur iter = recettes[nomrecette].ingredients.debut();
                for (; iter; ++iter) {
                    ingredientsRequis.ingredients[iter.cle()] += iter.valeur() * quantite;
                }
                *entree >> nomrecette;
            }

            // liquider l'inventaire des ingrédients périmés (tout stock avec date_expiration<=date_préparation)
            ArbreMap<string, ArbreMap<int, int> >::Iterateur iter = inventaire.items.debut();
            for (; iter; ++iter) {
                ArbreMap<int, int>::Iterateur iter2 = iter.valeur().debut();
                for (; iter2; ++iter2) {
                    if (iter2.cle() < date_preparation) {
                        inventaire.items[iter.cle()][iter2.cle()] = 0;
                    }
                }
            }

            // Vérifie si l'inventaire est suffisant
            ArbreMap<string, int>::Iterateur iter3 = ingredientsRequis.ingredients.debut();
            for (; iter3; ++iter3) {
                int quantiteIngredient = 0;
                ArbreMap<int, int>::Iterateur iter4 = inventaire.items[iter3.cle()].debut();
                for (; iter4; ++iter4) {
                    quantiteIngredient += iter4.valeur();
                }
                if (quantiteIngredient < iter3.valeur()) {
                    reussite = false;
                }
            }


            if (reussite == true) {
                cout << nocommande << " : OK" << endl;
                inventaire -= ingredientsRequis;
            } else {
                cout << nocommande << " : Echec" << endl;
            }

            ArbreMap<string, int>::Iterateur iter5 = ingredientsRequis.ingredients.debut();
            for (; iter5; ++iter5) {
                ingredientsRequis.ingredients[iter5.cle()] = 0;
            }

            reussite = true;
            nocommande++;
        } else {
            std::cout << "Type d'évènement '" << typeevenement << "' inconnu!" << std::endl;
            return 1;
        }
    }

    delete entree_fichier; // delete est sécuritaire même si entree_fichier==NULL
    return 0;
}

