#include "stdafx.h"
#include "Board.h"
#include "Drawer.h"
#include "aStar.h"



Board::Board()
{
	initBoard();
	random = gcnew Random();
}


void Board::initBoard()
{
	texas = new Texas();
	emptyCells.clear();
	gameOver = false;
	selected.x = -1;
	selected.y = -1;
	currentPieceColor = -1;
	score = 0;
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			mBoard[i][j] = 0;
			cells[i][j].x = i * BLOCK_SIZE;
			cells[i][j].y = j * BLOCK_SIZE;

			Cell tempCell;
			tempCell.x = i;
			tempCell.y = j;
			emptyCells.push_back(tempCell);
		}
	}

	System::IO::StreamReader^ notePadHighScore = gcnew System::IO::StreamReader("Highscore.txt");
	highScore = System::Convert::ToInt32(notePadHighScore->ReadLine());
	notePadHighScore->Close();
	
}

void Board::pickCell(int x, int y)
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			Rectangle^ rect = gcnew Rectangle(cells[i][j].x, cells[i][j].y, BLOCK_SIZE, BLOCK_SIZE);
			if (rect->Contains(x, y))
			{
				if (i == selected.x && j == selected.y)
				{
					selected.x = -1;
					selected.y = -1;
					mBoard[i][j] = currentPieceColor;
					currentPieceColor = -1;
				}
				else
				{
					if (mBoard[i][j] !=0)
					{
						if (selected.x != -1 && selected.y != -1)
						{
							mBoard[selected.x][selected.y] = currentPieceColor;
							selected.x = -1;
							selected.y = -1;
							currentPieceColor = -1;
						}
						selected.x = i;
						selected.y = j;
						currentPieceColor = mBoard[i][j];
					}
					else
					{
						string route = aStar::pathFind(selected.x, selected.y, i, j, mBoard);
						if (route.length() > 0)
						{
							int k; char c;
							int tempX = selected.x;
							int tempY = selected.y;
							mBoard[tempX][tempY] = 0;
							Cell tempPos;
							tempPos.x = tempX;
							tempPos.y = tempY;
							emptyCells.push_back(tempPos);

							for (size_t a = 0; a < route.length(); a++)
							{
								c = route.at(a);
								k = atoi(&c);
								tempX = tempX + dx[k];
								tempY = tempY + dy[k];
								mBoard[tempX][tempY] = currentPieceColor;
								draw();
								System::Threading::Thread::Sleep(100);
								mBoard[tempX][tempY] = 0;
							}
							mBoard[tempX][tempY] = currentPieceColor;
							selected.x = -1;
							selected.y = -1;
							currentPieceColor = -1;
							for (size_t a = 0; a < emptyCells.size(); a++)
							{
								if (emptyCells[a].x == tempX && emptyCells[a].y == tempY)
								{
									emptyCells.erase(emptyCells.begin() + a);
									if (remove(tempX, tempY) != true)
									{
										spawn();
									}									
									return;
								}
							}
						}
					}
				}
				return;
			}
		}
	}
}

int Board::cellContent(int x, int y)
{
	return mBoard[x][y];
}

void Board::runGame()
{
	draw();
	currentFlick();
}

void Board::draw()
{
	Drawer::draw(*this);
}

