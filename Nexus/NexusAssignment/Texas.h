#pragma once
#include <vector>

struct StateOfTexas
{
	int texasNo;
	int texasArray[9][9];
	int texasScore;
};

class Texas
{
	std::vector<StateOfTexas> texasVector;
	int currentState;
public:
	Texas();
	void addToTexas(const int(&mBoard)[9][9], const int &score);
	void removeFromTexas(int(&mBoard)[9][9],int &score);

	~Texas();
};

