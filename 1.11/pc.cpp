#include <ncurses.h>
#include <stdlib.h>

#include "core.h"
#include "debug.h"
#include "dungeon.h"
#include "pc.h"

PC *PC::pcInstance = NULL;

PC::PC() : Character()
{
	_isPC = true;

	speed = 10;
	symb = '@';

	seenDungeon = (char *)malloc(DUNGEON_W * DUNGEON_H);

	equipment = new Equipment;
	inventory = new Inventory;

	clearSeenDungeon();

	color = COLOR_PC;

	hp = _hpmax = 1000;
	mp = _mpmax = 1000;
	
	dam = new Dice(5, 5, 5);
	
	// global pointer
	PC::pcInstance = this;
	
	attacking = NULL; 
}

PC::~PC()
{
	PC::pcInstance = NULL;

	free(seenDungeon);

	delete equipment;
	delete inventory;

	delete dam;
}

void PC::clearSeenDungeon()
{
	for (int i = 0; i < DUNGEON_W * DUNGEON_H; i++)
		seenDungeon[i] = ' ';
}

void PC::dropItem(int i)
{
	if (i<0 || i>=(int)inventory->slots.size())
		return;
	Item *item = inventory->slots[i];
	dungeon->placeItem(item, x, y);
	inventory->remove(i);
}

void PC::dropItem(Item *item)
{
	dungeon->placeItem(item, x, y);
}

void PC::pickUpItem()
{
	if (inventory->isFull() || !dungeon->imap[y][x])
		return;
		
	Item *item;
	
	while (!inventory->isFull() && (item = dungeon->imap[y][x]))
	{
		dungeon->eraseItem(x, y);

		inventory->add(item);
		item->next=NULL; // unstack item
	}
}

void PC::setLocation(int x, int y)
{
	Character::setLocation(x, y);
	
	pcx = x;
	pcy = y;
		
	for (int r=0; r<DUNGEON_H; r++)
	{
		for (int c=0; c<DUNGEON_W; c++)
		{
			if (dungeon->isVisible(c, r))
				seenDungeon[r*DUNGEON_W+c]=dungeon->tmap[r][c];
		}
	}
}

void PC::takeOffItem(int index) // index:= equipment index
{
	if (equipment->slots[index]==NULL)
		return;
	
	if (!inventory->add(equipment->slots[index]))
		dropItem(equipment->slots[index]);

	equipment->slots[index]=NULL;
}

void PC::wearItem(int index) // index:= inventory index
{
	Item *item = inventory->slots[index];
	
	if (equipment->isEmpty(item->type))
	{
		equipment->add(item);
		inventory->remove(index);
	}
	else
	{
		int equipIndex = equipment->getSlotIndex(item->type);
		
		// swap
		Item *tmp = inventory->slots[index];
		inventory->slots[index] = equipment->slots[equipIndex];
		equipment->slots[equipIndex] = tmp;
	}
}

int PC::getSpeed()
{
	int total = Character::getSpeed();
	
	for (int i=0; i<12; i++)
	{
		Item *item = equipment->slots[i];
		if (item)
			total += item->speed;
	}

	if (total<=0) total = 1;

	return total;
}

int PC::getTotalDam()
{
	int total = dam->roll();
	
	for (int i=0; i<12; i++)
	{
		Item *item = equipment->slots[i];
		if (item)
			total += item->dam->roll();
	}

	return total;
}

int PC::getTotalDef()
{
	int total = 0;

	for (int i=0; i<12; i++)
	{
		Item *item = equipment->slots[i];
		if (item)
			total += item->def;
	}

	return total;
}

int PC::hpmax()
{
	int total = _hpmax;

	for (int i=0; i<12; i++)
	{
		Item *item = equipment->slots[i];
		if (item)
			total += item->hit;
	}

	return total;
}

/* singleton pc */
PC *pc;
