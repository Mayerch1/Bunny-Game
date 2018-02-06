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

	//create first five bunnys
	bunny *anchor;
	int bunnyCount = 0;
	int infects = 0;

	Point initCoord;

	initCoord.x = rand() % GRID;
	initCoord.y = rand() % GRID;

	anchor = NULL;
	anchor = createBunny(anchor, rand() % 4, &bunnyCount, &infects, initCoord);
	for (int i = 0; i < 4; i++) {
		initCoord.x = rand() % GRID;
		initCoord.y = rand() % GRID;
		bunny_append(anchor, createBunny(anchor, rand() % 4, &bunnyCount, &infects, initCoord));
	}

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
	//let them walk
	moveBunny(anchor);

	//infect healthy bunnies
	infectBunnies(anchor, bunnyCount, infects);

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
		offset = findFreeField(anchor, p->coord);

		//if there's a free field
		if (offset.x != 0 || offset.y != 0) {
			//add offset to coord
			p->coord.x += offset.x;
			p->coord.y += offset.y;
		}
		//else don't move it
	}
}//end moveBunny

//find a free field, returns Offset
Point findFreeField(bunny **anchor, Point coords) {
	//TODO: break, if every field is taken, 'optimize' random gen.

	Point tmpPt;
	Point offset;

	int tmp = 0;
	int xOff = 0, yOff = 0;

	tmpPt = coords;
	//as long as x AND y == 0 OR Field is taken OR coord+offset is out of range; 0 is valid, GRID is not valid (only <Grid)
	while ((xOff == 0 && yOff == 0) || testEmpty(anchor, tmpPt) == 1 ||
		(coords.x + xOff >= GRID || coords.x + xOff < 0 || coords.y + yOff >= GRID || coords.y + yOff < 0)) {
		//reset tmpPt
		tmpPt = coords;
		xOff = rand() % 3 - 1;
		yOff = rand() % 3 - 1;
		tmpPt.x += xOff;
		tmpPt.y += yOff;
		//tmp solution for no empty field
		if (tmp++ > 45)break;
	}

	//assign offsets to point
	offset.x = xOff;
	offset.y = yOff;

	//TODO: enter if, if no field is emtpy
	//same tmp solution as above
	if (tmp > 45) {
		offset.x = 0;
		offset.y = 0;
	}

	//return Point as offset
	return offset;
}//end findFreeField

//check if requested grid is emtpy, 0 or 1
int testEmpty(bunny **anchor, Point coords) {
	bunny *p;
	int isTaken = 0;
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