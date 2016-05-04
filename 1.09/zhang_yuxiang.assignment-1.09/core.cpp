#include <stdio.h>

#include "core.h"
	
char* loadp = NULL;
char* savep = NULL;

int invulnerable = 0;

int nummon = 10;

int nofog = 0;

int pcx = 0;
int pcy = 0;

int ptime = 100000; // pause time in milliseconds

unsigned int seed = SEED_UNSET;

int sight = 0; // 1 if display line of sight

int tol = DEFAULT_TOL;

int unify = 0;

