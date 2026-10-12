#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "grapheReseau.h"
#include "file.h"
#include "adjList.c"

#define LongMax 100

int trouverPosition(ELEMENT *articles, int taille, int id) {
    int i;
    for (i = 0; i < taille; i++) {
        if (articles[i] != NULL && articles[i]->id == id) {
            return i;
        }
    }
    return -1;
}

int estSaturee(grapheReseau g) {
    return (g->V == LongMax);
}

grapheReseau creerGraphe(int V) {
    grapheReseau g = (grapheReseau)malloc(sizeof(*g));
    if (g == NULL)
        return NULL;

    g->V = 0;
    g->articles = (ELEMENT *) malloc(LongMax * sizeof(ELEMENT));
    g->adjList  = (LISTE *)   malloc(LongMax * sizeof(LISTE));
    g->degre_in = (int *)     malloc(LongMax * sizeof(int));

    if (g->articles == NULL || g->adjList == NULL || g->degre_in == NULL) {
        free(g->articles);
        free(g->adjList);
        free(g->degre_in);
        free(g);
        return NULL;
    }

    for (int i = 0; i < LongMax; i++) {
        g->articles[i] =elementCreer();
        g->adjList[i]  = listeCreer();
        g->degre_in[i] = 0;
    }

    return g;
}

grapheReseau chargerGraphe(const char *filename) {

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur ouverture fichier\n");
        return NULL;
    }

    char ligne[256];
    int V = 0;
    while (fgets(ligne, sizeof(ligne), f)) {
        if (ligne[0] == 'A')
            V++;
    }
    grapheReseau g = creerGraphe(V);
    rewind(f);
    int id, score, jour, mois, annee, heure, minute;
    char titre[100], source[50];

    while (fgets(ligne, sizeof(ligne), f)) {
        if (ligne[0] == 'A') {
            sscanf(ligne, "A %d %s %s %d %d %d %d %d %d",
                   &id, titre, source, &score,
                   &jour, &mois, &annee, &heure, &minute);

            ELEMENT art = (ELEMENT) malloc(sizeof(articleStruct));
            art->id = id;
            strcpy(art->titre, titre);
            strcpy(art->source, source);
            art->score_fiabilite = score;
            art->jour = jour;
            art->mois = mois;
            art->annee = annee;
            art->heure = heure;
            art->minute = minute;

            ajouterArticle(g, art);
        }
    }
    rewind(f);

    int src, dest;
    while (fgets(ligne, sizeof(ligne), f)) {
        if (ligne[0] == 'C') {
            sscanf(ligne, "C %d %d", &src, &dest);
            ajouterCitation(g, src, dest);
        }
    }

    fclose(f);
    return g;
}

void detruireGraphe(grapheReseau g) {
    for (int i = 0; i < g->V; i++) {
        elementDetruire(g->articles[i]);
        listeDetruire(g->adjList[i]);
    }
    free(g->articles);
    free(g->adjList);
    free(g->degre_in);
    free(g);
}

int ajouterArticle(grapheReseau g, ELEMENT art){
    if(!g)
        return 0;
    if (estSaturee(g))
       return 0;
    else{
        g->articles[g->V] = art;
        g->adjList[g->V]  = listeCreer();
        g->degre_in[g->V] = 0;
        (g->V)++;
        return 1;
    }
}

