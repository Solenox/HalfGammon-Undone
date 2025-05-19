# HalfGammon-Undone
// This program is a simplified version of backgammon called HalfGammon. Players take turns rolling a die and moving checkers based on the roll, with rules for bumping and bearing off. The game uses a 16-space board and includes a random number generator for rolling. A undo feature was added so players can go back one move at a time using a customed linked list.

#include <iostream>
#include <string>
#include "HalfGammonBoard.h"
#include "mersenne-twister.h"

using namespace std;

// Function declaration for rolling the die
int rollDie();

// Simulates rolling a die, choosing a result 1 to 6
// The seed function must have already been called
// Returns an int, chosen randomly, 1-6
int rollDie() {
	return chooseRandomNumber(1, 6);
}

// Helper function that checks if a string contains only digits (0-9)
bool isNumeric(const string& str) {
	for (char c : str) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return !str.empty();	// Ensures string is not empty
}

// Custom stack class to implement undo functionality
class UndoStack {
private:
	// Node struct represents a snapshot of the board and turn
	struct Node {
		int xState[18];		// State of player X's checkers
		int oState[18];		// State of player O's checkers
		bool isXTurn;		// true if it's player X's turn
		Node* next;			// pointer to next node in the stack

		// Constructor copies game state into the node
		Node(const int x[], const int o[], bool turn) : isXTurn(turn), next(nullptr) {
			for (int i = 0; i < 18; i++) {
				xState[i] = x[i];
				oState[i] = o[i];
			}
		}
	};
	Node* head;		// top of the undo stack

public:
	UndoStack() {
		head = nullptr;
	}
	
	// Destructor clears the entire stack
	~UndoStack() {
		while (head) {
			Node* temp = head;
			head = head->next;
			delete temp;
		}
	}

	// Saves current board state to the undo stack
	void push(const HalfGammonBoard& board) {
		int x[18];
		int o[18];
		bool turn;
		board.getState(x, o, turn);		// gets the current game state
		Node* new_node = new Node(x, o, turn);		// creates the new node with state
		new_node->next = head;		// links new node to current top
		head = new_node;		// updates top to new node
	}

	// Restores the most recent saved board state
	bool pop(HalfGammonBoard& board) {
		if (!head) {
			return false;	// nothing to undo
		}
		Node* temp = head;
		head = head->next;
		board.restoreState(temp->xState, temp->oState, temp->isXTurn);	// restore previous state
		delete temp;
		return true;
	}

	// Checks if the stack is empty
	bool isEmpty() const {
		return head == nullptr;
	}
};


int main() {
	// Initializes the random number generator with seed from the user
	int randSeed;
	cout << "Enter seed: ";
	cin >> randSeed;
	seed(randSeed);

	// Repeat, allows user to play multiple games
	string keepPlaying;
	do {
		// Create new game board and undo stack for each game
		HalfGammonBoard board;
		UndoStack undoStack;
		board.displayBoard();	// show intial board
		int roll = rollDie();	// roll the die
		board.displayRoll(roll);

		// Main game loop
		while (!board.gameOver()) {
			// Checks if the player has a bumped piece that must be moved
			
			if (board.hasBumpedPiece()) {
				cout << "Bumped checker must move." << endl;
				if (!board.isMovePossible(roll)) {
					cout << "No move possible." << endl;
				}
				else {
					board.moveBumpedPiece(roll);	// move the bumped checker
					board.changePlayer();			// switch turn
					if (!board.gameOver()) {		
						board.displayBoard();		// update board display
						roll = rollDie();			// roll for next player
						board.displayRoll(roll);
					}
				}
				continue;	// skip to next iteration
			}
			
			// If no bumped checker, check if any move is possible
			if (!board.isMovePossible(roll)) {
				cout << "No move possible." << endl << endl;
				board.changePlayer();	// switch player
				if (!board.gameOver()) {
					board.displayBoard();
					roll = rollDie();
					board.displayRoll(roll);
				}
				continue;
			}
					
			string movePositionString;
			cout << "What position would you like to move (Q to quit, U to undo)? ";
			cin >> movePositionString;

			// Checks if user wants to quit the game
			if (movePositionString == "q" || movePositionString == "Q") {
				break;
			} 
			// Handle undo input
			else if (movePositionString == "u" || movePositionString == "U") {
				if (!undoStack.pop(board)) {
					cout << "Cannot undo." << endl;		// nothing to undo
					continue;
				} else {
					board.displayBoard();
					roll = rollDie();
					board.displayRoll(roll);
				}
				continue;
			} 
			// Otherwise, handle normal move input
			else {
				if (!isNumeric(movePositionString)) {
					cout << "Invalid input." << endl;
					continue;
				}

				int movePosition = stoi(movePositionString);	// convert string to int
				undoStack.push(board);		// save current state in case undo is needed

				if (!board.performMove(movePosition, roll)) {
					cout << "Invalid move. Try again." << endl;
					undoStack.pop(board);	// remove failed move state
					continue;
				}

				board.changePlayer();	// switch turn
				if (!board.gameOver()) {
					board.displayBoard();
					roll = rollDie();
					board.displayRoll(roll);
				}
			}
		}

		// If we have left the loop, someone has won--determine whether it's X or O
		if (board.isXWin()) {
			cout << "Player X Wins!" << endl;
		}
		else if (board.isOWin()) {
			cout << "Player O Wins!" << endl;
		}

		cout << endl;
		cout << "Do you want to play again (y/n)? ";
		cin >> keepPlaying;
	} while (tolower(keepPlaying.at(0)) == 'y');	// repeat loop if user enters 'y'
}
