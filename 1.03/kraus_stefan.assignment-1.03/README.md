README

Author: Stefan Kraus
Assignment: 1.03
Path Finding

To Run:
run ' make all '
run ' ./rlg327 ' with optional --save and --load switches. 

Description: 
This generates a 80 x 24 dungeon and two distance maps.
The dungeon has ' ' t denote walls, '.' to denote rooms,
and '#' to denote hallways.
The first distance map does not include wall terrain, and
does not include distances that require tunneling.
The second distance map includes wall terrain and distances
that may require tunneling. 
Distances are displayed in ASCII as [0-9][a-z][A-Z] ascending.
On both maps, distances over 62 are not displayed; instead,
the dungeon is displayed normally

Rooms are denoted by '.', hallways by '#' and rock walls by ' ' 

CHANGELOG

1.03
Added distance map generation, main method now renders the distance
maps for both tunneling and non- monsters. 