int supprimerArticle(grapheReseau g, int idArt) {
    int indice = trouverPosition(g->articles, g->V, idArt);
    if (indice < 0)
        return 0;
    else{
        for (int i = 0; i < g->V; i++) {
            if (i != indice) {
                LISTE L = g->adjList[i];
                NOEUD courant = L->tete;
                int trouve = 0;
                while (courant != NULL && trouve == 0) {
                    if (courant->info != NULL && courant->info->id == idArt) {
                        if (courant->precedent == NULL && courant->suivant == NULL) {
                            L->tete = NULL;
                            L->queue = NULL;
                        } else if (courant->precedent == NULL) {
                            L->tete = courant->suivant;
                            courant->suivant->precedent = NULL;
                        } else if (courant->suivant == NULL) {
                            L->queue = courant->precedent;
                            courant->precedent->suivant = NULL;
                        } else {
                            courant->precedent->suivant = courant->suivant;
                            courant->suivant->precedent = courant->precedent;
                        }
                        free(courant);
                        (L->lg)--;
                        trouve = 1;
                    } else {
                        courant = courant->suivant;
                    }
                }
            }
        }
        {
            LISTE L = g->adjList[indice];
            NOEUD courant = L->tete;
            while (courant != NULL) {
                NOEUD suivant = courant->suivant;
                free(courant);
                courant = suivant;
            }
            free(L);
        }
        elementDetruire(g->articles[indice]);
        for (int j = indice; j < g->V - 1; j++) {
            g->articles[j] = g->articles[j+1];
            g->adjList[j]  = g->adjList[j+1];
            g->degre_in[j] = g->degre_in[j+1];
        }
        g->articles[g->V - 1] = NULL;
        g->adjList[g->V - 1]  = listeCreer();
        g->degre_in[g->V - 1] = 0;
        (g->V)--;

        for (int i = 0; i < g->V; i++)
            g->degre_in[i] = 0;
        for (int i = 0; i < g->V; i++) {
            LISTE L = g->adjList[i];
            NOEUD p = L->tete;
            while (p != NULL) {
                if (p->info != NULL) {
                    int idx = trouverPosition(g->articles, g->V, p->info->id);
                    if (idx >= 0)
                        g->degre_in[idx]++;
                }
                p = p->suivant;
            }
        }
        return 1;
    }
}
int ajouterCitation(grapheReseau g, int idSrc, int idDest){
    if (!g)
        return 0;
    else{
        int indice  = trouverPosition(g->articles, g->V, idDest);
        int indice1 = trouverPosition(g->articles, g->V, idSrc);

        if (indice < 0 || indice1 < 0)
            return 0;
        else{
            if (listeEstSaturee(g->adjList[indice1]))
                return 0;
            else{
                inserer(g->adjList[indice1], g->articles[indice], g->adjList[indice1]->lg + 1);
                g->degre_in[indice]++;
                return 1;
            }
        }
    }
}

int supprimerCitation(grapheReseau g, int idSrc, int idDest){
    if (!g)
        return 0;
    else{
        int indice  = trouverPosition(g->articles, g->V, idDest);
        int indice1 = trouverPosition(g->articles, g->V, idSrc);

        if (indice < 0 || indice1 < 0)
            return 0;
        else{
            if (estVide(g->adjList[indice1]))
                return 0;
            else{
                int pos = 1;
                int trouve = 0;
                while (pos <= listeTaille(g->adjList[indice1]) && trouve == 0) {
                    ELEMENT e = recuperer(g->adjList[indice1], pos);
                    if (e->id == idDest) {
                        supprimer(g->adjList[indice1], pos);
                        g->degre_in[indice]--;
                        trouve = 1;
                    } else {
                        pos++;
                    }
                }
                if (!trouve)
                    return 0;
                else
                    return 1;
            }
        }
    }
}

void afficherGraphe(grapheReseau g) {
    for (int i = 0; i < g->V; i++) {
        elementAfficher(g->articles[i]);

        NOEUD p = g->adjList[i]->tete;

        if (p == NULL) {
            printf("(ne cite aucun article)\n");
        } else {
            while (p != NULL) {
                printf("--> %s\n", p->info->titre);
                p = p->suivant;
            }
        }
        printf("\n");
    }
}

void articlesCites(grapheReseau g, int idSrc){
    int indice = trouverPosition(g->articles, g->V, idSrc);

    if (!g || indice < 0)
        printf("Erreur\n");
    else {
        NOEUD p = g->adjList[indice]->tete;
        if (p == NULL) {
            printf("(ne cite aucun article)\n");
        } else {
            while (p != NULL) {
                printf("--> %s\n", p->info->titre);
                p = p->suivant;
            }
        }
    }
}

void sourcesOriginales(grapheReseau g){
    for (int i = 0; i < g->V; i++){
        if (g->articles[i] != NULL && listeTaille(g->adjList[i]) == 0)
            printf("-->%s\n", g->articles[i]->titre);
    }
}

void articlesIsoles(grapheReseau g){
    for (int i = 0; i < g->V; i++){
        if (g->articles[i] != NULL && g->degre_in[i] == 0)
            printf("-->%s\n", g->articles[i]->titre);
    }
}

