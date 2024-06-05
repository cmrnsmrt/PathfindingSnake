// Cameron-Stewart Smart
// CMP 304 Unit 1 Assessment

#pragma once
#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <string> 

using namespace std;

void initialiseDisplay();
void draw(int x, int y, string s);
void setColor(int fcolor, int bcolor);
void clearScreen(void);
void drawSnakeFrame();
void initialiseGrid();
void drawGrid();
int moveLoop();
int moveSnake(int moveDir);
void newFruit();
void endScreen(string score, string moves);
void updateScore(string score, string moves);
int finiteStateMachine();

#endif
