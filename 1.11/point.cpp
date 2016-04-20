#include <stdlib.h>

#include "point.h"

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

Point::~Point()
{
}

int point_neighbors(Point *ns[8], int x, int y)
{	
	ns[0] = new Point(x, y - 1); // N
	ns[1] = new Point(x, y + 1); // S
	ns[2] = new Point(x - 1, y); // W
	ns[3] = new Point(x + 1, y); // E
	ns[4] = new Point(x - 1, y - 1); // NW
	ns[5] = new Point(x + 1, y - 1); // NE
	ns[6] = new Point(x - 1, y + 1); // SW
	ns[7] = new Point(x + 1, y + 1); // SE
	return 0;
}

