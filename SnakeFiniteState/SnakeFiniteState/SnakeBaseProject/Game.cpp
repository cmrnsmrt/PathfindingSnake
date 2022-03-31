// Cameron-Stewart Smart
// CMP 304 Unit 1 Assessment

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
#define WIDTH 20
#define HEIGHT 10

CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
HANDLE hConsole;

using namespace std;

int moveDir = 0; // 1 = right, 2 = down, 3= left, 4 = up
int scrollNumber = 0;
int snaketrix[20][20]; // Like matrix, but a snake
struct space {
	double f;
	double g;
	double h;
	bool open;
	bool closed;
};

// Why a matrix?

// -> a matrix allows for a 2 dimenstional grid structure to be easily represented. This can be accessed
// using the coordinates and will hold the contents of each space. Eg. if the space is snake, empty or fruit.

struct coordinates { int x, y; }; // This struct is used anytime coordinates on the snaketrix are needed
std::deque<coordinates> body; // A double ended queue is used to hold the snake positions

// Why a deque?
 
// -> A deque allows for both ends of the queue to be read and popped. A deque works best for snake
// as when the snake moves a new position is added for where the head is moving at the back of the
// queue and the end of the snake can be popped from the front of the queue as the snake leaves
// that space.

void initialiseDisplay() // This sets up the display to have text displayed on it
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

	cout << s;
}

void setColor(int fcolor, int bcolor = 0) // Allows the colour of the text being printed to be set
{
	SetConsoleTextAttribute(hConsole, (WORD)((bcolor << 4) |

		fcolor));
}

void clearScreen(void) // Clears the display by manually overwriting with spaces
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

	setColor(1); // Blue
	draw((5), (1), "##########     #########");
	setColor(2); // Green
	draw((15), (1), "SNAKE");
	setColor(5); // Purple
	draw((5), (2), "#######################");
	setColor(1); // Blue
	draw((5), (2), "#");
	draw((28), (2), "#");

	for (int i = 0; i < 10; i++) { // Loop for center section
		setColor(1); // Blue
		draw((5), (3 + i), "#");
		setColor(5); // Purple
		draw((6), (3 + i), "#                    #");
		setColor(1); // Blue
		draw((28), (3 + i), "#");
	}

	setColor(5); // Purple
	draw((5), (13), "#######################");
	setColor(1); // Blue
	draw((5), (13), "#");
	draw((28), (13), "#");
	draw((5), (14), "########################");
	draw((0), (15), "                                              ");
	
}

void drawGrid() { // This draws the contents of the snake game by checking content of snaketrix
	for (int outer = 0; outer < 20; outer++) {
		for (int inner = 0; inner < 10; inner++) {
			if (snaketrix[outer][inner] == 2) { // Empty space. Even empty spaces must be redrawn each move because the snake could have been here and moved
				setColor(1); // "Blue"
				draw((outer+7), (inner+3), " ");
			}
			else if (snaketrix[outer][inner] == 1) { // Snake
				setColor(2); // Green
				draw((outer+7), (inner+3), "S");
			}
			else if (snaketrix[outer][inner] == 3) { // Fruit
				setColor(5); // Purple
				draw((outer + 7), (inner + 3), "O");
			}
		}
	}

	draw((0), (15), "                                              "); // This moves the cursor to bottom after drawing
}

void initialiseGrid() { // Sets up the snaketrix
	for (int outer = 0; outer < 20; outer++) { // Fills whole snaketrix with empty spaces
		for (int inner = 0; inner < 20; inner++) {
			snaketrix[outer][inner] = 2;
		}
	}

	srand(time(NULL)); // Sets random

	int startX = rand() % 18; // Starting x decided
	int startY = rand() % 9; // Starting y decided

	snaketrix[startX][startY] = 1; // Creates snake head part 1 here

	coordinates headXY; // Sets head position as space above
	headXY.x = startX;
	headXY.y = startY;
	body.push_back(headXY);

	//snaketrix[startX+1][startY] = 1; // Creates first part of body one to the right
	//headXY.x = startX+1;
	//body.push_back(headXY);
}

void newFruit() { // Adds a new fruit to snaketrix
	srand(time(NULL));
	int fruitSet = 0;
	int endCheck = 0;

	while (fruitSet == 0) { // Uses while loop to detect when a new space is found
		int startX = (rand() % 20); // Chooses random space for new fruit
		int startY = (rand() % 10);
		endCheck++; // Increases the amount of spaces checked

		if ((snaketrix[startX][startY] != 1) and (snaketrix[startX][startY] != 3)) { // Checks if space is fruit or snake
			snaketrix[startX][startY] = 3; // If space is empty then fruit is added
			setColor(5); // Purple
			draw((startX + 7), (startY + 3), "O"); // Draws fruit
			fruitSet = 1; // Ends while loop
		}
	}
}

