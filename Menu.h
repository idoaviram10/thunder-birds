#pragma once
#include "Utils.h"
#include "Game.h"
#include "Board.h"


#define ON 1
#define OFF 0

class Menu
{
private:
	Game* game;
	char choice = '1';
	char colorsChoice = '0';
	bool lexOn = false;
	vector<string> files;
	int countFiles = 0;
	int curFile = 0;
	bool isLastGameCompleted = false;
	bool isLexValid = true;
	int currLives = 3;
	int testResult = passed;
public:
	/*------------------------------ C'tors and D'tors ----------------------------------*/
	~Menu();
	/*-----------------------------------------------------------------------------------*/
	void menuManager(int gameMode);
	void colorsChoose();
	void showMenu() const;
	void selectChoice(int gameMode);
	void wrongInput()const;
	void printInstructions() const;
	void printFilesOrder();
	void checkValidOrder();
	void specificOrder(string& fileName, ifstream& infile);
	void lexOrder();
	bool endsWith(string mainStr, string toMatch);
	void printTestResult() const;
	void exitMassage() const;
};
