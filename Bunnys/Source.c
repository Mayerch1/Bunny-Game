#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef linux
#define clear() printf("\033[H\033[J")
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "functions.h"
#include "bunny.h"

//END OF STAGE ONE

int main(void) {
	srand(time(NULL));
	//int zf = rand() % 100 + 1; //1-100

	//create first 5 bunnies
	bunny *anchor;
	int bunnyCount = 0;
	int infects = 0;

	Point initCoord;

	initCoord.x = rand() % GRID;
	initCoord.y = rand() % GRID;

	anchor = NULL;
	anchor = createBunny(anchor, rand() % 4, &bunnyCount, &infects, initCoord);
	for (int i = 0; i < 8; i++) {
		initCoord.x = rand() % GRID;
		initCoord.y = rand() % GRID;
		bunny_append(anchor, createBunny(anchor, rand() % 4, &bunnyCount, &infects, initCoord));
	}
	//end create first 5 bunnies

	//init console color display
#ifdef _WIN32
	Sleep(1000);
	system("cls");
#endif
#ifdef linux
	sleep(1);
	clear();
#endif
	//end init color

	//start the game
	while (anchor->next != NULL) {
		//execute next cycle, incl move
		nextTurn(&anchor, &bunnyCount, &infects);

		//display infos and amount of bunny
		displayGrid(anchor);
		displayInfo(anchor, &bunnyCount, &infects);

		//wait, depending on OS
#ifdef _WIN32
		Sleep(1500);
		system("cls");
#endif
#ifdef linux
		sleep(2);
		clear();
#endif
		/*
				//only move, display, wait
				moveBunny(&anchor);
				displayGrid(anchor);
				displayInfo(anchor, &bunnyCount, &infects);

				Sleep(500);
				system("cls");
				*/
	}
	printf("The last survivor will die soon\n");
	printf("End of simulation");
	return 0;
}//end main

//age Bunnys, initiate regular actions
void nextTurn(bunny **anchor, int *bunnyCount, int *infects) {
	//infect healthy bunnies
	infectBunnies(anchor, bunnyCount, infects);

	//let them walk
	moveBunny(anchor);

	//inc age
	ageBunnies(anchor, bunnyCount, infects);

	//reproduce them
	reproduce(anchor, bunnyCount, infects);

	//food shortage
	if (*bunnyCount > MAX_BUNNIES) {
		starveBunnies(anchor, bunnyCount, infects);
	}
}//end nextTurn

//moves every Bunny randomly
void moveBunny(bunny **anchor) {
	Point coords;
	Point offset;
	bunny *p;

	for (p = *anchor; p != NULL; p = p->next) {
		//get random free Field next to p->coord
		//returns (0,0) if every field is taken
		offset = findField(anchor, 0, p->coord); //gets empty fields

		//if there's a free field
		if (offset.x != 0 || offset.y != 0) {
			//add offset to coord
			p->coord.x += offset.x;
			p->coord.y += offset.y;
		}
		//else don't move it
	}
}//end moveBunny

//find a free field, returns Offset to one of the free fields
//for state == 0 -> freeFields
//for state == 1 -> taken Fields
Point findField(bunny **anchor, int state, Point coords) {
	Point tmpPt;
	Point offset;

	offset.x = 0;
	offset.y = 0;
	//if state is != 0, 1
	if (state != 0 && state != 1) return offset;

	int targetField[9][2];

	//set Field to 0;
	for (int i = 0; i < 9; i++) {
		targetField[i][0] = 0;
		targetField[i][1] = 0;
	}

	//free Fields around bunny
	//-1 means every field is taken
	int freeFields = -1;
	tmpPt = coords;

	//start on the left, top
	tmpPt.x--;
	tmpPt.y--;

	//gets amount of free fields around bunny, and coords of it
	//move from left to right
	for (int i = 0; i < 3; i++) {
		//move from top to bottom
		for (int j = 0; j < 3; j++) {
			//check if field is taken
			int isFree = testEmpty(anchor, tmpPt);

			//if field is free
			if (isFree == state) {
				//set the field to the coordinate
				freeFields++;
				targetField[freeFields][0] = tmpPt.x;
				targetField[freeFields][1] = tmpPt.y;
			}
			tmpPt.y++;
		}
		tmpPt.y = coords.y - 1;
		tmpPt.x++;
	}

	//if no field is empty
	if (freeFields == -1) {
		offset.x = 0;
		offset.y = 0;

		return offset;
	}

	//rand Nr, amount of free Field = highest Number
	//from field nr 0 to 'emptyFields'-1
	int randNr = rand() % (freeFields + 1);

	//choose the corresponding field/coordinate to randNr
	offset.x = targetField[randNr][0] - coords.x;
	offset.y = targetField[randNr][1] - coords.y;

	return offset;
}//end findField

//check if requested grid is emtpy, 0 or 1
int testEmpty(bunny **anchor, Point coords) {
	bunny *p;
	int isTaken = 0;

	//if coords are out of range/Grid
	if (coords.x >= GRID || coords.x < 0 || coords.y >= GRID || coords.y < 0) {
		//handle as field would be taken
		return 1;
	}

	//go through list, break if grid is taken
	for (p = *anchor; p != NULL; p = p->next) {
		if (coords.x == p->coord.x && coords.y == p->coord.y) {
			isTaken = 1;
			break;
		}
	}
	//field is taken=1, field is emtpy=0
	return isTaken;
}//testEmtpy

//if bunny is in requested Grid, return pointer to him
bunny *matchToGrid(bunny **anchor, Point coords) {
	bunny *p;
	//go through, until coords of both match
	for (p = *anchor; p != NULL; p = p->next) {
		if (p->coord.x == coords.x && p->coord.y == coords.y) {
			return p;
		}
	}
	//return NULL if no bunny at coords
	return NULL;
}