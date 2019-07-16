#include <random> //for the first strategy
#include <time.h> //for the first strategy
#include "Player.h"

Player::Player(const char symbol, unsigned int strategy)
	:mSymbol(symbol)
	,strategy(strategy)
{
	srand(time(NULL)); //in order to choose random places when playing the first strategy
}

void Player::play(char**& board) const
{
	switch (strategy)
	{
	case 1:
		playFirstStrategy(board);
		break;
	case 2:
		playSecondStrategy(board);
		break;
	case 3:
		playThirdStrategy(board);
		break;
	case 4:
		playFourthStrategy(board);
		break;
	}
}

bool Player::hasWon(char **& board) const
{
	if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[2][2] == mSymbol) //first diagonal
		|| (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[2][0] == mSymbol) //second diagonal
		|| wonLine(board, 1) || wonLine(board, 0))  //we check if we have 3 of the same in a row and then in a col
		return true;
	return false;
}

void Player::playFirstStrategy(char**& board) const
{
	unsigned int row;
	unsigned int col;
	do
	{
		row = rand() % ROWSNUM;
		col = rand() % COLSNUM;
	} while (board[row][col] != ' ');

	board[row][col] = mSymbol;
}

void Player::playSecondStrategy(char **& board) const
{
	char oppSymbol = getOppSymbol();
	if (!canWin(board, oppSymbol, true))
		playFirstStrategy(board);
}

void Player::playThirdStrategy(char **& board) const
{
	if (!canWin(board, mSymbol, true)) //first we try to win directly
	{
		//then we just need to create a pair with another one of our symbols without being blocked by the opponent
		if (!canPlayToWin(board))
			playFirstStrategy(board); //if we cannot create a good pair, we just play at random
	}
}

void Player::playFourthStrategy(char **& board) const
{
	char oppSymbol = getOppSymbol();
	if (!canWin(board, oppSymbol, true)) //if the opponent cannot beat us, we try to beat him
		playThirdStrategy(board);
}

bool Player::canPlayToWin(char **& board) const
{
	for (unsigned int i = 0; i < ROWSNUM; ++i)
		for (unsigned int j = 0; j < COLSNUM; ++j)
			if (board[i][j] == mSymbol)
				if (findPlace(board, i, j)) //whenever we find a symbol of ours, we try to find a pair for it
					return true;
	return false;
}

bool Player::findPlace(char**& board, int row, int col) const
{
	char oppSymbol = getOppSymbol();
	for (unsigned int i = 0; i < ROWSNUM; ++i)
	{
		//we can create a pair on a row
		if (!rowContainsSymbol(board, row, oppSymbol) && rowContainsSymbol(board, row, ' '))
		{
			placeSymbolInRow(board, i);
			return true;
		}
		//on a column
		else if (!colContainsSymbol(board, col, oppSymbol) && colContainsSymbol(board, col, ' '))
		{
			placeSymbolInCol(board, i);
			return true;
		}
		//or on a diagonal
		else if (i > 0 && !diagContainsSymbol(board, i, oppSymbol) && diagContainsSymbol(board, i, ' '))
		{
			placeSymbolInDiag(board, i);
			return true;
		}
	}
	return false;
}

bool Player::canWin(char**& board, char symbol, bool putSymbol) const
{
	//we use this method both for our player and the opponent
	//the putSymbol variable lets us know if we have to react and place our symbol
	//which will either guarantee us the win or prevent the opponent from winning
	for (int i = 0; i < ROWSNUM; ++i)
		for (int j = 0; j < COLSNUM; ++j)
			if (board[i][j] == symbol)
				if (checkTileWin(board, i, j, symbol, putSymbol))
					return true;
	return false;
}

bool Player::checkTileWin(char**& board, int row, int col, char symbol, bool putSymbol) const
{
	//we avoid checking the center tile because we cover it in the other cases
	if (isCornerTile(row, col))
		return checkCornerTileWin(board, row, col, symbol, putSymbol);
	else if (isMiddleTile(row, col))
		return checkMiddleTileWin(board, row, col, symbol, putSymbol);
	return false;
}

bool Player::checkMiddleTileWin(char**& board, int row, int col, char symbol, bool putSymbol) const
{
	int rowM, colM, rowAddition, colAddition;
	rowAddition = colAddition = 0;

	if (row == 0)
		rowM = 1;
	else if (row == 1)
		rowM = 0;
	else
		rowM = -1;

	if (col == 0)
		colM = 1;
	else if (col == 1)
		colM = 0;
	else
		colM = -1;

	//we only have 1 row and col which do not involve corner tiles so we have only 2 cases
	if (board[row][col] == board[row + 1 * rowM][col + 1 * colM]
		&& board[row + 2 * rowM][col + 2 * colM] == ' ')
	{
		rowAddition = 2 * rowM;
		colAddition = 2 * colM;
	}
	else if (board[row][col] == board[row + 2 * rowM][col + 2 * colM]
		&& board[row + 1 * rowM][col + 1 * colM] == ' ')
	{
		rowAddition = 1 * rowM;
		colAddition = 1 * colM;
	}
	else
		return false;
	if (putSymbol)
		board[row + rowAddition][col + colAddition] = mSymbol;
	return true;

}

