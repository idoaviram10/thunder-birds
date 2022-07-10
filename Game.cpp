#include "Game.h"


/*------------------------------ C'tors and D'tors ----------------------------------*/
Game::Game() {
	pBoard = new Board();
}

Game::~Game() {
	delete smallShip;
	delete bigShip;
	for (int i = 0; i < blocks.size(); i++)
		delete blocks[i];
	for (int i = 0; i < ghosts.size(); i++)
		delete ghosts[i];
	blocks.clear();
	ghosts.clear();
	delete pBoard;
}
/*-----------------------------------------------------------------------------------*/


/*------------------------------ Gameplay functions ---------------------------------*/
void Game::initBoardAndGame(ifstream& infile, string& fileName) {
	/* This function initializes the game, according to the screen readed from the file. */

	if (gameMode == save)
		getOutFileName(fileName); // create steps file and result file
	if (gameMode == load || gameMode == silent)
		getInFileName(fileName); // reading steps file and result file
	pBoard->getBoardFromFile(infile);
	setGameObjects();
	run();
}

void Game::setGameObjects() {
	/* This function initializes all the objects of the game, according to the screen readed from the file. */

	int bigShipStartX, bigShipStartY, smallShipStartX, smallShipStartY;
	Ghost* tmpGhost;
	Block* tmpBlock;
	Point* tmpBlockPoints;
	char tmpBlockFigure;

	bigShipStartX = pBoard->getBigShipX();
	bigShipStartY = pBoard->getBigShipY();
	smallShipStartX = pBoard->getSmallShipX();
	smallShipStartY = pBoard->getSmallShipY();

	Point bigPoints[4] = { {bigShipStartX,bigShipStartY,'#',pBoard},{bigShipStartX + 1,bigShipStartY,'#',pBoard},
		{bigShipStartX,bigShipStartY + 1,'#',pBoard},{bigShipStartX + 1,bigShipStartY + 1,'#',pBoard} };
	bigShip = new Ship(bigPoints, 4);
	bigShip->setBoard(pBoard);

	Point smallPoints[2] = { {smallShipStartX,smallShipStartY,'@',pBoard},{smallShipStartX + 1,smallShipStartY,'@',pBoard} };
	smallShip = new Ship(smallPoints, 2);
	smallShip->setBoard(pBoard);


	for (int i = 0; i < pBoard->getAmountOfGhosts(GhostConstants::Horizontal); i++) {
		Point ghostPoints[1] = { {pBoard->getGhostX(GhostConstants::Horizontal, i, false), pBoard->getGhostY(GhostConstants::Horizontal, i, false),
			'%', pBoard } };
		tmpGhost = new Ghost(ghostPoints, 1, GhostConstants::Horizontal);
		ghosts.push_back(tmpGhost);
	}
	for (int i = 0; i < pBoard->getAmountOfGhosts(GhostConstants::Vertical); i++) {
		Point ghostPoints[1] = { {pBoard->getGhostX(GhostConstants::Vertical, i, false), pBoard->getGhostY(GhostConstants::Vertical, i, false),
			'%', pBoard } };
		tmpGhost = new Ghost(ghostPoints, 1, GhostConstants::Vertical);
		ghosts.push_back(tmpGhost);
	}
	for (int i = 0; i < pBoard->getAmountOfGhosts(GhostConstants::Wandering); i++) {
		Point ghostPoints[1] = { {pBoard->getGhostX(GhostConstants::Wandering, i, false), pBoard->getGhostY(GhostConstants::Wandering, i, false),
			'%', pBoard } };
		tmpGhost = new Ghost(ghostPoints, 1, GhostConstants::Wandering);
		ghosts.push_back(tmpGhost);
	}
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i]->setBoard(pBoard);

	for (int i = 0; i < pBoard->getAmountOfBlocks(); i++) {
		tmpBlockPoints = new Point[pBoard->getSizeOfBlock(i)];
		for (int j = 0; j < pBoard->getSizeOfBlock(i); j++) {
			tmpBlockPoints[j] = { pBoard->getBlockX(i,j), pBoard->getBlockY(i,j), pBoard->getBlockFigure(i,j), pBoard };
		}
		tmpBlock = new Block(tmpBlockPoints, pBoard->getSizeOfBlock(i));
		blocks.push_back(tmpBlock);
		blocks[i]->setBoard(pBoard);
	}
}

