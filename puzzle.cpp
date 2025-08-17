#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// converts a number to its char representation
char convertDigitToChar(int digit) {
	return digit + '0';
}

// an auxiliary function to generate numbers in a range
int generateNumberBetween(int low, int high) {
	return rand() % (high - low + 1) + low;
}

// "Tip" gotten from the PDF
void makeCinTidy() {
	if (cin.fail()) {
		cin.clear();
		cin.ignore(255, '\n');
	}
}

class Settings {
public:
	int lowerBound = 0, upperBound = 5; // PDF DEFAULT VALUES
	int numberOfPieces = 15; // PDF DEFAULT VALUE

	int generateDigit() {
		return generateNumberBetween(lowerBound, upperBound);
	}
};

// a part of runSettings()
void runSettings_changeRangeOfRandomNumbers(Settings& settings) {
	while (true) {
		cout << "Please enter new lower bound value (0-9) (Now is " << settings.lowerBound << "): ";
		int newLowerBound;
		cin >> newLowerBound;
		makeCinTidy();
		if (newLowerBound >= 0 && newLowerBound <= 9) {
			settings.lowerBound = newLowerBound;
			break;
		}
		else {
			cout << "Invalid value! Please try again!" << endl;
		}
	}

	while (true) {
		cout << "Please enter new upper bound value (" << settings.lowerBound << "-9) (Now is " << settings.upperBound << "): ";
		int newUpperBound;
		cin >> newUpperBound;
		makeCinTidy();
		// the upper bound must be greater than or equal to the lower bound, otherwise it makes no sense
		if (newUpperBound >= settings.lowerBound && newUpperBound <= 9) {
			settings.upperBound = newUpperBound;
			break;
		}
		else {
			cout << "Invalid value!" << endl;
		}
	}
}

// a part of runSettings()
void runSettings_changeNumberOfPuzzlePieces(Settings& settings) {
	while (true) {
		cout << "Please enter new number of puzzle pieces value (1-25) (Now is " << settings.numberOfPieces << "): ";
		int newValue;
		cin >> newValue;
		makeCinTidy();
		if (1 <= newValue && newValue <= 25) {
			settings.numberOfPieces = newValue;
			break;
		}
		else {
			cout << "Invalid value!" << endl;
		}
	}
}

// PDF: the whole "Settings" menu logic
void runSettings(Settings &settings) {
	bool running = true;
	while (running) {
		int optionNumMin = 1;
		int optionNumMax = 3;

		cout << "*** Settings Menu ***" << endl;
		cout << "[1] Number of puzzle pieces (" << settings.numberOfPieces << ")" << endl;
		cout << "[2] Range of random numbers (" << settings.lowerBound << "-" << settings.upperBound << ")" << endl;
		cout << "[3] Return to Main Menu" << endl;
		cout << "*********************" << endl;

		int chosenOption;
		while (true) {
			cout << "Options (" << optionNumMin << " - " << optionNumMax << "): ";
			cin >> chosenOption;
			makeCinTidy();
			if (optionNumMin <= chosenOption && chosenOption <= optionNumMax)
				break;
			else
				cout << "Please try again!" << endl;
		}

		switch (chosenOption) {
		case 1:
			runSettings_changeNumberOfPuzzlePieces(settings);
			break;
		case 2:
			runSettings_changeRangeOfRandomNumbers(settings);
			break;
		case 3:
			running = false; // exit the Settings menu
			break;
		}
	}
}

class Piece {
public:
	int up, down, left, right;
	char index;

	void rotateClockwise() {
		int up_ = up;
		up = left;
		left = down;
		down = right;
		right = up_;
	}

	void rotateAntiClockwise() {
		rotateClockwise();
		rotateClockwise();
		rotateClockwise();
	}

	void randomizeDigitsWithSettings(Settings &settings) {
		up = settings.generateDigit();
		down = settings.generateDigit();
		left = settings.generateDigit();
		right = settings.generateDigit();
	}

