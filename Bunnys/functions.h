#include "bunny.h"

#ifndef CREATEBUNNY
#define CREATEBUNNY

bunny *createBunny(bunny *anchor, int col, int *bunnyCount, int *infects, Point coords);
void bunny_append(bunny *anchor, bunny *e);
void initBunny(bunny *myBunny, int col, int *infects);
void chooseName(bunny *myBunny);

#endif

#ifndef REDUCEBUNNY
#define REDUCEBUNNY

void ageBunnies(bunny **anchor, int *bunnyCount, int *infects);
bunny *killBunny(bunny **anchor, bunny *victim, int *bunnyCount, int *infects);
void starveBunnies(bunny **anchor, int *bunnyCount, int *infects);
void infectBunnies(bunny **anchor, int *bunnyCount, int *infects);

#endif

#ifndef INCRBUNNY
#define INCRBUNNY

void reproduce(bunny **anchor, int *bunnyCount, int *infects);

#endif

#ifndef PRINTS
#define PRINTS
void displayInfo(bunny *anchor, int *bunnyCount, int *infects);
void displayGrid(bunny *anchor);

void bornMsg(bunny *born);
void infectMsg(bunny *victim);
void eolMsg(bunny *victim);
void starveMsg(int startm, int *bunnyCount);
#endif

#ifndef MAINFCT
#define MAINFCT

#define GRID 60
#define MAX_BUNNIES 1000

void nextTurn(bunny **anchor, int *bunnyCount, int *infects);

void moveBunny(bunny **anchor);
int testEmpty(bunny **anchor, Point coords);
Point findFreeField(bunny **anchor, Point coords);
bunny *matchToGrid(bunny **anchor, Point coords);

#endif