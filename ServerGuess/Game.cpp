#include "Game.h"

Game::Game()
{
	std::string number = "";

	srand(time(0));

	for (int i = 0; i < 4; ++i)
	{
		number += std::to_string(rand() % 10);
	}
	this->number = number;
	this->entered = "";
}

void Game::setNumber(std::string number)
{
	this->number = number;
}

void Game::setEntered(std::string number)
{
	this->entered = number;
}

bool Game::check()
{
	return entered == this->number;
}

int Game::findLetters()
{
	int res = 0;
	for (char item : entered)
	{
		if (number.find(item) < number.length())
			++res;
	}
	return res;
}

int Game::getSame()
{
	int res = 0;
	int i = 0;
	for (char item : entered)
	{
		if (number[i] == item)
			++res;
		++i;
	}
	return res;
}

std::string Game::getResult()
{
	if (check())
	{
		return "Right!";
	}

	std::string res = "Found: ";
	res += std::to_string(findLetters());
	res += " Same: ";
	res += std::to_string(getSame());

	return res;
}

std::string Game::getNumber()
{
	return this->number;
}

