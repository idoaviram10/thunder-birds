#pragma once
#include "Utils.h"


class Board
{
public:
	constexpr static int Width = 82;
	constexpr static int Height = 25;

private:
	char board[Height][Width];
	char tmpArr[Height][Width];
	vector<PositionAndSize> horizontalGstPosVec, verticalGstPosVec, wanderingGstPosVec;
	vector<vector<PositionAndSize>> vecOfVecOfBlockPos;
	PositionAndSize bigShipPos, smallShipPos;
	PositionAndSize tmpGhostPos;
	PositionAndSize tmpBlockPos;
	PositionAndSize informationLegendTopLeft, informationLegendTopRight, informationLegendBottomLeft, informationLegendBottomRight;
public:
	/*----------------------------- Read from file functions ----------------------------*/
	void getBoardFromFile(ifstream& infile);
	void getBlockFromTmpArr(char ch, int i, int j, vector<PositionAndSize>& tmpBlockVec, vector<PositionAndSize>& tmpVec);
	bool isPointAlreadySeen(int i, int j, char ch, vector<PositionAndSize>& tmpVec);
	bool isBlockChar(char c);
	/*-----------------------------------------------------------------------------------*/
	void printBoard(char colorsMode) const;
	/*------------------------------ Get and set functions ------------------------------*/
	char get(int x, int y) const;
	void set(int x, int y, char c);
	int getBigShipX() const;
	int getBigShipY() const;
	int getSmallShipX() const;
	int getSmallShipY() const;
	int getAmountOfBlocks() const;
	int getSizeOfBlock(int index) const;
	int getBlockX(int i, int j) const;
	int getBlockY(int i, int j) const;
	char getBlockFigure(int i, int j) const;
	int getAmountOfGhosts(int ghostType) const;
	int getGhostX(int ghostType, int i, bool isReinit) const;
	int getGhostY(int ghostType, int i, bool isReinit) const;
	int getInformationLegendTopLeft(char ch) const;
	int getInformationLegendTopRight(char ch) const;
	int getInformationLegendBottomLeft(char ch) const;
	int getInformationLegendBottomRight(char ch) const;
	/*-----------------------------------------------------------------------------------*/
};