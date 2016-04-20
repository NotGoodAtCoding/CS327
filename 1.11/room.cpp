#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "dungeon.h"
#include "util.h"
#include "room.h"

Room::Room()
{
	w = Util::getRandNum(ROOM_MIN_W, ROOM_MAX_W);
	h = Util::getRandNum(ROOM_MIN_H, ROOM_MAX_H);
	x = Util::getRandNum(1, DUNGEON_W - w - 1);
	y = Util::getRandNum(1, DUNGEON_H - h - 1);
}

Room::~Room()
{
}

bool Room::contains(int x, int y)
{
	return this->x <= x && this->x + this->w > x &&
	       this->y <= y && this->y + this->h > y;
}

int Room::getCentroid(int *x, int *y)
{
	*x = this->x + this->w/2;
	*y = this->y + this->h/2;

	return 0;
}

bool Room::collide(Room* r, Room* s)
{
	if(r->x+r->w<s->x || r->x>s->x+s->w)
		return false;
	if(r->y+r->h<s->y || r->y>s->y+s->h)
		return false;

	return true;
}

bool Room::isInSameRoom(Dungeon *dungeon, int x1, int y1, int x2, int y2)
{
	for (int i=0; i<(int)dungeon->roomv.size(); i++)
	{
		if (dungeon->roomv[i]->contains(x1, y1) &&
			dungeon->roomv[i]->contains(x2, y2))
			return true;
	}
	return false;
}

int Room::paintOn(Dungeon *dungeon)
{
	int r, c;
	for(r=y;r<y+h;r++)
	{
		for(c=x;c<x+w;c++)
		{
			dungeon->tmap[r][c] = ROOM;
			dungeon->hmap[r][c] = 0;
		}
	}
	
	return 0;
}

int Room::getRandLocation(int *x, int *y)
{
	*x = this->x + rand() % this->w;
	*y = this->y + rand() % this->h;

	return 0;
}

