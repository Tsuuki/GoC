#ifndef INTERSECTION_H
	#define INTERSECTION_H
	#include "intersection.h"
#endif

typedef struct Goban{
	int nbCase;
	int width;
	int height;
	Intersection ***intersections;
};

void initPlateau(Goban*, int, int, int);

Intersection* placerPion(Goban*,int, int, int, int);

int checkPosePion();