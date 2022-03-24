// Cameron-Stewart Smart

// Includes
#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>  
#include <conio.h> 
#include <windows.h> 
#include <time.h> 
#include <string> 
#include <iostream> 
#include <sstream>
#include "Game.h"

using namespace std;

// Globals



int main() {
	
	initialiseDisplay();
	clearScreen();
	drawSnakeFrame();
	initialiseSnake();
	drawSnake();
}