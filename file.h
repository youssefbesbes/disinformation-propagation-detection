#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED
#include "adjList.h"
#include "grapheReseau.h"
typedef LISTE file;

LISTE creerFile(void);
void detruireFile(LISTE file);
void enfiler(LISTE file, ELEMENT e);
ELEMENT defiler(LISTE file);
int fileVide(LISTE file);
int fileTaille(LISTE file);
void simulerPropagation(grapheReseau g, int idSrc);
void articlesAccessibles(grapheReseau g, int idSrc);

#endif
