//minesweeper
#include <iostream>
#include <cstdlib> //rand(), srand()
#include <ctime> // time()
#include <fstream> // ifstream(), ofstream()
#include <thread> // this_thread::sleep_for
#include <chrono> //

//CONSTANTS
const int WIDTH = 9, HEIGHT = 9, MINES = 10;

//GAME FIELD
int field[WIDTH][HEIGHT];
int fieldOpen[WIDTH][HEIGHT];
int counter; // counter steeps

bool isBot = false;

// Print game field
void printField() {
	system("cls");

	// Print header with coordinates
	std::cout << "STEEP: " << counter << std::endl
		<< "  ";

	for (int i = 0; i < WIDTH; i++) {
		std::cout << ' ' << i + 1;
		if (i < 9)
			std::cout << ' ';
	}
	std::cout << std::endl;

	// Print field
	for (int i = 0; i < HEIGHT; i++) {
		std::cout << i + 1 << ' ';
		if (i < 9)
			std::cout << ' ';
		// Print cell volue or placeholder
		for (int j = 0; j < WIDTH; j++) {
			if (fieldOpen[i][j] == -1)
				std::cout << "X  ";
			else if (fieldOpen[i][j] == -2)
				std:: cout << "_  ";
			else if (fieldOpen[i][j] == -3)
				std::cout << "f  ";
			else
				std::cout << fieldOpen[i][j] << "  ";
		}
		std::cout << std::endl;
	}
	return;
}


//Generate random indexes for mines
void generateMine() {

	srand(time(nullptr));// random seed

	int x, y;

	for (int i = 0; i < MINES; i++) {

		// General random x, y indexes
		do {
			x = rand() % WIDTH;
			y = rand() % HEIGHT;
		} while (field[x][y] == -1); // if already mine

		// Set mine on field
		field[x][y] = -1;
	}
}

// Checks bounds
bool isBound(int x, int y) {
	return (x >= 0 && x < WIDTH &&
		    y >= 0 && y < HEIGHT);
}

// Check adjacent cells for mines, update count
void checkMove(int cellX, const int cellY) {

	// Check surrounding 3x3 
	for (int x = cellX-1; x <= cellX+1; x++) {
		for (int y = cellY-1; y <= cellY+1; y++) {

			// Skip invalid index
			if (!isBound(x, y)) {
				continue;
			}

			// Check if adjacent cell is mine, 
			// increase  count
				if (field[x][y] == -1) {
				field[cellX][cellY]++;

			}
		}
	}
	return;
}

// Open empty adjacent cells recursively (DFS)
void openCell(const int& x, const int& y) {

	checkMove(x, y);

	fieldOpen[x][y] = field[x][y]; // opened

	if (field[x][y] != 0) {
		return;
	}
		// Check surrounding cells
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {

			// Check bounds
			if (!isBound(i, j)) {

				if (fieldOpen[i][j] == -2) {
					// Open adjacent empty cell
					openCell(i, j);
				}
			}
		}
	}
	return;
}

// Check win game
bool checkWin() {

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {

			// If closed cell is not mine, game not win
			if (fieldOpen[i][j] == -2 && field[i][j] != -1) {
				return false;
			}
		}
	}
	return true;

}


bool printEnd(bool fl) {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (field[i][j] == -1)
				fieldOpen[i][j] = field[i][j];
		}
	}
	printField();
	char end;
	std::cout << "\nYOU " << (fl ? "WIN!" : "GAME OVER!") << std::endl << std::endl;
	std::cout << "Enter anything to start new game, or q to exit: ";
	std::cin >> end;
	isBot = false;
	return (end == 'q');
}

void wait(const int& msc) {

	std::this_thread::sleep_for(std::chrono::milliseconds(msc));
	return;
}

void saveField() {

	std::ofstream fout("field.txt", std::ios_base::trunc);
	if (!fout.is_open()) {
		std::cout << "File field.txt not find!";
		exit(0);
	}
	fout << WIDTH << ' ' << HEIGHT << ' ' << MINES << '\n';
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			fout << fieldOpen[i][j] << ' ';
		}
		fout << '\n';
	}
	fout.close();
	return;
}

// GAME LOOP
int main() {

	bool gameOver = true;
	do {
		int counter = 0; // zeroing counter

		// Initialize field
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < HEIGHT; j++) {
				fieldOpen[i][j] = -2;
			}
		}
		
		//Generate mines

		generateMine();

		// Game round Loop
		bool newGame = true;

		do {

			// Code for player's move			
			char command;
			int x = 0, y = 0;
			
			printField();
			// Input command, and chack correct 
			do {
				std::cout << std::endl << std::endl << std::endl;
				std::cout << "Enter command (o - open, f - flage, b - bot, n - new, q - exit):\n";

				// Start bot if command b
				std::ifstream fin;	
				if (isBot) {
					saveField();
					system("bot.exe");
					fin.open("command.txt");

					// try open again
					if (!fin.is_open()) {
						int attemp = 5; // number of attemps
						int latensy = 1500;

						do {
							std::cout << attemp << " Try open command file\n";
							wait(latensy);
							fin.open("command.txt");

						} while(!fin.is_open() && attemp--);

						if (attemp == -1)
							throw("File command.txt not find!");
					}

					fin >> command;
					std::cout << command << '\n';
					
				}

				else {
					std::cin >> command;
					}

				if (command == 'q') {
					newGame = gameOver = false;
					return 0;
				}

				if (command == 'n') {
					newGame = false;
					break;
				}

				if (command == 'b') {
					// Enable bot
					isBot = true;
					break;
				}

				std::cout << "Enter coordinates: ";

				if (isBot) {
					fin >> x >> y;
					fin.close();
					std::cout << x << ' ' << y;
					wait(1500);
				}
				else {
					std::cin >> x >> y;
				}

				--x; // Corect of coordinates
				--y;

			} while(x >= WIDTH || y >= HEIGHT ||
					x < 0 || y < 0);

			if(command == 'f') {
				if (fieldOpen[x][y] == -2) {
					fieldOpen[x][y] = -3;
				}
				else if (fieldOpen[x][y] == -3)
					fieldOpen[x][y] = -2;
			}

			// Open cell
			else if (field[x][y] == -1) {
				// Game over
				printEnd(gameOver = false);
			}
			else {
				openCell(x, y);
				counter++;

				//Check if player win
				if (checkWin()) {
					if (printEnd(checkWin()))
						gameOver = false;
				}
			}
			

		} while (newGame && gameOver);


	} while (gameOver);

	return 0;
}