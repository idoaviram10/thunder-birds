#pragma once
#include "Utils.h"
#include "Point.h"
#include "Board.h"

class GameObject {
protected:
	Board* pBoard;
	Point* body;
	char figure;
	int size;
public:
	/*------------------------------ C'tors and D'tors ----------------------------------*/
	GameObject();
	~GameObject();
	/*-----------------------------------------------------------------------------------*/
	void draw(char colorsMode, int gameMode) const;
	void del() const;
	virtual void move(int difX, int difY, char colorsMode, int gameMode);
	void reInit(Point* _body, int _size);
	/*------------------------------ Get and set functions ------------------------------*/
	void setBoard(Board* pb);
	int getSize() const;
	Point* getBody() const;
	int getX(int index) const;
	int getY(int index) const;
	char getFigure() const;
	void setColor() const;
	/*-----------------------------------------------------------------------------------*/
};

