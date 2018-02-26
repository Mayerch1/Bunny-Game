/*
// bunny.h
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

#pragma once
#define NAME_LEN 108

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
