#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "core.h"
#include "character.h"
#include "debug.h"
#include "dungeon.h"
#include "monsterfactory.h"
#include "npc.h"
#include "path.h"
#include "pc.h"
#include "room.h"
#include "util.h"

static void print(int r, int c, char ch)
{
	mvprintw(r+1, c, "%c", ch);
}

Dungeon *dungeon;
int sightmap[DUNGEON_H][DUNGEON_W];

Dungeon::Dungeon()
{
	turn = new Turn;

	// init immutable borders
	int r, c;
	for (r=0;r<DUNGEON_H;r++)
	{
		tmap[r][0] = ROCK;
		tmap[r][DUNGEON_W-1] = ROCK;
			
		hmap[r][0] = IMMUTABLE;
		hmap[r][DUNGEON_W-1] = IMMUTABLE;
	}
	for (c=0;c<DUNGEON_W;c++)
	{
		tmap[0][c] = ROCK;
		tmap[DUNGEON_H-1][c] = ROCK;
		
		hmap[0][c] = IMMUTABLE;
		hmap[DUNGEON_H-1][c] = IMMUTABLE;
	}

	// init cmap and imap
	for (r=0;r<DUNGEON_H;r++)
	{
		for (c=0;c<DUNGEON_W;c++)
		{
			cmap[r][c] = NULL;
			imap[r][c] = NULL;
		}
	}
}

Dungeon::~Dungeon()
{	
	for (int i=0; i<(int)roomv.size(); i++)
		delete roomv[i];
	roomv.clear();

	for (int i=0; i<(int)npcv.size(); i++)
		delete npcv[i];
	npcv.clear();

	for (int i=0; i<(int)deadnpcv.size(); i++)
		delete deadnpcv[i];
	deadnpcv.clear();

	for (int i=0; i<(int)itemv.size(); i++)
		delete itemv[i];
	itemv.clear();
	
	delete turn;
}

int Dungeon::getFileSize()
{
	return strlen(MARKER) + sizeof(int) * 2 + MUTABLE_SIZE +
	roomv.size() * 4;
}

int Dungeon::getNPCIndex(NPC *c)
{
	for (int i=0; i<nummon(); i++)
	{
		if (npcv[i]==c)
			return i;
	}
	return -1;
}

int Dungeon::fill(char ch)
{
	int r, c;
	for(r=1;r<DUNGEON_H-1;r++)
	{
		for(c=1;c<DUNGEON_W-1;c++)
		{
			tmap[r][c] = ch;
			
			if(ch==ROCK)
			{
				hmap[r][c] = Util::getRandNum(1, 255 - 1);
			}
		}
	}
	return 0;
}

int Dungeon::placeCharacter(Character *cp)
{
	while (1)
	{
		int r = rand() % DUNGEON_H;
		int c = rand() % DUNGEON_W;

		if (!hmap[r][c])
		{
			placeCharacter(cp, c, r);

			break;
		}
	}

	if (cp->isPC())
		pc = (PC *)cp;

	return 0;
}

int Dungeon::placeCharacter(Character *c, int x, int y)
{
	cmap[y][x] = c;
	c->setLocation(x, y);
	
	if (!c->isPC())
		npcv.push_back((NPC *)c);
	
	return 0;
}

int Dungeon::placeItem(Item *item)
{
	while (1)
	{
		int r, c;

		// only place item in the room
		roomv[rand() % roomv.size()]->getRandLocation(&c, &r);

		if (!hmap[r][c])
		{
			placeItem(item, c, r);
			break;
		}
	}
	return 0;
}

int Dungeon::placeItem(Item *item, int x, int y)
{
	item->next = imap[y][x];
	imap[y][x] = item;
	
	itemv.push_back(item);
	
	return 0;
}

int Dungeon::eraseItem(int x, int y)
{
	Item *item = imap[y][x];
	imap[y][x] = item->next;

	int index = -1;

	for (int i=0; i<numobj(); i++)
	{
		if (itemv[i]==item)
		{
			index = i;
			break;
		}
	}
	
	if (index < 0) return -1;

	itemv.erase(itemv.begin() + index);

	return 0;
}