bool Player::checkCornerTileWin(char**& board, int row, int col, char symbol, bool putSymbol) const
{
	//depending on the position of the tile, we need to add or subtract the same values
	int rowM, colM, rowAddition, colAddition;
	rowAddition = colAddition = 0;
	row == 0 ? rowM = 1 : rowM = -1;
	col == 0 ? colM = 1 : colM = -1;

	//for every corner tile we have 2 possible pairs on the same row, 2 on the same column
	//and 2 on the same diagonal = 6 cases
	if (board[row][col + 1 * colM] == symbol
		&& board[row][col + 2 * colM] == ' ')
		colAddition = 2 * colM;
	else if (board[row][col + 2 * colM] == symbol
		&& board[row][col + 1 * colM] == ' ')
		colAddition = 1 * colM;
	else if (board[row + 2 * rowM][col] == symbol
		&& board[row + 1 * rowM][col] == ' ')
		rowAddition = 1 * rowM;
	else if (board[row + 1 * rowM][col] == symbol
		&& board[row + 2 * rowM][col] == ' ')
		rowAddition = 2 * rowM;
	else if (board[row + 1 * rowM][col + 1 * colM] == symbol
		&& board[row + 2 * rowM][col + 2 * colM] == ' ')
	{
		rowAddition = 2 * rowM;
		colAddition = 2 * colM;
	}
	else if (board[row + 2 * rowM][col + 2 * colM] == symbol
		&& board[row + 1 * rowM][col + 1 * colM] == ' ')
	{
		rowAddition = 1 * rowM;
		colAddition = 1 * colM;
	}
	else
		return false;

	if (putSymbol)
		board[row + rowAddition][col + colAddition] = mSymbol;
	return true;
}

bool Player::wonLine(char**& board, bool checkRow) const
{
	bool flag;

	for (unsigned int i = 0; i < ROWSNUM; ++i)
	{
		flag = true;
		for (unsigned int j = 1; j < COLSNUM; ++j)
		{
			//we are eliminating possible winning lines by checking if 2 nearby tiles are different
			//or if they are not both our symbol
			if (checkRow && (board[i][j] != board[i][j - 1] || board[i][j] != mSymbol))
				flag = false;
			else if (!checkRow && (board[j][i] != board[j - 1][i] || board[j][i] != mSymbol))
				flag = false;
		}
		if (flag)
			return true;
	}
	return false;
}

char Player::getOppSymbol() const
{
	return 'X' == mSymbol ? 'O' : 'X';
}

bool Player::isCornerTile(int row, int col) const
{
	//the sum of the rows and cols of a corner tile is always an even number
	//we don't include the center tile by making sure that the row and col are not both 1s
	if ((row + col) % 2 == 0 && row != 1 && col != 1)
		return true;
	return false;
}

bool Player::isMiddleTile(int row, int col) const
{
	if (!isCornerTile(row, col) && (row + col) != 2) //we check their sum in order to exclude the center piece
		return true;
	return false;
}

bool Player::rowContainsSymbol(char**& board, int row, char symbol) const
{
	for (unsigned int i = 0; i < COLSNUM; ++i)
		if (board[row][i] == symbol)
			return true;
	return false;
}

bool Player::colContainsSymbol(char**& board, int col, char symbol) const
{
	for (unsigned int i = 0; i < ROWSNUM; ++i)
		if (board[i][col] == symbol)
			return true;
	return false;
}

bool Player::diagContainsSymbol(char**& board, int diag, char symbol) const
{
	for (unsigned int i = 0; i < ROWSNUM; ++i)
	{
		if (diag == 1 && board[i][i] == symbol)
			return true;
		else if (diag == 2 && board[ROWSNUM - i - 1][ROWSNUM - i - 1] == symbol)
			return true;
	}
	return false;
}

void Player::placeSymbolInRow(char**& board, int row) const
{
	for (unsigned int i = 0; i < COLSNUM; ++i)
	{
		if (board[row][i] == ' ')
		{
			board[row][i] = mSymbol;
			if (canWin(board, mSymbol, false))
				break;
			else //if we are not making a valuable pair, we should try another
				board[row][i] = ' ';
		}
	}
}

void Player::placeSymbolInCol(char**& board, int col) const
{
	for (unsigned int i = 0; i < ROWSNUM; ++i)
	{
		if (board[i][col] == ' ')
		{
			board[i][col] = mSymbol;
			if (canWin(board, mSymbol, false)) 
				break;
			else //if we are not making a valuable pair, we should try another
				board[i][col] = ' '; 
		}
	}
}

void Player::placeSymbolInDiag(char**& board, int diag) const
{
	for (unsigned int i = 0; i < ROWSNUM; ++i)
	{
		if (diag == 1 && board[i][i] == ' ')
		{
			board[i][i] = mSymbol;
			if (canWin(board, mSymbol, false)) 
				return;
			else //if we are not making a valuable pair, we should try another
				board[i][i] = ' ';
		}
		else if (diag == 2 && board[ROWSNUM - i - 1][ROWSNUM - i - 1] == ' ')
		{
			board[ROWSNUM - i - 1][ROWSNUM - i - 1] = mSymbol;
			if (canWin(board, mSymbol, false)) 
				return;
			else //if we are not making a valuable pair, we should try another
				board[ROWSNUM - i - 1][ROWSNUM - i - 1] = ' ';
		}
	}
}
