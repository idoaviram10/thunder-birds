#include "GameObject.h"


/*------------------------------ C'tors and D'tors ----------------------------------*/
GameObject::GameObject() {}

GameObject::~GameObject() {}
/*-----------------------------------------------------------------------------------*/


void GameObject::draw(char colorsMode, int gameMode) const {
	/* This function draws the object on the screen according to the coordinates of the object.
	In addition it will update the board with the relevant character. */


	if (colorsMode == Colors::ON)
		setColor();

	for (size_t i = 0; i < size; i++) {
		if (gameMode != silent)
			body[i].drawPoint();
		pBoard->set(body[i].getX(), body[i].getY(), body[i].getFigure());
	}
}

void GameObject::del() const {
	/* This function deletes the object from the screen according to the coordinates of the object.
	In addition it will update the board with empty character. */

	for (size_t i = 0; i < size; i++) {
		body[i].delPoint();
		pBoard->set(body[i].getX(), body[i].getY(), ' ');
	}
}

void GameObject::move(int difX, int difY, char colorsMode, int gameMode) {
	/* This function moves the object on the screen according to the variables difX and difY
	calculated in the changeDir function in the game. */

	if (colorsMode == Colors::ON)
		setColor();

	for (size_t i = 0; i < size; i++) {
		if (gameMode != silent)
			body[i].delPoint();
		pBoard->set(body[i].getX(), body[i].getY(), ' ');
	}

	for (size_t i = 0; i < size; i++)
		body[i].movePoint(difX, difY, gameMode);
}

void GameObject::reInit(Point* _body, int _size) {
	body = new Point[_size];
	size = _size;
	for (int i = 0; i < size; i++)
		body[i] = _body[i];
	figure = _body[0].getFigure();
}


/*------------------------------ Get and set functions ------------------------------*/
void GameObject::setBoard(Board* pb) {
	/* This function sets the current game board to pBoard. */

	pBoard = pb;
}

void GameObject::setColor() const {
	if (figure == BoardSymbols::BIGSHIP || figure == BoardSymbols::SMALLSHIP)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Ships_Color);

	else if (figure == BoardSymbols::GHOST || (figure >= '1' && figure <= '9'))
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Blocks_Ghosts_Color);
}

int GameObject::getSize() const {
	return size;
}

Point* GameObject::getBody() const {
	/* This function return an array of the points of the block. */

	return body;
}

int GameObject::getX(int index) const {
	/* This function returns the x of one of the block's points -
	according to the index that the function receives. */

	return body[index].getX();
}

int GameObject::getY(int index) const {
	/* This function returns the y of one of the block's points -
	according to the index that the function receives. */

	return body[index].getY();
}

char GameObject::getFigure() const {
	return figure;
}
/*-----------------------------------------------------------------------------------*/


