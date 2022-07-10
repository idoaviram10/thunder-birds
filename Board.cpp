#include "Board.h"


/*----------------------------- Read from file functions ----------------------------*/
void Board::getBoardFromFile(ifstream& infile) {
	/* This function reads the game screen from the relevant text file.
	First, it reads all the text in the file into the auxilary array 'tmpArr'
	and then reads from the auxilary array all the relevant data and builds the game board. */

	int i = 0, j = 0;
	char tmp;
	vector<PositionAndSize> tmpBlockVec(0);
	vector<PositionAndSize> tmpVec(0);
	bool isBigShipSeen = false, isSmallShipSeen = false;
	bool isTopLeftSeen = false, isTopRightSeen = false;

	while (!infile.eof()) {
		char ch;
		ch = infile.get();

		if (ch == BoardSymbols::WALL)
			tmpArr[i][j] = ch;
		else if (ch == '&')
			tmpArr[i][j] = ch;
		else if (ch == BoardSymbols::EXIT)// && i > 2)
			tmpArr[i][j] = ch;
		else if (ch == BoardSymbols::BIGSHIP)
			tmpArr[i][j] = ch;
		else if (ch == BoardSymbols::SMALLSHIP)
			tmpArr[i][j] = ch;
		else if (ch == BoardSymbols::HORIZONTAL_GHOST)
			tmpArr[i][j] = ch;
		else if (ch == BoardSymbols::VERTICAL_GHOST)
			tmpArr[i][j] = ch;
		else if (ch == BoardSymbols::WANDERING_GHOST)
			tmpArr[i][j] = ch;
		else if (isBlockChar(ch))
			tmpArr[i][j] = ch;
		else if (ch == '\n') {
			i++;
			j = -1;
		}
		else if (i != Height - 1)
			tmpArr[i][j] = ch;
		j++;
	}

	for (i = 0; i < Height; i++) {
		for (j = 0; j < Width; j++) {
			char ch = tmpArr[i][j];

			if (ch == BoardSymbols::WALL)
				set(j, i, BoardSymbols::WALL);


			else if (ch == '&') {
				char prevCh = tmpArr[i][j - 1];
				char nextCh = tmpArr[i][j + 1];
				if (prevCh == BoardSymbols::WALL) {
					if (!isTopLeftSeen) {
						isTopLeftSeen = true;
						informationLegendTopLeft.row = j; informationLegendTopLeft.col = i;
						informationLegendTopLeft.figure = ch; informationLegendTopLeft.size = 1;
					}
					else {
						informationLegendBottomLeft.row = j; informationLegendBottomLeft.col = i;
						informationLegendBottomLeft.figure = ch; informationLegendBottomLeft.size = 1;
					}
				}
				if (nextCh == BoardSymbols::WALL) {
					if (!isTopRightSeen) {
						isTopRightSeen = true;
						informationLegendTopRight.row = j; informationLegendTopRight.col = i;
						informationLegendTopRight.figure = ch; informationLegendTopRight.size = 1;
					}
					else {
						informationLegendBottomRight.row = j; informationLegendBottomRight.col = i;
						informationLegendBottomRight.figure = ch; informationLegendBottomRight.size = 1;
					}
				}
				set(j, i, BoardSymbols::EMPTY);
			}

			else if (ch == BoardSymbols::EXIT)// && i > 2)
				set(j, i, BoardSymbols::EXIT);
			else if (ch == BoardSymbols::BIGSHIP) {
				if (!isBigShipSeen) {
					isBigShipSeen = true;
					bigShipPos.row = j; bigShipPos.col = i; bigShipPos.size = 4; bigShipPos.figure = ch;
				}
				set(j, i, BoardSymbols::EMPTY);
			}
			else if (ch == BoardSymbols::SMALLSHIP) {
				if (!isSmallShipSeen) {
					isSmallShipSeen = true;
					smallShipPos.row = j; smallShipPos.col = i; smallShipPos.size = 2; smallShipPos.figure = ch;
				}
				set(j, i, BoardSymbols::EMPTY);
			}
			else if (ch == BoardSymbols::HORIZONTAL_GHOST) {
				tmpGhostPos.row = j; tmpGhostPos.col = i; tmpGhostPos.size = 1; tmpGhostPos.figure = ch;
				horizontalGstPosVec.push_back(tmpGhostPos);
				set(j, i, BoardSymbols::EMPTY);
			}
			else if (ch == BoardSymbols::VERTICAL_GHOST) {
				tmpGhostPos.row = j; tmpGhostPos.col = i; tmpGhostPos.size = 1; tmpGhostPos.figure = ch;
				verticalGstPosVec.push_back(tmpGhostPos);
				set(j, i, BoardSymbols::EMPTY);
			}
			else if (ch == BoardSymbols::WANDERING_GHOST) {
				tmpGhostPos.row = j; tmpGhostPos.col = i; tmpGhostPos.size = 1; tmpGhostPos.figure = ch; wanderingGstPosVec.push_back(tmpGhostPos);
				set(j, i, BoardSymbols::EMPTY);
			}
			else if (isBlockChar(ch)) {
				tmpBlockPos.row = j;
				tmpBlockPos.col = i;
				tmpBlockPos.size = 1;
				tmpBlockPos.figure = ch;
				getBlockFromTmpArr(ch, i, j, tmpBlockVec, tmpVec);
				vecOfVecOfBlockPos.push_back(tmpBlockVec);
				tmpBlockVec.clear();

				set(j, i, BoardSymbols::EMPTY);
			}
			else
				set(j, i, ch);
		}
	}
	infile.close();
}

