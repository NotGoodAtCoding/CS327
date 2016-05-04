#include <stdlib.h>

#include <iostream>
#include <sstream>

#include "debug.h"
#include "objectfactory.h"
#include "util.h"

using namespace std;

vector<ObjectFactory *> ObjectFactory::factories;

char ObjectFactory::SYMB[] =
{
	// 0 ~ 4
	'|',
	')',
	'}',
	'[',
	']',
	// 5 ~ 9
	'(',
	'{',
	'\\',
	'=',
	'"',
	// 10 ~ 15
	'_',
	'~',
	'?',
	'!',
	'$',
	// 16 ~ 20
	'/',
	',',
	'-',
	'%',
	'&'
};

string ObjectFactory::TYPE[] =
{
	// 0 ~ 4
	"WEAPON",
	"OFFHAND",
	"RANGED",
	"ARMOR",
	"HELMET",
	// 5 ~ 9
	"CLOAK",
	"GLOVES",
	"BOOTS",
	"RING",
	"AMULET",
	// 10 ~ 15
	"LIGHT",
	"SCROLL",
	"BOOK",
	"FLASK",
	"GOLD",
	// 16 ~ 20
	"AMMUNITION",
	"FOOD",
	"WAND",
	"CONTAINER",
	"STACK"
};

ObjectFactory::ObjectFactory()
{
	dicesCreated = false;
}

ObjectFactory::~ObjectFactory()
{
	if (dicesCreated)
	{
		delete dhit;
		delete ddam;
		delete ddodge;
		delete ddef;
		delete dweight;
		delete dspeed;
		delete dattr;
		delete dval;
	}
}

int ObjectFactory::deleteFactories()
{
	for (int i = 0; i < (int)factories.size(); i++)
	{
		delete factories[i];
	}
	factories.clear();

	return 0;
}

Item *ObjectFactory::generateItem()
{
	Item *item = new Item(name, desc,
	parseType(type), Util::parseColor(color),
	dhit->roll(),
	ddam,
	ddodge->roll(), ddef ->roll(), dweight->roll(),
	dspeed->roll(), dattr->roll(), dval   ->roll());

	return item;
}

Item *ObjectFactory::generateRandItem()
{
	int index = rand() % (int)factories.size();

	return factories[index]->generateItem();
}

int ObjectFactory::initDices()
{
	dhit    = Dice::parseDice(hit   );
	ddam    = Dice::parseDice(dam   );
	ddodge  = Dice::parseDice(dodge );
	ddef    = Dice::parseDice(def   );
	dweight = Dice::parseDice(weight);
	dspeed  = Dice::parseDice(speed );
	dattr   = Dice::parseDice(attr  );
	dval    = Dice::parseDice(val   );

	dicesCreated = true;

	return 0;
}

int ObjectFactory::load(const char *path)
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
	if (line=="RLG327 OBJECT DESCRIPTION 1")
	{
		while (!ifs.eof())
		{
			ObjectFactory *of = next(ifs);

			if (of && of->good)
			{
				of->initDices();
				factories.push_back(of);
			}
			else
				delete of;
		}
		ifs.close();
		return 0;
	}
	
	ifs.close();
	return -1;
}

int ObjectFactory::parseType(string &s)
{
	for (int i = 0; i < 20; i++)
		if (s==TYPE[i])
			return i;
	return -1;
}

ObjectFactory *ObjectFactory::next(istream &is)
{
	ObjectFactory *of;
	
	while(!is.eof())
	{
		of = new ObjectFactory;
		of -> good = false;

		bool hasParsingError = false;

		string line;
		while (!is.eof())
		{
			getline(is, line);
			string k("BEGIN OBJECT");
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
				if (of->name.length())
				{
					Debug::log("duplicate NAME");
					hasParsingError = true;
				}
				Util::trim(ss);
				getline(ss, of->name);
			}
			else if (k=="DESC")
			{
				if (of->desc.length())
				{
					Debug::log("duplicate DESC");
					hasParsingError = true;
				}

				while(!is.eof())
				{
					getline(is, line);
					if (line.c_str()[0]=='.') break;
					of->desc += line;
					if (is.peek()!='.')
						of->desc +='\n';
				}
			}
			else if (k=="TYPE")
			{
				if (of->type.length())
				{
					Debug::log("duplicate TYPE");
					hasParsingError = true;
				}

				Util::trim(ss);
				getline(ss, of->type);

				if (-1==parseType(of->type))
				{
					Debug::log("error parsing TYPE %s",
					of->type.c_str());
					hasParsingError = true;
				}
			}
			else if (k=="COLOR")
			{
				if (of->color.length())
				{
					Debug::log("duplicate COLOR");
					hasParsingError = true;
				}

				Util::trim(ss);
				getline(ss, of->color);

				if (-1==Util::parseColor(of->color))
				{
					Debug::log("error parsing COLOR %s",
					of->color.c_str());
					hasParsingError = true;
				}
			}
			else if (k=="HIT")
			{	
				hasParsingError = Util::parseDiceField(ss, 
				of->hit, "HIT");
			}
			else if (k=="DAM")
			{	
				hasParsingError = Util::parseDiceField(ss, 
				of->dam, "DAM");
			}
			else if (k=="DODGE")
			{	
				hasParsingError = Util::parseDiceField(ss, 
				of->dodge, "DODGE");
			}
			else if (k=="DEF")
			{	
				hasParsingError = Util::parseDiceField(ss, 
				of->def, "DEF");
			}
			else if (k=="WEIGHT")
			{	
				hasParsingError = Util::parseDiceField(ss, 
				of->weight, "WEIGHT");
			}
			else if (k=="SPEED")
			{	
				hasParsingError = Util::parseDiceField(ss, 
				of->speed, "SPEED");
			}
			else if (k=="ATTR")
			{	
				hasParsingError = Util::parseDiceField(ss, 
				of->attr, "ATTR");
			}
			else if (k=="VAL")
			{	
				hasParsingError = Util::parseDiceField(ss, 
				of->val, "VAL");
			}
		}

		if (hasParsingError)
			continue;

		if (of->allFieldsFilled())
		{
			of->good = true;
			break;
		}
	}
	if (of->good)
		return of;
	else
	{
		delete of;
		return NULL;
	}
}

bool ObjectFactory::allFieldsFilled()
{
	return !(name.empty() || desc.empty() || type.empty() ||
	color.empty() || hit.empty() || dam.empty() ||
	dodge.empty() || def.empty() || weight.empty() ||
	speed.empty() || attr.empty() || val.empty());
}

ostream& operator<<(ostream& os, ObjectFactory &of)
{
	os << of.name   << endl;
	os << of.desc   << endl;
	os << of.type   << endl;
	os << of.color  << endl;
	os << of.hit    << endl;
	os << of.dam    << endl;
	os << of.dodge  << endl;
	os << of.def    << endl;
	os << of.weight << endl;
	os << of.speed  << endl;
	os << of.attr   << endl;
	os << of.val    << endl;
	return os;
}

