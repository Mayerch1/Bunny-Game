#include "bunny.h"

#ifndef MAINFCT
#define MAINFCT

#define FEED_RANGE 5
#define NAME_LEN 108

/*copys a string to another, with guaranted null termination*/
void strcpy_safe(char *output, int str_len, const char* input);

/*removes _ from names for output*/
//void fprint_name(FILE *fp, char *Name);

/*all important moves are managed and calles in here*/
void nextTurn(bunny **anchor, int *bunnyCount, int *infects, unsigned int max_colony_size,
	unsigned char infection_prob, Point food[], int max_hunger);

/*moves the bunny to a random free field*/
void moveBunny(bunny **anchor, Point food[]);

/*find a free field and return offset towards it
state==0	return a free field
state==1	returm a taken field*/
Point findField(bunny **anchor, int state, Point coords, Point food[]);

/*check if the requested field is already taken
return 0 if field is free, otherwise 1*/
int testEmpty(bunny **anchor, Point coords, Point food[]);

/*find the bunny to requested coordinates,
returns pointer to bunny or NULL-pointer*/
bunny *matchToGrid(bunny **anchor, Point coords);

/*converts the string to lower case*/
void toLowerCase(int argc, char *argv[]);

/*get all given arguments
considers limits and can handle scrambled orders*/
void getArgs(int argc, char *argv[], unsigned int *max_colony_size, unsigned char *infection_prob,
	char *log, char *noLog, unsigned int *start_Bunnies, unsigned int *sleep_time, char *save, char *load, char *fileName, int file_len);

#endif

#ifndef CREATEBUNNY
#define CREATEBUNNY
/*returns the important arguments of the saved game
returns number of Bunnies, when game was saved*/
int loadHead(int *gridX, int *gridY, int *foodCount, int *max_hunger, char fileName[]);

/*load all food sources*/
void loadFood(int foodCount, Point food[], char fileName[]);

/*load all bunnys, with attributes*/
void loadBunnies(int tmpFood, int oldBunnyCount, int *bunnyCount, int *infects, bunny *anchor, Point food[], char fileName[]);

/*create a Bunny and return pointer to it
sex, age, col must be given, mutant can be set to -1, Name set to NULL for random generation*/
bunny *createBunny(bunny *anchor, int sex, int col, int age, char Name[], int isMutant, int *bunnyCount, int *infects, Point coords, Point food[]);

/*append a pointer on bunny to the linked list, p->next*/
void bunny_append(bunny *anchor, bunny *e);

/*called from createBunny, sets color and mutant state*/
void initBunny(bunny *myBunny, int col, int *infects);

/*called from createBunny, sets Name*/
void chooseName(bunny *myBunny);

#endif

#ifndef REDUCEBUNNY
#define REDUCEBUNNY

/*age all bunnies, if neccessary call killBunny*/
void ageBunnies(bunny **anchor, int *bunnyCount, int *infects);

/*kills the *victim and returns pointer to bunny infront of victim in list */
bunny *killBunny(bunny **anchor, bunny *victim, int *bunnyCount, int *infects, char *deathMSG);

/*starve 1/2 of all bunnies to death*/
void famineBunnies(bunny **anchor, int *bunnyCount, int *infects);

/*mutant bunnies infects random bunny next to them with infection_prob*/
void infectBunnies(bunny **anchor, int *bunnyCount, int *infects, unsigned char infection_prob, Point food[]);

/*incr daySinceFeeded, if neccessary call killBunny*/
void starveBunnies(bunny **anchor, int *bunnyCount, int *infects, int max_hunger);

#endif

#ifndef INCRBUNNY
#define INCRBUNNY

/*check for available males, females, generate new bunny-babies*/
void reproduce(bunny **anchor, int *bunnyCount, int *infects, Point food[]);

/*feed a bunny, if in range of a food source*/
void feedBunnies(bunny **anchor, Point food[], int foodCount);

#endif

#ifndef PRINTS
#define PRINTS

/*prints the help page, if -h is feeded*/
void printHelp();

/*print amount of living bunnies, if --log print all information about any bunny to file*/
void displayInfo(bunny *anchor, int *bunnyCount, int *infects, int cycles, char log);

/*displays the grid with bunnies and food*/
void displayGrid(bunny *anchor, Point food[], int foodCount);

/*if a food source is at the given <x,y> coordinates, return 1*/
int printFoodSource(bunny *anchor, int x, int y, Point food[], int foodCount);

/*prints the bornMsg in file*/
void bornMsg(bunny *born);

/*prints infectMsg*/
void infectMsg(bunny *victim);

/*prints death Msg*/
void eolMsg(bunny *victim, char *deathMSG);

/*prints starve message (and reason)*/
void starveMsg(int start, int *bunnyCount);

/*if --save, print all bunnies with attributes to file for later --load*/
void saveGame(int gridX, int gridY, bunny *anchor, Point food[], int foodCount, int max_hunger, int bunnyCount, char fileName[]);

#endif