void endScreen(string score, string moves) { // Prints end screen

	setColor(1); // Blue
	draw((5), (19), "########################");
	draw((5), (20), "######            ######");
	draw((5), (21), "########################");
	setColor(2); // Green
	draw((12), (20), "GAME  OVER");

	draw((5), (16), "Your score was:        ");
	draw((22), (16), score);
	draw((5), (17), "Your moves count was:        ");
	draw((27), (17), moves);
	draw((5), (23), "Thank you for playing!");
}

void updateScore(string score, string moves) { // Prints score each move
	setColor(2); // Green
	draw((5), (16), "Your score is:        ");
	draw((21), (16), score);
	draw((5), (17), "Your moves count is:       ");
	draw((26), (17), moves);
}

int moveLoop() { // This is the first main element of the game, when the user chooses which way to move

	// This function will be replaced or altered to host the function where the AI chooses which way to move

	int c = 0;
	moveDir = 0; // Clear current move direction

	while (moveDir == 0) // Loops until move direction is set
	{
		c = 0;
		c = _getch(); // Gets keyboard input
		
		if (c == KEY_UP) { // If up, set move direction to up
			moveDir = 4;
		}
		else if (c == KEY_RIGHT) { // If right, set move direction to right
			moveDir = 1;
		}
		else if (c == KEY_DOWN) { // If down, set move direction to down
			moveDir = 2;
		}
		else if (c == KEY_LEFT) { // If left, set move direction to left
			moveDir = 3;
		}
	}
	return moveDir; // Returns move direction for use in moveSnake function
}