void Board::getBlockFromTmpArr(char ch, int i, int j, vector<PositionAndSize>& tmpBlockVec, vector<PositionAndSize>& tmpVec) {
	/* A recursive function that builds a block according to what is called from the file.
	Once a character is identified as a block (between '0' and '9'),
	we call this function that passes all the characters around the current character and builds a block. */

	char currChar = tmpArr[i][j];
	if (currChar != ch || isPointAlreadySeen(i, j, currChar, tmpVec))
		return;

	PositionAndSize tmp;
	tmp.col = j; tmp.row = i; tmp.figure = tmpArr[i][j]; tmp.size = 1;
	tmpVec.push_back(tmp);

	getBlockFromTmpArr(ch, i, j + 1, tmpBlockVec, tmpVec);
	getBlockFromTmpArr(ch, i + 1, j, tmpBlockVec, tmpVec);
	getBlockFromTmpArr(ch, i, j - 1, tmpBlockVec, tmpVec);

	tmpBlockPos.row = j; tmpBlockPos.col = i; tmpBlockPos.size = 1; tmpBlockPos.figure = ch;
	tmpBlockVec.push_back(tmpBlockPos);
}

bool Board::isPointAlreadySeen(int i, int j, char ch, vector<PositionAndSize>& tmpVec) {
	/* In the 'getBlockFromTmpArr' function, we use an auxiliary vector into which
	we enter the position of each of the points we pass.
	This function checks if a specific point is already in the auxiliary vector.
	If so, the function will return 'true'.
	Otherwise, 'lie'. */

	PositionAndSize tmp;
	tmp.col = j; tmp.row = i; tmp.figure = tmpArr[i][j]; tmp.size = 1;
	for (int k = 0; k < tmpVec.size(); k++) {
		if (tmp == tmpVec[k])
			return true;
	}
	return false;
}

bool Board::isBlockChar(char c) {
	if (c >= '0' && c <= '9')
		return true;
	return false;
}
/*-----------------------------------------------------------------------------------*/


void Board::printBoard(char colorsMode) const {
	/* This function prints the board to the screen.
	If the character W (marking a wall) is detected, a special character will be printed from the ASCII table. */

	gotoxy(0, 0);
	for (size_t i = 0; i < Height; i++) {
		for (size_t j = 0; j < Width; j++) {
			if (get(j, i) == BoardSymbols::WALL) {
				if (colorsMode == Colors::ON)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Walls_Color);
				cout << BoardSymbols::SPECIAL_CHAR_WALL;
			}
			else {
				if (colorsMode == Colors::ON) {
					if (get(j, i) == BoardSymbols::EXIT && i > 2)
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Exit_Color);
					else
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Data_Color);
				}
				cout << board[i][j];
			}
		}
	}
}


