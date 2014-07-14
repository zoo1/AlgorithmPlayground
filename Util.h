//Used to define random functions that are helpers spread throughout the program that clutter the code
#ifndef UTIL_H
#define UTIL_H
#include "room.h"
//structure for comparing multiple rooms, used in the minimum spanning tree algorithm
struct connect {
  Room* r1;
  Room* r2;
};
int compareconnect(const void * a, const void * b);
#endif // UTIL_H
