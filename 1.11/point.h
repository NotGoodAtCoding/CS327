#ifndef POINT_H
#define POINT_H

class Point
{
	public:
		int x, y;

		Point(int x, int y);
		~Point();
};

int point_neighbors(Point *neighbors[8], int x, int y);

#endif

