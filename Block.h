#pragma once
#include "GameObject.h"


class Block : public GameObject {
public:
	/*------------------------------ C'tors and D'tors ----------------------------------*/
	Block();
	Block(Point* _points, int _numPoints);
	~Block();
	/*-----------------------------------------------------------------------------------*/
};