#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <stdio.h>
#include <ncurses.h>
using namespace std;

#define RED "RED"
#define GREEN "GREEN"
#define BLUE "BLUE"
#define CYAN "CYAN"
#define YELLOW "YELLOW"
#define MAGENTA "MAGENTA"
#define WHITE "WHITE"
#define BLACK "BLACK"

#define BACKGROUND COLOR_BLACK

class color{
  
private:

  string type;
public:
  short int number;
    
    color(void){
    }

    color(string input){
      if(input == RED){
        type = RED;
	number = COLOR_RED;
	init_pair(number, number, BACKGROUND);
      }
      else if(input == GREEN){
        type = GREEN;
	number = COLOR_GREEN;
	init_pair(number, number, BACKGROUND);
      }
      else if(input == BLUE){
        type = BLUE;
	number = COLOR_BLUE;
	init_pair(number, number, BACKGROUND);
      }
      else if(input == CYAN){
        type = CYAN;
	number = COLOR_CYAN;
	init_pair(number, number, BACKGROUND);
      }
      else if(input == YELLOW){
        type = YELLOW;
	number = COLOR_YELLOW;
	init_pair(number, number, BACKGROUND);
      }
      else if(input == MAGENTA){
        type = MAGENTA;
	number = COLOR_MAGENTA;
	init_pair(number, number, BACKGROUND);
      }
      else if(input == WHITE){
        type = WHITE;
	number = (BACKGROUND == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;
	init_pair(number, number, BACKGROUND); 
     }
      else if(input == BLACK){
        type = BLACK;
	number = (BACKGROUND == COLOR_BLACK)? COLOR_WHITE : COLOR_BLACK;
	init_pair(number, number, BACKGROUND);
      }
      else{
        char msg[80];
        sprintf(msg, "Color mon_%s not supported\n", input.c_str());
          throw msg;
        }
    }
  
  friend ostream &operator<<(ostream &o, const color &input);

};

#endif
