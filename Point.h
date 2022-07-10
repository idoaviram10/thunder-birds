#pragma once
#include "Utils.h"
#include "Board.h"

class Board;

class Point {
	char figure;
	int x = -1, y = -1;
	int dirx = 0, diry = 0;
	Board* pBoard;
public:
	/*------------------------------ C'tors and D'tors ----------------------------------*/
	Point() {};
	Point(int _x, int _y, char c, Board* pb) : x(_x), y(_y), figure(c), pBoard(pb) {}
	/*-----------------------------------------------------------------------------------*/
	void drawPoint() const;
	void delPoint() const;
	void movePoint(int difx, int dify, int gameMode);
	/*------------------------------ Get and set functions ------------------------------*/
	int getX() const;
	int getY() const;
	char getFigure() const;
	/*-----------------------------------------------------------------------------------*/
};

