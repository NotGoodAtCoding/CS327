README

Author: Stefan Kraus
Assignment: 1.03
Path Finding

To Run:
run ' make all '
run ' ./rlg327 ' with optional --save and --load switches. 

Description: 
This generates a 80 x 24 dungeon, a Playable character, 
and up to 10 Monsters.
The dungeon has ' ' t denote walls, '.' to denote rooms,
and '#' to denote hallways.
The PC is rendered as a '@', and monsters are rendered as 'D'
Rooms are denoted by '.', hallways by '#' and rock walls by ' ' 

CHANGELOG

1.04
Added Monsters and PC movement.
Monsters have the attributes:
Intelligence - Move along shortest path, otherwise the monster
  moves in a straight line towards it's target.
Telepathy - Monster knows where the PC is at all times and moves
  towards it in its chosen method.
Tunneling Abillity - Monster hacks at and breaks down walls to
  chase PC, hitting for -85 hardness until the wal breaks and becomes
  a hallway terrain type.
Erratic - 50% chance of attempted movement in random direction.

1.03
Added distance map generation, main method now renders the distance
maps for both tunneling and non- monsters. 
