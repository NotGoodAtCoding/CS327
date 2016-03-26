#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "core.h"

int visited[DUNGEON_H][DUNGEON_W];

int distN[DUNGEON_H][DUNGEON_W]; // non-tunneling
int distT[DUNGEON_H][DUNGEON_W]; // tunneling

int dijkstra_next(int*, int*, int, int, int tunneling);
int dijkstra_print(int tunneling);

/* dijkstra starting at (x, y) */
int dijkstra(int x, int y, int tunneling);

#endif

