#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <conio.h> 
#include <Windows.h>


//TS == TILESTATE
enum TS { BOMB = -1, FLAG = -2, EMPTYCLICKED = -3, QUESTION = -4, INVALIDACCESS = -11};

//GS == GAMESTATE
enum GS { RUNNING = 1, WIN = 2, LOST = 3};



COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}


void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}


void moveCursor() {

	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);

	char Keys;
	COORD pos = GetConsoleCursorPosition(h);

	int poz_x = pos.X;
	int poz_y = pos.Y;
	Keys = _getch();
	if (Keys == 'd') {
		poz_x += 1;
		gotoxy(poz_x, poz_y);
	}

	if (Keys == 'w') {
		if (poz_y > 1) {
			poz_y -= 1;
			gotoxy(poz_x, poz_y);
		}
	}

	if (Keys == 's') {
		poz_y += 1;
		gotoxy(poz_x, poz_y);
	}

	if (Keys == 'a') {
		if (poz_x > 2) {
			poz_x -= 1;
			gotoxy(poz_x, poz_y);
		}
	}

}

class Game {

public:

	Game(int _size = 8, int _bombCount = 10) {

		bombCount = _bombCount;
		size = _size;
		gameState = RUNNING;

		field = new int* [size];
		for (int i = 0; i < size; ++i)
			field[i] = new int[size];

		playerField = new int* [size];
		for (int i = 0; i < size; ++i)
			playerField[i] = new int[size];

		h = GetStdHandle(STD_OUTPUT_HANDLE);

		initArrays();
		placeBombs();
		runGamePlayLoop();

	}


	void runGamePlayLoop() {
		while (gameState == RUNNING) {
			//system("cls");
			displayField(false);
			uiInteraction();
			moveCursor();
			checkForDone();

		}

		if (gameState == WIN) {
			std::cout << "You Won" << std::endl;
		}
		else {
			std::cout << "You Lost" << std::endl;
		}
	}

