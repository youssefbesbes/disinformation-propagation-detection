#include "file.h"
#include <stdlib.h>
#include <stdio.h>

LISTE creerFile() {
    return listeCreer();
}

void detruireFile(LISTE file) {
    listeDetruire(file);
}

void enfiler(LISTE file, ELEMENT e) {
    inserer(file, e, listeTaille(file) + 1);
}

ELEMENT defiler(LISTE file) {
    ELEMENT e = recuperer(file, 1);
    supprimer(file, 1);
    return e;
}

int fileTaille(LISTE file) {
    return (listeTaille(file));
}

int fileVide(LISTE file) {
    return listeTaille(file) == 0;
}

void simulerPropagation(grapheReseau g, int idSrc) {

    int visite[g->V];
    int niveau[g->V];

    for (int i = 0; i < g->V; i++) {
        visite[i] = 0;
        niveau[i] = -1;
    }

    LISTE file = creerFile();

    int indice_src = trouverPosition(g->articles, g->V, idSrc);
    if (indice_src < 0) { printf("Article introuvable\n"); return; }

    inserer(file, g->articles[indice_src], 1);
    visite[indice_src] = 1;
    niveau[indice_src] = 0;

    int niveauActuel = -1;
    int totalAtteints = 1;

    while (fileTaille(file) > 0) {

        ELEMENT u = defiler(file);
        int indice = trouverPosition(g->articles, g->V, u->id);

        int niveauU = niveau[indice];

        if (niveauU != niveauActuel) {
            if (niveauActuel != -1) printf("\n");
            printf("Niveau %d : %s", niveauU, u->titre);
            niveauActuel = niveauU;
        } else {
            printf(", %s", u->titre);
        }

        for (int i = 0; i < g->V; i++) {
            if (visite[i] == 0) {
                int dejaTrouve = 0;
                int pos = 1;
                while (pos <= listeTaille(g->adjList[i]) && !dejaTrouve) {
                    ELEMENT cite = recuperer(g->adjList[i], pos);
                    if (cite->id == u->id) {
                        enfiler(file, g->articles[i]);
                        visite[i] = 1;
                        niveau[i] = niveauU + 1;
                        totalAtteints++;
                        dejaTrouve = 1;
                    } else {
                        pos++;
                    }
                }
            }
        }
    }

    printf("\n\n%d niveaux, %d articles atteints.\n", niveauActuel + 1, totalAtteints);

    listeDetruire(file);
}
void articlesAccessibles(grapheReseau g, int idSrc) {
    int visite[g->V];
    for (int i = 0; i < g->V; i++) {
        visite[i] = 0;
    }

    LISTE file = creerFile();
    int indice_src = trouverPosition(g->articles, g->V, idSrc);
    if (indice_src < 0) { printf("Article introuvable\n");  }

    inserer(file, g->articles[indice_src], 1);
    visite[indice_src] = 1;

    printf("Articles accessibles depuis %s :\n", g->articles[indice_src]->titre);

    while (fileTaille(file) > 0) {
        ELEMENT u = defiler(file);
        int indice = trouverPosition(g->articles, g->V, u->id);

        printf("  %s\n", u->titre);

        for (int i = 0; i < g->V; i++) {
            if (visite[i] == 0) {
                int dejaTrouve = 0;
                int pos = 1;
                while (pos <= listeTaille(g->adjList[i]) && !dejaTrouve) {
                    ELEMENT cite = recuperer(g->adjList[i], pos);
                    if (cite->id == u->id) {
                        enfiler(file, g->articles[i]);
                        visite[i] = 1;
                        dejaTrouve = 1;
                    } else {
                        pos++;
                    }
                }
            }
        }
    }

    detruireFile(file);
}