int Dungeon::printDungeon()
{
	// build sight map
	if (sight)
	{
		// clear old sight map
		int r, c;
		for(r=0;r<DUNGEON_H;r++)
		{
			for(c=0;c<DUNGEON_W;c++)
			{
				sightmap[r][c] = 0;
			}
		}
		int i;
		for(i = 0; i<nummon(); i++)
		{
			int npcx, npcy;
			npcv[i]->getLocation(&npcx, &npcy);
			isVisible(npcx, npcy, pcx, pcy);
		}
	}

	int r, c;
	for(r=0;r<DUNGEON_H;r++)
	{
		for(c=0;c<DUNGEON_W;c++)
		{
			int visible = isVisible(c, r);

			// character
			if ((nofog || visible) && cmap[r][c])
			{
				int color = cmap[r][c]->getColor();
			
				// print character on the dungeon
				attron(COLOR_PAIR(color));
				print(r, c, cmap[r][c]->getSymb());
				attroff(COLOR_PAIR(color));
			}
			// line of sight (only when --sight is specified)
			else if (sight && sightmap[r][c]) 
			{
				int color;
				if (sightmap[r][c] > 0)
				{	
					init_pair(color = 3, COLOR_GREEN, COLOR_BLACK);
					attron(COLOR_PAIR(color));
					print(r, c, 'o');
				}
				else
				{
					init_pair(color = 30, COLOR_RED, COLOR_BLACK);
					attron(COLOR_PAIR(color));
					print(r, c, 'x');
				}
				attroff(COLOR_PAIR(color));
			}
			// item
			else if (imap[r][c] &&
				// explored
				(nofog || pc->getSeenDungeon()[r*DUNGEON_W + c]!=' '))
			{
				int color = imap[r][c]->color;
				attron(COLOR_PAIR(color));
				print(r, c, imap[r][c]->getSymb());
				attroff(COLOR_PAIR(color));
			}
			// dungeon terrain
			else
			{
				// print dungeon cell on the dungeon
				char *seenDungeon = pc->getSeenDungeon();
				
				char ch = seenDungeon[r*DUNGEON_W + c];
				
				if (visible)
					attron(COLOR_PAIR(COLOR_YELLOW));
				print(r, c, ch);
				attroff(COLOR_PAIR(COLOR_YELLOW));

				if (nofog)
					print(r, c, tmap[r][c]);
			}
		}
	}
	return 0;
}

int Dungeon::generate()
{
	Debug::log("generating new dungeon...\n");

	fill(ROCK); // fill the dungeon with rocks

	for (int t=0; t<tol; t++)
	{
		Room *room = new Room;
		
		roomv.push_back(room);

		// determine if the new room collides with any previous room	
		int hasCollision = 0;
		for (int i=0;i<(int)roomv.size()-1;i++)
		{
			if (Room::collide(room, roomv[i]))
			{
				hasCollision = 1;
				break;
			}
		}

		if (hasCollision)
		{
			delete roomv[roomv.size()-1];
			roomv.erase(roomv.begin()+roomv.size()-1);
		}
		else		
		{
			room->paintOn(this);

			Path::pavePath(this, room, roomv[0]);
		}
	}

	// place stairs
	int x, y;
	roomv[0]->getRandLocation(&x, &y);
	tmap[y][x] = STAIR_UP;
	roomv[1]->getRandLocation(&x, &y);
	tmap[y][x] = STAIR_DOWN;

	return 0;
}

// old version
int Dungeon::generateRandMonsters(int nummon)
{
	Debug::log("generating monsters...\n");

	// place NPC	
	for (int i=0; i<nummon; i++)
	{
		NPC *npc = new NPC;
		
		Debug::log("place NPC %c", npc->getSymb());

		placeCharacter(npc);
		turn->enqueue(npc);
	}

	Debug::log("\nmonsters generated.\n");
	
	return 0;	
}

int Dungeon::generateMonsters(int nummon)
{
	Debug::log("generating monsters...\n");

	// place NPC
	for (int i=0; i<nummon; i++)
	{
		NPC *npc = MonsterFactory::generateRandNPC();
	
		Debug::log("place NPC %c", npc->getSymb());

		placeCharacter(npc);
		turn->enqueue(npc);
	}

	Debug::log("\nmonsters generated.\n");
	
	return 0;
}

int Dungeon::generateObjects(int numobj)
{
	Debug::log("generating objects...\n");

	// place object
	for (int i=0; i<numobj; i++)
	{
		Item *item = ObjectFactory::generateRandItem();

		Debug::log("place item %c", item->getSymb());

		placeItem(item);
	}

	Debug::log("\nobjects generated.\n");

	return 0;
}

bool Dungeon::isVisible(int x, int y)
{
	int pcx, pcy, r = 3;
	pc->getLocation(&pcx, &pcy);

	if (!isVisible(pcx, pcy, x, y)) return 0;

	return (pcx-r<=x && pcx+r>=x && pcy-r<=y && pcy+r>=y) 
		|| Room::isInSameRoom(this, x, y, pcx, pcy);
}

