#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#ifndef LIBRARY_H
	#define LIBRARY_H
	#include "dessine.h"
	#include "goban.h"
	#include "menu.h"
	#include "file.h"
#endif

int nbCase, espaceCase, courFenetre;
typePlayer joueur1, joueur2;
colorPion tour;
Goban *goban;
FILE *file;
Button* pass;
Button* save;
Button* delete;
bool initialized, passer, deleteMode, gameFinished;

void setTour(colorPion p) {
	tour = p;
}

bool getInitialized() {
	return initialized;
}

bool calculTerritoire(Intersection* inter, colorPion* couleur, bool verif) {
	inter->alreadyUse = true;

	if(inter->interHaut && inter->interHaut->pion && verif) {
		printf("Oh un pion en haut !\n");
		if(!couleur) { couleur = &inter->interHaut->pion->couleur; }
		else if ( *couleur != inter->interHaut->pion->couleur ) { verif = false; }
	} else if (inter->interHaut && !inter->interHaut->pion && !inter->interHaut->alreadyUse) {
		printf("Passage en haut\n");
		verif = calculTerritoire(inter->interHaut, couleur, verif);
	}

	if(inter->interDroite && inter->interDroite->pion && verif) {
		printf("Oh un pion a droite !\n");
		if(!couleur) { couleur = &inter->interDroite->pion->couleur; }
		else if ( *couleur != inter->interDroite->pion->couleur ) { verif = false; }
	} else if (inter->interDroite && !inter->interDroite->pion && !inter->interDroite->alreadyUse) {
		printf("Passage a droite\n");
		verif = calculTerritoire(inter->interDroite, couleur, verif);
	}

	if(inter->interBas && inter->interBas->pion && verif) {
		printf("Oh un pion en bas !\n");
		if(!couleur) { couleur = &inter->interBas->pion->couleur; }
		else if ( *couleur != inter->interBas->pion->couleur ) { verif = false; }
	} else if (inter->interBas && !inter->interBas->pion && !inter->interBas->alreadyUse) {
		printf("Passage en bas\n");
		verif = calculTerritoire(inter->interBas, couleur, verif);
	}

	if(inter->interGauche && inter->interGauche->pion && verif) {
		printf("Oh un pion a gauche !\n");
		if(!couleur) { couleur = &inter->interGauche->pion->couleur; }
		else if ( *couleur != inter->interGauche->pion->couleur ) { verif = false; }
	} else if (inter->interGauche && !inter->interGauche->pion && !inter->interGauche->alreadyUse) {
		printf("Passage a gauche\n");
		verif = calculTerritoire(inter->interGauche, couleur, verif);
	}

	return verif;
}

void calculPoints() {
	// TODO: Ben... le calcul des points patate
	printf("Attention parce que là ya de l'ago qui va sortir\n");
	colorPion* couleur = NULL;
	int count = 0;
	bool verif = true;
	/*if(couleur) {
		printf("YA UNE COULEUR");
	} else {
		printf("YA PAS DE COULEUR");
	}*/

	for(int i = 0; i < nbCase; i++) {
		for(int y = 0; y < nbCase; y++) {
			if(!goban->intersections[i][y]->pion && !goban->intersections[i][y]->alreadyUse) {
				printf("----------- VERIFICATION TERRITOIRE [%d][%d] -----------\n", i, y);
				verif = calculTerritoire(goban->intersections[i][y], couleur, true);
				printf("----------- FIN CALCUL - TERRITOIRE : %d -----------\n", verif);
			}
		}
	}
}

void draw_pion(int x, int y, colorPion colorPion){

	if(colorPion == BLANC){ color(1,1,1); }
	else { color(0,0,0); }

	// Taille du pion
	filled_circle(x,y,(espaceCase /2)*0.85);
}