void Game::run() {
	/* This function is the engine of the game. */

	bool isNextMoveValidBlock;
	char lastKey = 0, blockFigure = ' ';


	while (lives > 0 && key != Keys::Exit && !isLevelCompleted) {
		/* The game continues to be played as long as the user has life,
		as long as he does not press 9 and as long as he has not completed the level. */
		printAll();
		while (key != Keys::Exit) {
			isNextMoveValidBlock = false;
			if (gameMode == simple || gameMode == save) {
				if (_kbhit()) {
					key = _getch();
					if (key == Keys::ESC) {
						pause();
						if (key == Keys::Exit)
							break;
					}
				}
			}

			if (gameMode == save) { // set output to file
				if (isValidKey(key)) {
					lastKey = key;
					outfile_steps << key;
				}
				else if (key == 0)
					outfile_steps << '0';
				else {
					if (lastKey == 0)
						outfile_steps << '0';
					else
						outfile_steps << lastKey;
				}
			}

			else if (gameMode == load || gameMode == silent)
				key = getStepFromFile();

			changeDir();

			if (activeShip != prevActive) {
				if (gameMode != silent)
					printActiveShip();
				prevActive = activeShip;
			}

			printTimer();
			moveObjects();

			if (time / LoopForTimer == 0) {
				timeOver();
				break;
			}
			if ((!isBigShipAlive) || (!isSmallShipAlive)) {
				shipDied();
				break;
			}
			if (isSmallShipFinished && isBigShipFinished) {
				/* The stage is completed once the two ships have left the screen. */
				isLevelCompleted = true;
				break;
			}

			if (gameMode != silent)
				Sleep(SleepTime);
		}
	}

	/* An appropriate message will be displayed according to the result of the game - loss or victory. */
	if (isLevelCompleted)
		levelCompleted();
	else
		gameOver();
}

bool Game::isValidKey(char _key) {
	_key = std::tolower(_key);
	if (_key == Keys::Up || _key == Keys::Down || _key == Keys::Right || _key == Keys::Left
		|| _key == Keys::Big_Ship || _key == Keys::Small_Ship || _key == Keys::Exit)
		return true;
	return false;
}

void Game::changeDir() {
	/* This function calculates the change that needs to be made in order to
	change the direction of movement of the ship according to the button that the user clicks on. */

	switch (std::tolower(key)) {
	case Keys::Up:
		dirX = 0;
		dirY = -1;
		break;
	case Keys::Right:
		dirX = 1;
		dirY = 0;
		break;
	case Keys::Down:
		dirX = 0;
		dirY = 1;
		break;
	case Keys::Left:
		dirX = -1;
		dirY = 0;
		break;
	case Keys::Big_Ship:
		if (!isBigShipFinished)
			activeShip = ShipConstants::Big_Ship;
		dirX = 0;
		dirY = 0;
		break;
	case Keys::Small_Ship:
		if (!isSmallShipFinished)
			activeShip = ShipConstants::Small_Ship;
		dirX = 0;
		dirY = 0;
		break;
	}
}

void Game::moveObjects() {
	/* This function moves the active ship according to the direction chosen by the user.
	In addition, the function is also responsible for the movement of the blocks (if possible)
	and the fall of the blocks if they are in the air.
	The function will also check if one of the ships has reached the Exit.
	(In this function we use an auxiliary pointer variable that will hold the address
	of the active ship in order to prevent duplication of code for the two ships). */

	int totalSize = 0, sizeOfBlock, indexOfBlock;
	char shipFigure, theOtherShip;
	Ship* tmpShip;

	/* ---------Set tmpShip to the active ship--------- */
	if (activeShip == ShipConstants::Big_Ship) {
		tmpShip = bigShip;
		theOtherShip = ShipConstants::Small_Ship;
	}
	else {
		tmpShip = smallShip;
		theOtherShip = ShipConstants::Big_Ship;
	}
	shipFigure = tmpShip->getFigure();
	/* ------------------------------------------------ */

	/* -----------Move ship and carry block------------ */
	if (isNextMoveValidForShip(dirX, dirY, shipFigure)) {
		if (isBlock(0, -1, shipFigure, &indexOfBlock)) {
			sizeOfBlock = blocks[indexOfBlock]->getSize();
			if (sizeOfBlock <= tmpShip->getMaxCarry()) {
				if (isNextMoveValidForBlock(dirX, dirY, indexOfBlock))
					blocks[indexOfBlock]->move(dirX, dirY, colorsMode, gameMode);
			}
		}
		tmpShip->move(dirX, dirY, colorsMode, gameMode);
	}
	/* ------------------------------------------------ */

	/* ---------Check if exit, block or ghost---------- */
	else {
		if (isExit(dirX, dirY, shipFigure)) {
			tmpShip->del();
			shipFinished(shipFigure, theOtherShip);
			return;
		}
		else if (isBlock(dirX, dirY, shipFigure, &indexOfBlock)) {
			moveBlocks(dirX, dirY, &totalSize, tmpShip->getMaxCarry(), indexOfBlock);
			if (isNextMoveValidForShip(dirX, dirY, shipFigure))
				tmpShip->move(dirX, dirY, colorsMode, gameMode);

		}
		else if (isGhost(dirX, dirY, shipFigure)) {
			if (shipFigure == ShipConstants::Big_Ship_Figure)
				isBigShipAlive = false;
			else
				isSmallShipAlive = false;
			tmpShip->del();
		}
	}
	/* ------------------------------------------------ */

	moveGhosts();
	droppingBlocks();
}

