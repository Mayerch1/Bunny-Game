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

#define COLOR_D_GRAY	"\x1b[90m"
#define COLOR_RED		"\x1b[91m"

#define COLOR_BROWN		"\x1b[38;5;130m"
#define COLOR_WHITE		"\x1b[97"
#define COLOR_GRAY		"\x1b[38;5;244m"
#define COLOR_CREME		"\x1b[38;5;172m"

#define COLOR_GREEN		"\x1b[92m"
#define COLOR_YELLOW	"\x1b[93m"
#define COLOR_BLUE		"\x1b[94m"
#define COLOR_MAGENTA	"\x1b[95m"
#define COLOR_CYAN		"\x1b[96m"

#define COLOR_RESET		"\x1b[0m"

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
	char Grid[GRIDX][GRIDY];
	char isInfect[GRIDX][GRIDY];
	char furCol[GRIDX][GRIDY];

	//assign blanc to Grid[][]
	//y-Grid
	for (int i = 0; i < GRIDY; i++) {
		//x-Grid
		for (int j = 0; j < GRIDX; j++) {
			Grid[j][i] = ' ';
			isInfect[j][i] = 0;
			//-1, no valid color
			furCol[j][i] = -1;
		}
	}

	//assign the bunnyPos x,y to Grid[x][y], and color
	for (p = anchor; p != NULL; p = p->next) {
		//decide, if X,m,M,...
		//get color of Bunny
		//infect
		if (p->radioactive_mutant_vampire_bunny == 1) {
			Grid[p->coord.x][p->coord.y] = 'X';
			isInfect[p->coord.x][p->coord.y] = 1;
		}
		//male
		else if (p->sex == male) {
			//adult
			if (p->age >= 2) {
				Grid[p->coord.x][p->coord.y] = 'M';
				furCol[p->coord.x][p->coord.y] = p->color;
			}
			else {
				Grid[p->coord.x][p->coord.y] = 'm';
				furCol[p->coord.x][p->coord.y] = p->color;
			}
		}
		//female
		else {
			//adult
			if (p->age >= 2) {
				Grid[p->coord.x][p->coord.y] = 'F';
				furCol[p->coord.x][p->coord.y] = p->color;
			}
			else {
				Grid[p->coord.x][p->coord.y] = 'f';
				furCol[p->coord.x][p->coord.y] = p->color;
			}
		}
	}

	//y-Grid
	for (int i = 0; i < GRIDY; i++) {
		//x-Grid
		for (int j = 0; j < GRIDX; j++) {
			printf(COLOR_D_GRAY "|" COLOR_RESET);
			if (isInfect[j][i] == 1)	printf(COLOR_RED "%c" COLOR_RESET, Grid[j][i]);

			else if (furCol[j][i] == 1) printf(COLOR_BROWN "%c" COLOR_RESET, Grid[j][i]);
			else if (furCol[j][i] == 2) printf(COLOR_GRAY "%c" COLOR_RESET, Grid[j][i]);
			else if (furCol[j][i] == 3) printf(COLOR_CREME "%c" COLOR_RESET, Grid[j][i]);
			else printf("%c", Grid[j][i]);
		}
		//newline at end of row
		printf(COLOR_D_GRAY "|\n" COLOR_RESET);
	}
}

void displayInfo(bunny *anchor, int *bunnyCount, int *infects, int cycles) {
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
	printf("There're now %d bunnies alive. Cycle Nr: %d\n", *bunnyCount, cycles);
	printf("--------------------------------------------------\n");
}//end displayInfo