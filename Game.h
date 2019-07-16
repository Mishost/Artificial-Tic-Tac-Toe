#ifndef __GAME__HEADER__INCLUDED
#define __GAME__HEADER__INCLUDED

#include "Player.h"
class Game
{
public:
	Game(unsigned int p1num, unsigned int p2num);
	~Game();

	void start();

private:
	void update(bool& turn, bool& flag);
	void print() const;
	void freeData();
	bool gameWasTied() const;

private:
	Player player1, player2;
	char** board;
};

#endif // !__GAME__HEADER__INCLUDED
