#include <iostream>
#include "Game.h"

void getValidInput(unsigned int& p1strategy, unsigned int& p2strategy)
{
	do
	{
		std::cout << "Enter a valid strategy for the first AI: ";
		std::cin >> p1strategy;
		std::cout << "Enter a valid strategy for the second AI: ";
		std::cin >> p2strategy;
	} while (p1strategy < 0 || p2strategy < 0 || p1strategy > 4 || p2strategy > 4);
}

int main()
{
	unsigned int p1strategy, p2strategy;
	getValidInput(p1strategy, p2strategy);
	Game game(p1strategy, p2strategy);
	game.start();

	return 0;
}