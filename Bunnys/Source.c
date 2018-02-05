#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <string.h>

#include <Windows.h>

#include "functions.h"
#include "bunny.h"

#define MAX_BUNNIES 1000

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
		//execute next cycle
		nextTurn(&anchor, &bunnyCount, &infects);

		//display infos and amount of bunny
		displayGrid(anchor);
		displayInfo(anchor, &bunnyCount, &infects);

		Sleep(2000);
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
	//as long as x AND y == 0 OR Field is taken OR coord+offset is out of range
	while ((xOff == 0 && yOff == 0) || testEmpty(anchor, tmpPt) == 1 || (coords.x + xOff > GRID || coords.x + xOff<0 || coords.y + yOff>GRID || coords.y + yOff < 0)) {
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

//show all bunnies in their grid
void displayGrid(bunny *anchor) {
	bunny *p;
	int i, j;
	int k = 100;

	//Grid Grid[x][y]
	char Grid[GRID][GRID];

	//assign blanc to Grid[][]
	//y-Grid
	for (int i = 0; i < GRID; i++) {
		//x-Grid
		for (int j = 0; j < GRID; j++) {
			Grid[j][i] = ' ';
		}
	}

	//assign the bunnyPos x,y to Grid[x][y]
	for (p = anchor; p != NULL; p = p->next) {
		//decide, if X,m,M,...
		if (p->radioactive_mutant_vampire_bunny == 1) {
			Grid[p->coord.x][p->coord.y] = 'X';
		}
		else if (p->sex == male) {
			if (p->age >= 2) Grid[p->coord.x][p->coord.y] = 'M';
			else Grid[p->coord.x][p->coord.y] = 'm';
		}
		else {
			if (p->age >= 2)Grid[p->coord.x][p->coord.y] = 'F';
			else Grid[p->coord.x][p->coord.y] = 'f';
		}
	}

	//y-Grid
	for (int i = 0; i < GRID; i++) {
		//x-Grid
		for (int j = 0; j < GRID; j++) {
			printf("|");
			//if no Bunny
			if (Grid[j][i] == -52) printf(" ");
			//if Bunny
			else printf("%c", Grid[j][i]);
		}
		printf("\n");
	}
}

void displayInfo(bunny *anchor, int *bunnyCount, int *infects) {
	/*
	//for displaying all of the shit

	bunny *p;

	printf("\n");

	for (p = anchor; p != NULL; p = p->next) {
		printf("Name: %s \nSex: %d \nColor: %d \nAge: %d \n", p->Name, p->sex, p->color, p->age);
		if (p->radioactive_mutant_vampire_bunny == 1) {
			printf("He's a radioactive-mutant-vampire-bunny\n");
		}
		Sleep(1000);
		printf("\n");
	}
	*/
	printf("There're now %d/%d bunnies healthy\n", *bunnyCount - *infects, *bunnyCount);
	printf("--------------------------------------------------\n");
}//end displayInfo