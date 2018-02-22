#include "bunny.h"

#ifndef MAINFCT
#define MAINFCT

//#define MAX_HUNGER 3
#define FEED_RANGE 5
#define NAME_LEN 108

void strcpy_safe(char *output, int str_len, const char* input);
//void fprint_name(FILE *fp, char *Name);

void nextTurn(bunny **anchor, int *bunnyCount, int *infects, unsigned int max_colony_size,
	unsigned char infection_prob, Point food[], int max_hunger);

void moveBunny(bunny **anchor, Point food[]);
int testEmpty(bunny **anchor, Point coords, Point food[]);
Point findField(bunny **anchor, int state, Point coords, Point food[]);
bunny *matchToGrid(bunny **anchor, Point coords);

void toLowerCase(int argc, char *argv[]);
void getArgs(int argc, char *argv[], unsigned int *max_colony_size, unsigned char *infection_prob,
	char *log, char *noLog, unsigned int *start_Bunnies, unsigned int *sleep_time, char *save, char *load, char *fileName, int file_len);

#endif

#ifndef CREATEBUNNY
#define CREATEBUNNY

int loadHead(int *gridX, int *gridY, int *foodCount, int *max_hunger, char fileName[]);
void loadFood(int foodCount, Point food[], char fileName[]);
void loadBunnies(int tmpFood, int oldBunnyCount, int *bunnyCount, int *infects, bunny *anchor, Point food[], char fileName[]);

bunny *createBunny(bunny *anchor, int sex, int col, int age, int isMutant, int *bunnyCount, int *infects, Point coords, Point food[]);
void bunny_append(bunny *anchor, bunny *e);
void initBunny(bunny *myBunny, int col, int *infects);
void chooseName(bunny *myBunny);

#endif

#ifndef REDUCEBUNNY
#define REDUCEBUNNY

void ageBunnies(bunny **anchor, int *bunnyCount, int *infects);
bunny *killBunny(bunny **anchor, bunny *victim, int *bunnyCount, int *infects, char *deathMSG);
void famineBunnies(bunny **anchor, int *bunnyCount, int *infects);
void infectBunnies(bunny **anchor, int *bunnyCount, int *infects, unsigned char infection_prob, Point food[]);

void starveBunnies(bunny **anchor, int *bunnyCount, int *infects, int max_hunger);

#endif

#ifndef INCRBUNNY
#define INCRBUNNY

void reproduce(bunny **anchor, int *bunnyCount, int *infects, Point food[]);
void feedBunnies(bunny **anchor, Point food[], int foodCount);

#endif

#ifndef PRINTS
#define PRINTS
void printHelp();

void displayInfo(bunny *anchor, int *bunnyCount, int *infects, int cycles, char log);
void displayGrid(bunny *anchor, Point food[], int foodCount);
int printFoodSource(bunny *anchor, int x, int y, Point food[], int foodCount);

void bornMsg(bunny *born);
void infectMsg(bunny *victim);
void eolMsg(bunny *victim, char *deathMSG);
void starveMsg(int start, int *bunnyCount);

void saveGame(int gridX, int gridY, bunny *anchor, Point food[], int foodCount, int max_hunger, int bunnyCount, char fileName[]);

#endif