	void rotateRandomly() {
		int n = rand() % 4;
		for (int i = 0; i < n; i++)
			rotateClockwise();
	}

	// " 1 "
	// "2A3"
	// " 4 "
	void print() {
		cout << " " << up << " " << endl;
		cout << left << index << right << endl;
		cout << " " << down << " " << endl;
	}
};

class Board {
public:
	//  COL>>>>>>>>>>
	//R X  X  X  X  X
	//O X  X  X  X  X
	//W X  X  X  X  X
	//V X  X  X  X  X
	//V X  X  X  X  X

	// note: value 0 means there is no piece
	Piece* grid[5][5]; // grid[row][col]

	Board() {
		for (int row = 0; row < 5; row++) {
			for (int col = 0; col < 5; col++) {
				grid[row][col] = NULL;
			}
		}
	}

	void removePieceWithIndex(char index) {
		for (int row = 0; row < 5; row++) {
			for (int col = 0; col < 5; col++) {
				Piece *piece = grid[row][col];
				// DO. NOT. SWAP. THE. ORDER.
				if (piece != NULL && piece->index == index)
					grid[row][col] = NULL;
			}
		}
	}

	bool hasPieceWithIndex(char index) {
		for (int row = 0; row < 5; row++) {
			for (int col = 0; col < 5; col++) {
				Piece *piece = grid[row][col];
				// DO. NOT. SWAP. THE. ORDER.
				if (piece != NULL && piece->index == index)
					return true;
			}
		}
		return false;
	}

	bool isPositionValid(int row, int col) {
		return row >= 0 && row < 5 && col >= 0 && col < 5;
	}

	Piece* getPieceAtOrNullPtr(int row, int col) {
		return isPositionValid(row, col) ? grid[row][col] : NULL;
	}

	bool canPieceBePlacedAt(Piece &piece, int row, int col) {
		// DO. NOT. SWAP. THE. ORDER.
		if (!(isPositionValid(row, col) && grid[row][col] == NULL))
			return false;
		// left
		Piece *piece_on_left = getPieceAtOrNullPtr(row, col - 1);
		if (piece_on_left != NULL && piece_on_left->right != piece.left)
			return false;
		// right
		Piece *piece_on_right = getPieceAtOrNullPtr(row, col + 1);
		if (piece_on_right != NULL && piece_on_right->left != piece.right)
			return false;
		// up
		Piece *piece_on_up = getPieceAtOrNullPtr(row - 1, col);
		if (piece_on_up != NULL && piece_on_up->down != piece.up)
			return false;
		// down
		Piece *piece_on_down = getPieceAtOrNullPtr(row + 1, col);
		if (piece_on_down != NULL && piece_on_down->up != piece.down)
			return false;
		return true;
	}

	// returns whether if that was successful or not
	bool tryPlacingPieceAt(Piece *piece, int row, int col) {
		// DO. NOT. SWAP. THE. ORDER.
		if (canPieceBePlacedAt(*piece, row, col)) {
			grid[row][col] = piece;
			return true;
		}
		else {
			return false;
		}
	}

	// PDF: print the board according to specification
	void print() {
		cout << "    A  B  C  D  E  " << endl;
		cout << "  +---------------+" << endl;

		for (int row = 0; row < 15; row++) {
			int grid_row = row / 3;

			if (row % 3 == 1) {
				cout << (grid_row + 1) << " ";
			}
			else {
				cout << "  ";
			}

			cout << "|";
			for (int col = 0; col < 15; col++) {
				int grid_col = col / 3;
				Piece* piece = grid[grid_row][grid_col];

				char pixel = ' ';
				if (piece != NULL) {
					if (row % 3 == 0 && col % 3 == 1) {
						pixel = convertDigitToChar(piece->up);
					}
					else if (row % 3 == 1 && col % 3 == 0) {
						pixel = convertDigitToChar(piece->left);
					}
					else if (row % 3 == 1 && col % 3 == 1) {
						pixel = piece->index;
					}
					else if (row % 3 == 1 && col % 3 == 2) {
						pixel = convertDigitToChar(piece->right);
					}
					else if (row % 3 == 2 && col % 3 == 1) {
						pixel = convertDigitToChar(piece->down);
					}
				}
				cout << pixel;
			}
			cout << "|" << endl;

		}

		cout << "  +---------------+" << endl;
	}
};

