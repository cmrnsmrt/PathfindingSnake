// Cameron-Stewart Smart
// CMP 304 Unit 1 Assessment
// 
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
	
	initialiseDisplay(); // Initialises display
	clearScreen(); // Clears any text on screen
	drawSnakeFrame(); // Draws frame for snake
	initialiseGrid(); // Sets up the snaketrix
	drawGrid(); // Draws main game contents
	newFruit(); // Adds starting fruit onto snaketrix and draws

	while (1) { // Loops until game ends
		dir = moveLoop(); // Gets direction from AI or user
		result = moveSnake(dir); // Moves snake
		moves++; // Increments move counter
		
		if (result == 1) { // If game over code is returned from snake move then game over
			break; // End game
		}

		if (result == 3) { // If fruit is hit by the snake increase score and add new fruit
			int endCheck = newFruit(); // New fruit
			if (endCheck == 1) { // If game over code is returned from new fruit (snaketrix full)
				return 0; // End game
			}
			score++; // Increment score counter
		}
		updateScore(to_string(score), to_string(moves)); // Reprints score
	}

	endScreen(to_string(score), to_string(moves)); // Displays end screen once game loop is broken

	return 0; // End
}