#include <iostream>
#include <windows.h>
#include <process.h>
#include "Utils.h"


void gotoxy(int x, int y) {
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	std::cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void clrscr() {
	system("cls");
}

void hide_cursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

bool PositionAndSize::operator==(const PositionAndSize& pos) const {
	if (this->row == pos.row && this->col == pos.col && this->size == pos.size && this->figure == pos.figure)
		return true;
	return false;
}