/*------------------------------ Get and set functions ------------------------------*/
char Board::get(int x, int y) const {
	/* This function returns the character found on the board in the relevant coordinate. */

	return board[y][x];
}

void Board::set(int x, int y, char c) {
	/* This function updates the game board with a character it receives in the relevant coordinate. */

	board[y][x] = c;
}

int Board::getBigShipX() const {
	return bigShipPos.row;
}

int Board::getBigShipY() const {
	return bigShipPos.col;
}

int Board::getSmallShipX() const {
	return smallShipPos.row;
}

int Board::getSmallShipY() const {
	return smallShipPos.col;
}

int Board::getAmountOfBlocks() const {
	return vecOfVecOfBlockPos.size();
}

int Board::getSizeOfBlock(int index) const {
	return vecOfVecOfBlockPos[index].size();
}

int Board::getBlockX(int i, int j) const {
	return vecOfVecOfBlockPos[i][j].row;
}

int Board::getBlockY(int i, int j) const {
	return vecOfVecOfBlockPos[i][j].col;
}

char Board::getBlockFigure(int i, int j) const {
	return vecOfVecOfBlockPos[i][j].figure;
}

int Board::getAmountOfGhosts(int ghostType) const {
	if (ghostType == GhostConstants::Horizontal)
		return horizontalGstPosVec.size();
	else if (ghostType == GhostConstants::Vertical)
		return verticalGstPosVec.size();
	else if (ghostType == GhostConstants::Wandering)
		return wanderingGstPosVec.size();
}

int Board::getGhostX(int ghostType, int i, bool isReinit) const {
	if (isReinit == false) {
		if (ghostType == GhostConstants::Horizontal)
			return horizontalGstPosVec[i].row;
		else if (ghostType == GhostConstants::Vertical)
			return verticalGstPosVec[i].row;
		else if (ghostType == GhostConstants::Wandering)
			return wanderingGstPosVec[i].row;
	}

	else {
		if (ghostType == GhostConstants::Horizontal)
			return horizontalGstPosVec[i].row;
		else if (ghostType == GhostConstants::Vertical)
			return verticalGstPosVec[i - horizontalGstPosVec.size()].row;
		else if (ghostType == GhostConstants::Wandering)
			return wanderingGstPosVec[i - verticalGstPosVec.size() - horizontalGstPosVec.size()].row;
	}
}

int Board::getGhostY(int ghostType, int i, bool isReinit) const {
	if (isReinit == false) {
		if (ghostType == GhostConstants::Horizontal)
			return horizontalGstPosVec[i].col;
		else if (ghostType == GhostConstants::Vertical)
			return verticalGstPosVec[i].col;
		else if (ghostType == GhostConstants::Wandering)
			return wanderingGstPosVec[i].col;
	}

	else {
		if (ghostType == GhostConstants::Horizontal)
			return horizontalGstPosVec[i].col;
		else if (ghostType == GhostConstants::Vertical)
			return verticalGstPosVec[i - horizontalGstPosVec.size()].col;
		else if (ghostType == GhostConstants::Wandering)
			return wanderingGstPosVec[i - verticalGstPosVec.size() - horizontalGstPosVec.size()].col;
	}
}

int Board::getInformationLegendTopLeft(char ch) const {
	if (ch == ROW)
		return informationLegendTopLeft.row;
	return informationLegendTopLeft.col;
}

int Board::getInformationLegendTopRight(char ch) const {
	if (ch == ROW)
		return informationLegendTopRight.row;
	return informationLegendTopRight.col;
}

int Board::getInformationLegendBottomLeft(char ch) const {
	if (ch == ROW)
		return informationLegendBottomLeft.row;
	return informationLegendBottomLeft.col;
}

int Board::getInformationLegendBottomRight(char ch) const {
	if (ch == ROW)
		return informationLegendBottomRight.row;
	return informationLegendBottomRight.col;
}
/*-----------------------------------------------------------------------------------*/