void draw_hoshi(int width, int height){

	int i,j;
	color(0,0,0);
	printf("\nDessine les Hoshis");

	switch(nbCase)
	{
		case 9:
				for(i = 2; i <=6; i+=4){
				for(j= 2; j<= 6; j+=4){
					int posX = espaceCase + (j * espaceCase);
					int posY = espaceCase + (i * espaceCase);
					filled_circle(posX, posY, ((float)1.5 * espaceCase / 50) + 1.9);
				}
			}
			break;
		case 13:
			for(i = 1; i <= 3; i++){
				for(j = 1; j <= 3; j++){
					int posX = espaceCase + (3 * j * espaceCase);
					int posY = espaceCase + (3 * i * espaceCase);
					filled_circle(posX, posY, ((float)2 * espaceCase / 50) + 1.9);
				}
			}
			break;
		case 19:
			for(i = 3; i <=15; i+=6){
				for(j= 3; j<= 15; j+=6){
					int posX = espaceCase + (j * espaceCase);
					int posY = espaceCase + (i * espaceCase);
					filled_circle(posX, posY, ((float)2.5 * espaceCase / 50) + 1.9);
				}
			}
			break;
	}
}

void draw_tour_jeu() {
	color(0, 0, 0);
	char tourLbl[] = "Prochaine pierre";
	int width = width_win();
	int height = height_win();

	string(espaceCase * nbCase + 65, espaceCase + (espaceCase / 2), tourLbl);
	draw_pion(espaceCase * nbCase + 110, espaceCase * 2 + (espaceCase / 4), tour);
}

void click_pass() {
	printf("----- ----- UN JOUEUR A PASSER SON TOUR ----- -----\n");
	if(passer) {
		gameFinished = true;
		calculPoints();
	}
	else {
		passer = true;
		if(tour == BLANC) { tour = NOIR; }
		else { tour = BLANC; }
		draw_tour_jeu();
	}
}

void click_deleteMode() {
	if(deleteMode) { deleteMode = false; }
	else {deleteMode = true; }
	//draw_plateau();
	draw_menu_game_delete();
}

void click_menu_game(int x, int y) {

	if(x > pass->x && x < pass->x + pass->w && y > pass->y && y < pass->y + pass->h) {
		pass->click();
	} else if (x > save->x && x < save->x + save->w && y > save->y && y < save->y + save->h) {
		save->click();
	} else if (x > delete->x && x < delete->x + delete->w && y > delete->y && y < delete->y + delete->h) {
		delete->click();
	}
}

void draw_menu_game_pass() {
	color(1, 1, 1);
	int pass_w = espaceCase * 3;
	int pass_h = pass_w / 2;
	int pass_x = espaceCase * (nbCase) + espaceCase * 0.80;
	int pass_y = espaceCase * (nbCase / 2) + 40;
	char passerLbl[] = "Passer";

	filled_rectangle(pass_x, pass_y, pass_w, pass_h);

	pass = init_button(pass_x, pass_y, pass_w, pass_h, &click_pass);

	color(0, 0, 0);
	rectangle(pass_x, pass_y, pass_w, pass_h);
	setLabelButton(passerLbl, pass_x, pass_y, pass_w, pass_h, 30);
}

void draw_menu_game_delete() {
	color(1, 1, 1);
	int del_w = espaceCase * 3;
	int del_h = del_w / 3;
	int del_x = espaceCase * (nbCase) + espaceCase * 0.80;
	int del_y = espaceCase * (nbCase / 2) + espaceCase * 1.5 + 60;
	char *deleteLbl = malloc(sizeof(char)*16);
	if(deleteMode) { deleteLbl = "Delete Mode: On "; }
	else { deleteLbl = "Delete Mode: Off"; }

	filled_rectangle(del_x, del_y, del_w, del_h);

	delete = init_button(del_x, del_y, del_w, del_h, &click_deleteMode);

	color(0, 0, 0);
	rectangle(del_x, del_y, del_w, del_h);
	setLabelButton(deleteLbl, del_x, del_y, del_w, del_h, 85);
}

