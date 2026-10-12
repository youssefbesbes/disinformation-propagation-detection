#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ELTARTICLE.h"

int lireEntier() {
    int n;
    int valide=0;
    do {
        valide = scanf("%d", &n);
        if (valide==0) {
            printf(" invalide ! Veuillez entrer un nombre.\n");
            while(getchar() != '\n');
        }
    } while (valide==0);
    return n;
}

int dateValide(int jour, int mois, int annee) {
    int joursMax;
    if (mois < 1 || mois > 12)
        return 0;
    switch (mois) {
     case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        joursMax = 31; break;
     case 4: case 6: case 9: case 11:
        joursMax = 30; break;
     case 2:
        if ((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0))
            joursMax = 29;
        else
            joursMax = 28;
        break;
     default:
        return 0;
    }
    if (jour < 1 || jour > joursMax)
        return 0;
    return 1;
}

ELEMENT elementCreer (void) {
    ELEMENT L;
    L = (ELEMENT) malloc(sizeof(articleStruct));
    return L;
}

void elementDetruire (ELEMENT elt) {
    free (elt);
}

void elementLire(ELEMENT *elt){
    printf("\nDonnez le titre : ");
    scanf(" %[^\n]", (*elt)->titre);
    printf("Donnez la source : ");
    scanf(" %[^\n]",(*elt)->source);
    do {
        printf("\nDonnez le score de fiabilite (0-100) : ");
        (*elt)->score_fiabilite=lireEntier();
        if ((*elt)->score_fiabilite < 0 || (*elt)->score_fiabilite > 100)
            printf("Score invalide ! Entrez un score entre 0 et 100.\n");
    } while ((*elt)->score_fiabilite < 0 || (*elt)->score_fiabilite > 100);
    do{
        printf("donner le jour courant\n");
        (*elt)->jour=lireEntier();
        printf("donner le moins courant\n");
        (*elt)->mois=lireEntier();
        printf("donner l'annee courante\n");
        (*elt)->annee=lireEntier();
        if (dateValide(((*elt)->jour), ((*elt)->mois), ((*elt)->annee))!=1)
            printf("saisie une date valide\n");
    } while (dateValide(((*elt)->jour), ((*elt)->mois), ((*elt)->annee))!=1);
    do {
        printf("\nDonnez l'heure (0-23) : ");
        (*elt)->heure=lireEntier();
        if ((*elt)->heure < 0 || (*elt)->heure > 23)
            printf("Heure invalide ! Entrez une heure entre 0 et 23.\n");
    } while ((*elt)->heure < 0 || (*elt)->heure > 23);
    do {
        printf("\nDonnez la minute (0-59) : ");
        (*elt)->minute=lireEntier();
        if ((*elt)->minute < 0 || (*elt)->minute > 59)
            printf("Minute invalide ! Entrez une minute entre 0 et 59.\n");
    } while ((*elt)->minute < 0 || (*elt)->minute > 59);
}

void elementAfficher(ELEMENT elt) {
    printf("%s (id:%d, source:%s,\n",
           elt->titre, elt->id, elt->source);
    printf("score:%d, %02d/%02d/%04d %02dh%02d)\n",
           elt->score_fiabilite,
           elt->jour, elt->mois, elt->annee,
           elt->heure, elt->minute);
}

int comparerDates (ELEMENT elt1, ELEMENT elt2) {
    if (elt1->annee != elt2->annee)
        return elt1->annee - elt2->annee;
    else if (elt1->mois != elt2->mois)
        return elt1->mois - elt2->mois;
    else if (elt1->jour != elt2->jour)
        return elt1->jour - elt2->jour;
    else if (elt1->heure != elt2->heure)
        return elt1->heure - elt2->heure;
    else
        return elt1->minute - elt2->minute;
}

void elementAffecter(ELEMENT* e1, ELEMENT e2) {
    *e1 = e2;
}

