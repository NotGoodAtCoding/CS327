#ifndef PATH_H
#define PATH_H

#include "room.h"

class Path
{
	public:
		/* pave a path from an unconnected room to a connected room */
		static void pavePath(Dungeon *, Room*, Room*);
};

#endif
