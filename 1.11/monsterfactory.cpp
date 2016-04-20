#include <stdlib.h>

#include <iostream>
#include <sstream>

#include "debug.h"
#include "monsterfactory.h"
#include "util.h"

using namespace std;

vector<MonsterFactory *> MonsterFactory::factories;

MonsterFactory::MonsterFactory()
{
	dicesCreated = false;
}

MonsterFactory::~MonsterFactory()
{
	if (dicesCreated)
	{
		delete dspeed;
		delete dhp;
		delete ddam;
	}
}

int MonsterFactory::deleteFactories()
{
	for (int i = 0; i < (int)factories.size(); i++)
	{
		delete factories[i];
	}
	factories.clear();

	return 0;
}

NPC *MonsterFactory::generateNPC()
{
	stringstream ss(abil);
	int abilval = 0;
	while (!ss.eof())
	{
		string a;
		ss >> a;
		abilval |= parseAbil(a);
	}

	NPC *npc = new NPC(name, desc,
	Util::parseSymb(symb),
	Util::parseColor(color), 
	dspeed->roll(), 
	abilval,
	dhp->roll(),
	ddam);

	return npc;
}

NPC *MonsterFactory::generateRandNPC()
{
	int index = rand() % factories.size();

	return factories[index]->generateNPC();
}

int MonsterFactory::initDices()
{
	if (!good)
		return -1;

	dspeed = Dice::parseDice(speed);
	dhp    = Dice::parseDice(hp);
	ddam   = Dice::parseDice(dam);

	dicesCreated = true;

	return 0;
}

int MonsterFactory::parseAbil(string &s)
{
	if (s=="SMART"  ) return 1<<0;
	if (s=="TELE"   ) return 1<<1;
	if (s=="TUNNEL" ) return 1<<2;
	if (s=="ERRATIC") return 1<<3;
	if (s=="PASS"   ) return 1<<4;
	return -1;
}

int MonsterFactory::load(const char *path)
{
	ifstream ifs(path);

	if (!ifs)
	{
		cerr << "Failed to open " << path << endl;
		return -1;
	}
	
	if (ifs.eof())
		return -1;

	// meta
	string line;
	getline(ifs, line);
	if (line=="RLG327 MONSTER DESCRIPTION 1")
	{
		while (!ifs.eof())
		{
			MonsterFactory *mf = next(ifs);

			if (mf && mf->good)
			{
				mf->initDices();
				factories.push_back(mf);
			}
			else
				delete mf;
		}
		ifs.close();
		return 0;
	}
	
	ifs.close();
	return -1;
}

MonsterFactory *MonsterFactory::next(istream &is)
{
	MonsterFactory *mf;
	
	while(!is.eof())
	{
		mf = new MonsterFactory;
		mf-> good = false;

		bool hasParsingError = false;

		string line;
		while (!is.eof())
		{
			getline(is, line);
			string k("BEGIN MONSTER");
			if (!line.compare(0, k.length(), k))
				break;
		}
		while (!is.eof() && !hasParsingError)
		{
			getline(is, line);
			string k;
			stringstream ss(line);
			ss >> k;
			if (k=="END")
			{
				break;
			}
			else if (k=="NAME")
			{
				if (mf->name.length())
				{
					Debug::log("duplicate NAME");
					hasParsingError = true;
				}
				Util::trim(ss);
				getline(ss, mf->name);
			}
			else if (k=="DESC")
			{
				if (mf->desc.length())
				{
					Debug::log("duplicate DESC");
					hasParsingError = true;
				}

				while(!is.eof())
				{
					getline(is, line);
					if (line.c_str()[0]=='.') break;
					mf->desc += line;
					if (is.peek()!='.')
						mf->desc +='\n';
				}
			}
			else if (k=="SYMB")
			{
				if (mf->symb.length())
				{
					Debug::log("duplicate SYMB");
					hasParsingError = true;
				}

				Util::trim(ss);
				getline(ss, mf->symb);

				if (-1==Util::parseSymb(mf->symb))
				{
					Debug::log("error parsing SYMB %s",
					mf->symb.c_str());
					hasParsingError = true;
				}
			}
			else if (k=="COLOR")
			{
				if (mf->color.length())
				{
					Debug::log("duplicate COLOR");
					hasParsingError = true;
				}

				Util::trim(ss);
				getline(ss, mf->color);

				if (-1==Util::parseColor(mf->color))
				{
					Debug::log("error parsing COLOR %s",
					mf->color.c_str());
					hasParsingError = true;
				}
			}
			else if (k=="SPEED")
			{
				hasParsingError = Util::parseDiceField(ss,
				mf->speed, "SPEED");
			}
			else if (k=="ABIL")
			{
				if (mf->abil.length())
				{
					Debug::log("duplicate ABIL");
					hasParsingError = true;
				}

				Util::trim(ss);
				getline(ss, mf->abil);

				stringstream ss(mf->abil);
				string t;
				while (!ss.eof())
				{
					ss >> t;
					if (-1==parseAbil(t))
					{
						Debug::log("error parsing ABIL %s",
						mf->abil.c_str());
						hasParsingError = true;
					}
				}
			}
			else if (k=="HP")
			{
				hasParsingError = Util::parseDiceField(ss,
				mf->hp, "HP");
			}
			else if (k=="DAM")
			{
				hasParsingError = Util::parseDiceField(ss,
				mf->dam, "DAM");
			}
		}

		if (hasParsingError)
			continue;

		if (mf->allFieldsFilled())
		{
			mf->good = true;
			break;
		}
	}
	if (mf->good)
		return mf;
	else
	{
		delete mf;
		return NULL;
	}
}

bool MonsterFactory::allFieldsFilled()
{
	return name.length() && desc.length() && symb.length() &&
	color.length() &&
	speed.length() &&
	abil.length() && hp.length() && dam.length();
}

ostream& operator<<(ostream& os, MonsterFactory &mf)
{
	os << mf.name  << endl;
	os << mf.desc  << endl;
	os << mf.symb  << endl;
	os << mf.color << endl;
	os << mf.speed << endl;
	os << mf.abil  << endl;
	os << mf.hp    << endl;
	os << mf.dam   << endl;
	return os;
}