bool Game::isNextMoveValidForShip(int difX, int difY, char shipFigure) const {
	/* This function checks whether the next step of the ship is valid or not.
	If the step is valid - the function returns true. Otherwise - false.
	(In this function we use an auxiliary pointer variable that will hold the address
	of the active ship in order to prevent duplication of code for the two ships). */

	Ship* tmpShip;
	if (shipFigure == ShipConstants::Big_Ship_Figure)
		tmpShip = bigShip;
	else
		tmpShip = smallShip;

	int sizeOfShip = getSizeOfShip(shipFigure);
	for (size_t i = 0; i < sizeOfShip; i++) {
		int nextX = tmpShip->getX(i) + difX;
		int nextY = tmpShip->getY(i) + difY;
		if (pBoard->get(nextX, nextY) != BoardSymbols::EMPTY && pBoard->get(nextX, nextY) != shipFigure)
			return false;
	}
	return true;
}

bool Game::isNextMoveValidForBlock(int difX, int difY, int indexOfBlock) const {
	/* This function checks whether the next step of the block is valid or not.
	If the step is valid - the function returns true. Otherwise - false. */
	int indexOfNextBlock;
	Block* tmpBlock = blocks[indexOfBlock];
	for (size_t i = 0; i < tmpBlock->getSize(); i++) {
		int nextX = tmpBlock->getX(i) + difX;
		int nextY = tmpBlock->getY(i) + difY;
		if ((pBoard->get(nextX, nextY) != BoardSymbols::EMPTY && pBoard->get(nextX, nextY) != tmpBlock->getFigure())
			|| isTouchAnotherBlock(nextX, nextY, indexOfBlock, &indexOfNextBlock)) {
			return false;
		}
	}
	return true;
}

bool Game::isTouchAnotherBlock(int nextX, int nextY, int indexOfBlock, int* indexOfNextBlock) const {
	/* This function checks if the block touches another block.
	If so, the function will return 'true' and as a variable output the index of the block that the current block touched.
	Otherwise, the function will return 'false'. */

	Block* currBlock;
	for (int i = 0; i < blocks.size(); i++) {
		currBlock = blocks[i];
		for (int j = 0; j < currBlock->getSize(); j++) {
			if (currBlock->getX(j) == nextX && currBlock->getY(j) == nextY)
				if (indexOfBlock != i) {
					*indexOfNextBlock = i;
					return true;
				}
		}
	}
	return false;
}

void Game::moveBlocks(int difX, int difY, int* totalSize, int maxCarry, int index) {
	/* A recursive function that aims to move the blocks when one of the moving ships hits them.
	The function calculates the total size of the blocks that touch each other,
	and if the active ship can move them, they will move. */

	bool flag = false;
	Block* currBlock = blocks[index];
	int indexOfNextBlock;
	(*totalSize) += currBlock->getSize();
	for (size_t i = 0; i < currBlock->getSize(); i++) {
		int nextX = currBlock->getX(i) + difX;
		int nextY = currBlock->getY(i) + difY;
		if (isTouchAnotherBlock(nextX, nextY, index, &indexOfNextBlock)) {
			flag = true;
			break;
		}
	}
	if (flag)
		moveBlocks(difX, difY, totalSize, maxCarry, indexOfNextBlock);

	if (*totalSize <= maxCarry && isNextMoveValidForBlock(difX, difY, index))
		currBlock->move(difX, difY, colorsMode, gameMode);
}

