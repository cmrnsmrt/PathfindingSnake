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
#include <time.h>
#include <queue>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

 
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
HANDLE hConsole;

using namespace std;

int moveDir = 0; // 1 = right, 2 = down, 3= left, 4 = up
int scrollNumber = 0;
int snaketrix[20][20]; // Like matrix, but a snake

struct coordinates { int x, y; };
std::deque<coordinates*> body;


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
	draw((5), (1), "##########     #########");
	setColor(2);
	draw((15), (1), "SNAKE");
	setColor(5);
	draw((5), (2), "#######################");
	setColor(1);
	draw((5), (2), "#");
	setColor(1);
	draw((28), (2), "#");

	for (int i = 0; i < 10; i++) { // Loop for center section
		setColor(1);
		draw((5), (3 + i), "#");
		setColor(5);
		draw((6), (3 + i), "#                    #");
		setColor(1);
		draw((28), (3 + i), "#");
	}

	setColor(5);
	draw((5), (13), "#######################");
	setColor(1);
	draw((5), (13), "#");
	setColor(1);
	draw((28), (13), "#");
	setColor(1);
	draw((5), (14), "########################");
	draw((0), (15), "                                              ");
	
}

void drawGrid() {
	for (int outer = 0; outer < 20; outer++) {
		for (int inner = 0; inner < 10; inner++) {
			if (snaketrix[outer][inner] == 0) {
				setColor(3);
				draw((outer+7), (inner+3), "X");
			}
			else if (snaketrix[outer][inner] == 1) {
				setColor(2);
				draw((outer+7), (inner+3), "S");
			}
			else if (snaketrix[outer][inner] == 3) {
				setColor(2);
				draw((outer + 7), (inner + 3), "O");
			}
		}
	}


	
	draw((0), (15), "                                              ");
}

void initialiseGrid() {
	for (int outer = 0; outer < 20; outer++) {
		for (int inner = 0; inner < 20; inner++) {
			snaketrix[outer][inner] = 0;
		}
	}

	srand(time(NULL));

	int startX = rand() % 19;
	int startY = rand() % 9;

	snaketrix[startX][startY] = 1;

	coordinates bodyXY = { startX, startY };
	body.push_back(new coordinates(bodyXY));

	 snaketrix[7][7] = 3;
}

void moveLoop() {

	int c = 0;
	while (1)
	{
		Sleep(1000);

		c = 0;

		switch ((c = _getch())) {
		case KEY_UP:
			moveDir = 4;
			break;
		case KEY_DOWN:
			moveDir = 2;
			break;
		case KEY_LEFT:
			moveDir = 3;
			break;
		case KEY_RIGHT:
			moveDir = 1;
			break;
		default:
			break;
		}

		if (moveDir == 1) {
			coordinates* headXY;
			headXY = body.back();
			cout << headXY;
		}

	}
}
