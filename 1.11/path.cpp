#include <stdio.h>
#include <time.h>

#include "core.h"
#include "dungeon.h"
#include "path.h"
#include "room.h"
#include "util.h"

static int numAdjacentPaths(Dungeon *dungeon, int r, int c)
{
	int num = 0;
	if(dungeon->tmap[r-1][c]==PATH)
		num++;
	if(dungeon->tmap[r+1][c]==PATH)
		num++;
	if(dungeon->tmap[r][c-1]==PATH)
		num++;
	if(dungeon->tmap[r][c+1]==PATH)
		num++;
	return num;
}

void Path::pavePath(Dungeon *dungeon, Room* from, Room* to)
{
	int fromx, fromy, tox, toy;
	
	from->getCentroid(&fromx, &fromy);
	to->getCentroid(&tox, &toy);

	int dx = tox - fromx;
	int dy = toy - fromy;

	int incx = dx > 0 ? 1 : -1;
	int incy = dy > 0 ? 1 : -1;
	
	int r = fromy, c = fromx;

	while (r!=toy && c!=tox)
	{	
		if (dungeon->tmap[r][c] != ROOM)
		{
			dungeon->tmap[r][c] = PATH;
			dungeon->hmap[r][c] = 0;
		}

		// if we connect to existing path, then stop paving
		if (numAdjacentPaths(dungeon, r, c)>1)
			break;
		
		// prefer less hard path
		if (dungeon->hmap[r][c+incx]<dungeon->hmap[r+incy][c])
			c+=incx;
		else
			r+=incy;
	}
	while (r != toy || c != tox)
	{
		if (dungeon->tmap[r][c] != ROOM)
		{
			dungeon->tmap[r][c] = PATH;
			dungeon->hmap[r][c] = 0;
		}

		// if we connect to existing path, then stop paving
		if (numAdjacentPaths(dungeon, r, c)>1)
			break;

		if (r!=toy)
			r+=incy;
		if (c!=tox)
			c+=incx;
	}
}