	void checkForDone() {
		bool done = true;
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				if (getPosition(x, y) != BOMB && getPosition(x, y, false) == FLAG) {
					done = false;
				}
				if (getPosition(x, y) == BOMB && getPosition(x, y, false) != FLAG) {
					done = false;
				}
			}
		}
		if (done) {
			gameState = WIN;
			system("cls");
			displayField();
		}
	}

	void uiInteraction() {
		int x, y = 0;
		bool useFLag = false;
		bool useQuestion = false;
		std::string extra = "";


		x = getInInput("Input X: ");
		y = getInInput("Input Y: ");
	

		std::cout << "Use [Flag F, Question ?]: ";
		getline(std::cin, extra);

		if (extra == "F" || extra == "f") {
			playerInput(x, y, true, false);
		}
		else if (extra == "?")
		{
			playerInput(x, y, false, true);
		}
		else {
			playerInput(x, y);
		}
	}


	int getInInput(std::string msg) {
		std::string in;
		int result;

		while (true)
		{
			std::cout << msg;
			std::getline(std::cin, in);
			std::stringstream s(in);
			if (s >> result && result >= 0 && result < size)
				break;
			std::cout << "Invalid number, please try again\n";
		}
		return result;
	}

	void displayField(bool useField = true) {

		std::cout << "    ";
		
		for (int x = 0; x < size; x++)
		{
			if (size > 10) {
				if (x < 10) {
					std::cout << " " << x << " ";
				}
				else {
					std::cout << " "<< x;
				}
			}
			else {
				std::cout << " " << x << " ";
			}
		}
		std::cout << std::endl;

		std::cout << "   ";
		for (int x = 0; x < size; x++)
		{
			std::cout << "---";
		}
		std::cout << std::endl;
		for (int y = 0; y < size; y++)
		{
			if (y < 10) {
				std::cout << " " <<  y << " : ";
			}
			else {
				std::cout << y << " : ";
			}
		
			for (int x = 0; x < size; x++)
			{

				int value = getPosition(x, y, useField);

				if (value == EMPTYCLICKED) {
					std::cout << "   ";
				}
				else if (value == BOMB) {
					SetConsoleTextAttribute(h, 4);
					std::cout << " B ";
				}
				else if (value == FLAG) {
					SetConsoleTextAttribute(h, 4);
					std::cout << " F ";
				}
				else if (value == QUESTION) {
					SetConsoleTextAttribute(h, 13);
					std::cout << " ? ";
				}
				else if (value == 0) {
					SetConsoleTextAttribute(h, 7);
					std::cout << " " << 0 << " ";
				}
				else if(value == 1){
					SetConsoleTextAttribute(h, 3);
					std::cout << " " << 1 << " ";
				}
				else if (value == 2) {
					SetConsoleTextAttribute(h, 5);
					std::cout << " " << 2 << " ";
				}
				else if (value == 3) {
					SetConsoleTextAttribute(h, 10);
					std::cout << " " << 3 << " ";
				}
				else if (value == 4) {
					SetConsoleTextAttribute(h, 15);
					std::cout << " " << 4 << " ";
				}
				else if (value == 5) {
					SetConsoleTextAttribute(h, 11);
					std::cout << " " << 5 << " ";
				}
				else if (value == 6) {
					SetConsoleTextAttribute(h, 3);
					std::cout << " " << 6 << " ";
				}
				else if (value == 7) {
					SetConsoleTextAttribute(h, 3);
					std::cout << " " << 7 << " ";
				}
				else if (value == 8) {
					SetConsoleTextAttribute(h, 3);
					std::cout << " " << 8 << " ";
				}
				SetConsoleTextAttribute(h, 7);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;

	}



	void playerInput(int xPos, int yPos, bool useFlag = false, bool useQuestion = false) {


		int localValue = getPosition(xPos, yPos);

		if (useFlag == true) {
			if (getPosition(xPos, yPos, false) == FLAG) {
				setPosition(xPos, yPos, 0, false);
			}
			else {
				setPosition(xPos, yPos, FLAG, false);
			}
			return;
		}

		if (useQuestion == true) {
			if (getPosition(xPos, yPos, false) == QUESTION) {
				setPosition(xPos, yPos, 0, false);
			}
			else {
				setPosition(xPos, yPos, QUESTION, false);
			}
			return;
		}

		if (localValue == BOMB) {
			gameState = LOST;
			system("cls");
			displayField(false);
			displayField();
			return;
		}



		if (localValue != 0) {
			setPosition(xPos, yPos, localValue, false);
			return;
		}
		else {
			setPosition(xPos, yPos, EMPTYCLICKED, false);
			for (int y = -1; y <= 1; y++)
			{
				for (int x = -1; x <= 1; x++) {
					if (getPosition(x + xPos, y + yPos, false) == 0) {
						playerInput(x + xPos, y + yPos);
					}
				}
			}


			return;
		}


	}


private:
	int size;
	int bombCount;
	int gameState;

	int** field;
	int** playerField;
	HANDLE h;

	int getPosition(int x, int y, bool useField = true) {
		if (x < 0 || x >= size || y < 0 || y >= size) {
			return INVALIDACCESS;
		}
		if (useField) {
			return field[x][y];
		}
		return playerField[x][y];
	}

	void setPosition(int x, int y, int value, bool useField = true) {
		if (x < 0 || x >= size || y < 0 || y >= size) {
			return;
		}
		if (useField) {
			field[x][y] = value;
		}
		else {
			playerField[x][y] = value;
		}
	}

	void initArrays() {
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				setPosition(x, y, 0);
				setPosition(x, y, 0, false);
			}
		}
	}

	void placeBombs() {
		for (int i = 0; i < bombCount; i++)
		{
			int x, y = 0;
			do {
				x = rand() % (size);
				y = rand() % (size);
			} while (getPosition(x, y) != 0);
			setPosition(x, y, -1);
		}

		updateField();
	}

	void updateField() {
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				if (getPosition(x, y) >= 0) {
					setPosition(x, y, getBombCount(x, y));
				}
			}
		}
	}

	int getBombCount(int xPos, int yPos) {
		int bombCount = 0;
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++) {
				 if(getPosition(x+xPos, y+yPos) == BOMB)
					bombCount++;
				
			}
		}
		return bombCount;
	}


};


int getInInput(std::string msg) {
	std::string in;
	int result;

	while (true)
	{
		std::cout << msg;
		std::getline(std::cin, in);
		std::stringstream s(in);
		if (s >> result)
			break;
		std::cout << "Invalid number, please try again\n";
	}
	return result;
}






int main() {

	srand(time(NULL));
	int wahl = 0;
		


	std::cout << "Welcome to Minesweeper made by Richard Linde" << std::endl;
	int size = getInInput("Field size:");
	int bombs = getInInput("Amount of bombs:");

	do {
		Game myGame(size, bombs);

		std::cout << "Play again [1], Exit [2] " << std::endl;
		std::cin >> wahl;
	} while (wahl == 1);
	
	return 0;
}