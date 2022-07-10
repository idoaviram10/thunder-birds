#include "Ghost.h"


/*------------------------------ C'tors and D'tors ----------------------------------*/
Ghost::Ghost() {}

Ghost::Ghost(Point* _points, int _numPoints, int _type) {
	body = new Point[_numPoints];
	size = _numPoints;
	type = _type;

	for (int i = 0; i < size; i++)
		body[i] = _points[i];
	figure = body[0].getFigure();
}

Ghost::~Ghost() {
	delete[] body;
}
/*-----------------------------------------------------------------------------------*/


/*------------------------ Moving functions unique to ghosts ------------------------*/
void Ghost::move(bool* shipShouldDie, char* whatShip, char colorsMode, int gameMode, ofstream& outfile_steps, ifstream& infile_steps) {
	/* This function moves the ghost on the screen according to the type. */

	switch (type) {
	case GhostConstants::Horizontal:
		moveHorizontal(shipShouldDie, whatShip, colorsMode, gameMode);
		break;
	case GhostConstants::Vertical:
		moveVertical(shipShouldDie, whatShip, colorsMode, gameMode);
		break;
	case GhostConstants::Wandering:
		moveWandering(shipShouldDie, whatShip, colorsMode, gameMode, outfile_steps, infile_steps);
		break;
	}

}

void Ghost::moveHorizontal(bool* shipShouldDie, char* whatShip, char colorsMode, int gameMode) {
	if (isNextMoveValid(1, 0, shipShouldDie, whatShip) && lastDir == 1) {
		GameObject::move(1, 0, colorsMode, gameMode);
		lastDir = 1;
	}
	else if (isNextMoveValid(-1, 0, shipShouldDie, whatShip)) {
		GameObject::move(-1, 0, colorsMode, gameMode);
		lastDir = -1;
	}
	else
		lastDir = 1;
}

void Ghost::moveVertical(bool* shipShouldDie, char* whatShip, char colorsMode, int gameMode) {
	if (isNextMoveValid(0, 1, shipShouldDie, whatShip) && lastDir == 1) {
		GameObject::move(0, 1, colorsMode, gameMode);
		lastDir = 1;
	}
	else if (isNextMoveValid(0, -1, shipShouldDie, whatShip)) {
		GameObject::move(0, -1, colorsMode, gameMode);
		lastDir = -1;
	}
	else
		lastDir = 1;
}

void Ghost::moveWandering(bool* shipShouldDie, char* whatShip, char colorsMode, int gameMode, ofstream& outfile_steps, ifstream& infile_steps) {
	srand(time(NULL));
	char tmpCh;

	if (gameMode == save || gameMode == simple) {
		if (rand() % 2 == 0) {
			if (gameMode == save)
				outfile_steps << '%'; /* rand() % 2 = 0 */
			randX = (rand() % 3) - 1;
			randY = (rand() % 3) - 1;
			while (!isNextMoveValid(randX, randY, shipShouldDie, whatShip)) {
				randX = (rand() % 3) - 1;
				randY = (rand() % 3) - 1;
			}
			if (gameMode == save)
				writeToFile(outfile_steps);
		}
		else if (isNextMoveValid(randX, randY, shipShouldDie, whatShip)) {
			if (gameMode == save)
				outfile_steps << '^'; /* rand() % 2 != 0 */
			GameObject::move(randX, randY, colorsMode, gameMode);
		}
		else if (gameMode == save)
			outfile_steps << '^'; /* rand() % 2 != 0 */
	}

	else if (gameMode == load || gameMode == silent) {
		infile_steps >> tmpCh; /* Check if(rand() % 2 == 0)  */
		if (tmpCh == '%') /* Wandering ghost changed direction in this moment. */
			readFromFile(infile_steps);
		else if (isNextMoveValid(randX, randY, shipShouldDie, whatShip)) {
			GameObject::move(randX, randY, colorsMode, gameMode);
		}
	}
}

bool Ghost::isNextMoveValid(int difX, int difY, bool* shipShouldDie, char* whatShip) const {
	/* This function checks whether the next step of the ghost is valid or not.
	If the step is valid - the function returns true. Otherwise - false. */

	int nextX = getX(0) + difX;
	int nextY = getY(0) + difY;
	char nextCh = pBoard->get(nextX, nextY);
	if (nextCh != BoardSymbols::EMPTY) {
		if (nextCh == BoardSymbols::BIGSHIP || nextCh == BoardSymbols::SMALLSHIP) {
			*shipShouldDie = true;
			*whatShip = nextCh;
		}
		return false;
	}

	return true;
}
/*-----------------------------------------------------------------------------------*/


/*--------------------------- Load and Save modes functions -------------------------*/
void Ghost::writeToFile(ofstream& outfile_steps) {

	/* Right after the sign 'G', we enter the randX as a string and then the randY as a string. */
	char tmp;

	if (randX == -1)
		outfile_steps << '-'; /* A minus symbol. */
	tmp = abs(randX) + '0';
	outfile_steps << tmp;

	if (randY == -1)
		outfile_steps << '-'; /* A minus symbol. */
	tmp = abs(randY) + '0';
	outfile_steps << tmp;


}

void Ghost::readFromFile(ifstream& infile_steps) {
	char tmpCh;

	/* ------------------- Get randX from infile_steps ------------------- */
	infile_steps >> tmpCh;
	if (tmpCh == '-') { /* Check if minus. */
		infile_steps >> tmpCh; /* Get the number (Of course this is '1'). */
		randX = -1;
	}
	else  /* randX should be 0 or 1. */
		randX = tmpCh - '0';
	/* ------------------------------------------------------------------- */


	/* ------------------- Get randY from infile_steps ------------------- */
	infile_steps >> tmpCh;
	if (tmpCh == '-') { /* Check if minus. */
		infile_steps >> tmpCh; /* Get the number (Of course this is '1'). */
		randY = -1;
	}
	else  /* randY should be 0 or 1. */
		randY = tmpCh - '0';
	/* ------------------------------------------------------------------- */

}
/*-----------------------------------------------------------------------------------*/


/*------------------------------ Get and set functions ------------------------------*/
int Ghost::getType() const {
	return type;
}
/*-----------------------------------------------------------------------------------*/
