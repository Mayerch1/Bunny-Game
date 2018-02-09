#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "bunny.h"

//inc age
void ageBunnies(bunny **anchor, int *bunnyCount, int *infects) {
	bunny *p;

	for (p = *anchor; p != NULL; p = p->next) {
		//kill normal bunnies
		if ((p->age++) >= 10 && p->radioactive_mutant_vampire_bunny == 0) {
			p = killBunny(anchor, p, bunnyCount, infects);
		}
		//kill radiation_vampire bunnies
		else if (p->age >= 50) {
			p = killBunny(anchor, p, bunnyCount, infects);
		}
	}
}

//kill designatet target
bunny *killBunny(bunny **anchor, bunny *victim, int *bunnyCount, int *infects) {
	eolMsg(victim);

	//if anchor dies
	if (victim == *anchor) {
		*anchor = victim->next;
		free(victim);
		(*bunnyCount)--;
		return *anchor;
	}
	else {
		bunny *transferP, *searchP, *lastP;

		//find bunny infront of victim
		lastP = anchor;
		for (searchP = *anchor; searchP != victim; searchP = searchP->next) {
			lastP = searchP;
		}

		//append to bunny before vicitm
		lastP->next = victim->next;
		if (victim->radioactive_mutant_vampire_bunny == 1) (*infects)--;
		//delete victim
		free(victim);
		(*bunnyCount)--;
		return lastP;
	}
}

//starve 1/2 of all bunnies
void starveBunnies(bunny **anchor, int *bunnyCount, int *infects) {
	int start = *bunnyCount;

	//repeat until 1/2 are dead
	while (*bunnyCount > (start / 2)) {
		//choose one bunny
		bunny *victim;
		int vicNr = rand() & *bunnyCount;

		//move *victim to victim
		for (victim = *anchor; vicNr > 1; vicNr--) {
			victim = victim->next;
		}
		//kill him
		killBunny(anchor, victim, bunnyCount, infects);
	}

	starveMsg(start, bunnyCount);
}//end starveBunnies

 //mutants infecting healthy bunnies
void infectBunnies(bunny **anchor, int *bunnyCount, int *infects) {
	bunny *p;
	int mutantCount = 0;
	int xOff = 0, yOff = 0;

	for (p = *anchor; p != NULL; p = p->next) {
		if (p->radioactive_mutant_vampire_bunny == 1) {
			//coords of infection victim
			Point coords;
			Point offset;

			//find taken fields
			offset = findField(anchor, 1, p->coord);

			if (offset.x == 0 && offset.y == 0) {
				//go to next mutant
				continue;
			}

			//assign absolute coords of victim
			coords.x = p->coord.x + offset.x;
			coords.y = p->coord.y + offset.y;

			//find *bunny to Gridslot
			bunny *victim;
			victim = matchToGrid(anchor, coords);

			//infect da bunny
			int chance = rand() % 100 + 1;

			//50% infection chance
			if (chance <= INFECTION_PROB) {
				if (victim != NULL && victim->radioactive_mutant_vampire_bunny == 0) {
					victim->radioactive_mutant_vampire_bunny = 1;
					(*infects)++;
					infectMsg(victim);
				}
			}
		}
	}
}//end infectBunnies