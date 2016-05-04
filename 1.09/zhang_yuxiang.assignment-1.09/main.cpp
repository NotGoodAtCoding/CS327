#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/stat.h> 
#include <sys/types.h> 

#include "core.h"
#include "character.h"
#include "debug.h"
#include "dijkstra.h"
#include "dungeon.h"
#include "monsterfactory.h"
#include "move.h"
#include "npc.h"
#include "path.h"
#include "pc.h"
#include "room.h"
#include "turn.h"
#include "ui.h"
#include "util.h"

static int regenerateDungeon();

char monpath[255], objpath[255];

int numobj = 10; // TODO probably should be configurable?

using namespace std;

int main(int argc, char** argv)
{
	char *env = getenv("HOME");

	char dirpath[255];
	sprintf(dirpath, "%s/.rlg327", env);
	mkdir(dirpath, S_IRUSR | S_IWUSR | S_IXUSR);

	char filepath[255]; 
	sprintf(filepath, "%s/.rlg327/dungeon", env);

	sprintf(monpath, "%s/.rlg327/monster_desc.txt", env);
	sprintf(objpath, "%s/.rlg327/object_desc.txt", env);
	
	int i;
	
	/* options */
	if (argc > 1)
	{
		for (i=1; i<argc; i++)
		{
			if (!strcmp("--debug", argv[i]))
			{
				debug = 1;
			}
			/* required options */
			else if (!strcmp("--load", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
					loadp = argv[++i];
				else
					loadp = filepath;
			}
			else if (!strcmp("--save", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
					savep = argv[++i];
				else
					savep = filepath;
			}
			else if (!strcmp("--nummon", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
				{
					nummon = atoi(argv[++i]);
					int max = 1600;
					if (nummon > max)
					{
						fprintf(stderr, "nummon should not exceed %d\n",
						max);
						return 1;
					}
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --nummon <number>\n");
					return 1;
				}
			}
			/* for fun options */
			else if (!strcmp("--cheat", argv[i]))
			{
				invulnerable = 1;
			}
			else if (!strcmp("--hpw", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
				{
					Util::hpw = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --hpw <hpw>\n");
					return 1;
				}
			}
			else if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i]))
			{
				// display help page (README.txt)
				Util::help();
				return 1;
			}
			else if (!strcmp("--nofog", argv[i]))
			{
				nofog = 1;
			}
			else if (!strcmp("--parse", argv[i]))
			{
				if (i+1<argc)
				{
					Util::parse(argv[++i]);
					return 0;
				}
				else
				{
					Util::parse(monpath);
					Util::parse(objpath);
					return 0;
				}
			}
			else if (!strcmp("--pc", argv[i]))
			{
				if (i+2<argc)
				{
					pcx = atoi(argv[++i]);
					pcy = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --pc <x> <y>\n");
					return 1;
				}
			}
			else if (!strcmp("--ptime", argv[i]))
			{
				if (i+1<argc)
				{
					ptime = atoi(argv[++i]) * 1000;
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --ptime <ptime>\n");
					return 1;
				}
			}
			else if (!strcmp("--seed", argv[i]))
			{
				if (i+1<argc)
				{
					seed = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --seed <seed>\n");
					return 1;
				}
			}
			else if (!strcmp("--sight", argv[i]))
			{
				sight = 1;
			}
			else if (!strcmp("--tol", argv[i]))
			{
				if(i+1<argc)
				{
					tol = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --tol <tolerance>\n");
					return 1;
				}
			}
			else if (!strcmp("--unify", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
				{
					unify = argv[++i][0];
					
					if (!(('0' <= unify && unify <= '9')
						||('a' <= unify && unify <= 'f')))
					{
						fprintf(stderr, "monster must be 0-9 a-f\n");
						return 1;
					}
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --unify <monster>\n");
					return 1;
				}
			}
			// undefined option
			else
			{
				fprintf(stderr, "option %s is undefined.\n", argv[i]);
				fprintf(stderr, "see available options using -h switch.\n");
				return 1;
			}
		}
	} // end of options

	if (!seed) 
		// if seed is unset
		srand(seed = time(NULL));
	else 
		// seed is set by -s option
		srand(seed);

	Debug::log("seed: %d", seed);
	Debug::log("save path: %s", savep);
	Debug::log("load path: %s\n", loadp);

	dungeon = new Dungeon;

	if (loadp)
	{
		if(dungeon->load(loadp))
		{
			fprintf(stderr, "dungeon: failed to load dungeon.\n");
			return 1; // load may fail when file does not exist
		}
	}
	else
	{
		dungeon->generate();
	}
	
	pc = new PC;	
	
	MonsterFactory::load(monpath);
	ObjectFactory::load(objpath);
	
	dungeon->generateMonsters(nummon);
	dungeon->generateObjects(numobj);

	dungeon->turn->enqueue(pc);

	// if PC location is unset
	if (!pcx && !pcy) 	{
		// place at random location
		dungeon->placeCharacter(pc);
	}
	// if PC location is specified
	else
	{
		// place at specified location
		dungeon->placeCharacter(pc, pcx, pcy);
	}
	
	// unify all monsters to one type
	if (unify)
	{
		// unify characteristics
		char unifyc = unify >= 'a' ? unify - 'a' + 10 : unify - '0'; 
		for (i=0; i<nummon; i++)
			dungeon->npcv[i]->setAbil(unifyc);
	}

	// init distance map
	int pcx, pcy;
	pc->getLocation(&pcx, &pcy);
	Dijkstra::run(pcx, pcy, 0);
	Dijkstra::run(pcx, pcy, 1);
	
	// ncurses stuff
	initscr();
	start_color();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);

	Util::initColor();

	// game simulation
	while (1)
	{
		UI::clearRow(22);

		char buffer[80];
		if (pc->isDead())
			sprintf(buffer, "YOU LOST ! (press Q to quit)");
		else if (dungeon->nummon() == 0)
			sprintf(buffer, "YOU WON !! (press Q to quit)");
		else
		{ 
			sprintf(buffer,"%02d MONSTERS LEFT !!",
			dungeon->nummon());

			mvprintw(22, 20, " A: AI C: Cast Spell");
		}
		
		mvprintw(22, 1, "%s", buffer);

		Debug::log("dungeon printing...");

		UI::reprint();

		Debug::log("dungoen printed.\n");
		
		// update attacked monster HP bar
		if (pc->attacking)
			UI::printMonsterHP();
		else
			UI::clearRow(0);
	
		refresh();

		if (pc->isDead() || !nummon)
		{
			char ch;
			while ((ch=getch())!='Q' && (ch!='q'));
			break;
		}

		// usleep(ptime);

		int quit = 0;

		int ch = getch();

		// 1 if user does some action that should cost no turn
		int noturn = 0;

		int pcx, pcy;
		pc->getLocation(&pcx, &pcy);

		switch (ch)
		{
			case 'Q':
			case 'q':
				quit = 1;
				if (!pc->isDead() && dungeon->nummon()>0 && ch=='q')
					quit = !quit; // don't quit if PC is not dead
				noturn = 1;
				break;
			case 'A':
			case 'a':
				Move::pcAI(); // automatic
				break;
			case '7':
			case 'Y':
			case 'y':
				noturn = Move::move(pc, pcx-1, pcy-1);
				break;
			case '8':
			case 'K':
			case 'k':
				noturn = Move::move(pc, pcx, pcy-1);
				break;
			case '9':
			case 'U':
			case 'u':
				noturn = Move::move(pc, pcx+1, pcy-1);
				break;
			case '6':
			case 'L':
			case 'l':
				noturn = Move::move(pc, pcx+1, pcy);
				break;
			case '3':
			case 'N':
			case 'n':
				noturn = Move::move(pc, pcx+1, pcy+1);
				break;
			case '2':
			case 'J':
			case 'j':
				noturn = Move::move(pc, pcx, pcy+1);
				break;
			case '1':
			case 'B':
			case 'b':
				noturn = Move::move(pc, pcx-1, pcy+1);
				break;
			case '4':
			case 'H':
			case 'h':
				noturn = Move::move(pc, pcx-1, pcy);
				break;
			case '>':
				if (dungeon->tmap[pcy][pcx]=='>')
					regenerateDungeon();
				else
					noturn = 1;
				break;
			case '<':
				if (dungeon->tmap[pcy][pcx]=='<')
					regenerateDungeon();
				else
					noturn = 1;
				break;
			case ' ':
				break;
			case 'E': case 'e':
			case 'T': case 't':
				UI::eList(); noturn = 1;
				break;
			case 'I': UI::setInspect(true);
			case 'i':
			case 'D': 
			case 'd':
			case 'W': 
			case 'w':
			case 'X': 
			case 'x':
				UI::iList(); noturn = 1;
				UI::setInspect(false);
				break;
			case 'M':
			case 'm':
				UI::mList(); noturn = 1;
				break;
			case 'O':
			case 'o':
				UI::oList(); noturn = 1;
				break;
			case 'S':
			case 's':
				quit = 1;
				if (!pc->isDead() && ch=='s')
					quit = !quit; // don't quit if PC is not dead
				noturn = 1;
				break;
			/* cast spell */
			case 'C':
			case 'c':
				noturn = 1;
				UI::sList();
				break;
			default:
				noturn = 1;
				break;
		}

		if (quit)
			break;

		if (ch == STAIR_UP || ch == STAIR_DOWN)
			continue;
		if (noturn)
			continue;
		
		while (1)
		{
			Character *c = dungeon->turn->dequeue();

			if (c->isDead())
			{
				dungeon->removeMonster((NPC *)c);
				continue;
			}

			dungeon->turn->enqueue(c);
			
			if (c->isPC())
			{					
				// increase HP
				pc->hp += 1;
				if (pc->hp > pc->hpmax())
					pc->hp = pc->hpmax();
				
				// increase MP
				pc->mp += pc->mpmax() * 0.001 * (rand() % 5 + 5);
				if (pc->mp > pc->mpmax())
					pc->mp = pc->mpmax();

				break;
			}
			else
			{
				Move::npc((NPC *)c);
				if (pc->isDead())
					break;
			}
		} // end of NPC turn
	} // end of game simulation

	endwin();

	if (savep)
	{
		dungeon->save(savep);
	}

	MonsterFactory::deleteFactories();
	ObjectFactory::deleteFactories();

	delete dungeon;

	delete pc;

	return 0;
}

static int regenerateDungeon()
{
	delete dungeon;
	
	dungeon = new Dungeon;

	dungeon->generate();
	
	dungeon->generateMonsters(nummon);
	dungeon->generateObjects(numobj);

	pc->clearSeenDungeon();

	// reset turn	
	pc->setTurn(0);

	dungeon->turn->enqueue(pc);

	dungeon->placeCharacter(pc);
	
	pc->getLocation(&pcx, &pcy);

	// init distance map
	Dijkstra::run(pcx, pcy, 0);
	Dijkstra::run(pcx, pcy, 1);
	
	return 0;
}

