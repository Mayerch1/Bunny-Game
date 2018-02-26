/*
// Source.c
//
// The Bunny.exe is a simulation, which simulates a virtual Bunny-colony
// Several different arguments can be feeded, to customize the experience
// As userinput at runtime, is the massmurder, acceleration and deceleration available (Windows only)

Copyright (C) 2018 Christian Mayer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

contact @https://github.com/mayerch1
//
*/

#ifdef _WIN32
#include <Windows.h>
#include <conio.h>

#define clrcsl() system("cls");
#define slp(t) Sleep(t)
#endif

#ifdef linux
#include <unistd.h>

#define clrcsl() printf("\033[H\033[J")
#define slp(t) sleep((float)(t/1000))
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#include "functions.h"
#include "bunny.h"

#define arr_len(x) sizeof(x) / sizeof((x)[0])

FILE *myfile;

int gridX = 100;	//
int gridY = 60;		//how large is your console?
int foodCount = 5;

void strcpy_safe(char *output, int str_len, const char* input) {
	strncpy(output, input, str_len);
	output[str_len - 1] = '\0';
}

void fprint_name(FILE *fp, char *Name) {
	int i = 0;
	while (Name[i] != '\0') {
		if (Name[i] == '_') {
			fprintf(fp, " ");
			i++;
		}
		else {
			fprintf(fp, "%c", Name[i++]);
		}
	}
}

int main(int argc, char *argv[]) {
	//-------------------------------------
	//#definees, but for arguments
	unsigned int max_colony_size = 1000;	//arg[1]; >GridX*GridY makes no sense
	unsigned char infection_prob = 80;		//0-100%
	unsigned int start_Bunnies = 15;		//first created bunnies
	unsigned int sleep_time = 1000;			//sleep between cycles

	//define as variable, for disabled food sources
	int max_hunger = 3;

	char noLog = 0, log = 0;		//arg for logfile
	char save = 0, load = 0;		//save the game, or load it
	char fileName[128];//= "game01.save";
	//end #defines
	//------------------------------------

	//posiiton of food sources
	Point *food = NULL;
	int *foodDur = NULL;

	//time for log-file
	time_t rawtime;
	struct tm * timeinfo;

	//create first n-bunnies
	bunny *anchor = NULL;
	int oldBunnyCount = 0;
	int bunnyCount = 0;
	int cycles = 0;

	Point initCoord;
	//end create first n

	//for log_name, with enough space, 15 for string, 25 for time
	char buff[40];

	//end of declarations
	//------------------------------------

	srand((unsigned int)time(NULL));

#ifdef _DEBUG
	//set arg-values for debug, analysis, balancing, etc
	//noLog = 1;
	log = 0;
	infection_prob = 0;

#endif

	//init color display
	clrcsl();
	slp(1000);
	//end init color display

	if (argc > 1) {
		//process arguments
		toLowerCase(argc, argv);
		getArgs(argc, argv, &max_colony_size, &infection_prob, &log, &noLog, &start_Bunnies, &sleep_time, &save, &load, fileName, arr_len(fileName));
		//collision of -log and -noLog
		if (log == 1 && noLog == 1) noLog = 0;
		//collision of save and load
		if (save == 1 && load == 1) save = 0;
	}

	//load the game, if wanted
	//-----------------------------------------------------------------
	//food count is  set to 0, to prevent checking for collision with food source
	//tmpFood stores real foodCount-val, to use it in loadBunnies
	//after every bunny is loaded, the food will be initialised
	//----------------------------------------------------------------
	if (load == 1) {
		oldBunnyCount = loadHead(&gridX, &gridY, &foodCount, &max_hunger, fileName);

		int tmpFood = foodCount;
		foodCount = 0;

		//create bunny which is going to die, as workaround
		//----
		//can't move
		initCoord.x = -10;
		initCoord.y = -10;
		anchor = createBunny(anchor, 1, 0, 51, 0, 0, &bunnyCount, initCoord, food);
		//----
		loadBunnies(tmpFood, oldBunnyCount, &bunnyCount, anchor, food, fileName);
		//reset foodCount
		foodCount = tmpFood;
	}

	//disable food sources
	if (foodCount == -1) {
		//no bunny will starve to death
		foodCount = 0;
		max_hunger = 51;
	}
	else if (foodCount > 0) {
		//equal to Point food[foodCount];
		food = (Point*)alloca(sizeof(Point) * foodCount);
		foodDur = (int *)alloca(sizeof(int) * foodCount);
	}

	//load food
	if (load == 1) loadFood(foodCount, food, foodDur, fileName);

	//get the time
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	//combine name with time
	//yyy_mm_dd_hh.mm.ss
	snprintf(buff, 40, "Log_Bunny-%d_%d_%d__%d.%d.%d.txt", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1,
		timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	//for -nolog
	if (noLog == 1) {
		myfile = NULL;
	}
	else {
		//fstream init for data log
		if ((myfile = fopen(buff, "w")) == NULL) fprintf(stderr, "Could not open Logfile\n");
	}

	//random position of food, except right and bottom border
	//exec only if no file was loaded
	if (load == 0) {
		for (int i = 0; i < foodCount; i++) {
			food[i].x = rand() % (gridX - 1);
			food[i].y = rand() % (gridY - 1);
			foodDur[i] = FOOD_DURATION;
		}

		//create first bunnys
		if (foodCount <= 0) {
			initCoord.x = rand() % gridX;
			initCoord.y = rand() % gridY;
		}
		else {
			//force first bunny next to food sourve
			initCoord.x = food[0].x + 1;
			initCoord.y = food[0].y + 1;
		}
		//first bunny is female
		anchor = createBunny(anchor, 1, rand() % 4, 0, 0, -1, &bunnyCount, initCoord, food);

		//next n-1 bunnies
		for (unsigned int i = 0; i < start_Bunnies - 1; i++) {
			initCoord.x = rand() % gridX;
			initCoord.y = rand() % gridY;
			bunny_append(anchor, createBunny(anchor, rand() % 2, rand() % 4, 0, 0, -1, &bunnyCount, initCoord, food));
		}
		//end create first n bunnies
	}

	//start the game
	//loop this until only one bunny is left
	//-------------------------------------------------------------------------------------------------
	while (anchor->next != NULL) {
		cycles++;
		//execute next cycle, incl move
		nextTurn(&anchor, &bunnyCount, max_colony_size, infection_prob, food, max_hunger, foodDur);

		//display infos and amount of bunny
		displayGrid(anchor, food, foodCount);
		displayInfo(anchor, &bunnyCount, cycles, log);

		//save the game
		if (save == 1) saveGame(gridX, gridY, anchor, food, foodCount, max_hunger, bunnyCount, fileName);

		//in case of program termination
		if (noLog != 1) fflush(myfile);

#ifdef _WIN32
		//handle key events
		if (_kbhit()) {
			char state = _getch();
			if (state == 'k' || state == 'K') {
				famineBunnies(&anchor, &bunnyCount);
			}
			else if (state == 'w' || state == 'W') {
				if (sleep_time < 250)
					sleep_time = 0;
				else
					sleep_time -= 250;
			}
			else if (state == 's' || state == 'S') {
				sleep_time += 250;
			}
			printf("sth happened\n");
		}
#endif // _WIN32

		//wait
		slp(sleep_time);
		clrcsl();
	}
	//end loop
	//-----------------------------------------------------------------------------------------------

	//end of game
	printf("The last survivor will die soon\n");
	printf("End of simulation\n");
	//free last bunny
	free(anchor);
	if (myfile != NULL) fclose(myfile);
	return 0;
}//end main

void nextTurn(bunny **anchor, int *bunnyCount, unsigned int max_colony_size,
	unsigned char infection_prob, Point food[], int max_hunger, int foodDur[]) {
	infectBunnies(anchor, bunnyCount, infection_prob, food);

	moveBunny(anchor, food);

	ageBunnies(anchor, bunnyCount);

	reproduce(anchor, bunnyCount, food);

	feedBunnies(anchor, food, foodCount, foodDur, bunnyCount);

	starveBunnies(anchor, bunnyCount, max_hunger);

	//food shortage
	if ((unsigned int)*bunnyCount > max_colony_size) {
		famineBunnies(anchor, bunnyCount);
	}
}//end nextTurn

int inBounds(int x, int y) {
	if (x < 0 || x > gridX || y < 0 || y > gridY) {
		return 0;
	}
	return 1;
}

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
}//end matchToGrid

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

