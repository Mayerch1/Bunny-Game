#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "bunny.h"

int loadHead(int *gridX, int *gridY, int *foodCount, int *max_hunger, char fileName[]) {
	FILE *loadGame;
	int oldBunnyCount;

	//open filestream, check for !=NULL
	if ((loadGame = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "Coult not read savegame\n");
		return 0;
	}

	//skip notes
	fscanf(loadGame, "%*[^\n]\n");

	//scan args, ignore last 3 %*c
	fscanf(loadGame, "{%d,%d,%d,%d,%d,%*c%*c%*c", gridX, gridY, &oldBunnyCount, foodCount, max_hunger);

	fclose(loadGame);
	return oldBunnyCount;
}//end loadHead

void loadFood(int foodCount, Point food[], char fileName[]) {
	FILE *loadGame;
	//open filestream, check for !=NULL
	if ((loadGame = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "Coult not read savegame\n");
		return;
	}
	//skip args, notes
	fscanf(loadGame, "%*[^\n]\n");
	fscanf(loadGame, "%*[^\n]\n");

	//loop throug all foodCounts
	for (int i = 0; i < foodCount; i++) {
		//scan x and y, ignore last to %*c
		fscanf(loadGame, "{%d,%d,%*c%*c", &food[i].x, &food[i].y);
	}
	fclose(loadGame);
}//end loadFood

void loadBunnies(int tmpFood, int oldBunnycount, int *bunnyCount, int *infects, bunny *anchor, Point food[], char fileName[]) {
	FILE *loadGame;

	//open filestream, check for !=NULL
	if ((loadGame = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "Coult not read savegame\n");
		return;
	}

	//attributes of bunny
	int sex, color;
	int age, isMutant;
	char Name[NAME_LEN];
	int daySinceEaten;
	int x, y;

	//skip args, notes
	fscanf(loadGame, "%*[^\n]\n");
	fscanf(loadGame, "%*[^\n]\n");

	//skip food saves, +1 for additional \n
	for (int i = 0; i < tmpFood; i++) {
		//TODO: if loading failes, add ,NULL
		fscanf(loadGame, "%*[^\n]\n");
	}

	//scanf all bunny attributes, ignore last 2 %*c
	for (int i = 0; i < oldBunnycount; i++) {
		fscanf(loadGame, "{%d %d %d %s %d %d %d %d %*c%*c", &sex, &color, &age, Name, &isMutant, &daySinceEaten, &x, &y);
		Point coords = { x, y };
		Point food[1];
		//create loaded bunny, food[] is not used but need to be passed
		bunny_append(anchor, createBunny(anchor, sex, color, (char)age, Name, isMutant, bunnyCount, infects, coords, food));
	}

	//close
	fclose(loadGame);
}//end loadBunnies

//---------------------------------------------------------------------------------------------------------------------------------------------

bunny *createBunny(bunny *anchor, int sex, int col, int age, char Name[], int isMutant, int *bunnyCount, int *infects, Point coords, Point food[]) {
	bunny *p;
	Point offset = { 0, 0 };
	int xOff = 0, yOff = 0;

	p = (bunny*)malloc(sizeof(bunny));
	if (p == NULL) {
		fprintf(stderr, "No memory available\n");
		return NULL;
	}

	//set age as given in function
	p->age = age;
	p->sex = sex;

	//choose one Grid beneath mother
	if (anchor == NULL) {
		p->coord.x = coords.x;
		p->coord.y = coords.y;
	}
	else {
		offset = findField(&anchor, 0, coords, food); //emtpy fields

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

	//if name is given by function call
	if (Name == NULL)
		chooseName(p);
	else
		strcpy(p->Name, Name);

	//if mutantState is set from function call, -1 will call rand()
	if (isMutant == 1) {
		p->radioactive_mutant_vampire_bunny = 1;
	}
	else if (isMutant == 0) {
		p->radioactive_mutant_vampire_bunny = 0;
	}

	bornMsg(p);

	(*bunnyCount)++;
	return p;
}//end createBunny

void bunny_append(bunny *anchor, bunny *e) {
	bunny *p;

	//if bunny cannot get created
	if (e == NULL) return;

	if (anchor == NULL) {
		anchor = e;
		return;
	}

	for (p = anchor; p->next != NULL; p = (bunny*)p->next);
	(bunny*)p->next = (bunny*)e;
}//end bunny_append

void initBunny(bunny *myBunny, int col, int *infects) {
	int xOff = 0, yOff = 0;

	//get color of mother
	myBunny->color = col;

	//mother's milk is extra nutritious
	myBunny->daySinceFeeded = -1;

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

void chooseName(bunny *myBunny) {
	/*DISCLAIMER: all names are unrelated to any real person, and are not chosen by possible associations with real people or groups*/

	const char mNames[][NAME_LEN] = { "Rudi_Rammler", "Kevin", "The_Hoff", "Pringle", "Fat_Boy", "Simon", "Lord", "Sir_Lancelot", "Can",
									"Sir_Oppenheimer", "Werner_von_Braun", "Heisenberg", "Alexander_der_Grosse", "Lamarck", "Herr_Reck",
									"Graf_von_Zeppelin", "Bugs_Bunny", "Rambo", "Pietro_Lombardi", "Tebartz_van_Elst", "Roooobert_Geiss",
									"El_Chapo", "Brad_Pitt", "Bill_Gates", "Gauland", "Roche_Gonzales", "Keoki",
		"Kar-Theodor_Maria_Nikolaus_Johann_Jacob_Philipp_Franz_Joseph_Sylvester_Buhl-Freiherr_von_und_zu_Guttenberg" };

	const char fNames[][NAME_LEN] = { "Jacqueline", "Chantal", "Crystal", "Kimberly", "Ebony", "Tiffany", "Amber",
									"Britney", "Becky", "Jessica", "Madison", "Katie", "Heather", "Amanda", "Lauren",
									"Caitlyn", "Rachel", "Allison", "Lacey", "Abby", "Claire", "Barbie", "Olivia",
									"Madeline", "Skylerr", "Milli" };

	//disting. betw. male/female
	if (myBunny->sex == male) {
		int random = rand() % (sizeof(mNames) / NAME_LEN);
		strcpy(myBunny->Name, mNames[random]);
		//strcpy(myBunny->Name, "Walter");
	}
	else {
		int random = rand() % (sizeof(fNames) / NAME_LEN);
		strcpy(myBunny->Name, fNames[random]);
	}
}//end chooseName