int finiteStateMachine() {

	coordinates targetXY;
	coordinates finiteHeadXY;

	targetXY.x = 0;
	targetXY.y = 0;

	for (int outer = 0; outer < 20; outer++) { // Lopps through snaketrix to find fruit
		for (int inner = 0; inner < 20; inner++) {
			if (snaketrix[outer][inner] == 3) {
				targetXY.x = outer;
				targetXY.y = inner;
				break;
			}
		}
	}

	finiteHeadXY = body.back();

	enum class SnakeStates { clear, blocked }; // Class that holds the state of the snake
	class SnakeSurroundings { // Class that holds the state of surrounding spaces
	public:
		bool up;
		bool upRight;
		bool right;
		bool downRight;
		bool down;
		bool downLeft;
		bool left;
		bool upLeft;
	};

	SnakeSurroundings snake_surroundings{}; // Initialise classes
	SnakeStates snake_states = SnakeStates::clear;
	snake_states = SnakeStates::clear;

	snake_surroundings.up = true; // True if space is empty
	snake_surroundings.upRight = true;
	snake_surroundings.right = true;
	snake_surroundings.downRight = true;
	snake_surroundings.down = true;
	snake_surroundings.downLeft = true;
	snake_surroundings.left = true;
	snake_surroundings.upLeft = true;

	if ((snaketrix[(finiteHeadXY.x)][(finiteHeadXY.y) - 1] == 1) or (snaketrix[(finiteHeadXY.x)][(finiteHeadXY.y) - 1] == 0)) { // Checks if space up is blocked
		snake_surroundings.up = false; // Sets space up as blocked in class
	}
	if ((snaketrix[(finiteHeadXY.x) + 1][(finiteHeadXY.y) - 1] == 1) or (snaketrix[(finiteHeadXY.x) + 1][(finiteHeadXY.y) - 1] == 0)) { // Checks if space up and right is blocked
		snake_surroundings.upRight = false; // Sets space up and right as blocked in class
	}
	if ((snaketrix[(finiteHeadXY.x) + 1][(finiteHeadXY.y)] == 1) or (snaketrix[(finiteHeadXY.x) + 1][(finiteHeadXY.y)] == 0)) { // Checks if space right is blocked
		snake_surroundings.right = false; // Sets space right as blocked in class
	}
	if ((snaketrix[(finiteHeadXY.x) + 1][(finiteHeadXY.y) + 1] == 1) or (snaketrix[(finiteHeadXY.x) + 1][(finiteHeadXY.y) + 1] == 0)) { // Checks if space down and right is blocked
		snake_surroundings.downRight = false; // Sets space down and right as blocked in class
	}
	if ((snaketrix[(finiteHeadXY.x)][(finiteHeadXY.y) + 1] == 1) or (snaketrix[(finiteHeadXY.x)][(finiteHeadXY.y) + 1] == 0)) { // Checks if space down is blocked
		snake_surroundings.down = false; // Sets space down as blocked in class
	}
	if ((snaketrix[(finiteHeadXY.x) - 1][(finiteHeadXY.y) + 1] == 1) or (snaketrix[(finiteHeadXY.x) - 1][(finiteHeadXY.y) + 1] == 0)) { // Checks if space down and left is blocked
		snake_surroundings.downLeft = false; // Sets space down and left as blocked in class
	}
	if ((snaketrix[(finiteHeadXY.x) - 1][(finiteHeadXY.y)] == 1) or (snaketrix[(finiteHeadXY.x) - 1][(finiteHeadXY.y)] == 0)) { // Checks if space left is blocked
		snake_surroundings.left = false; // Sets space left as blocked in class
	}
	if ((snaketrix[(finiteHeadXY.x) - 1][(finiteHeadXY.y) - 1] == 1) or (snaketrix[(finiteHeadXY.x) - 1][(finiteHeadXY.y) - 1] == 0)) { // Checks if space up and left is blocked
		snake_surroundings.upLeft = false; // Sets space up and left as blocked in class
	}

	if ((snake_surroundings.up == true) and (snake_surroundings.upRight == true) and (snake_surroundings.right == true) and (snake_surroundings.downRight == true) and (snake_surroundings.down == true) and (snake_surroundings.downLeft == true) and (snake_surroundings.left == true) and (snake_surroundings.upLeft == true)) {
		snake_states = SnakeStates::clear; // If all spaces are clear then the snakes status is set and clear
	}
	else {
		snake_states = SnakeStates::blocked; // If any spaces are blocked then the snakes status is set as blocked
	}

	switch (snake_states) { // Switches depending on the status of the snake
	case SnakeStates::clear: // If there are no obstacles snake can simply move
		if (targetXY.x < finiteHeadXY.x) { // If target is left
			return 3; 
		}
		else if (targetXY.x > finiteHeadXY.x) {  // If target is right
			return 1;
		}
		else if (targetXY.y < finiteHeadXY.y) { // If target is up
			return 4;
		}
		else if (targetXY.y > finiteHeadXY.y) {  // If target is down
			return 2;
		}
	case SnakeStates::blocked: // If the snake is set as blockd then obstacles must be negotiated
		if (targetXY.x < finiteHeadXY.x) { // If target is left
			if (snake_surroundings.left == true) { // Checks if left is clear
				return 3;
			} else if (snake_surroundings.up == true) { // If left is not clear and up is clear
				return 4;
			} else if (snake_surroundings.down == true) { // If left and up are not clear and down is clear
				return 2;
			} else if (snake_surroundings.right == true) { // If left, up and down are not clear and right is clear
				return 1;
			}
		} 
		else if (targetXY.x > finiteHeadXY.x) {  // If target is right
			if (snake_surroundings.right == true) { // Checks if right is clear
				return 1;
			} else if (snake_surroundings.up == true) { // If right is not clear and up is clear
				return 4;
			} else if (snake_surroundings.down == true) { // If right and up are not clear and down is clear
				return 2;
			} else if (snake_surroundings.left == true) { // If right, up and dowb are not clear and left is clear
				return 3;
			}
		}
		else if (targetXY.y < finiteHeadXY.y) { // If target is up
			if (snake_surroundings.up == true) { // Checks if right is clear
				return 4;
			} else if (snake_surroundings.left == true) { // If up is not clear and left is clear
				return 3;
			} else if (snake_surroundings.right == true) { // If up and left are not clear and right is clear
				return 1;
			} else if (snake_surroundings.down == true) { // If up, left and right are not clear and dowb is clear
				return 2;
			}
		}
		else if (targetXY.y > finiteHeadXY.y) {  // If target is down
			if (snake_surroundings.down == true) { // Checks if down is clear
				return 2;
			} else if (snake_surroundings.left == true) { // If down is not clear and left is clear
				return 3;
			} else if (snake_surroundings.right == true) { // If down and left are not clear and right is clear
				return 1;
			} else if (snake_surroundings.up == true) { // If down, left right are not clear and up is clear
				return 4;
			}
		}
	}
	return 5;
}