bool Game::isBlock(int difX, int difY, char shipFigure, int* indexOfBlock) const {
	/* This function checks if the next step is a block.
	If it is a block - the function will return true and in addition,
	will return in the variable 'indexOfBlock' the index of the relevant block from the array of blocks.
	(In this function we use an auxiliary pointer variable that will hold the address
	of the active ship in order to prevent duplication of code for the two ships). */

	int i, j, k;
	Ship* tmpShip;
	Block* currBlock;

	if (shipFigure == ShipConstants::Big_Ship_Figure)
		tmpShip = bigShip;
	else
		tmpShip = smallShip;

	int sizeOfShip = getSizeOfShip(shipFigure);
	for (i = 0; i < sizeOfShip; i++) {
		int nextX = tmpShip->getX(i) + difX;
		int nextY = tmpShip->getY(i) + difY;
		for (j = 0; j < blocks.size(); j++) {
			currBlock = blocks[j];
			for (k = 0; k < currBlock->getSize(); k++)
				if (currBlock->getX(k) == nextX && currBlock->getY(k) == nextY) {
					*indexOfBlock = j;
					return true;
				}
		}
	}
	return false;

}

bool Game::isGhost(int difX, int difY, char shipFigure) const {
	int i, j, k;
	Ship* tmpShip;
	Ghost* currGhost;

	if (shipFigure == ShipConstants::Big_Ship_Figure)
		tmpShip = bigShip;
	else
		tmpShip = smallShip;

	int sizeOfShip = getSizeOfShip(shipFigure);
	for (i = 0; i < sizeOfShip; i++) {
		int nextX = tmpShip->getX(i) + difX;
		int nextY = tmpShip->getY(i) + difY;
		for (j = 0; j < ghosts.size(); j++) {
			currGhost = ghosts[j];
			if (currGhost->getX(0) == nextX && currGhost->getY(0) == nextY)
				return true;
		}
	}
	return false;
}

void Game::moveGhosts() {
	/* This function goes over the ghost vector and moves all the ghosts on the screen.
	If one of the ghosts touched one of the ships, the ship would die. */

	bool shipShouldDie = false;
	char whatShip = ShipConstants::No_Ship;

	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i]->move(&shipShouldDie, &whatShip, colorsMode, gameMode, outfile_steps, infile_steps);

	if (shipShouldDie) {
		if (whatShip == ShipConstants::Big_Ship_Figure) {
			isBigShipAlive = false;
			bigShip->del();
		}
		else {
			isSmallShipAlive = false;
			smallShip->del();
		}
	}
}

void Game::droppingBlocks() {
	/* This function is responsible for the fall of the blocks if they are in the air.
	In addition, the function checks whether one of the ships should die
	(due to a block fall) and updates the relevant data member. */

	bool shipShouldDie = false;
	char whatShip = ShipConstants::No_Ship;

	for (int i = 0; i < blocks.size(); i++)
		fallingDown(blocks[i], i, &shipShouldDie, &whatShip, colorsMode);

	if (shipShouldDie) {
		if (whatShip == ShipConstants::Big_Ship) {
			isBigShipAlive = false;
			bigShip->del();
		}
		else {
			isSmallShipAlive = false;
			smallShip->del();
		}
	}
}

bool Game::isAbleToFall(Block* currBlock, int indexOfBlock) const {
	/* This function checks whether the block should fall or not.
	That is, whether there is something beneath it or not. */

	int indexOfNextBlock;
	for (int i = 0; i < currBlock->getSize(); i++) {
		int nextX = currBlock->getX(i);
		int nextY = currBlock->getY(i) + 1;
		if ((pBoard->get(nextX, nextY) != BoardSymbols::EMPTY && pBoard->get(nextX, nextY) != currBlock->getFigure()) ||
			isTouchAnotherBlock(nextX, nextY, indexOfBlock, &indexOfNextBlock))
			return false;
	}
	return true;
}

