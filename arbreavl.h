/*
  INF3105 - Structures de données et algorithmes
  Session Hiver 2013.
  TP2
  Auteur : Mathieu Pronovost PROM18118300
  Version : 21 mars 2013
*/

#if !defined(__ARBREAVL_H__)
#define __ARBREAVL_H__
#include <assert.h>
#include <stdio.h>
#include "pile.h"

template <class T>
class ArbreAVL {
public:
    ArbreAVL();
    ArbreAVL(const ArbreAVL&);
    ~ArbreAVL();
    void inserer(const T&);
    bool contient(const T&) const;
    bool vide() const;
    void vider();
    int  hauteur() const;
    ArbreAVL& operator = (const ArbreAVL&);


    class Iterateur;
    Iterateur debut() const;
    Iterateur fin() const;
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;
    const T& operator[](const Iterateur&) const;
    T& operator[](const Iterateur&);

private:
    class Noeud;
    Noeud* racine;
    bool inserer(const T&, Noeud*&);
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;
    bool contient(const T& element, Noeud* noeud) const;
    const T& max(Noeud*) const;

public:
    class Iterateur {
    public:
        Iterateur(const ArbreAVL& a);
        Iterateur(const Iterateur& a);
        Iterateur(const ArbreAVL& a, Noeud* c);
        operator bool() const;
        bool operator!() const;
        bool operator==(const Iterateur&) const;
        bool operator!=(const Iterateur&) const;
        const T& operator*() const;
        Iterateur& operator++();
        Iterateur operator++(int);
        Iterateur& operator = (const Iterateur&);
    private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        Pile<Noeud*> chemin;
        friend class ArbreAVL;
    };

private:
    class Noeud {
    public:
        Noeud(const T&);
        Noeud *gauche;
        Noeud *droite;
        int equilibre;
        T element;
    };
};


//-----------------------------------------------------------------------------

template <class T>
ArbreAVL<T>::Noeud::Noeud(const T& c):
    gauche(NULL), droite(NULL), equilibre(0), element(c) {
}

template <class T>
ArbreAVL<T>::ArbreAVL() : racine(NULL) {
}

template <class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL& autre) : racine(NULL) {
    copier(autre.racine, racine);
}

template <class T>
ArbreAVL<T>::~ArbreAVL() {
    vider(racine);
}

template <class T>
void ArbreAVL<T>::inserer(const T& element) {
    inserer(element, racine);
}

template <class T>
bool ArbreAVL<T>::inserer(const T& element, Noeud*& noeud) {
    if(noeud==NULL) {
        noeud = new Noeud(element);
        return true;
    }
    if(element < noeud->element) {
        if(inserer(element, noeud->gauche)) {
            noeud->equilibre++;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == 1) return true;
            assert(noeud->equilibre==2);
            if(noeud->gauche->equilibre == -1)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    } else if(noeud->element < element) {
        if(inserer(element, noeud->droite)) {
            noeud->equilibre--;
            if(noeud->equilibre == 0) return false;
            if(noeud->equilibre == -1) return true;
            assert(noeud->equilibre==-2);
            if(noeud->droite->equilibre == 1)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }
    noeud->element = element;
    return false;
}


template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre) {
    Noeud *temp = racinesousarbre->gauche;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int  neb = -(ea>0 ? ea : 0) - 1 + eb;
    int  nea = ea + (neb < 0 ? neb : 0) - 1;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->gauche = temp->droite;
    temp->droite = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre) {
    Noeud *temp = racinesousarbre->droite;
    int  ea = temp->equilibre;
    int  eb = racinesousarbre->equilibre;
    int neb = -(ea<0 ? ea : 0) + 1 + eb ;
    int nea = ea + (neb > 0 ? neb : 0) + 1 ;

    temp->equilibre = nea;
    racinesousarbre->equilibre = neb;
    racinesousarbre->droite = temp->gauche;
    temp->gauche = racinesousarbre;
    racinesousarbre = temp;
}

template <class T>
void ArbreAVL<T>::vider() {
    vider(racine);
}

