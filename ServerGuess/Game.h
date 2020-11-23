#pragma once
#include <string>
#include <ctime>

class Game
{
private:

	std::string number;
	std::string entered;

	void setNumber(std::string number);

public:

	Game();

	void setEntered(std::string number);

	bool check();

	int findLetters();

	int getSame();

	std::string getNumber();

	std::string getResult();
};

