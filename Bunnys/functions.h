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
void infectBunnies(bunny **anchor, int *bunnyCount, int *infects, unsigned char infection_prob);

#endif

#ifndef INCRBUNNY
#define INCRBUNNY

void reproduce(bunny **anchor, int *bunnyCount, int *infects);

#endif

#ifndef PRINTS
#define PRINTS
void displayInfo(bunny *anchor, int *bunnyCount, int *infects, int cycles);
void displayGrid(bunny *anchor);

void bornMsg(bunny *born);
void infectMsg(bunny *victim);
void eolMsg(bunny *victim);
void starveMsg(int startm, int *bunnyCount);
#endif

#ifndef MAINFCT
#define MAINFCT

#define GRIDX 80				//
#define GRIDY 45				//how large is your console?	
;	
void nextTurn(bunny **anchor, int *bunnyCount, int *infects, unsigned int max_colony_size, unsigned char infection_prob);

void moveBunny(bunny **anchor);
int testEmpty(bunny **anchor, Point coords);
Point findField(bunny **anchor, int state, Point coords);
bunny *matchToGrid(bunny **anchor, Point coords);

#endif