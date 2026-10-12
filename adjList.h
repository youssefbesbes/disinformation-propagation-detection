#ifndef ADJLIST_H_INCLUDED
#define ADJLIST_H_INCLUDED
#include "ELTARTICLE.h"

typedef struct structNoeud {
    ELEMENT info;
    struct structNoeud * precedent;
    struct structNoeud * suivant;
} structNoeud, * NOEUD;

typedef struct {
    NOEUD tete;
    NOEUD queue;
    int lg;
} laStruct,*LISTE;

int estVide(LISTE L);
int listeEstSaturee(LISTE L);
int listeTaille(LISTE L);
int inserer (LISTE L, ELEMENT e, int pos);
int supprimer (LISTE L , int pos );
ELEMENT recuperer(LISTE L, int pos);
LISTE listeCreer(void);
void listeDetruire(LISTE L);
void listeAfficher(LISTE L);

#endif
