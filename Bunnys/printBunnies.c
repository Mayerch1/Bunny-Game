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

#include "functions.h"
#include <stdio.h>
#include <malloc.h>

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

extern FILE *myfile;
extern int gridX, gridY;

//printfs help, if argument was received
void printHelp() {
	printf("\n-------------------------------------\n");
	printf("Bunnys.exe help:\n");
	printf("Following arguments are allowed (not case sensitive):\n");

	printf("\n-x <int>\thorizontal Gridsize \n-y <int>\tvertical Gridsize\n");
	printf("-n <uint>\tmax number of bunnys before famine\n");
	printf("-inf <uint>\tprobability to infect a healthy bunny\n");
	printf("-f <int>\tamount of food sources\n");
	printf("--log\t\toutput more detailed logfile (priorised over --nolog)\n");
	printf("--nolog\t\tdoesnt create log-file\n");

	printf("\nThe order can be scrambled, if an argument is not feeded, default values will be used\n");

	printf("-------------------------------------\n");
}//end printHelp

//all commented printf() is for detailed display
//disabled for better "Grid-experience"
void bornMsg(bunny *born) {
	if (myfile != NULL) {
		fprintf(myfile, "%s was born ", born->Name);
		//printf color of bunny
		if (born->color == 0) {
			fprintf(myfile, "with a white fur ");
		}
		else if (born->color == 1) {
			fprintf(myfile, "with a brown fur ");
		}
		else if (born->color == 2) {
			fprintf(myfile, "with a gray fur ");
		}
		else if (born->color == 3) {
			fprintf(myfile, "with a creme-colored fur ");
		}

		if (born->radioactive_mutant_vampire_bunny == 1) fprintf(myfile, " and it is a RADIOACTIVE-MUTANT-VAMPIRE-BUNNY!");
		fprintf(myfile, "\n");
	}
}

void infectMsg(bunny *victim) {
	if (myfile != NULL) {
		fprintf(myfile, "%s was INFECTED with the virus\n", victim->Name);
	}
}

void eolMsg(bunny *victim, char *deathMSG) {
	if (myfile != NULL) {
		fprintf(myfile, "%s ", victim->Name);
		fprintf(myfile, "%s", deathMSG);
		fprintf(myfile, "\n");
	}
}

void starveMsg(int start, int *bunnyCount) {
	if (myfile != NULL) {
		fprintf(myfile, "\n\nFood shortage killed %d bunnies\n\n", start - *bunnyCount);
	}
}

//show all bunnies in their grid
void displayGrid(bunny *anchor, Point food[], int foodCount) {
	bunny *p;
	int k = 100;

	//char Grid[x][y]
	char **Grid = (char**)alloca(sizeof(char*)*gridX);
	char **isInfect = (char**)alloca(sizeof(char*)*gridX);
	char **furCol = (char**)alloca(sizeof(char*)*gridX);

	for (int i = 0; i < gridX; i++) {
		Grid[i] = (char *)alloca(gridY * sizeof(char));
		isInfect[i] = (char *)alloca(gridY * sizeof(char));
		furCol[i] = (char *)alloca(gridY * sizeof(char));
	}

	//assign blanc to Grid[][]
	//y-Grid
	for (int i = 0; i < gridY; i++) {
		//x-Grid
		for (int j = 0; j < gridX; j++) {
			Grid[j][i] = ' ';
			isInfect[j][i] = 0;
			//-1, no valid color
			furCol[j][i] = -1;
		}
	}

	//assign the bunnyPos x,y to Grid[x][y], and color
	for (p = anchor; p != NULL; p = (bunny*)p->next) {
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
	for (int i = 0; i < gridY; i++) {
		//x-Grid
		for (int j = 0; j < gridX; j++) {
			printf(COLOR_D_GRAY "|" COLOR_RESET);
			//printf food source
			if (printFoodSource(anchor, j, i, food, foodCount) == 1) printf(COLOR_GREEN "O" COLOR_RESET);
			//print any type of bunny
			else {
				if (isInfect[j][i] == 1)	printf(COLOR_RED "%c" COLOR_RESET, Grid[j][i]);

				else if (furCol[j][i] == 1) printf(COLOR_BROWN "%c" COLOR_RESET, Grid[j][i]);
				else if (furCol[j][i] == 2) printf(COLOR_GRAY "%c" COLOR_RESET, Grid[j][i]);
				else if (furCol[j][i] == 3) printf(COLOR_CREME "%c" COLOR_RESET, Grid[j][i]);
				else printf("%c", Grid[j][i]);
			}
		}
		//newline at end of row
		printf(COLOR_D_GRAY "|\n" COLOR_RESET);
	}
}

//determins, if asked x, y is taken by food source
int printFoodSource(bunny *anchor, int x, int y, Point food[], int foodCount) {
	//go throug all food sources
	for (int i = 0; i < foodCount; i++) {
		if (food[i].x == x && food[i].y == y) return 1;
	}
	return 0;
}//end printFoodSource

void displayInfo(bunny *anchor, int *bunnyCount, int *infects, int cycles, char log) {
	if (myfile != NULL) {
		if (log == 1) {
			//for displaying all of the shit
			bunny *p;

			fprintf(myfile, "\n");
			for (p = anchor; p != NULL; p = p->next) {
				fprintf(myfile, "Name: %s \nSex: %d \nColor: %d \nAge: %d \n", p->Name, p->sex, p->color, p->age);
				if (p->radioactive_mutant_vampire_bunny == 1) {
					fprintf(myfile, "He's a radioactive-mutant-vampire-bunny\n");
				}
				fprintf(myfile, "\n");
			}
		}
		//print for texfile
		fprintf(myfile, "\nThere're now %d bunnies alive. Cycle Nr: %d\n", *bunnyCount, cycles);
		fprintf(myfile, "--------------------------------------------------\n");
	}
	//printf for console
	printf("There're now %d bunnies alive. Cycle Nr: %d\n", *bunnyCount, cycles);
	printf("--------------------------------------------------\n");
}//end displayInfo