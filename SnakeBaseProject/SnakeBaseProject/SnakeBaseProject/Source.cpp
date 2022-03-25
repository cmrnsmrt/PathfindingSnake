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

	int dir = 1;
	int result;
	int moves = 0;
	int score = 0;

	
	clearScreen();
	initialiseDisplay();
	clearScreen();
	drawSnakeFrame();
	initialiseGrid();
	drawGrid();
	newFruit();

	while (1) {
		dir = moveLoop();
		result = moveSnake(dir);
		moves++;


		if (result == 1) {
			break;
		}

		if (result == 3) {
			newFruit();
			score++;
		}
	}

	string output = "Your score was: " + score;

	draw((7), (2), output);

	output = "Your moves were: " + moves;

	draw((7), (3), output);

	endScreen(moves, score);
	return 0;
}