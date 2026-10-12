#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grapheReseau.h"
#include "grapheReseau.c"
#include "file.h"
#include "file.c"
int prochainId(grapheReseau g){
    int maxId = -1;
    for (int i = 0; i < g->V; i++){
        if (g->articles[i] != NULL && g->articles[i]->id > maxId)
            maxId = g->articles[i]->id;
    }
    return maxId + 1;
}



int main()
{
    int choix;
    grapheReseau g = NULL;
    char fichier[200];

    do {
        printf("\n===== Reseau d'Information =====\n");
        printf(" 1. Charger un reseau depuis un fichier \n");
        printf(" 2. Ajouter un article \n");
        printf(" 3. Ajouter une citation \n");
        printf(" 4. Supprimer un article \n");
        printf(" 5. Supprimer une citation \n");
        printf(" 6. Afficher le reseau \n");
        printf(" 7. Articles cites par un article \n");
        printf(" 8. Articles qui citent un article \n");
        printf(" 9. Sources originales / articles isoles \n");
        printf(" 10. Article le plus cite \n");
        printf(" 11. Trier par date de publication \n");
        printf(" 12. Premier article citant \n");
        printf(" 13. Chaine de propagation \n");
        printf(" 14. Simuler la propagation (BFS) \n");
        printf(" 15.  Analyser les articles (fake news) \n");
        printf(" 16. Articles suspects les plus cites \n");
        printf(" 17. Simuler la suppression d’un article \n");
        printf(" 0. Quitter \n");
        printf("================================== \n");
        printf("veuillez choisir : ");
        scanf("%d", &choix);
        getchar();
        system("cls");

        switch(choix) {

        case 1:
            printf("Nom du fichier : ");
            fgets(fichier, 200, stdin);
            fichier[strcspn(fichier, "\n")] = '\0';
            if (g)
                detruireGraphe(g);
            g = chargerGraphe(fichier);
            if (g){
                printf("Reseau charge avec succes\n");}
            else
                printf("Echec du chargement.\n");
            break;

         case 2: {
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else {
                ELEMENT art = elementCreer();
                art->id = prochainId(g);
                printf("Nouvel article id  : %d\n", art->id);
                elementLire(&art);
                if (ajouterArticle(g, art))
                    printf("Article ajoute avec id = %d.\n", art->id);
                else
                    printf("Echec ajout article.\n");
            }
            break;
        }
        case 3: {
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            int src, dest;
            printf("id source : "); scanf("%d", &src);
            printf("id destination : "); scanf("%d", &dest);
            if (ajouterCitation(g, src, dest))
                printf("Citation ajoutee.\n");
            else
                printf("Echec ajout citation.\n");}
            break;
        }

        case 4: {
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            int id;
            printf("id de l'article a supprimer : ");
            scanf("%d", &id);
            if (supprimerArticle(g, id))
                printf("Article supprime.\n");
            else
                printf("Article introuvable.\n");}
            break;
        }

        case 5: {
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            int src, dest;
            printf("id source : "); scanf("%d", &src);
            printf("id destination : "); scanf("%d", &dest);
            if (supprimerCitation(g, src, dest))
                printf("Citation supprimee.\n");
            else
                printf("Citation introuvable.\n");}
            break;
        }

        case 6:
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            afficherGraphe(g);
            break;

        case 7: {
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            int id;
            printf("id de l'article : ");
            id=lireEntier();
            articlesCites(g, id);}
            break;
        }

        case 8: {
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            int id;
            printf("id de l'article : ");
            id=lireEntier();
            articlesCitants(g, id);}
            break;
        }

        case 9:
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            printf("Sources originales :\n");
            sourcesOriginales(g);
            printf("Articles isoles :\n");
            articlesIsoles(g);}
            break;

        case 10:
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            printf("Article(s) le(s) plus cite(s) :\n");
            articlePlusCite(g);}
            break;

        case 11:
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            trierParDate(g);}
            break;

        case 12: {
            if (!g)
                printf("Chargez d'abord un reseau.\n");
            else{
            int id;
            printf("id de l'article : ");
            id=lireEntier();
            premierCitant(g, id);}
            break;
        }

        case 13: {
            if (!g)
                    printf("Chargez d'abord un reseau.\n");
            else{
            int id;
            printf("id de l'article source : ");
            id=lireEntier();
            chainePropagation(g, id);}
            break;
        }
        case 14:{
            if (!g)
                    printf("Chargez d'abord un reseau.\n");
            else{
            int id;
             printf("id de l'article");
             id=lireEntier();
            simulerPropagation(g,id);}
            break;}
        case 15:{
             if (!g){
                    printf("Chargez d'abord un reseau.\n");}
            else{

            analyserReseau(g);}
            break;}
        case 16:{
             if (!g){
                    printf("Chargez d'abord un reseau.\n");}
            else{
            articlesSuspectsCites(g);}
            break;}
        case 17:{
             if (!g){
                    printf("Chargez d'abord un reseau.\n");}
            else{
                int id;

                printf("id de l'article");
                id=lireEntier();
                simulerSuppression(g,id);}
            break;}




        case 0:
            printf("Au revoir !\n");
            break;

        default:
            printf("Choix invalide.\n");
        }
    printf("\nAppuyez sur Entree pour revenir au menu principal...\n");
    getchar();
     } while (choix != 0);

    return 0;
}