int moveSnake(int moveDir) { // This is the second main element of the game, where the snake is moved.

	coordinates headXY; // Coordinates to hold current head of snake

	switch (moveDir) { //This switches what code to use base on chosen move direction

	case 1:
		headXY = body.back(); // Sets current position of head to the body item at back of queue (front of snake)
		headXY.x = headXY.x + 1; // Sets new position of head right one space
		body.push_back(headXY); // Pushes new head position onto queue
		setColor(2); // Green
		draw((headXY.x + 7), (headXY.y + 3), "S"); // Draws new snake head

		if (snaketrix[headXY.x][headXY.y] == 1) { // Checks the space of the new head is not already snake
			return 1; // Returns end game code
		}

		if (snaketrix[headXY.x][headXY.y] == 3) { // Checks if space of the new head contains fruit
			snaketrix[headXY.x][headXY.y] = 1; // Sets fruit space to now contain snake
			return 3; // Returns fruit hit code
		}

		// This code runs when the new space is empty 

		snaketrix[headXY.x][headXY.y] = 1; // Sets new space to be snake

		headXY = body.front(); // Changes head holder to back of snake in order to draw over space and pop front of queue
		setColor(3);
		draw((headXY.x + 7), (headXY.y + 3), " "); // Draws empty space over old tail of snake
		snaketrix[headXY.x][headXY.y] = 2; // Sets position of old tail in snaketrix to empty
		body.pop_front(); // Pops old tail from queue

		break;
	case 2:
		headXY = body.back(); // Sets current position of head to the body item at back of queue (front of snake)
		headXY.y = headXY.y + 1; // Sets new position of head down one space
		body.push_back(headXY); // Pushes new head position onto queue
		setColor(2); // Green
		draw((headXY.x + 7), (headXY.y + 3), "S"); // Draws new snake head

		if (snaketrix[headXY.x][headXY.y] == 1) { // Checks the space of the new head is not already snake
			return 1; // Returns end game code
		}

		if (snaketrix[headXY.x][headXY.y] == 3) { // Checks if space of the new head contains fruit
			snaketrix[headXY.x][headXY.y] = 1; // Sets fruit space to now contain snake
			return 3; // Returns fruit hit code
		}

		// This code runs when the new space is empty 

		snaketrix[headXY.x][headXY.y] = 1; // Sets new space to be snake

		headXY = body.front(); // Changes head holder to back of snake in order to draw over space and pop front of queue
		setColor(3);
		draw((headXY.x + 7), (headXY.y + 3), " "); // Draws empty space over old tail of snake
		snaketrix[headXY.x][headXY.y] = 2; // Sets position of old tail in snaketrix to empty
		body.pop_front(); // Pops old tail from queue

		break;
	case 3:
		headXY = body.back(); // Sets current position of head to the body item at back of queue (front of snake)
		headXY.x = headXY.x - 1; // Sets new position of head left one space
		body.push_back(headXY); // Pushes new head position onto queue
		setColor(2); // Green
		draw((headXY.x + 7), (headXY.y + 3), "S");  // Draws new snake head

		if (snaketrix[headXY.x][headXY.y] == 1) { // Checks the space of the new head is not already snake
			return 1; // Returns end game code
		}

		if (snaketrix[headXY.x][headXY.y] == 3) { // Checks if space of the new head contains fruit
			snaketrix[headXY.x][headXY.y] = 1; // Sets fruit space to now contain snake
			return 3; // Returns fruit hit code
		}

		// This code runs when the new space is empty 

		snaketrix[headXY.x][headXY.y] = 1; // Sets new space to be snake

		headXY = body.front(); // Changes head holder to back of snake in order to draw over space and pop front of queue
		setColor(3);
		draw((headXY.x + 7), (headXY.y + 3), " "); // Draws empty space over old tail of snake
		snaketrix[headXY.x][headXY.y] = 2; // Sets position of old tail in snaketrix to empty
		body.pop_front(); // Pops old tail from queue

		break;
	case 4:
		headXY = body.back(); // Sets current position of head to the body item at back of queue (front of snake)
		headXY.y = headXY.y - 1; // Sets new position of head up one space
		body.push_back(headXY); // Pushes new head position onto queue
		setColor(2); // Green
		draw((headXY.x + 7), (headXY.y + 3), "S"); // Draws new snake head

		if (snaketrix[headXY.x][headXY.y] == 1) { // Checks the space of the new head is not already snake
			return 1; // Returns end game code
		}

		if (snaketrix[headXY.x][headXY.y] == 3) { // Checks if space of the new head contains fruit
			snaketrix[headXY.x][headXY.y] = 1; // Sets fruit space to now contain snake
			return 3; // Returns fruit hit code
		}

		// This code runs when the new space is empty 

		snaketrix[headXY.x][headXY.y] = 1; // Sets new space to be snake

		headXY = body.front(); // Changes head holder to back of snake in order to draw over space and pop front of queue
		setColor(3);
		draw((headXY.x + 7), (headXY.y + 3), " "); // Draws empty space over old tail of snake
		snaketrix[headXY.x][headXY.y] = 2; // Sets position of old tail in snaketrix to empty
		body.pop_front(); // Pops old tail from queue

		break;
	default:
		break;
	}

	headXY = body.back(); // Sets holder to head of snake to check if in bounds of game area

	if ((headXY.x >= 20 or headXY.x < 0) or (headXY.y >= 10 or headXY.y < 0)) { // Checks snake has not hit wall
		return 1; // Returns game over code
	}

	return 0;
}
