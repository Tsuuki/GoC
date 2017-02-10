#ifndef INTERSECTION_H
	#define INTERSECTION_H
	#include "intersection.h"
#endif
#include "file.h"

typedef struct Goban Goban;
struct Goban{
	int nbCase;
	int width;
	int height;
	Intersection ***intersections;
};

int getSizeCaseOccupe();

void initPlateau(Goban*, int, int, int);

bool placerPion(FILE*, Goban*, Intersection*, colorPion, int, int);

bool checkPosePion(Goban*, Intersection*, colorPion);

bool checkIfPion(Goban*, int, int);

bool checkLiberte(Intersection*);

bool checkPriseChaine(Intersection*, colorPion, Intersection**, int*, bool);

bool checkAlreadyUse(Intersection** intersections, int*, Intersection* inter);

void createGameFromFile(FILE*, Goban*);

int calculLiberte(Intersection **chaine, int longueurChaine);

void supprimerChaine(Intersection**, int*);

bool checkLiberteAllie(Intersection*, colorPion, Intersection**, int*, bool);
