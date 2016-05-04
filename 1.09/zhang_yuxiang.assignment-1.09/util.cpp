#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sstream>

#include "core.h"
#include "dice.h"
#include "debug.h"
#include "monsterfactory.h"
#include "objectfactory.h"
#include "ui.h"
#include "util.h"

char Util::base62(int dist)
{
	if(dist<10)
		return '0'+dist;
	else if(dist>=10 && dist<36)
		return 'a'+(dist-10);
	else if(dist>=36 && dist<62)
		return 'A'+(dist-36);
	
	return -1;
}

int Util::help()
{
	FILE* fp = fopen("README.txt", "r");
	
	if(!fp)
	{
		fprintf(stderr, "README.txt not found\n");
		return -1;
	}
		
	const int buf_size = 80;

	char buf[buf_size];

	// skip lines before user manual
	while(1)
	{
		fgets(buf, buf_size, fp);
		if(*buf=='/' && *(buf+1)=='/')
			break;
	}
	
	// ncurses stuff
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	
	int r = 0, c = 0;
	char buffer[255][80];
	while(fgets(buffer[r++], 80, fp));
	fclose(fp);

	int rmax = r;

	int from = 0;

	while(1)
	{
		clear();

		char tmp[80];
		char *p;

		for (r=0; r<22; r++)
		{
			p = tmp;

			for (c=0; c<80; c++)
			{
				if ('\t'==buffer[from+r][c])
				{
					sprintf(p, "    ");
					p+=4;
				}
				else *p++ = buffer[from+r][c];
			}
			mvprintw(r, 0, "%s", tmp);
		}

		int color;
		init_pair(color = 9, COLOR_GREEN, COLOR_BLACK);

		attron(COLOR_PAIR(color));

		mvprintw(24-1, 0, 
		"Use arrow keys to scroll up and down. Press Q to quit.");

		attroff(COLOR_PAIR(color));

		refresh();

		int ch = getch();
		if (ch=='Q' || ch=='q') break;
		if (ch==KEY_DOWN) 
			if (from+22<rmax) 
				from++;
		if (ch==KEY_UP)   
			if (from>0)
				from--;
	}
		
	endwin();

	return 0;
}

int Util::initColor()
{
	init_pair(COLOR_PC, COLOR_RED, COLOR_YELLOW);
	// HP
	init_pair(COLOR_HP, COLOR_WHITE, COLOR_RED);
	// MP
	init_pair(COLOR_MP, COLOR_WHITE, COLOR_BLUE);
	
	init_pair(COLOR_ZB, COLOR_BLUE, COLOR_CYAN);

	for (int i = 0; i < 8; i++)
		init_pair(i,i,COLOR_BLACK);
	
	return 0;
}

int Util::parse(const char *path)
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
			MonsterFactory *mf = MonsterFactory::next(ifs);

			if (mf && mf->good)
				cerr << *mf << endl;
			delete mf;
		}
		ifs.close();
		return 0;
	}
	if (line=="RLG327 OBJECT DESCRIPTION 1")
	{
		while (!ifs.eof())
		{
			ObjectFactory *of = ObjectFactory::next(ifs);

			if (of && of->good)
				cerr << *of << endl;
			delete of;
		}
		ifs.close();
		return 0;
	}

	ifs.close();
	return -1;
}

int Util::parseColor(string &s)
{
	if (s=="BLACK"  ) return COLOR_WHITE;
	if (s=="RED"    ) return COLOR_RED;
	if (s=="GREEN"  ) return COLOR_GREEN;
	if (s=="YELLOW" ) return COLOR_YELLOW;
	if (s=="BLUE"   ) return COLOR_BLUE;
	if (s=="MAGENTA") return COLOR_MAGENTA;
	if (s=="CYAN"   ) return COLOR_CYAN;
	if (s=="WHITE"  ) return COLOR_WHITE;
	return -1;
}

int Util::parseDiceField(stringstream &ss, string &field, const char *name)
{
	if (field.length())
	{
		Debug::log("duplicate %s", name);
		return 1;
	}

	trim(ss);
	getline(ss, field);

	Dice *dice = Dice::parseDice(field);
	if (NULL==dice)
	{
		Debug::log("error parsing %s %s", name, field.c_str());
		return 1;
	}
	delete dice;
	
	return 0;
}

int Util::parseSymb(string &s)
{
	if (1==s.length()) return s.c_str()[0];
	return -1;
}

unsigned int Util::reverse(unsigned int i)
{
	int r = 0;
	r |= (0x000000ff & (i>>24));
	r |= (0x0000ff00 & (i>>8));
	r |= (0x00ff0000 & (i<<8));
	r |= (0xff000000 & (i<<24));
	
	return r;
}

int Util::getRandNum(int from, int to)
{
	return from + rand() % (to - from + 1);
}

int Util::getWeight(int hardness)
{
	if (hpw) // if hardness per weight is defined
		return hardness / hpw + 1;
	
	if (0 <= hardness && hardness <= 84)
		return 1;
	else if ( 85 <= hardness && hardness <= 170)
		return 2;
	else if (171 <= hardness && hardness <= 254)
		return 3;
	else
		return -1;
}

int Util::hpw = 0; // weight = hardness / hpw + 1

int Util::trim(stringstream &ss)
{
	while(ss.peek()==' ')
		ss.get();
	
	return 0;
}

string &Util::trim(string &s)
{
	stringstream ss(s);
	trim(ss);
	s = ss.str();
	return s;
}