void articlesCitants(grapheReseau g, int idDest){
        int indice = trouverPosition(g->articles, g->V, idDest);
        if (indice < 0)
            printf("indice invalide ");
        else {
            int test=0;
            for (int i = 0; i < g->V; i++){
                int pos = 1;
                int trouve = 0;
                while (pos <= listeTaille(g->adjList[i]) && trouve == 0) {
                    ELEMENT e = recuperer(g->adjList[i], pos);
                    if (e->id == idDest) {
                        printf("-->%s\n", g->articles[i]->titre);
                        trouve = 1;
                        test=1;
                    } else {
                        pos++;
                    }
                }
            }
            if (test==0)
                printf("l'article n'etait pas cite par aucun article");
        }

}


ELEMENT articlePlusCite(grapheReseau g){
    int c = 0;
    for (int i = 0; i < g->V; i++){
        if (g->degre_in[i] > c)
            c = g->degre_in[i];
    }
    for (int i = 0; i < g->V; i++){
        if (c == g->degre_in[i])
            printf("-->%s(cite par %d articles)\n", g->articles[i]->titre,g->degre_in[i]);
    }
    return 0;
}

void trierParDate(grapheReseau g)
{
    ELEMENT *tab = malloc(g->V * sizeof(ELEMENT));
    int n = 0;
    for (int i = 0; i < g->V; i++) {
            tab[n] = g->articles[i];
            n++;

    }
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (comparerDates(tab[j], tab[min]) < 0) {
                min = j;
            }
        }
        if (min != i) {
            ELEMENT temp = tab[i];
            tab[i] = tab[min];
            tab[min] = temp;
        }
    }
    printf("Articles triés par date de publication :\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %s (%02d/%02d/%d %02dh%02d)\n",
               i + 1,
               tab[i]->titre,
               tab[i]->jour, tab[i]->mois, tab[i]->annee,
               tab[i]->heure, tab[i]->minute);
    }
    free(tab);
}

void premierCitant(grapheReseau g, int idDest)
{
    int n = 0;
    ELEMENT *tab = malloc(g->V * sizeof(ELEMENT));
    int indice = trouverPosition(g->articles, g->V, idDest);

    if (indice < 0)
        printf("indice invalide ");

    else {
        for (int i = 0; i < g->V; i++) {
            if (i != indice && g->articles[i] != NULL) {
                int pos = 1;
                int trouve = 0;
                while (pos <= listeTaille(g->adjList[i]) && trouve == 0) {
                    ELEMENT e = recuperer(g->adjList[i], pos);
                    if (e->id == idDest) {
                        tab[n] = g->articles[i];
                        n++;
                        trouve = 1;
                    } else {
                        pos++;
                    }
                }
            }
        }
        for (int i = 0; i < n - 1; i++) {
            int min = i;
            for (int j = i + 1; j < n; j++) {
                if (comparerDates(tab[j], tab[min]) < 0) {
                    min = j;
                }
            }
            if (min != i) {
                ELEMENT temp = tab[i];
                tab[i] = tab[min];
                tab[min] = temp;
            }
        }
        printf("l' article est :\n");
            printf("--->%s(%02d/%02d/%d %02dh%02d)", tab[0]->titre,tab[0]->jour, tab[0]->mois, tab[0]->annee,
               tab[0]->heure, tab[0]->minute);
        }

    free(tab);
}

int trouverProchain(grapheReseau g, int visite[])
{
    int prochain = -1;

    for (int i = 0; i < g->V; i++)
    {
        if (!visite[i])
        {
            LISTE L = g->adjList[i];

            for (int j = 1; j <= listeTaille(L); j++)
            {
                ELEMENT e = recuperer(L, j);
                int indice = trouverPosition(g->articles, g->V, e->id);

                if (indice >= 0 && visite[indice])
                {
                    if (prochain == -1 ||
                        comparerDates(g->articles[i], g->articles[prochain]) < 0)
                    {
                        prochain = i;
                    }
                }
            }
        }
    }

    return prochain;
}

