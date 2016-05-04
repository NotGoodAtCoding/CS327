#ifndef CHARACTER_H
#define CHARACTER_H

#include "core.h"
#include "dice.h"

class Character
{
	protected:
		int x, y;
		char symb;
		int color;
		int speed;
		
		bool _isPC;

		int _hpmax;
		int _mpmax;

	public:
		bool dead;
		
		Dice *dam;
		
		int turn;
		
		int hp;
		int mp;

	public:
		Character();
		virtual ~Character();

		virtual int hpmax() { return _hpmax; } 
		virtual int mpmax() { return _mpmax; }

		inline bool isDead() { return dead; }

		inline bool isPC() { return _isPC; }

		void getLocation(int *, int *);
		
		inline int getX() { return x; }
		inline int getY() { return y; }
		
		inline int getColor() { return color; }		
		
		virtual int getSpeed() { return speed; }
		
		inline char getSymb() { return symb; } 
		inline int  getTurn() { return turn; }

		inline void setDead() { dead = true; }

		virtual void setLocation(int x, int y);

		inline void setTurn(int turn) { this->turn = turn; }

		virtual int getTotalDam();
		virtual int getTotalDef();
};

#endif

