#include <stdio.h>
#include <stdlib.h>

#include "point.h"
#include "dijkstra.h"
#include "dungeon.h"
#include "util.h"
#include "heap.h"

Point *arr[DUNGEON_H * DUNGEON_W];

int Dijkstra::visited[DUNGEON_H][DUNGEON_W];

int distN[DUNGEON_H][DUNGEON_W]; // non-tunneling
int distT[DUNGEON_H][DUNGEON_W]; // tunneling

int arrsize = 0;

// compare two points in non-tunneling distance view
static int comparePointN(const void *pv, const void *qv)
{
	Point *p = (Point *) pv;
	Point *q = (Point *) qv;
	return distN[p->y][p->x] - distN[q->y][q->x];
}

// compare two points in tunneling distance view
static int comparePointT(const void *pv, const void *qv)
{
	Point *p = (Point *) pv;
	Point *q = (Point *) qv;
	return distT[p->y][p->x] - distT[q->y][q->x];
}

int Dijkstra::next(int *nextx, int *nexty, int x, int y, int t)
{
	int closest = 1<<8;

	Point *ns[8];
	point_neighbors(ns, x, y);
	
	if (t)
	{
		int i;
		for(i=0; i<8; i++)
		{
			Point *n = ns[i];
			if (distT[n->y][n->x] < closest)
			{
				x = n->x;
				y = n->y;
				closest = distT[n->y][n->x];
			}
			delete n;
		}
	}
	else
	{
		int i;
		for(i=0; i<8; i++)
		{
			Point *n = ns[i];
			if (distN[n->y][n->x] < closest)
			{
				x = n->x;
				y = n->y;
				closest = distN[n->y][n->x];
			}
			delete n;
		}
	}

	*nextx = x;
	*nexty = y;
	
	return 0;
}

int Dijkstra::print(int t)
{
	int r, c;

	for (r=0; r<DUNGEON_H; r++)
	{
		for (c=0; c<DUNGEON_W; c++)
		{
			char ch = Util::base62((t ? distT[r][c] : distN[r][c]));
			
			if (ch<0)
				printf("%c", dungeon->tmap[r][c]);
			else
				printf("%c", ch);
		}
		printf("\n");
	}
	return 0;
}

int Dijkstra::run(int srcx, int srcy, int t)
{
	Point *src = new Point(srcx, srcy);

	// init
	int r, c;
	for (r=0; r<DUNGEON_H ; r++)
	{
		for (c=0; c<DUNGEON_W; c++)
		{
			if(t)
				distT[r][c] = 1<<10;
			else
				distN[r][c] = 1<<10;
			
			visited[r][c] = 0;
		}
	}
	
	// set src to have distance 0
	if (t)
		distT[src->y][src->x] = 0;
	else
		distN[src->y][src->x] = 0;

	Heap *h = new Heap(t ? comparePointT : comparePointN);
	
	h->insert(src);

	Point *min;
	
	while ((min = (Point *)h->extract()))
	{
		int x = min->x;
		int y = min->y;
		delete min;
		
		visited[y][x] = 1;

		Point *ns[8]; // neighbors
		
		point_neighbors(ns, x, y);
		
		int weight = Util::getWeight(dungeon->hmap[y][x]);
		
		int alt = weight + (t ? distT[y][x] : distN[y][x]);

		int i;
		for(i=0; i<8; i++)
		{
			Point *p = ns[i];
			int px = p->x;
			int py = p->y;
			delete p;

			if (visited[py][px] ||
					py < 1 || py >=DUNGEON_H-1 || 
					px < 1 || px >=DUNGEON_W-1)
			{
				continue;
			}
			int hard = dungeon->hmap[py][px];

			if (!t && hard>0)
			{
				continue;
			}

			int weight = Util::getWeight(hard);
				
			if (weight<0)
			{
				continue;
			}
			
			if (alt < (t ? distT[py][px] : distN[py][px]))
			{
				h->insert(new Point(px, py));
				
				if (t)
					distT[py][px] = alt;
				else
					distN[py][px] = alt;
			}
		}
	}

	delete h;

	return 0;
}
