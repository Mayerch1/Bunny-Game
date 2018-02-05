#pragma once

typedef struct {
	int x, y;
}Point;

typedef struct {
	enum { male, female } sex;
	enum { white, brown, black, spotted } color;
	char age;
	char Name[21];
	char radioactive_mutant_vampire_bunny;
	Point coord;
	struct bunny *next;
}bunny;
