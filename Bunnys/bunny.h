#pragma once
#define NAME_LEN 21

typedef struct {
	int x, y;
}Point;

typedef struct {
	enum { male, female } sex;
	enum { white, brown, gray, creme } color;
	char age;
	char Name[NAME_LEN];
	char radioactive_mutant_vampire_bunny;
	int daySinceFeeded;
	Point coord;
	struct bunny *next;
}bunny;