// an auxiliary function to generate random indices for pieces
char generateRandomIndex() {
	while (true) {
		char index = rand() % 26 + 'A';
		if (index != 'Q')
			return index;
	}
}

// generate a solution sample board given an empty board and returns the pieces used in the solution
Piece* generateSolution(Settings& settings, Board* board) {
	Piece* pieces = new Piece[settings.numberOfPieces];
	for (int i = 0; i < settings.numberOfPieces; i++) {
		Piece* piece = &(pieces[i]);
		char index;
		while (true) {
			index = generateRandomIndex();
			bool ok = true;
			for (int k = 0; k < i; k++) {
				if (pieces[k].index == index) {
					ok = false;
					break;
				}
			}
			if (ok)
				break;
		}
		piece->index = index;

		int row, col;
		do {
			row = rand() % 5;
			col = rand() % 5;
		} while (board->grid[row][col] != NULL);

		while (true) {
			piece->randomizeDigitsWithSettings(settings);
			bool successful = board->tryPlacingPieceAt(piece, row, col);
			if (successful)
				break;
		}
	}
	return pieces;
}

// PDF: the user can enter "2A" and it would be interpreted as a position on the board
bool userInputToGridPosition(int *row, int *col, string input) {
	// input = "4C"
	int row_ = input[0] - '1';
	if (!(row_ >= 0 && row_ < 5)) {
		return false;
	}

	int col_ = input[1] - 'A';
	if (!(col_ >= 0 && col_ < 5)) {
		return false;
	}

	*row = row_;
	*col = col_;

	return true;
}

// Useful feature
void printPuzzleGameBanner() {
	cout << "__________                     .__           ________                       " << endl;
	cout << "\\______   \\__ _________________|  |   ____  /  _____/_____    _____   ____  " << endl;
	cout << " |     ___/  |  \\___   /\\___   /  | _/ __ \\/   \\  ___\\__  \\  /     \\_/ __ \\ " << endl;
	cout << " |    |   |  |  //    /  /    /|  |_\\  ___/\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/ " << endl;
	cout << " |____|   |____//_____ \\/_____ \\____/\\___  >\\______  (____  /__|_|  /\\___  >" << endl;
	cout << "                      \\/      \\/         \\/        \\/     \\/      \\/     \\/ " << endl;
}

// Useful feature
void printCrown() {
	cout << "                  .       |         .    .            " << endl;
	cout << "            .  *         -*-          *               " << endl;
	cout << "                 \\        |         /   .             " << endl;
	cout << ".    .            .      /^\\     .              .    ." << endl;
	cout << "   *    |\\   /\\    /\\  / / \\ \\  /\\    /\\   /|    *    " << endl;
	cout << " .   .  |  \\ \\/ /\\ \\ / /     \\ \\ / /\\ \\/ /  | .     . " << endl;
	cout << "         \\ | _ _\\/_ _ \\_\\_ _ /_/_ _\\/_ _ \\_/          " << endl;
	cout << "           \\  *  *  *   \\ \\/ /  *  *  *  /            " << endl;
	cout << "            ` ~ ~ ~ ~ ~  ~\\/~ ~ ~ ~ ~ ~ '             " << endl;
}

