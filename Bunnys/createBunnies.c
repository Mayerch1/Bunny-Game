#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "bunny.h"

//create Bunny, alloc mem, takes coords of mother-bunny
bunny *createBunny(bunny *anchor, int col, int *bunnyCount, int *infects, Point coords) {
	bunny *p;
	Point offset = { 0, 0 };
	int xOff = 0, yOff = 0;

	p = (bunny*)malloc(sizeof(bunny));
	if (p == NULL) {
		printf("No memory\n");
		return NULL;
	}

	//choose one Grid beneath mother
	if (anchor == NULL) {
		//choose random start grid
		p->coord.x = rand() % GRIDX;
		p->coord.y = rand() % GRIDY;
	}
	else {
		offset = findField(&anchor, 0, coords); //emtpy fields

		//if no free field, delete born bunny
		if (offset.x == 0 && offset.y == 0) {
			free(p);
			return NULL;
		}

		//add offsets to mother's coords
		p->coord.x = (coords.x + offset.x);
		p->coord.y = (coords.y + offset.y);
	}

	p->next = NULL;

	initBunny(p, col, infects);

	bornMsg(p);

	(*bunnyCount)++;
	return p;
}//end createBunny

 //apend new bunny to p->next
void bunny_append(bunny *anchor, bunny *e) {
	bunny *p;

	//if bunny cannot get created
	if (e == NULL) return;

	if (anchor == NULL) {
		anchor = e;
		return;
	}

	for (p = anchor; p->next != NULL; p = p->next);
	p->next = e;
}//end bunny_append

 //set init values of bunny
void initBunny(bunny *myBunny, int col, int *infects) {
	int xOff = 0, yOff = 0;
	myBunny->sex = rand() % 2;

	//get color of mother
	myBunny->color = col;

	myBunny->age = 0;
	chooseName(myBunny);

	//mutant or not
	if (rand() % 100 + 1 <= 1) {
		myBunny->radioactive_mutant_vampire_bunny = 1;
		(*infects)++;
	}
	else {
		myBunny->radioactive_mutant_vampire_bunny = 0;
	}

	//place it random
}//end initBunny

 //choose Name of Bunny out of list
void chooseName(bunny *myBunny) {
	const char mNames[25][21] = { "Kevin", "The Hoff", "Pringle", "Fat Boy", "Simon", "Lord", "Sir Lancelot", "Can",
									"Sir Oppenheimer", "Werner von Braun", "Heisenberg", "Alexander der Grosse", "Lamarck", "Herr Reck",
									"Graf von Zeppelin", "Bugs Bunny", "Rambo", "Pietro Lombardi", "Tebartz van Elst", "Roooobert Geiss",
									"El Chapo", "Brad Pitt", "Bill Gates", "Gauland", "Roche Gonzales" };

	const char fNames[25][21] = { "Jacqueline", "Chantal", "Crystal", "Kimberly", "Ebony", "Tiffany", "Amber",
									"Britney", "Becky", "Jessica", "Madison", "Katie", "Heather", "Amanda", "Lauren",
									"Caitlyn", "Rachel", "Allison", "Lacey", "Abby", "Claire", "Barbie", "Olivia", "Madeline", "Skylerr" };

	int random = rand() % 25;

	//disting. betw. male/female
	if (myBunny->sex == male) {
		strcpy(myBunny->Name, mNames[random]);
		//strcpy(myBunny->Name, "Walter");
	}
	else {
		strcpy(myBunny->Name, fNames[random]);
	}
}//end chooseName