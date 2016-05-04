#ifndef MONSTERFACTORY_H
#define MONSTERFACTORY_H

#include <fstream>
#include <iostream>
#include <vector>

#include "dice.h"
#include "npc.h"

using namespace std;

class MonsterFactory
{
	private:
		bool dicesCreated;

	public:
		MonsterFactory();
		~MonsterFactory();
		
		string name;
		string desc;
		string symb;
		string color;
		string speed;
		string abil;
		string hp;
		string dam;

		Dice *dspeed;
		Dice *dhp;
		Dice *ddam;

		bool good; // true if all fields are present and valid

		bool allFieldsFilled();

		static int deleteFactories();

		NPC *generateNPC();

		/* generate a random monster from factories */
		static NPC *generateRandNPC();

		int initDices();

		static MonsterFactory *next(istream &is);
				
		static int parseAbil(string &);

		static int load(const char *path);

		static vector<MonsterFactory *> factories;
};

ostream &operator<<(ostream &os, MonsterFactory &);

#endif
