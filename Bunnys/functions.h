#include "bunny.h"

#ifndef MAINFCT
#define MAINFCT

#define GRIDX 80				//
#define GRIDY 45				//how large is your console?	

#define FOOD_COUNT 5
#define MAX_HUNGER 3
#define FEED_RANGE 5


void nextTurn(bunny **anchor, int *bunnyCount, int *infects, unsigned int max_colony_size, 
				unsigned char infection_prob, Point food[]);

void moveBunny(bunny **anchor, Point food[]);
int testEmpty(bunny **anchor, Point coords, Point food[]);
Point findField(bunny **anchor, int state, Point coords, Point food[]);
bunny *matchToGrid(bunny **anchor, Point coords);

#endif



#ifndef CREATEBUNNY
#define CREATEBUNNY

bunny *createBunny(bunny *anchor, int col, int *bunnyCount, int *infects, Point coords, Point food[]);
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

void starveBunnies(bunny **anchor, int *bunnyCount, int *infects);

#endif

#ifndef INCRBUNNY
#define INCRBUNNY

void reproduce(bunny **anchor, int *bunnyCount, int *infects, Point food[]);
void feedBunnies(bunny **anchor, Point food[]);

#endif

#ifndef PRINTS
#define PRINTS
void displayInfo(bunny *anchor, int *bunnyCount, int *infects, int cycles);
void displayGrid(bunny *anchor, Point food[]);
int printFoodSource(bunny *anchor, int x, int y, Point food[]);

void bornMsg(bunny *born);
void infectMsg(bunny *victim);
void eolMsg(bunny *victim, int *deathMSG);
void starveMsg(int start, int *bunnyCount);
#endif