void Game::fallingDown(Block* currBlock, int indexOfBlock, bool* shipShouldDie, char* whatShip, char colorsMode) {
	/* This function will drop the block as long as it can fall.
	In addition this function checks if the block has fallen on one of the ships.
	If the block did fall on one of the ships, the function will check if the ship should die.
	If so, the function will update the relevant output variable. */

	bool isFalling = true, isShipUnder = false;
	char chUnder = ' ', shipUnder = ShipConstants::No_Ship;

	if (isAbleToFall(currBlock, indexOfBlock)) {
		currBlock->del();
		currBlock->move(0, 1, colorsMode, gameMode);
	}

	for (size_t i = 0; i < currBlock->getSize(); i++) {
		chUnder = pBoard->get(currBlock->getX(i), currBlock->getY(i) + 1);
		if (chUnder == ShipConstants::Big_Ship_Figure || chUnder == ShipConstants::Small_Ship_Figure) {
			isShipUnder = true;
			shipUnder = chUnder;
		}
		else if (chUnder == char(BoardSymbols::WALL))
			isFalling = false;
	}

	switch (shipUnder) {
	case ShipConstants::Big_Ship_Figure:
		if (currBlock->getSize() > ShipConstants::Big_Ship_Max_Carry && isFalling) {
			*shipShouldDie = true;
			*whatShip = shipUnder;
		}
		break;
	case ShipConstants::Small_Ship_Figure:
		if (currBlock->getSize() > ShipConstants::Small_Ship_Max_Carry && isFalling) {
			*shipShouldDie = true;
			*whatShip = shipUnder;
		}
		break;
	}
}

void Game::pause() {
	/* This function pauses the game until the user presses ESC again.
	If the user clicks the number 9, the game will end. */

	char tmp = 0;
	bool hit9orESC = false;
	removeAllFromScreen();
	dirX = dirY = 0;

	gotoxy(DisplayConstants::PosPauseY, DisplayConstants::PosPauseX);
	cout << "               GAME PAUSED!                ";
	gotoxy(DisplayConstants::PosPauseY, DisplayConstants::PosPauseX + 1);
	cout << "  Press ESC again to continue or 9 to Exit ";

	while (hit9orESC == false) {
		key = 0;
		if (_kbhit())
			key = _getch();
		if (key == Keys::ESC || key == Keys::Exit) {
			hit9orESC = true;
			break;
		}
	}
	if (hit9orESC == true) {
		gotoxy(DisplayConstants::PosPauseY, DisplayConstants::PosPauseX);
		cout << "                                           " << endl;
		gotoxy(DisplayConstants::PosPauseY, DisplayConstants::PosPauseX + 1);
		cout << "                                           " << endl;
		if (key != Keys::Exit)
			printAll();
		return;
	}
}

void Game::timeOver() {
	/* Each time the time runs out, this function will print a relevant message
	to the screen and in addition will reduce 1 life. */

	int timeOverPosRow = ((pBoard->getInformationLegendBottomRight(ROW) + pBoard->getInformationLegendBottomLeft(ROW)) / 2) - 5;
	int timeOverCol = pBoard->getInformationLegendBottomRight(COL);

	lives--;

	if (gameMode != silent) {
		if (colorsMode == Colors::ON)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Time_Color);
		gotoxy(timeOverPosRow, timeOverCol);
		cout << "TIME OVER!";
		Sleep(3000);
		gotoxy(timeOverPosRow, timeOverCol);
		cout << "          ";
	}

	if (gameMode == save)
		outfile_result << 'D' << ' ' << (time / LoopForTimer) << endl;

	if (gameMode == silent)
		if (!isResultsEqual('D'))
			silentTestResult = failed;


	if (lives != 0)
		reInitGame();
}

void Game::shipDied() {
	/* Each time one of the ships died, this function will print a relevant message
	to the screen and in addition will reduce 1 life. */

	int shipDiedPosRow = ((pBoard->getInformationLegendBottomRight(ROW) + pBoard->getInformationLegendBottomLeft(ROW)) / 2) - 5;
	int shipDiedCol = pBoard->getInformationLegendBottomRight(COL);

	lives--;

	if (gameMode != silent) {
		if (colorsMode == Colors::ON)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Time_Color);
		if (!isBigShipAlive) {
			gotoxy(shipDiedPosRow, shipDiedCol);
			cout << "BIG SHIP DIED!";
			Sleep(3000);
			gotoxy(shipDiedPosRow, shipDiedCol);
			cout << "              ";
		}
		else {
			gotoxy(shipDiedPosRow, shipDiedCol);
			cout << "SMALL SHIP DIED!";
			Sleep(3000);
			gotoxy(shipDiedPosRow, shipDiedCol);
			cout << "                ";
		}
	}

	if (gameMode == save)
		outfile_result << 'D' << ' ' << floor(time / LoopForTimer) << endl;

	if (gameMode == silent)
		if (!isResultsEqual('D'))
			silentTestResult = failed;

	if (lives != 0)
		reInitGame();
}

