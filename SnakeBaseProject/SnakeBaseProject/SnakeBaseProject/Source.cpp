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

		updateScore(to_string(score), to_string(moves));
	}

	endScreen(to_string(score), to_string(moves));

	return 0;
}