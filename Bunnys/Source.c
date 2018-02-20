#ifdef _WIN32
#include <Windows.h>

#define clrcsl() system("cls");
#define slp(t) Sleep(t)
#endif

#ifdef linux
#include <unistd.h>

#define clrcsl() printf("\033[H\033[J")
#define slp(t) sleep(t/1000)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#include "functions.h"
#include "bunny.h"

FILE *myfile;

int gridX = 100;		//
int gridY = 60;		//how large is your console?
int foodCount = 5;

int main(int argc, char *argv[]) {
	//#definees, but for arguments
	unsigned int max_colony_size = 1000;	//arg[1]; >GridX*GridY makes no sense
	unsigned char infection_prob = 100;	//0-100%

	char noLog = 0, log = 0;		//arg for logfile

	//init color display
	clrcsl();
	slp(1000);
	//end init color display

	if (argc > 1) {
		//process arguments
		toLowerCase(argc, argv);
		getArgs(argc, argv, &max_colony_size, &infection_prob, &log, &noLog);
		//collision of -log and -noLog
		if (log == 1 && noLog == 1) noLog = 0;
	}

	char buff[40];	//for log_name, with enough space, 15 for string, 25 for time
	srand((unsigned int)time(NULL));

	//posiiton of food sources
	//equal to Point food[foodCount];
	Point *food = (Point*)alloca(sizeof(Point) * foodCount);

	//time for log-file
	time_t rawtime;
	struct tm * timeinfo;

	//get the time
	time(&rawtime);
	//into struct
	timeinfo = localtime(&rawtime);

	//combine name with time
	//yyy_mm_dd_hh.mm.ss
	snprintf(buff, 40, "Log_Bunny-%d_%d_%d__%d.%d.%d.txt", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,
		timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min,
		timeinfo->tm_sec);

	//for -nolog
	if (noLog == 1) {
		myfile = NULL;
	}
	else {
		//fstream init for data log
		if ((myfile = fopen(buff, "w")) == NULL) fprintf(stderr, "Could not open Logfile\n");
	}

	//random position of food
	//except right and bottom border
	for (int i = 0; i < foodCount; i++) {
		food[i].x = rand() % (gridX - 1);
		food[i].y = rand() % (gridY - 1);
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

	//next 14 bunnies
	for (int i = 0; i < 15; i++) {
		initCoord.x = rand() % gridX;
		initCoord.y = rand() % gridY;
		bunny_append(anchor, createBunny(anchor, rand() % 4, &bunnyCount, &infects, initCoord, food));
	}
	//end create first 15 bunnies

	//TODO: debug flags

	//start the game
	while (anchor->next != NULL) {
		cycles++;
		//execute next cycle, incl move
		nextTurn(&anchor, &bunnyCount, &infects, max_colony_size, infection_prob, food);

		//display infos and amount of bunny
		displayGrid(anchor, food, foodCount);
		displayInfo(anchor, &bunnyCount, &infects, cycles, log);

		//in case of program termination
		if (noLog != 1) fflush(myfile);

#ifdef _WIN32
		//mass murder 1/2 of all bunnys, with h
		if (kbhit()) {
			char state = getch();
			if (state == 'k' || state == 'K') {
				famineBunnies(&anchor, &bunnyCount, &infects);
			}
		}
#endif // _WIN32

		//wait
		slp(1000);
		clrcsl();
	}
	printf("The last survivor will die soon\n");
	printf("End of simulation");
	//free last bunny
	free(anchor);
	if (myfile != NULL) fclose(myfile);
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
	feedBunnies(anchor, food, foodCount);

	//die, if longer without food
	starveBunnies(anchor, bunnyCount, infects);

	//food shortage
	if ((unsigned int)*bunnyCount > max_colony_size) {
		famineBunnies(anchor, bunnyCount, infects);
	}
}//end nextTurn

//moves every Bunny randomly
void moveBunny(bunny **anchor, Point food[]) {
	Point offset;
	bunny *p;

	for (p = *anchor; p != NULL; p = (bunny*)p->next) {
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

	//set whole array to 0
	int targetField[9][2] = { {0,0} };

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
	for (int i = 0; i < foodCount; i++) {
		if (food[i].x == coords.x && food[i].y == coords.y) {
			isTaken = 1;
			return isTaken;
		}
	}

	//if coords are out of range/Grid
	if (coords.x >= gridX || coords.x < 0 || coords.y >= gridY || coords.y < 0) {
		//handle as field would be taken
		return 1;
	}

	//go through list, break if grid is taken
	for (p = *anchor; p != NULL; p = (bunny*)p->next) {
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
	for (p = *anchor; p != NULL; p = (bunny*)p->next) {
		if (p->coord.x == coords.x && p->coord.y == coords.y) {
			return p;
		}
	}
	//return NULL if no bunny at coords
	return NULL;
}

//converts given argv into lower case
void toLowerCase(int argc, char *argv[]) {
	//go throug all args
	for (int i = 1; i < argc; i++) {
		int j = 0;
		//parse to \0
		while (argv[i][j] != '\0') {
			//if capital, convert to non-capital
			if (argv[i][j] >= 0x41 && argv[i][j] <= 0x5A) {
				argv[i][j] += 0x20;
			}
			j++;
		}
	}
}//end toLowerCase

//saves feeded arguments into variables
void getArgs(int argc, char *argv[], unsigned int *max_colony_size, unsigned char *infection_prob, char *log, char *noLog) {
	for (int i = 1; i < argc; i++) {
		//display Help-page, then terminate
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
			printHelp();
			return 0;
		}

		//grid Xdsize
		if (strcmp(argv[i], "-x") == 0) {
			if (atoi(argv[i + 1]) > 0) gridX = atoi(argv[i + 1]);
		}
		//grid >dsize
		if (strcmp(argv[i], "-y") == 0) {
			if (atoi(argv[i + 1]) > 0) gridY = atoi(argv[i + 1]);
		}
		//max population size
		if (strcmp(argv[i], "-n") == 0) {
			if (atoi(argv[i + 1]) > 0) *max_colony_size = atoi(argv[i + 1]);
		}
		//infection rate
		if (strcmp(argv[i], "-inf") == 0) {
			if (atoi(argv[i + 1]) >= 0 && atoi(argv[i + 1]) <= 100) *infection_prob = atoi(argv[i + 1]);
		}
		//amount of food sources
		if (strcmp(argv[i], "-f") == 0) {
			if (atoi(argv[i + 1]) >= 0) foodCount = atoi(argv[i + 1]);
		}
		//detailed logfile
		if (strcmp(argv[i], "--log") == 0) {
			*log = 1;
		}
		//no logfile
		if (strcmp(argv[i], "--nolog") == 0) {
			*noLog = 1;
		}
	}
}//end getArgs