void chainePropagation(grapheReseau g, int idSrc)
{
    int visite[g->V];
    for (int i = 0; i < g->V; i++)
        visite[i] = 0;

    int indice = trouverPosition(g->articles, g->V, idSrc);
    if (indice < 0) {
            printf("Article introuvable\n");}
    else{
    printf("%s\n", g->articles[indice]->titre);
    visite[indice] = 1;
    int test = 1;

    while (test)
    {
        int prochain = trouverProchain(g, visite);

        if (prochain == -1)
            test = 0;
        else {
            printf("--> cite par %s\n", g->articles[prochain]->titre);
            visite[prochain] = 1;
        }
    }
}}
int analyserArticle(ELEMENT art) {
    int score_suspicion = 0;

    char titre_lower[100];
    int i = 0;
    while (art->titre[i] != '\0') {
        if (art->titre[i] == '_') {
            titre_lower[i] = ' ';
        } else {
            titre_lower[i] = tolower(art->titre[i]);
        }
        i++;
    }
    titre_lower[i] = '\0';

    int f = 0;
    while (f < NB_FAKES) {
        if (strstr(titre_lower, BASE_FAKES[f]) != NULL) {
            score_suspicion += 40;
        }
        f++;
    }

    for (int m = 0; m < NB_SUSPECTS; m++) {
        if (strstr(titre_lower, MOTS_SUSPECTS[m]) != NULL) {
            score_suspicion += 10;
        }
    }

    int nouveau_score = 100 - score_suspicion;
    if (nouveau_score < 0) {
        nouveau_score = 0;
    }
    art->score_fiabilite = nouveau_score;

    return score_suspicion;
}

void analyserReseau(grapheReseau g) {
    printf("=== Rapport d'analyse du reseau ===\n\n");
    for (int i = 0; i < g->V; i++) {

        analyserArticle(g->articles[i]);
        int score_fib = g->articles[i]->score_fiabilite;

        if (score_fib < 40) {
            printf("[SUSPECT] %s (score: %d)\n", g->articles[i]->titre, score_fib);
        } else if (score_fib < 70) {
            printf("[DOUTEUX] %s (score: %d)\n", g->articles[i]->titre, score_fib);
        } else {
            printf("[FIABLE ] %s (score: %d)\n", g->articles[i]->titre, score_fib);
        }

        char titre_lower[100];
        int j = 0;
        while (g->articles[i]->titre[j] != '\0') {
            if (g->articles[i]->titre[j] == '_') {
                titre_lower[j] = ' ';
            } else {
                titre_lower[j] = tolower(g->articles[i]->titre[j]);
            }
            j++;
        }
        titre_lower[j] = '\0';

        int f = 0;
        int premier_raison = 0;
        while (f < NB_FAKES) {
            if (strstr(titre_lower, BASE_FAKES[f]) != NULL) {
                if (premier_raison == 0) {
                    printf("   raison : \"%s\"", BASE_FAKES[f]);
                    premier_raison = 1;
                } else {
                    printf(", \"%s\"", BASE_FAKES[f]);
                }
            }
            f++;
        }
        if (premier_raison == 1) {
            printf("\n");
        }

        int premier_mot = 0;
        for (int m = 0; m < NB_SUSPECTS; m++) {
            if (strstr(titre_lower, MOTS_SUSPECTS[m]) != NULL) {
                if (premier_mot == 0) {
                    printf("   mots  : %s", MOTS_SUSPECTS[m]);
                    premier_mot = 1;
                } else {
                    printf(", %s", MOTS_SUSPECTS[m]);
                }
            }
        }
        if (premier_mot == 1) {
            printf("\n");
        }

        printf("\n");
    }
}
void articlesSuspectsCites(grapheReseau g){
    int c = 0;
    for (int i = 0; i < g->V; i++){
        if ((g->degre_in[i] > c)&& (g->articles[i]->score_fiabilite<40))
            c = g->degre_in[i];
    }
    for (int i = 0; i < g->V; i++){
        if ((c == g->degre_in[i])&&(g->articles[i]->score_fiabilite<40))
            printf("-->%s(score:%d,cite par %d articles)\n", g->articles[i]->titre,g->articles[i]->score_fiabilite,g->degre_in[i]);
    }}
