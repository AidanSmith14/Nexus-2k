#pragma once
#ifndef _BOARD
#define _BOARD

#include <vcclr.h>
#include <vector>
#include "Texas.h"

#define BLOCK_SIZE 40
#define BOARD_WIDTH 9
#define BOARD_HEIGHT 9

using namespace std;
using namespace System;

struct Cell
{
	int x;
	int y;
};

class Board
{
	Cell cells[BOARD_WIDTH][BOARD_HEIGHT];
	int mBoard[BOARD_HEIGHT][BOARD_WIDTH];
	vector<Cell> emptyCells;
	gcroot<Random^> random;
	Cell selected;
	int currentPieceColor;
	int score;
	int highScore;
	bool gameOver;
	Texas* texas;
public:
	Board();
	int cellContent(int, int);
	void pickCell(int x, int y);
	void initBoard();
	void draw();
	bool remove(int, int);
	void spawn();
	void currentFlick();
	void runGame();
	void boardUndo();
	bool GameOver();
	const int getScore() { return score; };
	const int HighScore() { return highScore; };
	~Board();
};

#endif