bool Dungeon::isVisible(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	int incx = 0;
	int incy = 0;

	if (dx > 0) incx =  1;
	if (dx < 0) incx = -1;
	if (dy > 0) incy =  1;
	if (dy < 0) incy = -1;

	int curx = x1;
	int cury = y1;

	int blocked = 0;

	while (curx!=x2 || cury!=y2)
	{

		if (hmap[cury][curx])
			blocked = 1;
		
		if (cmap[y1][x1] && cmap[y2][x2])
		{
			if (blocked)
				sightmap[cury][curx] = -1; // -1 means blocked
			else
				sightmap[cury][curx] =  1; //  1 means not blocked
		}

		if (ABS(dx)>ABS(dy))
		{
			curx += incx;
			cury = y1 + (curx - x1) * dy / dx;
		}
		else
		{
			cury += incy;
			curx = x1 + (cury - y1) * dx / dy;
		}
	}
	return !blocked;
}

int Dungeon::removeMonster(NPC *npc)
{
	int index = getNPCIndex(npc);
	npcv.erase(npcv.begin() + index);
	deadnpcv.push_back(npc);
	
	return 0;
}

int Dungeon::load(const char *loadpath)
{
	FILE *ifp = fopen(loadpath, "r");
	if(!ifp)
	{
		fprintf(stderr, "Could not load file: %s not exist\n", loadp);
		return 1;
	}

	char buffer[BUFFER_SIZE];

	// 0-5 RLG327
	fread(buffer, sizeof(*buffer), 6, ifp);
	buffer[6] = '\0';	

	// 6-13
	unsigned int buf32[2];
	fread(buf32, sizeof(*buf32), 2, ifp);

	Debug::log("debug: load 0-5: %s\n", buffer);
	Debug::log("debug: load 6-9: %u\n", Util::reverse(buf32[0])); 
	Debug::log("debug: load 10-13: %u\n", Util::reverse(buf32[1]));
	
	if(strcmp(buffer, "RLG327"))
	{
		fprintf(stderr, "Could not load file: %s: not a RLG327\n", loadp);
		fclose(ifp);
		return 1;
	}

	// 14-1495
	unsigned char buf8[MUTABLE_SIZE];
	fread(buf8, sizeof(*buf8), MUTABLE_SIZE, ifp);
	int r, c;
	for(r=1;r<DUNGEON_H-1;r++)
	{
		for(c=1;c<DUNGEON_W-1;c++)
		{
			int index = (r-1)*78+(c-1);
		
			hmap[r][c] = buf8[index];

			// floor
			if(!buf8[index])
				tmap[r][c] = PATH;
			else
				tmap[r][c] = ROCK;
		}
	}
	
	int i=0;
		
	// 1495-end
	while(fread(buf8, sizeof(*buf8), 4, ifp))
	{
		Debug::log("load room %d: x=%u, y=%u, w=%u, h=%u\n",
		i, buf8[0], buf8[1], buf8[2], buf8[3]);

		Room *room = new Room;

		roomv.push_back(room);

		room->x = buf8[0];
		room->y = buf8[1];
		room->w = buf8[2];
		room->h = buf8[3];

		roomv[i]->paintOn(this);

		i++;
	}

	Debug::log("load room count: %d\n", roomv.size());
	
	fclose(ifp);

	return 0;
}

int Dungeon::save(const char *savepath)
{
	FILE *ofp = fopen(savepath, "w");
	if(!ofp)
	{
		fprintf(stderr, "Could not save file: %s\n", savep);
		return 1;
	}
	
	// 0-5 RLG327
	fwrite("RLG327", 6, 1, ofp);
	
	// 6-13
	unsigned int buf32[2];
	buf32[0] = Util::reverse(0);
	buf32[1] = Util::reverse(getFileSize());
	fwrite(buf32, sizeof(*buf32), 2, ofp);

	unsigned char buf8[MUTABLE_SIZE];
	
	// 14-1495
	int r, c;
	for(r=1;r<DUNGEON_H-1;r++)
	{
		for(c=1;c<DUNGEON_W-1;c++)
		{
			int index = (r-1)*78+(c-1);
			
			buf8[index] = hmap[r][c];
		}
	}
	fwrite(buf8, sizeof(*buf8), MUTABLE_SIZE, ofp);

	// 1495-end
	int i;
	for(i=0;i<(int)roomv.size();i++)
	{
		buf8[0] = roomv[i]->x;
		buf8[1] = roomv[i]->y;
		buf8[2] = roomv[i]->w;
		buf8[3] = roomv[i]->h;	
		
		fwrite(buf8, sizeof(*buf8), 4, ofp);
	}

	fclose(ofp);
		
	return 0;
}