void articlesCitants1(grapheReseau g, int idDest){
    int premier = 1;
    for (int i = 0; i < g->V; i++){
        int pos = 1;
        int trouve = 0;
        while (pos <= listeTaille(g->adjList[i]) && trouve == 0) {
            ELEMENT e = recuperer(g->adjList[i], pos);
            if (e->id == idDest) {
                if (premier == 1) {
                    printf("%s", g->articles[i]->titre);
                    premier = 0;
                } else {
                    printf(", %s", g->articles[i]->titre);
                }
                trouve = 1;
            } else {
                pos++;
            }
        }
    }
}
void articlesCites1(grapheReseau g, int idSrc){
    int indice = trouverPosition(g->articles, g->V, idSrc);
    if (indice < 0) return;
    NOEUD p = g->adjList[indice]->tete;
    int premier = 1;
    while (p != NULL) {
        if (premier == 1) {
            printf("%s", p->info->titre);
            premier = 0;
        } else {
            printf(", %s", p->info->titre);
        }
        p = p->suivant;
    }
}
void simulerSuppression(grapheReseau g, int idArt) {
    int indice = trouverPosition(g->articles, g->V, idArt);
    if (indice < 0) {
        printf("indice invalide ");
    } else {
        printf("suppression de %s\n", g->articles[indice]->titre);
        if ((g->degre_in[indice] == 0) && (g->adjList[indice]->lg == 0)) {
            printf("la suppression de cet article n'a pas d'impact sur la propagation");
        } else {
            printf("Etait cite par : ");
            articlesCitants1(g, idArt);
            printf("(%d)\n", g->degre_in[indice]);
            printf("Citait : ");
            articlesCites1(g,idArt);
            printf("(%d)\n", g->adjList[indice]->lg);
            printf("Articles deconnectes : ");
            int accessiblesAvant[g->V];
            for (int i = 0; i < g->V; i++) {
                accessiblesAvant[i] = 0;
            }
            for (int i = 0; i < g->V; i++) {
                if (i != indice) {
                    int pos = 1;
                    int trouve = 0;
                    while (pos <= listeTaille(g->adjList[i]) && trouve == 0) {
                        ELEMENT cite = recuperer(g->adjList[i], pos);
                        if (cite->id == idArt) {
                            accessiblesAvant[i] = 1;
                            trouve = 1;
                        } else {
                            pos++;
                        }
                    }
                }
            }
            int idsAvant[g->V];
            int nbAvant = 0;
            for (int i = 0; i < g->V; i++) {
                if (accessiblesAvant[i] == 1) {
                    idsAvant[nbAvant] = g->articles[i]->id;
                    nbAvant++;
                }
            }
            supprimerArticle(g, idArt);
            int idSource = -1;
            for (int i = 0; i < g->V; i++) {
                if (g->degre_in[i] == 0) {
                    idSource = g->articles[i]->id;
                }
            }
            int accessiblesApres[g->V];
            for (int i = 0; i < g->V; i++) {
                accessiblesApres[i] = 0;
            }

            if (idSource != -1) {
                int visite[g->V];
                for (int i = 0; i < g->V; i++) {
                    visite[i] = 0;
                }
                LISTE file = creerFile();
                int indiceSource = trouverPosition(g->articles, g->V, idSource);
                enfiler(file, g->articles[indiceSource]);
                visite[indiceSource] = 1;
                accessiblesApres[indiceSource] = 1;

                while (fileTaille(file) > 0) {
                    ELEMENT u = defiler(file);
                    for (int i = 0; i < g->V; i++) {
                        int pos = 1;
                        int trouve = 0;
                        while (pos <= listeTaille(g->adjList[i]) && trouve == 0) {
                            ELEMENT cite = recuperer(g->adjList[i], pos);
                            if (cite->id == u->id) {
                                if (visite[i] == 0) {
                                    visite[i] = 1;
                                    accessiblesApres[i] = 1;
                                    enfiler(file, g->articles[i]);
                                }
                                trouve = 1;
                            } else {
                                pos++;
                            }
                        }
                    }
                }                {
                    NOEUD p = file->tete;
                    while (p != NULL) {
                        NOEUD s = p->suivant;
                        free(p);
                        p = s;
                    }
                    free(file);
                }
            }
            int premier = 1;
            for (int k = 0; k < nbAvant; k++) {
                int indiceApres = trouverPosition(g->articles, g->V, idsAvant[k]);
                if (indiceApres < 0 || accessiblesApres[indiceApres] == 0) {
                    if (indiceApres >= 0) {
                        if (premier == 1) {
                            printf("%s", g->articles[indiceApres]->titre);
                            premier = 0;
                        } else {
                            printf(", %s", g->articles[indiceApres]->titre);
                        }
                    }
                }
            }
            if (premier == 1) {
                printf("(aucun)");
            }
            printf("\n");
        }
    }
}














































































