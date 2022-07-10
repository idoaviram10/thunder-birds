#pragma once
#include "GameObject.h"


class Ghost : public GameObject {
	int x = -1, y = -1;
	int lastDir = 1;
	int type = -1;
	int randX = 0, randY = 0;
public:
	/*------------------------------ C'tors and D'tors ----------------------------------*/
	Ghost();
	Ghost(Point* _points, int _numPoints, int _type);
	~Ghost();
	/*-----------------------------------------------------------------------------------*/
	/*------------------------ Moving functions unique to ghosts ------------------------*/
	virtual void move(bool* shipShouldDie, char* whatShip, char colorsMode, int gameMode, ofstream& outfile_steps, ifstream& infile_steps);
	void moveHorizontal(bool* shipShouldDie, char* whatShip, char colorsMode, int gameMode);
	void moveVertical(bool* shipShouldDie, char* whatShip, char colorsMode, int gameMode);
	void moveWandering(bool* shipShouldDie, char* whatShip, char colorsMode, int gameMode, ofstream& outfile_steps, ifstream& infile_steps);
	bool isNextMoveValid(int difX, int difY, bool* shipShouldDie, char* whatShip) const;
	/*-----------------------------------------------------------------------------------*/
	/*--------------------------- Load and Save modes functions -------------------------*/
	void writeToFile(ofstream& outfile_steps);
	void readFromFile(ifstream& infile_steps);
	/*-----------------------------------------------------------------------------------*/
	/*------------------------------ Get and set functions ------------------------------*/
	int getType() const;
	/*-----------------------------------------------------------------------------------*/
};