bool Game::isExit(int difX, int difY, char shipFigure) const {
	/* This function checks if the next step is the exit.
	If it is the exit - the function will return true. Otherwise - false.
	(In this function we use an auxiliary pointer variable that will hold the address
	of the active ship in order to prevent duplication of code for the two ships). */

	Ship* tmpShip;
	if (shipFigure == ShipConstants::Big_Ship_Figure)
		tmpShip = bigShip;
	else
		tmpShip = smallShip;

	int sizeOfShip = getSizeOfShip(shipFigure);
	for (size_t i = 0; i < sizeOfShip; i++) {
		int nextX = tmpShip->getX(i) + difX;
		int nextY = tmpShip->getY(i) + difY;
		if (pBoard->get(nextX, nextY) == BoardSymbols::EXIT)
			return true;
	}
	return false;
}

void Game::shipFinished(char shipFigure, char theOtherShip) {
	key = 0;
	activeShip = theOtherShip;
	dirX = dirY = 0;

	if (shipFigure == ShipConstants::Big_Ship_Figure)
		isBigShipFinished = true;
	else
		isSmallShipFinished = true;
}

void Game::removeAllFromScreen() const {
	/* This function deletes from the screen all the data except the borders of the board. */

	for (size_t i = 1; i < pBoard->Height - 1; i++) {
		for (size_t j = 1; j < pBoard->Width - 3; j++) {
			gotoxy(j, i);
			cout << ' ';
		}
	}

}

void Game::reInitGame() {
	/* This function reinitializing the game whenever time runs out or when one of the ships dies. */

	Point* tmpBlockPoints;
	char tmpBlockFigure;
	int bigShipStartX, bigShipStartY, smallShipStartX, smallShipStartY;
	bigShipStartX = pBoard->getBigShipX();
	bigShipStartY = pBoard->getBigShipY();
	smallShipStartX = pBoard->getSmallShipX();
	smallShipStartY = pBoard->getSmallShipY();


	/*------------------------------ Delete objects from the board ------------------------------*/
	for (int i = 0; i < blocks.size(); i++)
		blocks[i]->del();
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i]->del();
	bigShip->del();
	smallShip->del();
	/*-------------------------------------------------------------------------------------------*/


	/*---------------------------- Reinitialize objects on the board ----------------------------*/
	for (int i = 0; i < ghosts.size(); i++) {
		int currGhostType = ghosts[i]->getType();
		Point ghostPoints[1] = { {pBoard->getGhostX(currGhostType, i, true), pBoard->getGhostY(currGhostType, i, true),
			'%', pBoard } };
		ghosts[i]->reInit(ghostPoints, 1);
	}
	for (int i = 0; i < blocks.size(); i++) {
		tmpBlockPoints = new Point[pBoard->getSizeOfBlock(i)];
		for (int j = 0; j < pBoard->getSizeOfBlock(i); j++) {
			tmpBlockPoints[j] = { pBoard->getBlockX(i,j), pBoard->getBlockY(i,j), pBoard->getBlockFigure(i,j), pBoard };
		}
		blocks[i]->reInit(tmpBlockPoints, pBoard->getSizeOfBlock(i));
	}
	Point bigPoints[4] = { {bigShipStartX,bigShipStartY,'#',pBoard},{bigShipStartX + 1,bigShipStartY,'#',pBoard},
		{bigShipStartX,bigShipStartY + 1,'#',pBoard},{bigShipStartX + 1,bigShipStartY + 1,'#',pBoard} };
	Point smallPoints[2] = { {smallShipStartX,smallShipStartY,'@',pBoard},{smallShipStartX + 1,smallShipStartY,'@',pBoard} };
	bigShip->reInit(bigPoints, ShipConstants::Big_Ship_Size);
	smallShip->reInit(smallPoints, ShipConstants::Small_Ship_Size);
	/*-------------------------------------------------------------------------------------------*/


	for (int i = 0; i < blocks.size(); i++)
		blocks[i]->setBoard(pBoard);
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i]->setBoard(pBoard);
	bigShip->setBoard(pBoard);
	smallShip->setBoard(pBoard);


	/*------------------------- Reinitialize data members of "Game" class ------------------------*/
	time = 30000;
	prevActive = activeShip = ShipConstants::Big_Ship;
	dirX = dirY = 0;
	key = 0;
	isBigShipAlive = isSmallShipAlive = true;
	isBigShipFinished = isSmallShipFinished = false;
	/*-------------------------------------------------------------------------------------------*/
}
/*-----------------------------------------------------------------------------------*/


