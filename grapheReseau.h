#ifndef GRAPHERESEAU_H_INCLUDED
#define GRAPHERESEAU_H_INCLUDED
#include "ELTARTICLE.h"
#include "adjList.h"
#include "fakesdb.h"


typedef struct {
int V;
ELEMENT *articles;
LISTE *adjList;
int *degre_in;
} *grapheReseau;

int trouverPosition(ELEMENT *articles, int taille, int id);
grapheReseau creerGraphe(int V);
grapheReseau chargerGraphe(const char *filename);
void detruireGraphe(grapheReseau g);
int ajouterArticle(grapheReseau g, ELEMENT art);
int supprimerArticle(grapheReseau g, int idArt);
int ajouterCitation(grapheReseau g, int idSrc, int idDest);
int supprimerCitation(grapheReseau g, int idSrc, int idDest);
void afficherGraphe(grapheReseau g);
void articlesCites(grapheReseau g, int idSrc);
void sourcesOriginales(grapheReseau g);
void articlesIsoles(grapheReseau g);
void articlesCitants(grapheReseau g, int idDest);
ELEMENT   articlePlusCite(grapheReseau g);
void trierParDate(grapheReseau g);
void premierCitant(grapheReseau g, int idDest);
void chainePropagation(grapheReseau g, int idSrc);
int trouverProchain(grapheReseau g, int visite[]);
int estSaturee(grapheReseau g);
int analyserArticle(ELEMENT art);
void analyserReseau(grapheReseau g);
void articlesSuspectsCites(grapheReseau g);
void simulerSuppression(grapheReseau g, int idArt);
int neutraliserPropagation(grapheReseau g,int idSrc, int idDest);


#endif
