#include "Menu.h"

using std::filesystem::directory_iterator;


/*------------------------------ C'tors and D'tors ----------------------------------*/
Menu::~Menu() {
	files.clear();
}
/*-----------------------------------------------------------------------------------*/


void  Menu::menuManager(int gameMode) {
	/* This function manages the menu. */
	game = new Game;
	game->setGameMode(gameMode);

	colorsChoose();
	if(gameMode == load || gameMode == silent)
		game->setColorsMode(colorsChoice);

	while (choice != '9') {
		game->setGameMode(gameMode);
		clrscr();
		if (!isLastGameCompleted && (gameMode == simple || gameMode == save))
			showMenu();

		if (!isLastGameCompleted && (gameMode == load || gameMode == silent))
			if (curFile > 0) {
				if (gameMode == load)
					exitMassage();
				if (gameMode == silent)
					printTestResult();
				break;
			}

		selectChoice(gameMode);

		if (!isLexValid) {
			if (gameMode != silent) {
				clrscr();
				cout << endl << "ERROR: There are no screen files in path!" << endl;
			}
			break;
		}

		if ((gameMode == load || gameMode == silent) && curFile == countFiles && lexOn) {
			if(gameMode == load)
				exitMassage();
			if (gameMode == silent)
				printTestResult();
			break;
		}
		game = new Game;
	}
	delete game;
}

void Menu::colorsChoose() {
	/* This function presents the user a small menu that
	asks him if he wants to play with colors or without colors.
	The user selection will affect the game until the program runs. */

	cout << "(0) Play without colors" << endl << endl;
	cout << "(1) Play with colors" << endl << endl << endl;

	cout << "Enter your choice:   ";
	colorsChoice = _getch();

	while (colorsChoice != '0' && colorsChoice != '1') {
		clrscr();
		cout << endl << " Wrong input! " << endl << endl;
		cout << "(0) Play without colors" << endl;
		cout << "(1) Play with colors" << endl << endl << endl;
		cout << " Please select a valid option from the menu  ";
		colorsChoice = _getch();
	}
}

void Menu::showMenu() const {
	/* This function presents the user the main menu of the game. */

	if (colorsChoice == '1')
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Menu_Color);
	cout << "(1) Start a new game" << endl << endl;
	cout << "(8) Present instructions and keys" << endl << endl;
	cout << "(9) Exit" << endl << endl << endl;

}

void Menu::selectChoice(int gameMode) {
	/* A function that captures the user's selection from the menu displayed on the screen. */

	char ch;
	string fileName;
	ifstream infile;

	if (!isLastGameCompleted) {
		lexOn = false;
		countFiles = 0;
		curFile = 0;
		files.clear();
		if (gameMode == simple || gameMode == save) {
			cout << "Enter your choice:  ";
			choice = _getch();
		}

		if (choice != '1' && choice != '8' && choice != '9' && choice != '\n' && choice != '0')
			wrongInput();

		else if (choice == '1') {
			clrscr();
			if (!lexOn) {
				if (gameMode == simple || gameMode == save) {
					printFilesOrder();
					ch = _getch();
					game->setGameOrder(ch);
					while (game->getGameOrder() != '1' && game->getGameOrder() != '2')
						checkValidOrder();
				}
				if (game->getGameOrder() == '1') {
					specificOrder(fileName, infile);
				}
				else if (game->getGameOrder() == '2') {
					lexOrder();
					lexOn = true;
					if (!isLexValid)
						return;
				}
			}

			if (lexOn) {
				fileName = files[curFile];
				infile.open(fileName);
				curFile++;
			}

			game->setColorsMode(colorsChoice);
			game->initBoardAndGame(infile, fileName);

			if (game->getGameResult() == GameResult::Level_Completed)
				isLastGameCompleted = true;
			else
				isLastGameCompleted = false;

		}

		else if (choice == '8')
			printInstructions();

		else if (choice == '9') {
			exitMassage();
			return;
		}
	}
	else {
		if (lexOn) {
			if (curFile != countFiles) {
				fileName = files[curFile];
				infile.open(fileName);
				curFile++;
				game->setCurrLives(currLives);
				game->setColorsMode(colorsChoice);
				game->setTestResult(testResult);
				game->initBoardAndGame(infile, fileName);
				if (game->getGameResult() == GameResult::Level_Completed)
					isLastGameCompleted = true;
				else
					isLastGameCompleted = false;
			}
			else {
				lexOn = false;
				countFiles = 0;
				curFile = 0;
				files.clear();
			}
		}
		else {
			isLastGameCompleted = false;
		}
	}
	currLives = game->getCurrLives();
	testResult = game->getTestResult();

}

