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
std::deque<coordinates> body;


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
				draw((outer+7), (inner+3), " ");
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

	int startX = rand() % 18;
	int startY = rand() % 9;

	snaketrix[startX][startY] = 1;

	coordinates headXY;
	headXY.x = startX;
	headXY.y = startY;
	body.push_back(headXY);

	snaketrix[startX+1][startY] = 1;

	headXY.x = startX+1;
	body.push_back(headXY);

	snaketrix[7][7] = 3;
}

void newFruit() {
	srand(time(NULL));
	int fruitSet = 0;

	while (fruitSet == 0) {
		int startX = rand() % 18;
		int startY = rand() % 9;

		if ((snaketrix[startX][startY] != 1) and (snaketrix[startX][startY] != 3)) {
			snaketrix[startX][startY] = 3;
			setColor(2);
			draw((startX + 7), (startY + 3), "O");
			fruitSet = 1;
		}
	}
	

	
}

void endScreen(string score, string moves) {

	setColor(1);
	draw((5), (19), "########################");
	draw((5), (20), "######            ######");
	draw((5), (21), "########################");
	setColor(2);
	draw((12), (20), "GAME  OVER");

	draw((5), (16), "Your score was:        ");
	draw((22), (16), score);

	draw((5), (17), "Your moves count was:        ");
	draw((27), (17), moves);

	draw((5), (23), "Thank you for playing!");
}

void updateScore(string score, string moves) {
	setColor(2);
	draw((5), (16), "Your score is:        ");
	draw((21), (16), score);

	draw((5), (17), "Your moves count is:       ");
	draw((26), (17), moves);
}

int moveLoop() {

	int c = 0;

	moveDir = 0;

	while (moveDir == 0)
	{
		int snakeMoved = 0;

		c = 0;

		c = _getch();
		
		if (c == KEY_UP) {
			moveDir = 4;
		}
		else if (c == KEY_RIGHT) {
			moveDir = 1;
		}
		else if (c == KEY_DOWN) {
			moveDir = 2;
		}
		else if (c == KEY_LEFT) {
			moveDir = 3;
		}
	}

	return moveDir;
}

int moveSnake(int moveDir) {

	coordinates headXY;

	switch (moveDir) {
	case 4:

		headXY = body.back();
		headXY.y = headXY.y - 1;
		body.push_back(headXY);
		setColor(2);
		draw((headXY.x + 7), (headXY.y + 3), "S");

		if (snaketrix[headXY.x][headXY.y] == 1) {
			return 1;
		}

		if (snaketrix[headXY.x][headXY.y] == 3) {
			snaketrix[headXY.x][headXY.y] = 1;
			return 3;
		}

		snaketrix[headXY.x][headXY.y] = 1;

		headXY = body.front();
		setColor(3);
		draw((headXY.x + 7), (headXY.y + 3), " ");
		snaketrix[headXY.x][headXY.y] = 0;
		body.pop_front();

		break;
	case 2:

		headXY = body.back();
		headXY.y = headXY.y + 1;
		body.push_back(headXY);
		setColor(2);
		draw((headXY.x + 7), (headXY.y + 3), "S");

		if (snaketrix[headXY.x][headXY.y] == 1) {
			return 1;
		}

		if (snaketrix[headXY.x][headXY.y] == 3) {
			snaketrix[headXY.x][headXY.y] = 1;
			return 3;
		}

		snaketrix[headXY.x][headXY.y] = 1;

		headXY = body.front();
		setColor(3);
		draw((headXY.x + 7), (headXY.y + 3), " ");
		snaketrix[headXY.x][headXY.y] = 0;
		body.pop_front();

		break;
	case 3:

		headXY = body.back();
		headXY.x = headXY.x - 1;
		body.push_back(headXY);
		setColor(2);
		draw((headXY.x + 7), (headXY.y + 3), "S");

		if (snaketrix[headXY.x][headXY.y] == 1) {
			return 1;
		}

		if (snaketrix[headXY.x][headXY.y] == 3) {
			snaketrix[headXY.x][headXY.y] = 1;
			return 3;
		}

		snaketrix[headXY.x][headXY.y] = 1;

		headXY = body.front();
		setColor(3);
		draw((headXY.x + 7), (headXY.y + 3), " ");
		snaketrix[headXY.x][headXY.y] = 0;
		body.pop_front();

		break;
	case 1:

		headXY = body.back();
		headXY.x = headXY.x + 1;
		body.push_back(headXY);
		setColor(2);
		draw((headXY.x + 7), (headXY.y + 3), "S");

		if (snaketrix[headXY.x][headXY.y] == 1) {
			return 1;
		}

		if (snaketrix[headXY.x][headXY.y] == 3) {
			snaketrix[headXY.x][headXY.y] = 1;
			return 3;
		}

		snaketrix[headXY.x][headXY.y] = 1;

		headXY = body.front();
		setColor(3);
		draw((headXY.x + 7), (headXY.y + 3), " ");
		snaketrix[headXY.x][headXY.y] = 0;
		body.pop_front();

		break;
	default:
		break;
	}

	headXY = body.back();

	if ((headXY.x >= 20 or headXY.x < 0) or (headXY.y >= 10 or headXY.y < 0)) {
		return 1;
	}

	return 0;
}
