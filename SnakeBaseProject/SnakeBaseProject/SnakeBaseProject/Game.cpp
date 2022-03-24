#include "Game.h"
#include <stdio.h>
#include <stdlib.h> 
#include <ctype.h>  
#include <conio.h> 
#include <windows.h> 
#include <time.h> 
#include <string> 
#include <iostream> 
#include <sstream>
 
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
HANDLE hConsole;

using namespace std;

int scrollNumber = 0;

void initialiseDisplay()
{
	COORD console_size = { 110, 30 }; // Sets size of console
	hConsole = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,

		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);

	SetConsoleScreenBufferSize(hConsole, console_size);
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
}

void draw(int x, int y, string s) // For drawing anything on the screen (position x, position y, string output)
{
	COORD cursorPos; // Creates a variable to hold the coordinates

	cursorPos.X = x;
	cursorPos.Y = y + scrollNumber;

	SetConsoleCursorPosition(hConsole, cursorPos);

	// printf(s);
	cout << s;
}

void setColor(int fcolor, int bcolor = 0)
{
	SetConsoleTextAttribute(hConsole, (WORD)((bcolor << 4) |

		fcolor));
}

void clearScreen(void)
{
	scrollNumber = 0;
	setColor(15, 0); // Selects colour to reset with (white foreground and black background)

	for (int index = 0; index <= 30; index++) { // Goes through all the lines and wipes them
		draw(0, scrollNumber, "                                                                                                                                                                                                                                        \n");
	}
}

int checkColor(string symbol) // This allows each symbol to be printed in a unique colour
{
	int colorNum = 0;

	if (symbol == "^") { // If the symbol is a carrat then it is colour 3
		colorNum = 3;
	}
	else if (symbol == "&") { // If the symbol is an ampersand then it is colour 5
		colorNum = 5;
	}
	else if (symbol == "@") { // If the symbol is a at symbol then it is colour 7
		colorNum = 6;
	}
	return colorNum; // Returns the colour value to be used in a draw statement
}

void drawSnakeFrame() // Draws the box that the snake is confined to
{
	stringstream ss; // Creates a string stream variable to allow characters to be converted into strings for output

	setColor(1);
	draw((5), (1), "##########     ##########");
	setColor(2);
	draw((15), (1), "SNAKE");
	setColor(5);
	draw((5), (2), "#########################");
	setColor(1);
	draw((5), (2), "#");
	setColor(1);
	draw((29), (2), "#");

	for (int i = 0; i < 10; i++) { // Loop for center section
		setColor(1);
		draw((5), (3 + i), "#");
		setColor(5);
		draw((6), (3 + i), "#                     #");
		setColor(1);
		draw((29), (3 + i), "#");
	}

	setColor(5);
	draw((5), (13), "#########################");
	setColor(1);
	draw((5), (13), "#");
	setColor(1);
	draw((29), (13), "#");
	setColor(1);
	draw((5), (14), "#########################");
	

}