void getArgs(int argc, char *argv[], unsigned int *max_colony_size, unsigned char *infection_prob, char *log, char *noLog, unsigned int *start_Bunnies,
	unsigned int *sleep_time, char *save, char *load, char *fileName, int file_len) {
	for (int i = 1; i < argc; i++) {
		//display Help-page, then terminate
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--h") == 0 || strcmp(argv[i], "--help") == 0) {
			printHelp();
			return;
		}

		//load the current game from a generic file
		if (strcmp(argv[i], "--load") == 0) {
			*load = 1;
		}
		//load the current game from a specific file
		if (strcmp(argv[i], "-load") == 0) {
			strcpy_safe(fileName, file_len, argv[i + 1]);
			*load = 1;
		}
		//save the current game to a generic file
		if (strcmp(argv[i], "--save") == 0) {
			*save = 1;
		}
		//save the current game to a specific file
		if (strcmp(argv[i], "-save") == 0) {
			strcpy_safe(fileName, file_len, argv[i + 1]);
			*save = 1;
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
			if (atoi(argv[i + 1]) >= -1) foodCount = atoi(argv[i + 1]);
		}
		//number of bunnies in the beginning
		if (strcmp(argv[i], "-s") == 0) {
			if (atoi(argv[i + 1]) >= 1) *start_Bunnies = atoi(argv[i + 1]);
		}
		//time between cycles
		if (strcmp(argv[i], "-slp") == 0) {
			int x = atoi(argv[i + 1]);
			if (atoi(argv[i + 1]) >= 0) *sleep_time = atoi(argv[i + 1]);
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