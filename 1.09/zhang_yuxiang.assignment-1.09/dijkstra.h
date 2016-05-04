#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "core.h"

extern int distN[DUNGEON_H][DUNGEON_W]; // non-tunneling
extern int distT[DUNGEON_H][DUNGEON_W]; // tunneling

class Dijkstra
{
	private:
		static int visited[DUNGEON_H][DUNGEON_W];
	
	public:
		static int next(int*, int*, int, int, int tunneling);
		static int print(int tunneling);

		/* run Dijkstra starting at (x, y) */
		static int run(int x, int y, int tunneling);
};

#endif

