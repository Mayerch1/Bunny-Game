#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef linux
#define clear() printf("\033[H\033[J")
#include <unistd.h>
#endif

#include "functions.h"
#include <stdio.h>

#include "bunny.h"

#define COLOR_GRAY	  "\x1b[90m"
#define COLOR_RED     "\x1b[91m"
#define COLOR_GREEN   "\x1b[92m"
#define COLOR_YELLOW  "\x1b[93m"
#define COLOR_BLUE    "\x1b[94m"
#define COLOR_MAGENTA "\x1b[95m"
#define COLOR_CYAN    "\x1b[96m"
#define COLOR_RESET   "\x1b[0m"

//all commented printf() is for detailed display
//disabled for better "Grid-experience"
void bornMsg(bunny *born) {
	/*
	printf("%s was born ", born->Name);
	if (born->radioactive_mutant_vampire_bunny == 1) printf(" and it is a RADIOACTIVE-MUTANT-VAMPIRE-BUNNY!");
	printf("\n");
	*/
}

void infectMsg(bunny *victim) {
	//printf("%s was INFECTED with the virus\n", victim->Name);
}

void eolMsg(bunny *victim) {
	//printf("%s is EOL! :'( \n", victim->Name);
}

void starveMsg(int start, int *bunnyCount) {
	/*
	printf("\n\nFood shortage killed %d bunnies\n\n", start - *bunnyCount);
	#ifdef _WIN32
	Sleep(500);
	#endif
	#ifdef linux
	sleep(1);
	#endif
	*/
}

//show all bunnies in their grid
void displayGrid(bunny *anchor) {
	bunny *p;
	int k = 100;

	//Grid Grid[x][y]
	char Grid[GRID][GRID];
	char isInfect[GRID][GRID];

	//assign blanc to Grid[][]
	//y-Grid
	for (int i = 0; i < GRID; i++) {
		//x-Grid
		for (int j = 0; j < GRID; j++) {
			Grid[j][i] = ' ';
			isInfect[j][i] = 0;
		}
	}

	//assign the bunnyPos x,y to Grid[x][y]
	for (p = anchor; p != NULL; p = p->next) {
		//decide, if X,m,M,...
		if (p->radioactive_mutant_vampire_bunny == 1) {
			Grid[p->coord.x][p->coord.y] = 'X';
			isInfect[p->coord.x][p->coord.y] = 1;
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
			printf(COLOR_GRAY "|" COLOR_RESET);
			if (isInfect[j][i] == 1) printf(COLOR_RED "%c" COLOR_RESET, Grid[j][i]);
			else printf("%c", Grid[j][i]);
		}
		//newline at end of row
		printf(COLOR_GRAY "|\n" COLOR_RESET);
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

	#ifdef _WIN32
	Sleep(500);
	#endif
	#ifdef linux
	sleep(1);
	#endif

	printf("\n");
	}
	*/
	printf("There're now %d/%d bunnies healthy\n", *bunnyCount - *infects, *bunnyCount);
	printf("--------------------------------------------------\n");
}//end displayInfo