// Inspiré du code donné dans les notes de cours
template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud) {
    if(noeud == NULL) return;
    vider(noeud->gauche);
    vider(noeud->droite);
    delete noeud;
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const {
    if (source == NULL) {
        noeud = NULL;
    } else if(source!=NULL) {
        noeud = new Noeud(source->element);
        noeud->equilibre = source->equilibre;
        copier(source->gauche,noeud->gauche);
        copier(source->droite,noeud->droite);
    }
}

template <class T>
bool ArbreAVL<T>::contient(const T& element) const {
    return contient(element, racine);
}

template <class T>
bool ArbreAVL<T>::contient(const T& element, Noeud* noeud) const {
    if(noeud == NULL) return false;
    if (element == noeud->element) return true;
    if (element < noeud->element)
        return contient(element, noeud->gauche);
    else
        return contient(element, noeud->droite);
}

// Code donné dans la pratique pour l'examen intra
template <class T>
int  ArbreAVL<T>::hauteur() const {
    Noeud* n = racine;
    int h = 0;
    while(n != NULL) {
        ++h;
        n = n->equilibre<0 ? n->droite : n->gauche;
    }
    return h;
}

template <class T>
const T& ArbreAVL<T>::max(Noeud* n) const {
    while (n->droite != NULL) {
        n = n->droite;
    }
    return n->element;
}

// Code provenant des notes de cours
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const {
    Iterateur iter(*this);
    iter.courant = racine;
    if(iter.courant!=NULL)
        while(iter.courant->gauche!=NULL) {
            iter.chemin.empiler(iter.courant);
            iter.courant = iter.courant->gauche;
        }
    return iter;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const {
    return Iterateur(*this);
}

// Code provenant des notes de cours
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const {
    Iterateur iter(*this);
    Noeud* n = racine;
    while(n) {
        if(e < n->element) {
            iter.chemin.empiler(n);
            n = n->gauche;
        } else if(n->element < e)
            n = n->droite;
        else {
            iter.courant = n;
            return iter;
        }
    }
    iter.chemin.vider();
    return iter;
}


template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const {
    Noeud* n = racine;
    Pile<Noeud*> pile;
    while(n) {
        pile.empiler(n);
        if(e < n->element) {
            n = n->gauche;
        } else if(n->element < e)
            n = n->droite;
        else {
            break;
        }
    }
    while(!pile.vide()) {
        n = pile.depiler();
        if(!(n->element<e))
            return rechercher(n->element);
    }
    return Iterateur(*this);
}

// Code provenant des notes de cours
template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const {
    Noeud* n = racine;
    Pile<Noeud*> pile;
    while(n) {
        pile.empiler(n);
        if(e < n->element) {
            n = n->gauche;
        } else if(n->element < e)
            n = n->droite;
        else {
            break;
        }
    }
    while(!pile.vide()) {
        n = pile.depiler();
        if(!(e<n->element))
            return rechercher(n->element);
    }
    return Iterateur(*this);
}

template <class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->element;
}

template <class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur) {
    assert(&iterateur.arbre_associe == this);
    assert(iterateur.courant);
    return iterateur.courant->element;
}

template <class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
    vider();
    copier(autre.racine, racine);
    return *this;
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
    : arbre_associe(a), courant(NULL) {
}

template <class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
    : arbre_associe(a.arbre_associe) {
    courant = a.courant;
    chemin = a.chemin;
}

// Code provenant des notes de cours
template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++() {
    assert(courant);
    Noeud* suivant = courant->droite;
    while(suivant) {
        chemin.empiler(suivant);
        suivant = suivant->gauche;
    }
    if(!chemin.vide())
        courant = chemin.depiler();
    else
        courant = NULL;
    return *this;
}

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int) {
    Iterateur copie(*this);
    operator++();
    return copie;
}

template <class T>
ArbreAVL<T>::Iterateur::operator bool() const {
    return courant!=NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!() const {
    return courant==NULL;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const {
    assert(&arbre_associe==&o.arbre_associe);
    return courant==o.courant;
}

template <class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const {
    assert(&arbre_associe==&o.arbre_associe);
    return courant!=o.courant;
}

template <class T>
const T& ArbreAVL<T>::Iterateur::operator *() const {
    assert(courant!=NULL);
    return courant->element;
}

template <class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator = (const Iterateur& autre) {
    assert(&arbre_associe==&autre.arbre_associe);
    courant = autre.courant;
    chemin = autre.chemin;
    return *this;
}

#endif


