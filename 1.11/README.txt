=================================
Name: Yuxiang Zhang ID: 242882640
=================================

How to compile:
	Simply make, then you should get an executable program called rlg327

Specifications met:
	Specifications for version 1.09:
	- use 'w' to wear an selected item
	- use 't' to take off item
	- use 'd' to drop item
	- use 'x' to expunge item
	- use 'i' to view inventory
	- use 'e' to view equipment
	- use 'I' ('i' also works) to view item details when in inventory view
	- object attributes are applied to game
	  - hit changes max HP
	  - dam increases damage
	  - def increases defense
	  - speed increases speed
	- combat
	  - NPC does not attach other NPC
	  - NPC is displaced to a random neighbor cell when collision occurs
	  - character dies once HP falls below zero
	  - damage is rolled from all items plus character's own damage
	- extra
	  - HP and MP bars
	  - HP and MP slowly increases per PC turn
	  - PC can cast spells
	    - B: Blast nearby monsters and crack rocks
	    - H: Heal PC by 100
		- T: Teleport to a random open space
	  - monster HP bar is displayed when attacked by PC

Functions:
	All the static functions look like <source>::<function>
	It should be very clear where each function comes from.
	
	Important functions for version 1.09:
	- UI::eList()
	  - equipment view
	- UI::iList()
	  - inventory view
	- UI::setInspect()
	  - toggle on inspection details
	- Inventory::expunge()
	  - expunge selected item
	- PC::dropItem()
	- PC::pickUpItem()
	- PC::takeOffItem()
	- PC::wearItem()

Structure:
	main.cpp
	- Main program that identifies switches and run desired functions
	
	character.cpp character.h
	- Include characteristics and status of PC and NPC

	core.cpp core.h
	- Most common variables are declared in core.h
	
	debug.cpp debug.h
	- Export debug information to DEBUG.log
	
	dijkstra.cpp dijkstra.h
	- Generate non-tunneling and tunneling distance view

	dungeon.cpp dungeon.h
	- Can generate random dungeon
	- Can load and save a dungeon
	- Use methods from room.c to create and draw rooms
	- Use methods from path.c to connect rooms
	
	equipment.cpp equipment.h
	- PC equipment that PC is wearing

	heap.cpp heap.h
	- Heap used by Dijkstra's algorithm and game turn priority queue

	inventory.cpp inventory.h
	- PC inventory contains items picked up by PC

	item.cpp item.h
	- Item generated from ObjectFactory

	turn.c turn.h
	- Simulate game turn with a priority queue
	
	monsterfactory.cpp monsterfactory.h
	- Parsing monster description files and generate randomly selected NPC

	move.cpp move.h
	- Implement moving algorithms for PC and NPC

	npc.cpp npc.h
	- Implement NPC properties such as abilities.

	objectfactory.cpp objectfactory.h
	- Parse object description files and generate randomly selected item

	path.cpp path.h
	- Mainly responsible for paving a random path from room to room
	
	pc.cpp pc.h
	- Implement PC remembered seen dungeon.

	point.cpp point.h
	- Mainly used by Dijkstra's algorithm as elements to be stored in queue

	room.cpp room.h
	- Represent a room in a dungeon
	
	ui.cpp
	- Implement monster list GUI
	
	util.cpp util.h
	- All independent utility methods are implemented here
	
// To grader: you can ignore the following optional features.

===========
User Manual
===========

NAME
	rlg327 -- rogue like game

SYNOPSIS
	rlg327 [options]

DESCRIPTION
	When program dungeon is executed, a randomly generated dungeon of size 
	21 * 80 will be generated and displayed. A player character '@' will
	be moving around to find monsters to fight!
	 
	The following options are available:

	--load <file>
		This will load a dungeon from a saved file.
		If <file> is not specified, then by default it is ~/.rlg327/dungeon

	--save <file>
		This will save a dungeon to a file.
		If <file> is not specified, then by default it is ~/.rlg327/dungeon
	
	--nummon <number>
		Set the number of monsters.
	
	--cheat
		Makke PC invulnerable.

	--debug
		Turn on debug mode to display seed number when dungeon is generated.
	
	--dense
		Generate dense paths instead of sparse paths.
	
	--hpw <hpw>
		Use weight conversion formula: weight = hardness / hpw + 1
	
	--help, -h
		Display a help page. For this program, the help page is README.txt

	--nofog
		Set no fog of war.

	--pc <x> <y>
		Set the starting location for the player character.
	
	--parse <file>
		This will parse a description file for either monster or object.
		If <file> is not specified, then by default it will parse both:
		~/.rlg327/monster_desc.txt
		~/.rlg327/object_desc.txt
	
	--ptime <milliseconds>
		Set the pause time in milliseconds.

	--seed <seed>
		Generate dungeon using the given seed value.
	
	--sight
		Display line of sight for each monster.

	--tol <tolerance>
		Generate dungeon using the given tolerance value.
		The tolerance value is an upper bound for number of rooms.