void runGame(Settings& settings) {
	// PDF: generate a sample solution, the board must be playable
	Board solution;
	Piece* piecesOfSolution = generateSolution(settings, &solution);

	Piece* pieces = new Piece[settings.numberOfPieces];
	for (int i = 0; i < settings.numberOfPieces; i++) {
		pieces[i] = piecesOfSolution[i];
		pieces[i].rotateRandomly();
	}

	Board board;

	bool gameFinished = false;
	time_t gameStartTime = time(NULL);
	while (!gameFinished) {
		// Useful feature & PDF: display the unused pieces
		for (int i = 0; i < settings.numberOfPieces; i++) {
			Piece& piece = pieces[i];
			if (!board.hasPieceWithIndex(piece.index)) {
				cout << "---" << endl;
				piece.print();
			}
		}
		cout << "^^^ Unused pieces ^^^" << endl;

		// PDF: print the board or else the user wouldn't know anything
		board.print();

		bool endControl = false;
		while (!endControl) {
			// PDF: user can enter an index of a piece or enter 'Q' to quit the game
			cout << "Select/Pop out a piece with index...: ";
			char userSelectedIndex;
			cin >> userSelectedIndex;
			makeCinTidy();
			if (userSelectedIndex == 'Q') {
				// PDF: quitting the game will result in the user losing and a sample solution would be displayed
				solution.print();
				cout << "^^^ sample solution ^^^ QUITTER!!! YOU LOSE!!!" << endl;
				return;
			}
			else if (userSelectedIndex >= 'A' && userSelectedIndex <= 'Z') { // if input is an index
				Piece *userSelectedPiece = NULL;
				// find the piece with the index
				for (int i = 0; i < settings.numberOfPieces; i++) {
					if (pieces[i].index == userSelectedIndex) {
						userSelectedPiece = &(pieces[i]);
						break;
					}
				}
				if (userSelectedPiece == NULL) { // if can't then error and tell the user
					cout << "No piece with index " << userSelectedIndex << "! Please try again!" << endl;
					continue;
				}

				if (board.hasPieceWithIndex(userSelectedPiece->index)) { // if the piece is on the board
					// PDF: if the user selects a piece that is already on the board, remove it from the board
					board.removePieceWithIndex(userSelectedPiece->index); // remove it
					endControl = true;
				}
				else {
					// start of control
					while (true) {
						userSelectedPiece->print(); // print the selected piece
						cout << "Instructions:" << endl;
						cout << "- '<'/'>' to rotate anti/clockwise" << endl;
						cout << "- 'q' to put it back" << endl;
						cout << "- 'p' to print the board again" << endl;
						cout << "- '2A' to place it at 2A... etc" << endl;
						cout << "What do you want to do with it? ";
						string user_input;
						cin >> user_input;
						makeCinTidy();
						if (user_input == "<") { // PDF: the user can rotate clockwise
							userSelectedPiece->rotateAntiClockwise();
						}
						else if (user_input == ">") { // PDF: the user can rotate anticlockwise
							userSelectedPiece->rotateClockwise();
						}
						else if (user_input == "q") { // PDF: the user can deselect
							endControl = true;
							break;
						}
						else if (user_input == "p") { // Useful feature: print the board again
							board.print();
						}
						else if (user_input.length() == 2) { // if length == 2, then it is like "2A" or "3B"
						 // PDF: the user can enter "2A" and it would be interpreted as a position on the board
							int row, col;
							if (userInputToGridPosition(&row, &col, user_input)) {
								bool successful = board.tryPlacingPieceAt(userSelectedPiece, row, col);
								if (successful) {
									endControl = true;
									break;
								}
								else {
									cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
									cout << "INVALID SPOT! PLEASE CHOOSE ANOTHER SPOT!" << endl;
									cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
								}
							}
							else {
								cout << "Invalid coordinate! Please try again!" << endl;
							}
						}
						else {
							cout << "Unknown input! Please try again!" << endl;
						}
					}
					// end of control
					// check if all pieces are used up
					bool finished = true;
					for (int i = 0; i < settings.numberOfPieces; i++) {
						if (!board.hasPieceWithIndex(pieces[i].index)) {
							finished = false;
							break;
						}
					}
					if (finished) {
						endControl = true;
						gameFinished = true;
						break;
					}
				}
			}
			else {
				cout << "Unknown input! Please try again!" << endl;
			}
		}
	}
	// Useful feature: calculate the time took by the user
	time_t gameEndTime = time(NULL);
	time_t timeUsedToPlay = gameEndTime - gameStartTime;
	int minutes = timeUsedToPlay / 60;
	int seconds = timeUsedToPlay % 60;

	// PDF: the user can win the game
	printCrown();
	cout << "GG!!!!!!!!!! You won!!!!!!!" << endl;
	cout << "It took you " << minutes << "m" << seconds << "s! CAN YOU DO BETTER???" << endl;

	// cleanup
	delete[] pieces;
	delete piecesOfSolution;
}

