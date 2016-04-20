#ifndef UI_H
#define UI_H

#define COLOR_HP 10
#define COLOR_MP 20
#define COLOR_ZB 30

class UI
{
	private:
		// true if display item details
		static bool inspect;

		static int eListSelect(int);
		static int iListSelect(int);

		static int printEquipmentSlots();
		
	public:
		static int clearRow(int row);
		static int reprint();
		
		static int eList();
		static int iList();
		static int mList();
		static int oList();
		static int sList(); // spell list

		// turn on inspect
		static int setInspect(bool inspect);

		static int printHP();
		static int printMP();

		static int printMonsterHP();
};

#endif
