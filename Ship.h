#pragma once
#include "GameObject.h"

class Ship : public GameObject {
	int maxCarry;
public:
	/*------------------------------ C'tors and D'tors ----------------------------------*/
	Ship();
	Ship(Point* _points, int _numPoints);
	~Ship();
	/*-----------------------------------------------------------------------------------*/
	/*------------------------------ Get and set functions ------------------------------*/
	int getMaxCarry() const;
	/*-----------------------------------------------------------------------------------*/
};