void draw_menu_game_save() {
	color(1, 1, 1);
	int save_w = espaceCase * 3;
	int save_h = save_w / 3;
	int save_x = espaceCase * (nbCase) + espaceCase * 0.80;
	int save_y = espaceCase * (nbCase / 2) - save_h + 20;
	char saveLbl[] = "Sauvegarder";

	filled_rectangle(save_x, save_y, save_w, save_h);

	save = init_button(save_x, save_y, save_w, save_h, &click_pass);

	color(0, 0, 0);
	rectangle(save_x, save_y, save_w, save_h);
	setLabelButton(saveLbl, save_x, save_y, save_w, save_h, 50);
}

void draw_init_menu_game(int width, int height) {
	// TODO: bouton passer tour / bouton sauvegarder / bouton enlever pierres mortes / tour de jeu
	draw_menu_game_pass();
	draw_menu_game_save();
	draw_menu_game_delete();
	draw_tour_jeu();
}

void draw_plateau(int width, int height)
{
	clear_win();
	color(0,0,0);

	// On calcul le ratio pour garder un Goban carré
	if(height < width) {
		espaceCase = height / (nbCase + 1);
	}
	else {
		espaceCase = width / (nbCase + 1);
	}

	printf("\nEspacement entre ligne : %d\n",espaceCase);
	printf("\nEspacement entre colonne : %d\n",espaceCase);

	int i;

	// On redessine le plateau
	for(i=0; i < nbCase ; i++){
		line(espaceCase, espaceCase + (espaceCase * i), nbCase*espaceCase, espaceCase + (espaceCase * i));
	}

	for(i=0; i < nbCase ; i++){
		line(espaceCase + (espaceCase * i), espaceCase, espaceCase + (espaceCase * i), espaceCase * nbCase);
	}

	// On redessine les hoshi
	draw_hoshi(width, height);

	// On réaffecte les positions des intersections puis on redessine les pions
	for(int ligne = 0; ligne < nbCase; ligne++) {

		int posY = espaceCase + (espaceCase * ligne);

		for(int colonne = 0; colonne < nbCase; colonne++) {

			int posX = espaceCase + (espaceCase * colonne);
			updatePosIntersection(goban->intersections[ligne][colonne], posX, posY);

			Intersection* inter = goban->intersections[ligne][colonne];

			if(inter->pion && inter->pion->visible) {
				printf("Ya un pion à redessiner en %d %d", ligne, colonne);
					draw_pion(inter->x, inter->y, inter->pion->couleur);
			}
		}
	}
	draw_init_menu_game(width, height);
}

bool checkBoundsGoban(int x, int y){
	// On ne dessine pas hors du plateau
	if(x < espaceCase || y < espaceCase || x > nbCase * espaceCase || y > nbCase * espaceCase) {
		return false;
	}

	return true;
}

bool playIA(int ligne, int colonne) {

	Intersection *inter = goban->intersections[ligne][colonne];
	int random = rand() % 100;
	int size = getSizeCaseOccupe();
	printf("RANDOM GENERE: %d\n", random);

	if(size > (nbCase*nbCase) * 0.5 && size < (nbCase*nbCase) * 0.6){
		if(random < 3) { click_pass(); return false; }
	} else if (size > (nbCase*nbCase) * 0.6 && size < (nbCase*nbCase) * 0.7){
		if(random < 7) { click_pass(); return false; }
	} else if (size > (nbCase*nbCase) * 0.7 && size < (nbCase*nbCase) * 0.8) {
		if(random < 15) { click_pass(); return false; }
	} else if (size > (nbCase*nbCase) * 0.8 && size < (nbCase*nbCase) * 0.9) {
		if(random < 45) { click_pass(); return false; }
	} else if (size > (nbCase*nbCase) * 0.9 && size < (nbCase*nbCase)) {
		if(random < 75) { click_pass(); return false; }
	} else if (size == (nbCase*nbCase)) {
		click_pass();
		return false;
	}

	if(placerPion(file, goban, inter, tour, colonne, ligne)) {

		printf("Intersection -> pion %p\n", inter->pion);
		printf("Pion dessiné de couleur : %d visible : %d\n", inter->pion->couleur, inter->pion->visible);
		passer = false;
		draw_pion(inter->x,inter->y,tour);

		if(tour == BLANC) { tour = NOIR; }
		else { tour = BLANC; }
		draw_tour_jeu();

		printf("La couleur de la case est %d\n", goban->intersections[ligne][colonne]->pion->couleur);
		return true;
	} else {
		colonne++;
		if(colonne == nbCase) {
			colonne = 0;
			ligne++;
		}
		if(ligne == nbCase) {
			ligne = 0;
		}
		return(playIA(ligne, colonne));
	}
}