void printUsefulFeatures() {
	// PDF: print out useful features
	cout << "USEFUL FEATURES:" << endl;
	cout << "- print unused pieces above the board" << endl;
	cout << "- print banner when game starts" << endl;
	cout << "- print crown when user wins" << endl;
	cout << "- show values of settings immediately when in the 'Settings' menu" << endl;
	cout << "- game timer" << endl;
	cout << "- when a piece is selected, user can enter 'p' to print the board again" << endl;
	cout << "- helpful instructions are given when the user has selected a piece" << endl;
}

void printCredits() {
	// PDF: print out credits
	cout << "SEHH2042-202-Group 5" << endl;
	cout << left;
	cout << setw(20) << "Student Name" << setw(20) << "Student Number" << setw(20)
		<< "Tutorial Group" << endl;
	cout << setw(20) << "Wong Wing Yi" << setw(20) << "21034305A" << setw(20)
		<< "202C" << endl;
	cout << setw(20) << "Kwong Kwok Hei" << setw(20) << "21001114A" << setw(20)
		<< "202B" << endl;
	cout << setw(20) << "Tai Tsz Fai" << setw(20) << "21071675A" << setw(20)
		<< "202A" << endl;
	cout << setw(20) << "Ngai Yi Shun" << setw(20) << "21127675A" << setw(20)
		<< "202B" << endl;
	cout << setw(20) << "Cheng Yan Long" << setw(20) << "21092996A" << setw(20)
		<< "202C"
		<< "\n\n";
}

bool askQuit() {
	// PDF: when quitting the program, the program must ask for a confirmation
	while (true) {
		cout << "DO YOU WANT TO QUIT ('Y' or 'y' for yes; 'N' or 'n' for no): ";
		char c; // YyNn
		cin >> c;
		makeCinTidy();
		switch (c) {
		case 'Y': case 'y': return true;
		case 'N': case 'n': return false;
		default:
			cout << "Please try again!" << endl;
		}
	}
}

void run() {
	// fixed seed: srand(9999);
	srand(time(NULL));

	Settings settings; // default constructor sets values to default as specified by the PDF

	bool running = true;
	while (running) {
		int optionNumMin = 1;
		int optionNumMax = 5;
		// PDF: print out the Main Menu
		cout << "*** Main Menu ***" << endl;
		cout << "[1] Start Game" << endl;
		cout << "[2] Settings" << endl;
		cout << "[3] Useful feature(s) added" << endl;
		cout << "[4] Credits" << endl;
		cout << "[5] Exit" << endl;
		cout << "****************" << endl;
		int chosenOption;
		while (true) {
			cout << "Options (" << optionNumMin << " - " << optionNumMax << "): ";
			cin >> chosenOption;
			makeCinTidy();
			if (optionNumMin <= chosenOption && chosenOption <= optionNumMax)
				break;
			else
				cout << "Please try again!" << endl;
		}

		switch (chosenOption) {
		case 1:
			printPuzzleGameBanner();
			runGame(settings);
			break;
		case 2:
			runSettings(settings);
			break;
		case 3:
			printUsefulFeatures();
			break;
		case 4:
			printCredits();
			break;
		case 5:
			bool quit = askQuit();
			if (quit)
				running = false;
			break; // could just return 0 here but it would look really out of place
		}
	}
}

int main() {
	run();
	return 0;
}