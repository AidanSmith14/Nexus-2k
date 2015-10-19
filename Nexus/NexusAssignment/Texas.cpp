#include "stdafx.h"
#include "Texas.h"


Texas::Texas()
{
	currentState = -1;
}

void Texas::addToTexas(const int(&mBoard)[9][9],const  int &score)
{
	currentState = currentState + 1;
	StateOfTexas stateOfTexas;
	stateOfTexas.texasNo = currentState;
	for (size_t i = 0; i < 9; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			stateOfTexas.texasArray[i][j] = mBoard[i][j];
		}
	}	
	stateOfTexas.texasScore = score;
	texasVector.push_back(stateOfTexas);
}

void Texas::removeFromTexas(int(&mBoard)[9][9],int &score)
{
	if (currentState > 0)
	{
		currentState = currentState - 1;
		score = texasVector[currentState].texasScore;
		for (size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				mBoard[i][j] = texasVector[currentState].texasArray[i][j];
			}
		}
		texasVector.pop_back();
	}
	else
	{
		System::Windows::Forms::MessageBox::Show("You're at the start of the game!");
	}

}

Texas::~Texas()
{
	texasVector.clear();
	delete &currentState;
}