void whoIsPlaying() {
	int milisec = 100; // Millisecondes entre chaqu coup joue
	bool verif = true;

	if(nbCase == 19) {
		milisec = 150;
	} else if (nbCase == 13) {
		milisec = 300;
	}

	struct timespec req = {0};
	req.tv_sec = 0;
	req.tv_nsec = milisec * 1000000L;

	srand(time(NULL));
	int colonne1 = rand() % nbCase;
	int ligne1 = rand() % nbCase;
	int colonne2 = rand() % nbCase;
	int ligne2 = rand() % nbCase;

	if(tour == NOIR && joueur1 == IA && !gameFinished) {
			printf("\nJe suis le joueur 1 et je suis une IA\n");
			xflush();
			nanosleep(&req, (struct timespec *)NULL);
			verif = playIA(ligne1, colonne1);
	}
	if(tour == BLANC && joueur2 == IA && !gameFinished){
			printf("\nJe suis le joueur 2 et je suis une IA\n");
			xflush();
			nanosleep(&req, (struct timespec *)NULL);
			verif = playIA(ligne2, colonne2);
	}
	if(joueur1 == IA && joueur2 == IA && !gameFinished) { whoIsPlaying(); }
}

void refresh_plateau(int width, int height) {
	clear_win();
	draw_plateau(width, height);
}

// Manage changing window
void refresh_manager(int width, int height)
{
	clear_win();
	freeButtons();
	if(courFenetre == 0) {
		draw_menu_load(width, height);
	} else if (courFenetre == 1) {
		draw_goban_size(width, height);
	} else if (courFenetre == 2) {
		draw_choix_adversaire(width, height, joueur1, joueur2);
	} else if (courFenetre == 3) {
		initPlateau(goban, width, height, nbCase);
		file = createSGF(nbCase);
		draw_plateau(width, height);
		initialized = true;
		whoIsPlaying();
	} else if (courFenetre == 4) {
		draw_plateau(width, height);
		initialized = true;
		whoIsPlaying();
	}
}

void newParty() {
	courFenetre = 1;
	int w = width_win();
	int h = height_win();
	refresh_manager(w, h);
}

void loadParty() {

	char fileName[255];

	printf("\n========== Chargement de partie ==========\n");
	printf("\nVeuillez indiquer le chemin absolu du fichier SGF : \n");

	scanf("%s", fileName);

	file = fopen(fileName, "r+");

	if(file == NULL) {
		printf("ERROR : IMPOSSIBLE D'OUVRIR LE FICHIER : %s\n",fileName);
		loadParty();
	}
	else {
		printf("Chargement de la partie\n");
		nbCase = getNbCaseFromFile(file);
		initPlateau(goban, width_win(), height_win(), nbCase);

		createGameFromFile(file, goban);
		courFenetre = 4;
		int w = width_win();
		int h = height_win();
		refresh_manager(w, h);
	}
}

void choixTaille() {
	courFenetre = 2;
	int w = width_win();
	int h = height_win();
	refresh_manager(w, h);
}

void setGoban19() {
	nbCase =  19;
	choixTaille();
}

void setGoban13() {
	nbCase =  13;
	choixTaille();
}

void setGoban9() {
	nbCase =  9;
	choixTaille();
}

void choixAdversaire() {
	courFenetre = 2;
	int w = width_win();
	int h = height_win();
	refresh_manager(w, h);
}

void validationAdversaire() {
	courFenetre = 3;
	int w = width_win();
	int h = height_win();
	refresh_manager(w, h);
}

void choixJoueur1() {
	if(joueur1 == 0) {
		joueur1 = JOUEUR;
	} else {
		joueur1 = IA;
	}
	choixAdversaire();
}

