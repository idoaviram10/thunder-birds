#include "Ship.h"


/*------------------------------ C'tors and D'tors ----------------------------------*/
Ship::Ship() {}

Ship::Ship(Point* _points, int _numPoints) {
	body = new Point[_numPoints];
	size = _numPoints;

	for (int i = 0; i < size; i++)
		body[i] = _points[i];
	figure = body[0].getFigure();

	if (figure == ShipConstants::Big_Ship_Figure)
		maxCarry = ShipConstants::Big_Ship_Max_Carry;
	else
		maxCarry = ShipConstants::Small_Ship_Max_Carry;
}

Ship::~Ship() {
	delete[] body;
}
/*-----------------------------------------------------------------------------------*/


/*------------------------------ Get and set functions ------------------------------*/
int Ship::getMaxCarry() const {
	/* This function returns the maximum size of block each ship can carry. */

	return maxCarry;
}
/*-----------------------------------------------------------------------------------*/

