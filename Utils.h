#pragma once
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <conio.h>
#include <cctype>
#include <windows.h>
#include <fstream>
#include <time.h> 
#include <vector>
#include <filesystem>


using namespace std;

#define ROW 'r'
#define COL 'c'


void gotoxy(int, int);
void clrscr();
void hide_cursor();

struct PositionAndSize {
	int row;
	int col;
	int size;
	char figure;
public:
	bool operator==(const PositionAndSize& pos) const;
};

enum SilentTestResult { failed, passed };

enum GameMode { load, save, silent, simple };

struct Colors {
	constexpr static char OFF = '0';
	constexpr static char ON = '1';
	constexpr static int Menu_Color = 3;
	constexpr static int Ships_Color = 12;
	constexpr static int GameOver_Color = 4;
	constexpr static int LevelCompleted_Color = 9;
	constexpr static int ActiveShip_Color = 12;
	constexpr static int Walls_Color = 14;
	constexpr static int Blocks_Ghosts_Color = 3;
	constexpr static int Lives_Color = 12;
	constexpr static int Time_Color = 12;
	constexpr static int Data_Color = 15;
	constexpr static int Exit_Color = 10;
};

struct Keys {
	constexpr static char ESC = 27;
	constexpr static char Up = 'w';
	constexpr static char Right = 'd';
	constexpr static char Down = 'x';
	constexpr static char Left = 'a';
	constexpr static char Big_Ship = 'b';
	constexpr static char Small_Ship = 's';
	constexpr static char Exit = '9';
};

struct GameResult {
	constexpr static int Game_Over = 0;
	constexpr static int Level_Completed = 1;
};

struct BoardSymbols {
	constexpr static char BIGSHIP = '#';
	constexpr static char SMALLSHIP = '@';
	constexpr static char WALL = 'W';
	constexpr static char GHOST = '%';
	constexpr static char HORIZONTAL_GHOST = '$';
	constexpr static char VERTICAL_GHOST = '!';
	constexpr static char WANDERING_GHOST = '%';
	constexpr static char SPECIAL_CHAR_WALL = 178;
	constexpr static char EMPTY = ' ';
	constexpr static char EXIT = 'E';
};

struct DisplayConstants {
	constexpr static int PosGameOverX = 10;
	constexpr static int PosGameOverY = 25;
	constexpr static int PosPauseX = 10;
	constexpr static int PosPauseY = 20;
};

struct ShipConstants {
	constexpr static char Big_Ship = 'b';
	constexpr static char Small_Ship = 's';
	constexpr static char No_Ship = ' ';
	constexpr static char Big_Ship_Figure = '#';
	constexpr static char Small_Ship_Figure = '@';
	constexpr static int Big_Ship_Size = 4;
	constexpr static int Small_Ship_Size = 2;
	constexpr static int Big_Ship_Max_Carry = 6;
	constexpr static int Small_Ship_Max_Carry = 2;
};

struct GhostConstants {
	constexpr static int Horizontal = 1;
	constexpr static int Vertical = 2;
	constexpr static int Wandering = 3;
};
