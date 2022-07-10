#include "Point.h"


void Point::drawPoint() const {
	/* This function draws a certain character on the screen according to the relevant x and y. */

	gotoxy(x, y);
	cout << figure;
}

void Point::delPoint() const {
	/* This function deletes a certain character from the screen according to the relevant x and y. */

	gotoxy(x, y);
	cout << ' ';
}

void Point::movePoint(int difX, int difY, int gameMode) {
	/* This function moves a point on the screen according to the variables difX and difY
	calculated in the changeDir function in the game. */

	int newX = x + difX;
	int newY = y + difY;

	x = newX;
	y = newY;

	if(gameMode != silent)
		drawPoint();
	pBoard->set(x, y, figure);
}

int Point::getX() const {
	/* This function returns the x of of the point. */

	return x;
}

int Point::getY() const {
	/* This function returns the y of of the point. */

	return y;
}

char Point::getFigure() const {
	/* This function returns the figure of of the point. */

	return figure;
}

