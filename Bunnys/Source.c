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

FILE *myfile;

int main(int argc, char *argv[]) {
	//#definees, but for arguments
	unsigned int max_colony_size = 1000;	//arg[1]; >GridX*GridY makes no sense
	unsigned char infection_prob = 100;	//0-100%
	unsigned int food_count = 2;			

	srand(time(NULL));
	
	//posiiton of food sources
	//except right and bottom border
	Point food[FOOD_COUNT];

	for (int i = 0; i < FOOD_COUNT; i++) {
		food[i].x = rand() % (GRIDX - 1);
		food[i].y = rand() % (GRIDY - 1);
	}

	//fstream init for data log
	myfile = fopen("Log_Bunny.txt", "w");
	fprintf(myfile, "Test123\n");

	//maxbunnies, infection_prob, +1 for appl. name
	if (argc >= 3) {
		//max population size
		if(atoi(argv[1]) > 0)  max_colony_size = atoi(argv[1]);
		//infection rate
		if (atoi(argv[2]) >= 0 && atoi(argv[2]) <= 100) infection_prob = atoi(argv[2]);
	}
	
	//create first 5 bunnies
	bunny *anchor;
	int bunnyCount = 0;
	int infects = 0;
	int cycles = 0;

	Point initCoord;

	//force first bunny next to food sourve
	initCoord.x = food[0].x + 1;
	initCoord.y = food[0].y + 1;

	anchor = NULL;
	anchor = createBunny(anchor, rand() % 4, &bunnyCount, &infects, initCoord, food);
	for (int i = 0; i < 15; i++) {
		initCoord.x = rand() % GRIDX;
		initCoord.y = rand() % GRIDY;
		bunny_append(anchor, createBunny(anchor, rand() % 4, &bunnyCount, &infects, initCoord, food));
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


	//TODO: debug flags
	infection_prob = 80;


	//start the game
	while (anchor->next != NULL) {
		cycles++;
		//execute next cycle, incl move
		nextTurn(&anchor, &bunnyCount, &infects, max_colony_size, infection_prob, food);

		//display infos and amount of bunny
		displayGrid(anchor, food);
		displayInfo(anchor, &bunnyCount, &infects, cycles);

		//wait, depending on OS
#ifdef _WIN32
		Sleep(1500);
		system("cls");
#endif
#ifdef linux
		sleep(2);
		clear();
#endif
	}
	printf("The last survivor will die soon\n");
	printf("End of simulation");
	fclose(myfile);
	return 0;
}//end main

//age Bunnys, initiate regular actions
void nextTurn(bunny **anchor, int *bunnyCount, int *infects, unsigned int max_colony_size, 
				unsigned char infection_prob, Point food[]) {
	//infect healthy bunnies
	infectBunnies(anchor, bunnyCount, infects, infection_prob, food);

	//let them walk
	moveBunny(anchor, food);

	//inc age
	ageBunnies(anchor, bunnyCount, infects);

	//reproduce them
	reproduce(anchor, bunnyCount, infects, food);

	//get feeded, if near source
	feedBunnies(anchor, food);

	//die, if longer without food
	starveBunnies(anchor, bunnyCount, infects);


	//food shortage
	if (*bunnyCount > max_colony_size) {
		famineBunnies(anchor, bunnyCount, infects);
	}
}//end nextTurn

//moves every Bunny randomly
void moveBunny(bunny **anchor, Point food[]) {
	Point coords;
	Point offset;
	bunny *p;

	for (p = *anchor; p != NULL; p = p->next) {
		//get random free Field next to p->coord
		//returns (0,0) if every field is taken
		offset = findField(anchor, 0, p->coord, food); //gets empty fields

		//if there's a free field
		if (offset.x != 0 || offset.y != 0) {
			//add offset to coord
			p->coord.x += offset.x;
			p->coord.y += offset.y;
		}
		//else don't move it
	}
}//end moveBunny

//find a field, returns Offset to one random out of the fields
//for state == 0 -> ret freeFields
//for state == 1 -> ret taken Fields
Point findField(bunny **anchor, int state, Point coords, Point food[]) {
	Point tmpPt;
	Point offset;

	offset.x = 0;
	offset.y = 0;
	//if state is != 0, 1; ret 0
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
			int isFree = testEmpty(anchor, tmpPt, food);

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
int testEmpty(bunny **anchor, Point coords, Point food[]) {
	bunny *p;
	int isTaken = 0;
	
	//test for food source
	for (int i = 0; i < FOOD_COUNT; i++) {
		if (food[i].x == coords.x && food[i].y == coords.y) {
			isTaken = 1;
			return isTaken;
		}
	}

	//if coords are out of range/Grid
	if (coords.x >= GRIDX || coords.x < 0 || coords.y >= GRIDY || coords.y < 0) {
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