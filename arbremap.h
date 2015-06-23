/*
  INF3105 - Structures de données et algorithmes
  Squelette pour Lab8 + TP2
  Code pris dans les notes de cours
*/

#if !defined(__ARBRE_MAP_H__)
#define __ARBRE_MAP_H__
#include "arbreavl.h"

template <class K, class V>
class ArbreMap {
    class Entree {
    public:
        Entree() {}
        Entree(const K& c):cle(c) {}
        K cle;
        V valeur;
        bool operator < (const Entree& e) const {
            return cle < e.cle;
        }
    };
    ArbreAVL<Entree> entrees;
public:
    class Iterateur {
    public:
        Iterateur(ArbreMap& a) : iter(a.entrees.debut()) {}
        Iterateur(typename ArbreAVL<Entree>::Iterateur i) : iter(i) {}
        operator bool() const {
            return iter.operator bool();
        };
        Iterateur& operator++() {
            ++iter;
            return *this;
        }
        const K& cle() const {
            return (*iter).cle;
        }
        V& valeur() {
            return (V&) (*iter).valeur;
        }
    private:
        typename ArbreAVL<Entree>::Iterateur iter;
    };
    Iterateur debut() {
        return Iterateur(*this);
    }
    Iterateur fin() {
        return Iterateur(entrees.fin());
    }
    Iterateur rechercher(const K& cle) {
        return Iterateur(entrees.rechercher(cle));
    }
    Iterateur rechercherEgalOuSuivant(const K& cle) {
        return
            Iterateur(entrees.rechercherEgalOuSuivant(cle));
    }
    Iterateur rechercherEgalOuPrecedent(const K& cle) {
        return
            Iterateur(entrees.rechercherEgalOuPrecedent(cle));
    }

public:
    bool contient(const K&) const;
    void vider();
    const V& operator[] (const K&) const;
    V& operator[] (const K&);

};

template <class K, class V>
void ArbreMap<K,V>::vider() {
    entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const {
    return entrees.rechercher(c) != NULL;
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const {
    Entree* entreetrouvee = entrees.rechercher(c);
    return entreetrouvee->valeur;
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) {
    typename ArbreAVL<Entree>::Iterateur iter = entrees.rechercher(Entree(c));
    if(!iter) {
        entrees.inserer(Entree(c));
        iter = entrees.rechercher(c);
    }
    return entrees[iter].valeur;
}




#endif