void Menu::wrongInput() const {
	/* As long as the user selection is invalid,
	a relevant massage will be printed and the game menu will be displayed again and again. */

	clrscr();
	cout << " Wrong input! " << endl;
	cout << " Press any key to return to the main menu..." << endl;
	_getch();
}

void Menu::printInstructions() const {
	/* This function prints to the screen the instructions of the game. */

	clrscr();
	cout << " Two ships are trapped inside an ancient Egyptian tomb." << endl;
	cout << " You need to get the two ships out of the tomb in the given time." << endl;
	cout << " Beware of falling blocks!" << endl;
	cout << " You have 3 lives. If you are failed 3 times, the game is over!" << endl << endl;
	cout << "  The keys for playing: " << endl;
	cout << "   UP - W or w" << endl << "   DOWN - X or x" << endl << "   LEFT - A or a" << endl
		<< "   RIGHT - D or d" << endl << "   Switch to the Big Ship - B or b" << endl
		<< "   Switch to the Small Ship - S or s" << endl << endl;
	cout << " Let's start playing!" << endl << endl << endl;
	cout << " Press any key to return to the main menu..." << endl;
	_getch();
}

void Menu::printFilesOrder() {
	/* This function asks the user to select the game order - load a specific board or lexicographic order. */

	clrscr();
	cout << "(1) Upload specific screen\n\n(2) Upload lexicographic order" << endl << endl << endl;
	cout << "Enter your choice:   ";
}

void Menu::checkValidOrder() {
	/* As long as the user selection is invalid,
	a relevant massage will be printed and the game order menu will be displayed again and again. */

	char ch;
	system("cls");
	wrongInput();
	printFilesOrder();
	ch = _getch();
	game->setGameOrder(ch);
}

void Menu::specificOrder(string& fileName, ifstream& infile) {
	/* This function asks the user to write a file name to upload.
	If the file does not exist, a relevant message will appear. */

	clrscr();
	cout << "Please enter name of file:  ";
	cin >> fileName;
	fileName += ".screen";
	infile.open(fileName);
	while (!infile) {
		clrscr();
		cout << "File is not found! Please enter a valid name:  ";
		cin >> fileName;
		fileName += ".screen";
		infile.open(fileName);
	}
}

void Menu::lexOrder() {
	/* This function searches in the project folder all the files that end in the "screen" extension.
	Each such file pushes into the 'files' vector and they will load in lexicographic order
	(only on condition that the user manages to complete each step). */
	vector<string> _files(0);
	string path = ".";

	for (const auto& file : directory_iterator(path)) {
		if (endsWith(file.path().string(), "screen")) {
			_files.push_back(file.path().string());
			countFiles++;
		}
	}
	files = _files;
	if (countFiles == 0) 
		isLexValid = false;
}

bool Menu::endsWith(string fileName, const string ending) {
	/* This function checks if the string 'fileName' ends with given string 'ending' ("screen"). */

	if (fileName.size() >= ending.size() &&
		fileName.compare(fileName.size() - ending.size(), ending.size(), ending) == 0)
		return true;
	else
		return false;
}

void Menu::printTestResult() const {

	gotoxy(35, 8);
	if (testResult == failed) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::GameOver_Color);
		cout << "Test Failed!" << "\n\n\n\n\n\n";
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::LevelCompleted_Color);
		cout << "Test Passed!" << "\n\n\n\n\n\n";
	}
}

void Menu::exitMassage() const {
	/* If the user chooses to exit the game,
	a relevant message will be printed on the screen and the program will end. */

	clrscr();
	gotoxy(35, 8);
	cout << "Bye bye!" << endl;
	gotoxy(31, 9);
	cout << "Hope you enjoyed!" << "\n\n\n\n\n\n";
}