bool Board::remove(int x, int y)
{
	bool removed = false;
	std::vector<Cell> deleteCells;
	int tempX = x;
	int tempY = y;
	int color = mBoard[x][y];
	int tempColor = -1;
	int count = 1;

	Cell tempCell;
	tempCell.x = tempX;
	tempCell.y = tempY;
	deleteCells.push_back(tempCell);

	//Checking left
	do
	{
		tempX -= 1;
		if (tempX >= 0 && tempX < 9 && tempY >= 0 && tempY < 9)
		{
			tempColor = mBoard[tempX][tempY];
			if (mBoard[tempX][tempY] == color)
			{
				count += 1;
				Cell tempCell;
				tempCell.x = tempX;
				tempCell.y = tempY;
				deleteCells.push_back(tempCell);
			}
		}
		else
		{
			tempColor = -1;
		}
	} while (tempColor == color);

	//Checking right
	tempX = x;
	do
	{
		tempX += 1;
		if (tempX >= 0 && tempX < 9 && tempY >= 0 && tempY < 9)
		{
			tempColor = mBoard[tempX][tempY];
			if (mBoard[tempX][tempY] == color)
			{
				count += 1;
				Cell tempCell;
				tempCell.x = tempX;
				tempCell.y = tempY;
				deleteCells.push_back(tempCell);
			}
		}
		else
		{
			tempColor = -1;
		}
	} while (tempColor == color);

	if (count < 5)
	{
		for (int i = 0; i < count - 1; i++)
		{
			deleteCells.pop_back();
		}
	}

	count = 1;
	tempColor = -1;
	tempX = x;
	tempY = y;

	//Checking up
	do
	{
		tempY -= 1;
		if (tempX >= 0 && tempX < 9 && tempY >= 0 && tempY < 9)
		{
			tempColor = mBoard[tempX][tempY];
			if (mBoard[tempX][tempY] == color)
			{
				count += 1;
				Cell tempCell;
				tempCell.x = tempX;
				tempCell.y = tempY;
				deleteCells.push_back(tempCell);
			}
		}
		else
		{
			tempColor = -1;
		}
	} while (tempColor == color);

	//Checking down
	tempY = y;
	do
	{
		tempY += 1;
		if (tempX >= 0 && tempX < 9 && tempY >= 0 && tempY < 9)
		{
			tempColor = mBoard[tempX][tempY];
			if (mBoard[tempX][tempY] == color)
			{
				count += 1;
				Cell tempCell;
				tempCell.x = tempX;
				tempCell.y = tempY;
				deleteCells.push_back(tempCell);
			}
		}
		else
		{
			tempColor = -1;
		}
	} while (tempColor == color);

	if (count < 5)
	{
		for (int i = 0; i < count - 1; i++)
		{
			deleteCells.pop_back();
		}
	}

	count = 1;
	tempColor = -1;
	tempX = x;
	tempY = y;

	//Checking top right
	do
	{
		tempY -= 1;
		tempX += 1;
		if (tempX >= 0 && tempX < 9 && tempY >= 0 && tempY < 9)
		{
			tempColor = mBoard[tempX][tempY];
			if (mBoard[tempX][tempY] == color)
			{
				count += 1;
				Cell tempCell;
				tempCell.x = tempX;
				tempCell.y = tempY;
				deleteCells.push_back(tempCell);
			}
		}
		else
		{
			tempColor = -1;
		}
	} while (tempColor == color);

	//Checking bottom left
	tempX = x;
	tempY = y;
	do
	{
		tempY += 1;
		tempX -= 1;
		if (tempX >= 0 && tempX < 9 && tempY >= 0 && tempY < 9)
		{
			tempColor = mBoard[tempX][tempY];
			if (mBoard[tempX][tempY] == color)
			{
				count += 1;
				Cell tempCell;
				tempCell.x = tempX;
				tempCell.y = tempY;
				deleteCells.push_back(tempCell);
			}
		}
		else
		{
			tempColor = -1;
		}
	} while (tempColor == color);

	if (count < 5)
	{
		for (int i = 0; i < count - 1; i++)
		{
			deleteCells.pop_back();
		}
	}

	count = 1;
	tempColor = -1;
	tempX = x;
	tempY = y;

	//Checking top left
	do
	{
		tempY -= 1;
		tempX -= 1;
		if (tempX >= 0 && tempX < 9 && tempY >= 0 && tempY < 9)
		{
			tempColor = mBoard[tempX][tempY];
			if (mBoard[tempX][tempY] == color)
			{
				count += 1;
				Cell tempCell;
				tempCell.x = tempX;
				tempCell.y = tempY;
				deleteCells.push_back(tempCell);
			}
		}
		else
		{
			tempColor = -1;
		}
	} while (tempColor == color);

	//Checking bottom right
	tempX = x;
	tempY = y;
	do
	{
		tempY += 1;
		tempX += 1;
		if (tempX >= 0 && tempX < 9 && tempY >= 0 && tempY < 9)
		{
			tempColor = mBoard[tempX][tempY];
			if (mBoard[tempX][tempY] == color)
			{
				count += 1;
				Cell tempCell;
				tempCell.x = tempX;
				tempCell.y = tempY;
				deleteCells.push_back(tempCell);
			}
		}
		else
		{
			tempColor = -1;
		}
	} while (tempColor == color);

	if (count < 5)
	{
		for (int i = 0; i < count - 1; i++)
		{
			deleteCells.pop_back();
		}
	}

	if (deleteCells.size() >= 4)
	{
		removed = true;
		for (int j = 0; j < 4; j++)
		{

			for (size_t i = 0; i < deleteCells.size(); i++)
			{
				if (mBoard[deleteCells[i].x][deleteCells[i].y] != color)
				{
					mBoard[deleteCells[i].x][deleteCells[i].y] = color;
				}
				else
				{
					mBoard[deleteCells[i].x][deleteCells[i].y] = 7;
				}

			}
			draw();
			System::Threading::Thread::Sleep(100);
		}

		for (size_t i = 0; i < deleteCells.size(); i++)
		{
			
			mBoard[deleteCells[i].x][deleteCells[i].y] = 0;
			Cell tempPos;
			tempPos.x = deleteCells[i].x;
			tempPos.y = deleteCells[i].y;
			emptyCells.push_back(tempPos);
			score = score + 100;
		}
		if (deleteCells.size() == 9)
		{
			Windows::Forms::MessageBox::Show("Full Line Clear! Double Points!!!");
			score = score + 900;
		}
		if (selected.x != -1 && selected.y != -1)
		{
			mBoard[selected.x][selected.y] = currentPieceColor;
			selected.x = -1;
			selected.y = -1;
			currentPieceColor = -1;
		}
		texas->addToTexas(mBoard, score);
	}
	deleteCells.clear();
	return removed;
}

