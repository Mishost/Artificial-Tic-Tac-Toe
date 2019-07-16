#ifndef __PLAYER__HEADER__INCLUDED
#define __PLAYER__HEADER__INCLUDED

const unsigned int ROWSNUM = 3;
const unsigned int COLSNUM = 3;

class Player
{
public:
	Player(const char symbol, unsigned int strategy);
	~Player() = default;

	void play(char**& board) const;
	bool hasWon(char**& board) const;

private:
	void playFirstStrategy(char**& board) const;
	void playSecondStrategy(char**& board) const;
	void playThirdStrategy(char**& board) const;
	void playFourthStrategy(char**& board) const;

	bool canPlayToWin(char**& board) const;
	bool findPlace(char**& board, unsigned int row, unsigned int col) const;
	bool canWin(char**& board, char symbol, bool putSymbol) const;
	bool checkTileWin(char**& board, unsigned int row, unsigned int col,
		char symbol, bool putSymbol) const;
	bool checkMiddleTileWin(char**& board, unsigned int row, unsigned int col,
		char symbol, bool putSymbol) const;
	bool checkCornerTileWin(char**& board, unsigned int row, unsigned int col,
		char symbol, bool putSymbol) const;

	bool wonLine(char**& board, bool checkRow) const;
	char getOppSymbol() const;
	bool isCornerTile(unsigned int row, unsigned int col) const;
	bool isMiddleTile(unsigned int row, unsigned int col) const;
	bool rowContainsSymbol(char**& board, unsigned int row, char symbol) const;
	bool colContainsSymbol(char**& board, unsigned int col, char symbol) const;
	bool diagContainsSymbol(char**& board, unsigned int diag, char symbol) const;

	bool placeSymbolInRow(char**& board, unsigned int row) const;
	bool placeSymbolInCol(char**& board, unsigned int col) const;
	bool placeSymbolInDiag(char**& board, unsigned int diag) const;
private:
	unsigned int strategy;
	char mSymbol; //X or O
};
#endif // !__PLAYER__HEADER__INCLUDED
