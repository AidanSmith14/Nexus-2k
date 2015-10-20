#include "stdafx.h"
#include "Board.h"
#include "Drawer.h"
#include "aStar.h"
//the cpp file for the board class


Board::Board()
:random(gcnew Random())
{
	initBoard();
}

//initialises the variables when game starts
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
		//reads in the highscore from the text file
		System::IO::StreamReader^ notePadHighScore = gcnew System::IO::StreamReader("Highscore.txt");
		highScore = System::Convert::ToInt32(notePadHighScore->ReadLine());
		notePadHighScore->Close();
	
}

//picks a cell using the x and y co ordinates of the mouse
void Board::pickCell(int x, int y)
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{	
			//draws a rectangle around the selected cell
			Rectangle^ rect = gcnew Rectangle(cells[i][j].x, cells[i][j].y, BLOCK_SIZE, BLOCK_SIZE);

			//checks for the cell with the rectangle
			if (rect->Contains(x, y))
			{
				//deselects the currently selected cell
				if (i == selected.x && j == selected.y)
				{
					selected.x = -1;
					selected.y = -1;
					mBoard[i][j] = currentPieceColor;
					currentPieceColor = -1;
				}
				else
				{
					//checks if the square is filled
					if (mBoard[i][j] !=0)
					{
						//checking if a cell is already selected
						if (selected.x != -1 && selected.y != -1)
						{
							//deselects the current cell
							mBoard[selected.x][selected.y] = currentPieceColor;
							selected.x = -1;
							selected.y = -1;
							currentPieceColor = -1;
						}
						//selects the new cell
						selected.x = i;
						selected.y = j;
						currentPieceColor = mBoard[i][j];
					}
					else
					{
						//A star path finding, (help from lewis with implementation)
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
							//adds tempPos to the empty cells vector
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
							//runs through the empty cell vector
							for (size_t a = 0; a < emptyCells.size(); a++)
							{
								//finds the empty cell the ball is being moved to
								if (emptyCells[a].x == tempX && emptyCells[a].y == tempY)
								{
									//removes the cell from the vector as it is no longer empty
									emptyCells.erase(emptyCells.begin() + a);
									//checks if balls have been deleted
									if (remove(tempX, tempY) != true)
									{
										//spawn method called
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

//contains the cells contents
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

//method used to delete balls from the board
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

	//Checking left for balls of the same colour
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

	//Checking right for balls of the same colour
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

	//checks if there is 5 balls in a row of the same colour
	if (count < 5)
	{
		//adds balls to be deleted
		for (int i = 0; i < count - 1; i++)
		{
			deleteCells.pop_back();
		}
	}

	count = 1;
	tempColor = -1;
	tempX = x;
	tempY = y;

	//Checking up for balls of the same colour
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

	//Checking down for balls of the same colour
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
	
	//checks if there is 5 balls in a row of the same colour
	if (count < 5)
	{
		//adds balls to be deleted
		for (int i = 0; i < count - 1; i++)
		{
			deleteCells.pop_back();
		}
	}

	count = 1;
	tempColor = -1;
	tempX = x;
	tempY = y;

	//Checking top right for balls of the same colour
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

	//Checking bottom left for balls of the same colour
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

	//checks if there is 5 balls in a row of the same colour
	if (count < 5)
	{
		//adds balls to be deleted
		for (int i = 0; i < count - 1; i++)
		{
			deleteCells.pop_back();
		}
	}

	count = 1;
	tempColor = -1;
	tempX = x;
	tempY = y;

	//Checking top left for balls of the same colour
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

	//Checking bottom right for balls of the same colour
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

	//checks if there is 5 balls in a row of the same colour
	if (count < 5)
	{
		//adds balls to be deleted
		for (int i = 0; i < count - 1; i++)
		{
			deleteCells.pop_back();
		}
	}

	//checks if there is enough balls to delete
	if (deleteCells.size() >= 4)
	{
		removed = true;
		//flickers balls 4 times before deleting them
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
			//adds the deleted balls cells back into empty cells vector
			emptyCells.push_back(tempPos);
			//adds 100 for each ball deleted
			score = score + 100;
		}
		//checks if an entire row has been deleted
		if (deleteCells.size() == 9)
		{
			Windows::Forms::MessageBox::Show("Full Line Clear! Double Points!!!");
			//double points for entire row clear
			score = score + 900;
		}
		if (selected.x != -1 && selected.y != -1)
		{
			mBoard[selected.x][selected.y] = currentPieceColor;
			selected.x = -1;
			selected.y = -1;
			currentPieceColor = -1;
		}
		//adds to state for undo purposes
		texas->addToTexas(mBoard, score);
	}
	//clears the deleted cells vector
	deleteCells.clear();
	return removed;
}


//makes the currently selected ball flicker
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

//goes back one move
void Board::boardUndo()
{
	if (selected.x != -1 && selected.y != -1)
	{
		mBoard[selected.x][selected.y] = currentPieceColor;
		selected.x = -1;
		selected.y = -1;
		currentPieceColor = -1;
	}
	//removes the most recent state
	texas->removeFromTexas(mBoard, score);
	//resets the empty cells vector
	emptyCells.clear();

	//re populates the empty cells vector
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

//method for spawning new balls
void Board::spawn()
{
	//runs 3 times to spawn 3 balls
	for (int i = 0; i < 3; i++)
	{
		//checks if there is any empty cells
		if (!emptyCells.empty())
		{
			//finds a random location from the empty cells
			int location = random->Next(emptyCells.size());
			//gives it a random color
			int color = random->Next(1, 7);
			mBoard[emptyCells[location].x][emptyCells[location].y] = color;
			int tempX = emptyCells[location].x;
			int tempY = emptyCells[location].y;
			//removes the new balls cell from the empty cells vector
			emptyCells.erase(emptyCells.begin() + location);
			remove(tempX, tempY);
			//checks if the empty cells vector is empty
			if (emptyCells.empty())
			{
				//displays losing message
				System::Windows::Forms::MessageBox::Show("You Lost! your final score was " + score);
				//checks if highscore has been beaten
				if (score > highScore)
				{
					//feedback for new highscore
					System::Windows::Forms::MessageBox::Show("You set the new highscore!");
					highScore = score;
					//saves new highscore to text file
					System::IO::StreamWriter^ notePadHighScore = gcnew System::IO::StreamWriter("Highscore.txt");
					notePadHighScore->WriteLine(score);
					notePadHighScore->Close();
					score = 0;
				}
				//sets game over to true
				gameOver = true;
				return;
			}
		}
		else
		{
			//message displayed for losing
			System::Windows::Forms::MessageBox::Show("You Lost! your final score was " + score);
			//checks if highscore has been beaten
			if (score > highScore)
			{
				//feedback for new highscore
				System::Windows::Forms::MessageBox::Show("You set the new highscore!");
				highScore = score;
				//saves new highscore to text file
				System::IO::StreamWriter^ notePadHighScore = gcnew System::IO::StreamWriter("Highscore.txt");
				notePadHighScore->WriteLine(score);
				notePadHighScore->Close();
				score = 0;
			}
			//sets game over to true
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
		//adds new state
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
