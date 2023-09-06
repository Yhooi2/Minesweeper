//minesweeper
#include <iostream>
#include <cstdlib> //rand(), srand()
#include <ctime> // time()
#include <fstream> // ifstream(), ofstream()
#include <thread> // this_thread::sleep_for
#include <chrono> //

const int WIDTH = 9, HEIGHT = 9, MINES = 9,
SIZE = 100;

int field[SIZE][SIZE];
int fieldFerst[SIZE][SIZE];
int counter; // counter steeps
using namespace std;
bool isBot = false;

// Print game field
void printField() {
	system("cls");

	// Print header with coordinates
	cout << "STEEP: " << counter << endl
		<< "  ";

	for (int i = 0; i < WIDTH; i++) {
		cout << ' ' << i + 1;
		if (i < 9)
			cout << ' ';
	}
	cout << endl;

	// Print field
	for (int i = 0; i < HEIGHT; i++) {
		cout << i + 1 << ' ';
		if (i < 9)
			cout << ' ';
		// Print cell volue or placeholder
		for (int j = 0; j < WIDTH; j++) {
			if (fieldFerst[i][j] == -1)
				cout << "X  ";
			else if (fieldFerst[i][j] == -2)
				cout << "_  ";
			else if (fieldFerst[i][j] == -3)
				cout << "f  ";
			else
				cout << fieldFerst[i][j] << "  ";
		}
		cout << endl;
	}
	return;
}

// Deep First Serch(DFS)
void checkMove(const int& i, const int& j) {

	// Check surrounding cells
	for (int a = i - 1; a < i + 2; a++) {
		for (int b = j - 1; b < j + 2; b++) {

			// Open empty adjacent cells
			if (a >= 0 && a < WIDTH &&
				b >= 0 && b < HEIGHT &&
				field[a][b] == 0 &&
				fieldFerst[a][b] == -2) {

				fieldFerst[a][b] = field[a][b];
				checkMove(a, b);
			}
		}
	}
}

bool openCell(int x, int y) {
	if (field[x][y] == -1)
		return false;
	if (field[x][y] > 0) {
		fieldFerst[x][y] = field[x][y];
		return true;
	}
	checkMove(x, y);
	return true;
}

bool checkWin() {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (fieldFerst[i][j] == -2 && field[i][j] != -1)
				return false;
		}
	}
	return true;
}
bool printEnd(bool fl) {
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (field[i][j] == -1)
				fieldFerst[i][j] = field[i][j];
		}
	}
	printField();
	char end;
	cout << "\nYOU " << (fl ? "WIN!" : "GAME OVER!") << endl << endl;
	cout << "Enter anything to start new game, or q to exit: ";
	cin >> end;
	isBot = false;
	return (end == 'q');
}

void wait(int msc) {

	this_thread::sleep_for(chrono::milliseconds(msc));
	return;
}

void saveField() {

	ofstream fout("field.txt", ios_base::trunc);
	if (!fout.is_open()) {
		cout << "File field.txt not find!";
		exit(0);
	}
	fout << WIDTH << ' ' << HEIGHT << ' ' << MINES << '\n';
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			fout << fieldFerst[i][j] << ' ';
		}
		fout << '\n';
	}
	fout.close();
	return;
}


int main() {

	bool exit = true;
	do {
		counter = 0; // zeroing counter

		// Initialize field
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < HEIGHT; j++) {
				fieldFerst[i][j] = -2;
			}
		}
		printField();

		srand(time(nullptr));// Generate random seed

		// Set random cells as mines
		for (int i = 0; i < MINES; i++) {
			int x, y;
			do {
				x = rand() % WIDTH;
				y = rand() % HEIGHT;
			} while (field[x][y] == -1);

			field[x][y] = -1;
		}

		// Calculate number of adjacent mines
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < HEIGHT; j++) {
				if (field[i][j] == 0) {
					for (int a = i - 1; a < i + 2; a++) {
						for (int b = j - 1; b < j + 2; b++) {
							if (a >= 0 && a <= WIDTH &&
								b >= 0 && b <= HEIGHT &&
								field[a][b] == -1)
								++field[i][j];
						}
					}
				}
			}
		}

		bool newGame = true;
		// Game Loop
		do {
			counter++;
			//code for player's move
			char command;
			int width = 0,
				height = 0;

			do {
				cout << endl << endl << endl << "Enter command (o - open, f - flage, b - bot, n - new, q - exit): ";
				
				ifstream fin("command.txt"); 
				if (isBot) {
					
					saveField();
					system("bot.exe");

					if (!fin.is_open()) {
						cout << "File command.txt not find!";
							return 1;
					}
					fin >> command;
					cout << command << '\n';
					
				}
				else
					cin >> command;

				if (command == 'q') {
					newGame = exit = false;
					return 0;
				}

				if (command == 'n') {
					newGame = false;
					break;
				}
				if (command == 'b') {
				
					isBot = true;
					break;
				}
				cout << "Enter coordinates: ";

				if (isBot) {
					
					fin >> width >> height;
					fin.close();
					cout << width << ' ' << height;
					wait(1500);
				}
				else {
					cin >> width >> height;
				}

				--width; // Corect of coordinates
				--height;

			} while (width >= WIDTH || height >= HEIGHT ||
				width < 0 || height < 0);


			if (command == 'f') {
				if (fieldFerst[width][height] == -2)
					fieldFerst[width][height] = -3;
				else if (fieldFerst[width][height] == -3)
					fieldFerst[width][height] = -2;
				--counter;
			}

			//Game over
			else if (!openCell(width, height)) {
				if (printEnd(openCell(width, height)))
					exit = false;
				newGame = false;
			}

			printField();
			if (checkWin()) {
				if (printEnd(checkWin()))
					exit = false;
				newGame = false;
			}

		} while (newGame);
	} while (exit);
	return 0;
}