/*------------------------------ Printing functions ---------------------------------*/
void Game::printAll() {
	/* This function will print the entire board to the screen -
	including the time left, life left, active ship, ships and blocks. */

	if (gameMode != silent) {
		pBoard->printBoard(colorsMode);
		printActiveShip();
		printLives();
	}
	if (!isBigShipFinished)
		bigShip->draw(colorsMode, gameMode);
	if (!isSmallShipFinished)
		smallShip->draw(colorsMode, gameMode);
	for (int i = 0; i < blocks.size(); i++)
		blocks[i]->draw(colorsMode, gameMode);
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i]->draw(colorsMode, gameMode);

	int timerPosRow = pBoard->getInformationLegendTopLeft(ROW) + 2;
	int timerPosCol = pBoard->getInformationLegendTopLeft(COL);

	if (colorsMode == Colors::ON)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Time_Color);

	if (gameMode != silent) {
		gotoxy(timerPosRow, timerPosCol);
		cout << "TIME LEFT: " << time / LoopForTimer << ' ';
	}
}

void Game::printLives() const {
	/* This function will print the lives left to the screen. */

	int livesPosRow = pBoard->getInformationLegendTopRight(ROW) - 12;
	int livesPosCol = pBoard->getInformationLegendTopRight(COL);

	if (colorsMode == Colors::ON)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Lives_Color);

	switch (lives) {
	case 3:
		gotoxy(livesPosRow, livesPosCol);
		cout << "LIVES: $ $ $";
		break;
	case 2:
		gotoxy(livesPosRow, livesPosCol);
		cout << "LIVES: $ $  ";
		break;
	case 1:
		gotoxy(livesPosRow, livesPosCol);
		cout << "LIVES: $    ";
		break;
	case 0:
		gotoxy(livesPosRow, livesPosCol);
		cout << "LIVES:      ";
		break;
	}
}

void Game::printActiveShip() const {
	/* This function prints the active ship to the screen. */

	int actShipPosRow = pBoard->getInformationLegendBottomLeft(ROW) + 2;
	int actShipPosCol = pBoard->getInformationLegendBottomLeft(COL);

	if (colorsMode == Colors::ON)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::ActiveShip_Color);

	if (activeShip == ShipConstants::Big_Ship) {
		gotoxy(actShipPosRow, actShipPosCol);
		cout << "ACTIVE: BIG SHIP  ";
	}
	else {
		gotoxy(actShipPosRow, actShipPosCol);
		cout << "ACTIVE: SMALL SHIP";
	}
}

void Game::printTimer() {
	/* This function prints the time left to the screen. */

	time = time - SleepTime;

	int timerPosRow = pBoard->getInformationLegendTopLeft(ROW) + 2;
	int timerPosCol = pBoard->getInformationLegendTopLeft(COL);

	if (colorsMode == Colors::ON)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::Time_Color);

	if (gameMode != silent) {
		if (time % 10 == 0) {
			gotoxy(timerPosRow, timerPosCol);
			cout << "TIME LEFT: " << time / LoopForTimer << ' ';
		}
	}
}

void Game::levelCompleted() {
	/* If both ships were able to exit the board in time, the user successfully passed the stage.
	A relevant message will be printed to the screen. */

	removeAllFromScreen();

	if (colorsMode == Colors::ON)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::LevelCompleted_Color);

	if (gameMode != silent) {
		gotoxy(DisplayConstants::PosGameOverY, DisplayConstants::PosGameOverX - 1);
		cout << "***************************";
		gotoxy(DisplayConstants::PosGameOverY, DisplayConstants::PosGameOverX);
		cout << "*    Y O U    W I N   !   *";
		gotoxy(DisplayConstants::PosGameOverY, DisplayConstants::PosGameOverX + 1);
		cout << "***************************";
	}

	if (gameMode == save)
		outfile_result << 'W' << ' ' << floor(time / LoopForTimer);


	if (gameMode == silent)
		if (!isResultsEqual('W'))
			silentTestResult = failed;

	if (gameMode == save) {
		outfile_steps.close();
		outfile_result.close();
	}
	else if (gameMode == load || gameMode == silent) {
		infile_steps.close();
		infile_result.close();
	}

	if (gameMode != silent)
		Sleep(3000);
}

