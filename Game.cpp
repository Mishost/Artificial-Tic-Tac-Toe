#include <iostream>
#include <thread> // std::this_thread::sleep_for
#include <chrono> //std::chrono::seconds
#include "Game.h"

Game::Game(unsigned int p1num, unsigned int p2num)
	:player1('X', p1num)
	,player2('O', p2num)
{
	board = new char *[ROWSNUM];
	for (unsigned int i = 0; i < ROWSNUM; ++i)
	{
		board[i] = new char[COLSNUM];
		for (unsigned int j = 0; j < COLSNUM; ++j)
			board[i][j] = ' ';
	}
}

Game::~Game()
{
	freeData();
}

void Game::start()
{
	bool flag = true;
	bool turn = true;
	print();
	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (flag)
		update(turn, flag);
}

void Game::update(bool& turn, bool& flag)
{
	if (turn)
	{
		player1.play(board);
		turn = false;
	}
	else
	{
		player2.play(board);
		turn = true;
	}
	print();
	if (player1.hasWon(board))
	{
		std::cout << "Player1 won!\n";
		flag = false;
	}
	else if (player2.hasWon(board))
	{
		std::cout << "Player2 won!\n";
		flag = false;
	}
	else if (gameWasTied())
	{
		std::cout << "It's a tie!\n";
		flag = false;
	}
	else
		std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Game::print() const
{
	for (unsigned int i = 0; i < ROWSNUM; ++i)
	{
		for (unsigned int j = 0; j < COLSNUM; ++j)
			std::cout << '|' << board[i][j];

		std::cout << '|' << '\n';
	}
	std::cout << '\n';
}

void Game::freeData()
{
	for (unsigned int i = 0; i < ROWSNUM; ++i)
	{
		delete[] board[i];
	}
	delete[] board;
}

bool Game::gameWasTied() const
{
	for (unsigned int i = 0; i < ROWSNUM; ++i)
		for (unsigned int j = 0; j < COLSNUM; ++j)
			if (board[i][j] == ' ') //this means we still have empty spaces, so the game can continue
				return false;
	return true;
}