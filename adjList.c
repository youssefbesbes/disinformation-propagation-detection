#include <stdio.h>
#include <stdlib.h>
#include "adjList.h"
#include "ELTARTICLE.c"

NOEUD noeudCreer(ELEMENT e){
    NOEUD n = NULL;
    n = (NOEUD)malloc(sizeof(structNoeud));
    if(!n)
        printf ("\nPlus d'espace");
    else
    {
        elementAffecter(&n->info, e);
        n->suivant = NULL;
        n->precedent = NULL;
    }
    return n;
}

void noeudDetruire(NOEUD n){
    elementDetruire(n->info);
    free(n);
}

int estVide(LISTE L) {
    return (L->lg == 0);
}

int listeEstSaturee(LISTE L) {
    NOEUD temp;
    int saturee = 1;
    temp = (NOEUD) malloc (sizeof(structNoeud));
    if(temp != NULL) {
        saturee = 0;
        free(temp);
    }
    return saturee;
}

int inserer (LISTE L, ELEMENT e, int pos){
    int succee = 1;
    int i;
    NOEUD n, p, q;
    if (listeEstSaturee(L))
    {
        printf ("\nListe saturée");
        succee = 0;
    }
    else
    {
        if ((pos < 1) || (pos > L->lg + 1))
        {
            printf ("\nPosition invalide");
            succee = 0;
        }
        else
        {
            n = noeudCreer(e);
            if (L->lg == 0)
            {
                L->tete = n;
                L->queue = n;
            }
            else
            {
                if (pos == 1)
                {
                    L->tete->precedent = n;
                    n->suivant = L->tete;
                    L->tete = n;
                }
                else
                {
                    if (pos == (L->lg + 1))
                    {
                        L->queue->suivant = n;
                        n->precedent = L->queue;
                        L->queue = n;
                    }
                    else
                    {
                        q = L->tete;
                        for (i=1; i<pos; i++)
                        {
                            p = q;
                            q = q->suivant;
                        }
                        p->suivant = n;
                        n->precedent = p;
                        n->suivant = q;
                        q->precedent = n;
                    }
                }
            }
            (L->lg)++;
        }
    }
    return succee;
}

int supprimer (LISTE L, int pos){
    int i;
    int succee = 1;
    NOEUD p, q;
    if (estVide(L))
    {
        printf ("\nListe vide");
        succee = 0;
    }
    else
    {
        if ((pos < 1) || (pos > L->lg))
        {
            printf ("\nPosition invalide");
            succee = 0;
        }
        else
        {
            if (L->lg == 1)
            {
                q = L->tete;
                L->tete = NULL;
                L->queue = NULL;
            }
            else
            {
                if (pos == 1)
                {
                    q = L->tete;
                    L->tete = L->tete->suivant;
                    L->tete->precedent = NULL;
                }
                else
                {
                    if (pos == L->lg)
                    {
                        q = L->queue;
                        L->queue = L->queue->precedent;
                        L->queue->suivant = NULL;
                    }
                    else
                    {
                        q = L->tete;
                        for (i=1; i<pos; i++)
                        {
                            p = q;
                            q = q->suivant;
                        }
                        q->suivant->precedent = p;
                        p->suivant = q->suivant;
                    }
                }
            }
            noeudDetruire(q);
            (L->lg)--;
        }
    }
    return succee;
}

ELEMENT recuperer(LISTE L, int pos) {
    ELEMENT elt = NULL;
    int i;
    NOEUD p;
    if (estVide(L))
        printf ("\nListe vide");
    else
    {
        if ((pos < 1) || (pos > L->lg))
            printf ("\nPosition invalide");
        else
        {
            p = L->tete;
            for (i=1; i<pos; i++)
            {
                p = p->suivant;
            }
            elementAffecter(&elt, p->info);
        }
    }
    return(elt);
}

LISTE listeCreer(void) {
    LISTE L;
    L = (LISTE)malloc(sizeof(laStruct));
    if (!L)
        printf("\nProblème de mémoire");
    else
    {
        L->lg = 0;
        L->tete = NULL;
        L->queue = NULL;
    }
    return(L);
}

void listeDetruire(LISTE L){
    int i;
    NOEUD p, q;
    q = L->tete;
    for(i = 1; i <= L->lg; i++)
    {
        p = q;
        q = q->suivant;
        noeudDetruire(p);
    }
    free(L);
}

void listeAfficher(LISTE L){
    int i;
    NOEUD p;
    p = L->tete;
    for(i = 1; i <= L->lg; i++)
    {
        elementAfficher(p->info);
        p = p->suivant;
    }
}

int listeTaille(LISTE L) {
    return (L->lg);
}