void Game::gameOver() {
	/* If not enough life was left, the user lost and the game was over.
	A relevant message will be printed to the screen. */

	removeAllFromScreen();
	isLevelCompleted = false;

	if (colorsMode == Colors::ON)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Colors::GameOver_Color);

	if (gameMode != silent) {
		gotoxy(DisplayConstants::PosGameOverY, DisplayConstants::PosGameOverX - 1);
		cout << "***************************";
		gotoxy(DisplayConstants::PosGameOverY, DisplayConstants::PosGameOverX);
		cout << "*  G A M E    O V E R  !  *";
		gotoxy(DisplayConstants::PosGameOverY, DisplayConstants::PosGameOverX + 1);
		cout << "***************************";
	}

	if (gameMode == save)
		outfile_result << 'L' << ' ' << floor(time / LoopForTimer);

	if (gameMode == silent)
		if (!isResultsEqual('L'))
			silentTestResult = failed;

	if (gameMode == save) {
		outfile_steps.close();
		outfile_result.close();
	}
	else if (gameMode == load || gameMode == silent) {
		infile_steps.close();
		infile_result.close();
	}

	if (gameMode != silent)
		Sleep(3000);
}
/*-----------------------------------------------------------------------------------*/


/*--------------------------- Load and save mode functions --------------------------*/
void Game::getOutFileName(string& fileName) {
	string steps;
	string result;
	steps = fileName;
	result = fileName;

	for (int i = 0; i < 7; i++) {
		/* deleting the ".screen" */
		steps.pop_back();
		result.pop_back();
	}

	steps += ".steps";
	result += ".result";

	outfile_steps.open(steps);
	outfile_result.open(result);
}

void Game::getInFileName(string& fileName) {
	string steps;
	string result;
	steps = fileName;
	result = fileName;

	for (int i = 0; i < 7; i++) {
		/* deleting the ".screen" */
		steps.pop_back();
		result.pop_back();
	}

	steps += ".steps";
	result += ".result";


	infile_steps.open(steps);
	if (!infile_steps) {
		checkFileExist();
		return;
	}

	infile_result.open(result);
	if (!infile_result) {
		checkFileExist();
		return;
	}
}

void Game::checkFileExist() {
	if (gameMode == load) {
		clrscr();
		cout << "There is no saved file!";
		Sleep(3000);
		clrscr();
	}

	else if (gameMode == silent)
		silentTestResult = failed;

	key = Keys::Exit;
}

char Game::getStepFromFile() {
	char c;
	infile_steps >> c;
	if (c == '0')
		return 0;
	return c;
}

bool Game::isResultsEqual(char c) {
	char resCh;
	int resTime;

	infile_result >> resCh;
	infile_result >> resTime;

	if (resCh == c) {
		if (resTime != floor(time / LoopForTimer))
			return false;
	}
	else
		return false;

	return true;
}
/*-----------------------------------------------------------------------------------*/


/*------------------------------ Get and set functions ------------------------------*/
void Game::setGameMode(int _gameMode) {
	gameMode = _gameMode;
}

int Game::getGameMode() {
	return gameMode;
}

void Game::setColorsMode(char colorChoice) {
	/* This function checks if the user has chosen to play with colors or without colors.
	If the user chooses to play with colors, the function will update the appropriate data member
	that will indicate throughout the game that it should be with colors. */

	if (colorChoice == Colors::ON)
		colorsMode = Colors::ON;
}

void Game::setGameOrder(char order) {
	/* This function gets a char that tell us if the user has chosen to play a lexicographic game
	or to load a specific screen, and then updates the game order. */

	gameOrder = order;
}

char Game::getGameOrder() const {
	/* This function checks if the user has chosen to play a lexicographic game or to load a specific screen,
	and then returns the game order. */

	return gameOrder;
}

int Game::getGameResult() const {
	if (isLevelCompleted)
		return GameResult::Level_Completed;
	else
		return GameResult::Game_Over;
}

int Game::getCurrLives() const {
	/* This function returns the current amount of lives.  */

	return lives;
}

void Game::setCurrLives(int _lives) {
	/* This function sets the live to a new value. */

	lives = _lives;
}

void Game::setTestResult(int _silentTestResult) {
	silentTestResult = _silentTestResult;
}

int Game::getTestResult() const {
	return silentTestResult;
}

int Game::getSizeOfShip(char shipFigure) const {
	/* This function will return the size of the ship according to the figure it receives. */

	if (shipFigure == ShipConstants::Big_Ship_Figure)
		return ShipConstants::Big_Ship_Size;
	else
		return ShipConstants::Small_Ship_Size;
}
/*-----------------------------------------------------------------------------------*/