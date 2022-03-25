#pragma once


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
void initialiseGrid();
void drawGrid();
int moveLoop();
int moveSnake(int moveDir);
void newFruit();
void endScreen(int moves, int score);

#endif
