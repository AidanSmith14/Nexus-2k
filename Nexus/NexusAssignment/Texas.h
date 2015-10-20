#pragma once
#include <vector>
//header file for the Texas class

//struct that contains an array of the board, and the score
struct StateOfTexas
{
	int texasArray[9][9];
	int texasScore;
};

class Texas
{
	std::vector<StateOfTexas> texasVector;
	int currentState;
public:
	Texas();
	//method for adding the board and score to the vector
	void addToTexas(const int(&mBoard)[9][9], const int &score);
	//method for removing the board and score from the vector
	void removeFromTexas(int(&mBoard)[9][9],int &score);

	~Texas();
};

