#ifndef NPC_H
#define NPC_H

#include "character.h"

#ifdef __cplusplus

class __NPC : public __Character
{
	private:
		int mempcx;
		int mempcy;
		int abil; // abilities

	public:
		__NPC();
		~__NPC();

		inline int getAbil() {return abil;}

		void getMemPCLocation(int *, int *);
	
		inline void setAbil(int abil) {this->abil = abil;}
		
		void setMemPCLocation(int x, int y);
};

#endif // c++

#ifdef __cplusplus
extern "C" {
#endif

/* delete all NPCs */
int npc_deleteAll();

#ifdef __cplusplus
}
#endif

#endif

