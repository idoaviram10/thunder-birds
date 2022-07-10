#include "Block.h"


/*------------------------------ C'tors and D'tors ----------------------------------*/
Block::Block() {}

Block::Block(Point* _points, int _numPoints) {
	body = new Point[_numPoints];
	size = _numPoints;

	for (int i = 0; i < size; i++)
		body[i] = _points[i];
	figure = body[0].getFigure();
}

Block::~Block() {
	delete[] body;
}
/*-----------------------------------------------------------------------------------*/


