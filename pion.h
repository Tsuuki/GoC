#ifndef BOOL_H
	#define BOOL_H
	#include "boolean.h"
#endif

typedef enum { UNDEFINED, BLANC, NOIR} colorPion;


typedef struct Pion Pion;
struct Pion{
	colorPion couleur;
	bool visible;
};

Pion *initPion(colorPion);
