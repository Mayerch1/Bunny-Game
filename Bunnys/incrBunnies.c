#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "bunny.h"

void reproduce(bunny **anchor, int *bunnyCount, int *infects) {
	int maleCount = 0;
	int femaleCount = 0;
	bunny *p;

	for (p = *anchor; p != NULL; p = p->next) {
		//count males capable of reproduction
		if (p->sex == male && p->age >= 2 && p->radioactive_mutant_vampire_bunny == 0) {
			maleCount++;
			break;
		}
	}
	if (maleCount == 0) return;

	//create new bunny for each female
	for (p = *anchor; p != NULL; p = p->next) {
		if (p->sex == female && p->age >= 2 && p->radioactive_mutant_vampire_bunny == 0) {
			bunny_append(*anchor, createBunny(*anchor, p->color, bunnyCount, infects, p->coord));
		}
	}
}