/*
// incrbunnies.c
//
// The Bunny.exe is a simulation, which simulates a virtual Bunny-colony
// Several different arguments can be feeded, to customize the experience
// As userinput at runtime, is the massmurder, acceleration and deceleration available (Windows only)

Copyright (C) 2018 Christian Mayer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

contact @https://github.com/mayerch1
//
*/

#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "bunny.h"

void reproduce(bunny **anchor, int *bunnyCount, Point food[]) {
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
			bunny_append(*anchor, createBunny(*anchor, rand() % 2, p->color, 0, 0, -1, bunnyCount, p->coord, food));
		}
	}
}//end reproduce

void feedBunnies(bunny **anchor, Point food[], int foodCount, int foodDur[], int *bunnyCount, int food_duration) {
	bunny *p;
	//go throug all bunnies
	for (p = *anchor; p != NULL; p = (bunny*)p->next) {
		//go throug all food sources
		for (int i = 0; i < foodCount; i++) {
			//if bunny in range, feed him
			if (abs(food[i].x - p->coord.x) <= FEED_RANGE && abs(food[i].y - p->coord.y) <= FEED_RANGE) {
				p->daySinceFeeded = 0;

				emptyFood(anchor, food, foodDur, foodCount, i, bunnyCount, food_duration);
			}
		}
	}
}//end feedBunnies

void emptyFood(bunny **anchor, Point food[], int foodDur[], int foodCount, int foodPos, int *bunnyCount, int food_duration) {
	if (--(foodDur[foodPos]) <= 0) {
		//create random new food source, in radius 10
		int x, y;
		do {
			x = food[foodPos].x + rand() % 30 - 15;
			y = food[foodPos].y + rand() % 30 - 15;
		} while (!inBounds(x, y));

		food[foodPos].x = x;
		food[foodPos].y = y;
		foodDur[foodPos] = food_duration;

		bunny *p;
		if ((p = matchToGrid(anchor, food[foodPos])) != NULL) {
			killBunny(anchor, p, bunnyCount, "ate poisonous buds");
		}
	}
}//end emptyFood