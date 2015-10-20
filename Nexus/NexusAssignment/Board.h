#pragma once
#ifndef _BOARD
#define _BOARD
//header file for the board class

//needed for gcroot
#include <vcclr.h>
#include <vector>
#include "Texas.h"

//sets size of block, width and height of board
#define BLOCK_SIZE 40
#define BOARD_WIDTH 9
#define BOARD_HEIGHT 9

using namespace std;
using namespace System;

//struct containing x and y co ordinates of the cell
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
	//used to get a random in an unmanaged class
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
	//used to select the cell at the location clicked
	void pickCell(int x, int y);
	//initialises the board and the variables, reads in highscore
	void initBoard();
	void draw();
	//used to remove balls from the game
	bool remove(int, int);
	//spawns balls
	void spawn();
	//flickers currently selected ball
	void currentFlick();
	void runGame();
	//reverses the last move made
	void boardUndo();
	bool GameOver();
	const int getScore() { return score; };
	const int HighScore() { return highScore; };
	~Board();
};

#endif