void Board::currentFlick()
{
	if (selected.x != -1 && selected.y != -1)
	{
		if (mBoard[selected.x][selected.y] != currentPieceColor)
		{
			mBoard[selected.x][selected.y] = currentPieceColor;
		}
		else
		{
			mBoard[selected.x][selected.y] = 7;
		}
	}
}

bool Board::GameOver()
{
	return gameOver;
}

void Board::boardUndo()
{
	if (selected.x != -1 && selected.y != -1)
	{
		mBoard[selected.x][selected.y] = currentPieceColor;
		selected.x = -1;
		selected.y = -1;
		currentPieceColor = -1;
	}
	texas->removeFromTexas(mBoard, score);

	emptyCells.clear();

	for (size_t i = 0; i < BOARD_HEIGHT; i++)
	{
		for (size_t j = 0; j < BOARD_WIDTH; j++)
		{
			if (mBoard[i][j] == 0)
			{
				Cell tempPos;
				tempPos.x = i;
				tempPos.y = j;
				emptyCells.push_back(tempPos);
			}
		}
	}
}


void Board::spawn()
{
	for (int i = 0; i < 3; i++)
	{
		if (!emptyCells.empty())
		{
			int location = random->Next(emptyCells.size());
			int color = random->Next(1, 7);
			mBoard[emptyCells[location].x][emptyCells[location].y] = color;
			int tempX = emptyCells[location].x;
			int tempY = emptyCells[location].y;
			emptyCells.erase(emptyCells.begin() + location);
			remove(tempX, tempY);
			if (emptyCells.empty())
			{
				System::Windows::Forms::MessageBox::Show("You Lost! your final score was " + score);
				if (score > highScore)
				{
					System::Windows::Forms::MessageBox::Show("You set the new highscore!");
					highScore = score;
					System::IO::StreamWriter^ notePadHighScore = gcnew System::IO::StreamWriter("Highscore.txt");
					notePadHighScore->WriteLine(score);
					notePadHighScore->Close();
					score = 0;
				}
				gameOver = true;
				return;
			}
		}
		else
		{
			System::Windows::Forms::MessageBox::Show("You Lost! your final score was " + score);
			if (score > highScore)
			{
				System::Windows::Forms::MessageBox::Show("You set the new highscore!");
				highScore = score;
				System::IO::StreamWriter^ notePadHighScore = gcnew System::IO::StreamWriter("Highscore.txt");
				notePadHighScore->WriteLine(score);
				notePadHighScore->Close();
				score = 0;
			}
			gameOver = true;
			return;
		}
	}
	if (selected.x != -1 && selected.y != -1)
	{
		mBoard[selected.x][selected.y] = currentPieceColor;
		selected.x = -1;
		selected.y = -1;
		currentPieceColor = -1;
	}
		texas->addToTexas(mBoard, score);
}

Board::~Board()
{
	delete random;
	delete[] &mBoard;
	delete[] &cells;
	emptyCells.clear();
	delete[] &emptyCells;
	delete &score;
	delete &selected;
	delete &currentPieceColor;
	delete &texas;
}
