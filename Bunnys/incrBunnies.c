#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "bunny.h"

void reproduce(bunny **anchor, int *bunnyCount, int *infects, Point food[]) {
	int maleCount = 0;
	int femaleCount = 0;
	bunny *p;

	for (p = *anchor; p != NULL; p = (bunny*)p->next) {
		//count males capable of reproduction
		if (p->sex == male && p->age >= 2 && p->radioactive_mutant_vampire_bunny == 0) {
			maleCount++;
			break;
		}
	}
	if (maleCount == 0) return;

	//create new bunny for each female
	for (p = *anchor; p != NULL; p = (bunny*)p->next) {
		if (p->sex == female && p->age >= 2 && p->radioactive_mutant_vampire_bunny == 0) {
			bunny_append(*anchor, createBunny(*anchor, p->color, bunnyCount, infects, p->coord, food));
		}
	}
}//end reproduce

void feedBunnies(bunny **anchor, Point food[], int foodCount) {
	bunny *p;
	//go throug all bunnies
	for (p = *anchor; p != NULL; p = (bunny*)p->next) {
		//go throug all foos sources
		for (int i = 0; i < foodCount; i++) {
			//if bunny in range, feed him
			if (abs(food[i].x - p->coord.x) <= FEED_RANGE && abs(food[i].y - p->coord.y) <= FEED_RANGE) p->daySinceFeeded = 0;
		}
	}
}//end feedBunnies