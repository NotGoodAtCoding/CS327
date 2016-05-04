#ifndef DUNGEON_HH
#define DUNGEON_HH

#include <vector>

#include "character.h"
#include "item.h"
#include "npc.h"
#include "room.h"
#include "turn.h"

using namespace std;

class Dungeon
{
	private:
		vector<NPC *> deadnpcv;

		int getNPCIndex(NPC *);

	public:
		/* terrain */
		char tmap[DUNGEON_H][DUNGEON_W];
		/* hardness */
		unsigned char hmap[DUNGEON_H][DUNGEON_W];
		/* characters */
		Character *cmap[DUNGEON_H][DUNGEON_W];
		/* items */
		Item *imap[DUNGEON_H][DUNGEON_W];

		vector<NPC *> npcv;

		vector<Item *> itemv;

		vector<Room *> roomv;

		Turn *turn;

		Dungeon();
		~Dungeon();

		int fill(char);
		int generate();
		int getFileSize();

		/* true if location is visible by PC */
		bool isVisible(int x, int y);
		/* true if line of sight is not blocked between two locations */
		bool isVisible(int x, int y, int ox, int oy); 

		inline int nummon() { return  npcv.size(); }
		inline int numobj() { return itemv.size(); }

		int removeMonster(NPC *);

		int printDungeon();
		
		int placeCharacter(Character *);
		int placeCharacter(Character *, int x, int y);
		int placeItem(Item *);
		int placeItem(Item *, int x, int y);

		int eraseItem(int x, int y);

		int load(const char *path);
		int save(const char *path);
		
		// old version - generate 0 ~ f
		int generateRandMonsters(int nummon);

		// new version - generate from factories
		int generateMonsters(int nummon);
		int generateObjects(int numobj);

		int generateObjects(const char *path);
};

/* current active dungeon */
extern Dungeon *dungeon;

/* 1 if display line of sight */
extern int sight;

#endif

