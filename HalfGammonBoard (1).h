#pragma once
class HalfGammonBoard
{
private:
	static const int VALID_SPACES = 16;
	static const int ARRAY_SIZE = 18;
	static const int O_BUMP_LOCATION = 17;
	static const int X_BUMP_LOCATION = 0;
	static const int O_HOME_LOCATION = 0;
	static const int X_HOME_LOCATION = 17;

	int xBoard[ARRAY_SIZE];
	int oBoard[ARRAY_SIZE];
	bool xTurn;

	bool performXMove(int movePosition, int moveSize);
	bool performOMove(int movePosition, int moveSize);

public:
	HalfGammonBoard();
	void displayBoard();
	void displayRoll(int roll);
	bool gameOver();
	bool isMovePossible(int move);
	bool performMove(int movePosition, int moveSize);
	bool moveBumpedPiece(int moveSize);
	bool isXWin();
	bool isOWin();
	bool isValidDestination(int position);
	bool hasBumpedPiece();
	void changePlayer();
	// Copies the current game state into the provided arrays and bool reference
	// x_pos and o_pos will recieve the positions of X and O checekrs
	// is_x_turn will recieve whos turn it currently is.
	void getState(int x_pos[], int o_pos[], bool& is_x_turn) const; 

	// Restores the game board to a previous state
	// x_pos and o_pos represent the saved checker positions
	// is_x_turn tells whose turn it should be after restoring.	
	void restoreState(const int x_pos[], const int o_pos[], bool is_x_turn);	// Restores the game board to a previous state
};

