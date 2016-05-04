#ifndef UTIL_H
#define UTIL_H

#include <iostream>

using namespace std;

class Util
{
	public:
		static int hpw;

		/* convert to base 62 single char 0-9 a-z A-Z */
		static char base62(int);

		/* generate a random number */
		static int getRandNum(int from, int to);

		/* convert hardness to weight */
		static int getWeight(int hardness);

		/* display a help page when -h is specified */
		static int help();

		/* init ncurses colors */
		static int initColor();

		/* parse a given file specified by the path */
		static int parse(const char *);
		
		/* get int associated with the give color string */
		static int parseColor(string &s);

		/* return 1 if there are parsing errors */
		static int parseDiceField(stringstream &ss, 
		string &field, const char *);

		/* get first char from string, return -1 if s.length > 1 */
		static int parseSymb(string &s);

		/* reverse order of bytes */
		static unsigned int reverse(unsigned int);

		static int trim(stringstream &);
		static string &trim(string &);
};

#endif
