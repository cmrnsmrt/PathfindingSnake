#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <string> 

using namespace std;

void initialiseDisplay();
void draw(int x, int y, string s);
void setColor(int fcolor, int bcolor);
void clearScreen(void);
int checkColor(string symbol);
void drawSnakeFrame();
void initialiseSnake();
void drawSnake();

#endif

