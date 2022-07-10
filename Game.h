#pragma once
#include "Board.h"
#include "Block.h"
#include "Ship.h"
#include "Ghost.h"


class Game
{
public:
	constexpr static int SleepTime = 50;
	constexpr static int LoopForTimer = 1000;
private:
	Board* pBoard;
	Ship* bigShip;
	Ship* smallShip;
	vector<Ghost*> ghosts; //horizontalGhosts, verticalGhosts, wanderingGhosts;
	vector<Block*> blocks;
	char activeShip = ShipConstants::Big_Ship;
	char prevActive = ShipConstants::Big_Ship;
	char key = 0;
	int dirX = 0;
	int dirY = 0;
	int time = 30000;
	int lives = 3;
	bool isBigShipAlive = true;
	bool isSmallShipAlive = true;
	bool isBigShipFinished = false;
	bool isSmallShipFinished = false;
	bool isLevelCompleted = false;
	char colorsMode = Colors::OFF;
	char gameOrder = '2'; //Lexi or specific
	int gameMode = simple;
	ofstream outfile_steps;
	ofstream outfile_result;
	ifstream infile_steps;
	ifstream infile_result;
	int silentTestResult = passed;
public:
	/*------------------------------ C'tors and D'tors ----------------------------------*/
	Game();
	~Game();
	/*-----------------------------------------------------------------------------------*/
	/*------------------------------ Gameplay functions ---------------------------------*/
	void initBoardAndGame(ifstream& infile, string& fileName);
	void setGameObjects();
	void run();
	bool isValidKey(char _key);
	void changeDir();
	void moveObjects();
	bool isNextMoveValidForShip(int difX, int difY, char shipFigure) const;
	bool isNextMoveValidForBlock(int difX, int difY, int indexOfBlock) const;
	bool isTouchAnotherBlock(int nextX, int nextY, int indexOfBlock, int* indexOfNextBlock) const;
	void moveBlocks(int difX, int difY, int* totalSize, int maxCarry, int index);
	bool isBlock(int difX, int difY, char shipFigure, int* indexOfBlock) const;
	bool isGhost(int difX, int difY, char shipFigure) const;
	void moveGhosts();
	void droppingBlocks();
	bool isAbleToFall(Block* currBlock, int indexOfBlock) const;
	void fallingDown(Block* currBlock, int indexOfBlock, bool* shipShouldDie, char* whatShip, char colorsMode);
	void timeOver();
	void shipDied();
	bool isExit(int difX, int difY, char shipFigure) const;
	void shipFinished(char shipFigure, char theOtherShip);
	void pause();
	void removeAllFromScreen() const;
	void reInitGame();
	/*-----------------------------------------------------------------------------------*/
	/*------------------------------ Printing functions ---------------------------------*/
	void printAll();
	void printLives() const;
	void printActiveShip() const;
	void printTimer();
	void levelCompleted();
	void gameOver();
	/*-----------------------------------------------------------------------------------*/
	/*--------------------------- Load and save mode functions --------------------------*/
	char getStepFromFile();
	void getOutFileName(string& fileName);
	void getInFileName(string& fileName);
	void checkFileExist();
	bool isResultsEqual(char c);
	/*-----------------------------------------------------------------------------------*/
	/*------------------------------ Get and set functions ------------------------------*/
	void setGameMode(int _gameMode);
	int getGameMode();
	void setColorsMode(char colorChoice);
	void setGameOrder(char order);
	char getGameOrder() const;
	int getGameResult() const;
	int getCurrLives() const;
	void setCurrLives(int _lives);
	void setTestResult(int _silentTestResult);
	int getTestResult() const;
	int getSizeOfShip(char shipFigure) const;
	/*-----------------------------------------------------------------------------------*/
};