void choixJoueur2() {
	if(joueur2 == 0) {
		joueur2 = JOUEUR;
	} else {
		joueur2 = IA;
	}
	choixAdversaire();
}

// Handle user click
void mouse_clicked(int bouton, int x, int y) {

	printf("\nBouton %d presse au coord. %d,%d \n",bouton,x,y);

	// Menu purpose
	bool verif = false;
	if(courFenetre < 3) {
		checkClick(x, y);
		verif = true;
	}

	if(courFenetre > 2) {
		click_menu_game(x, y);
	}

	if(tour == NOIR && joueur1 == IA) {
		printf("Laisse le temp à l'IA de jouer petit batard !\n");
		return;
	}
	if(tour == BLANC && joueur2 == IA) {
		printf("Laisse le temp à l'IA de jouer petit batard !\n");
		return;
	}

	if(checkBoundsGoban(x, y) && !verif && !deleteMode) {
		// On défini la case la plus proche du click
		int colonne = ((x + (espaceCase /2)) / espaceCase) -1;
		int ligne = ((y + (espaceCase /2)) / espaceCase) -1;

		Intersection *inter = goban->intersections[ligne][colonne];

		// Routine de vérification de placement de pion
		if(placerPion(file, goban, inter, tour, colonne, ligne)) {

			printf("Intersection -> pion %p\n", inter->pion);
			printf("Pion dessiné de couleur : %d visible : %d\n", inter->pion->couleur, inter->pion->visible);
			passer = false;
			//draw_pion(inter->x,inter->y,tour);
			draw_plateau(width_win(),height_win());
			if(tour == BLANC) { tour = NOIR; }
			else { tour = BLANC; }
			draw_tour_jeu();

			whoIsPlaying();
		}
	} else if(checkBoundsGoban(x, y) && deleteMode) {
		int colonne = ((x + (espaceCase /2)) / espaceCase) -1;
		int ligne = ((y + (espaceCase /2)) / espaceCase) -1;

		if(checkIfPion(goban, ligne, colonne)) {
			printf("THERE IS A STONE HERE, DELETE IT !!!\n");
			goban->intersections[ligne][colonne]->pion = NULL;
			int width = width_win();
			int height = height_win();
			draw_plateau(width, height);
		}
	}
}


/**
 * on a appuyé sur une touche
 * code: code touche x11 (XK_...)
 * c caractère correspondant si caractere
 * x_souris,y_souris position de la souris
 */
void key_pressed(KeySym code, char c, int x_souris, int y_souris)
{
	switch(code)
	{
		case XK_Down:
			printf("bas\n");
			break;
		case XK_Up:
			printf("haut\n");
			break;
		case XK_Left:
			printf("gauche\n");
			break;
		case XK_Right:
			printf("droite\n");
			break;
	//~ case XK_Return:
	//~ case XK_Shift_L:
	//~ case XK_Control_R:
	//~ ....
		default:
			break;
	}

	if (c>' ' && c<'z') {
		printf("char: %c \n",c);

		if(c == 'e'){
			endGameSGF(file, "(;RE[B+R]");
		}
	}

	printf(" avec pos souris: %d,%d \n",x_souris,y_souris);

}

int main(int argc, char **argv) {

	int width, height;
	courFenetre = 3;
	tour = NOIR;
	joueur1 = IA;
	joueur2 = IA;
	passer = false;
	deleteMode = false;
	initialized = false;
	gameFinished = false;

	if(argc >= 2) {
		sscanf(argv[1],"%d",&nbCase);
	}

	if(nbCase == 0){
		nbCase = 9;
	}

	switch(nbCase)
	{
		case 9:
			width = 700;
			height = 500;
			break;
		case 13:
			width = 700;
			height = 700;
			break;
		case 19:
			width = 900;
			height = 900;
			break;
		default:
			break;
	}

	init_win(width, height, "Go Go MoMo Game Desu",0.988,0.807,0.611);

	goban = malloc(sizeof *goban);

	event_loop();
	return EXIT_SUCCESS;
}
