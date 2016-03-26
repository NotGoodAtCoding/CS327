=================================
Name: Yuxiang Zhang ID: 242882640
=================================

How to compile:
	Simply make, then you should get an executable program called rlg327

Specifications met:
	Specifications for version 1.06:
	- all characters are implemented in class __Character __PC __NPC
	- export C interface in character.cpp pc.cpp npc.cpp
	- only display PC visible area of radius 3
	- only display cells that are not blocked on line of sight
	- once PC enters a room, the entire room is visible
	- store PC visible area in __PC class
	- visible area is highlighted in yellow
				
Functions:
	I use convention <source>_<function> to name the functions, so it should
	be clear where each function comes from. e.g. room_new() from room.c
	
	Important functions for version 1.05:
	- all getters and setters in character.cpp and character.h
	- dungeon_isVisible() returns 1 if given location is visible by PC
	- pc_getSeenDungeon() returns PC seen dungeon (fog of war effect)

Structures:
	main.c
	- Main program that identifies switches and run desired functions
	
	character.cpp character.h
	- Include characteristics and status of PC and NPC

	core.c core.h
	- Most common variables are declared in core.h
	
	debug.c debug.h
	- Export debug information to DEBUG.log
	
	dijkstra.c dijkstra.h
	- Generate non-tunneling and tunneling distance view

	dungeon.c dungeon.h
	- Can generate random dungeon
	- Can load and save a dungeon
	- Use methods from room.c to create and draw rooms
	- Use methods from path.c to connect rooms
	
	heap.c heap.h
	- Heap used by Dijkstra's algorithm and game turn priority queue
	turn.c turn.h
	- Simulate game turn with a priority queue
	
	move.c move.h
	- Implement moving algorithms for PC and NPC

	npc.cpp npc.h
	- Implement NPC properties such as abilities.

	path.c path.h
	- Mainly responsible for paving a random path from room to room
	
	pc.cpp pc.h
	- Implement PC remembered seen dungeon.

	point.c point.h
	- Mainly used by Dijkstra's algorithm as elements to be stored in queue

	room.c room.h
	- Room struct is defined in room.h
	- Room related methods are implemented in room.c
	
	ui.c
	- Implement monster list GUI
	
	util.c util.h
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
	
	--ptime <milliseconds>
		Set the pause time in milliseconds.

	--seed <seed>
		Generate dungeon using the given seed value.
	
	--sight
		Display line of sight for each monster.

	--tol <tolerance>
		Generate dungeon using the given tolerance value.
		The tolerance value is an upper bound for number of rooms.

