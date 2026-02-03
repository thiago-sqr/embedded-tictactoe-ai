#ifndef BOARD_H
#define BOARD_H

// ============================================================================
// GameBoard
// ----------------------------------------------------------------------------
// This header defines the GameBoard class, which represents the state and
// rules of a Tic-Tac-Toe game. The board is a fixed 3x3 grid and provides
// utilities to query game state, apply moves, and evaluate outcomes.
// ============================================================================

#include "actions.h"

// ============================================================================
// Game constants
// ============================================================================

// Board dimension (3x3 Tic-Tac-Toe)
const int SIZE = 3;

// Player symbols
const char X    = 'X';
const char O    = 'O';

// Empty cell symbol
const char VOID = ' ';

// ============================================================================
// GameBoard class
// ============================================================================

class GameBoard {

private:
    // Internal board representation: board[row][column]
    char board[SIZE][SIZE];

public:
    // ------------------------------------------------------------------------
    // Construction and initialization
    // ------------------------------------------------------------------------

    // Creates an empty board
    GameBoard();

    // Clears the board, setting all cells to VOID
    void clear();

    // ------------------------------------------------------------------------
    // Operators
    // ------------------------------------------------------------------------

    // Read-only access to a board cell. Example: board(i, j)
    char operator()(int i, int j) const;

    // Compares two boards for equality
    bool operator==(const GameBoard& other) const;

    // Compares two boards for inequality
    bool operator!=(const GameBoard& other) const;

    // Assignment operator (deep copy)
    GameBoard& operator=(const GameBoard& other);

    // ------------------------------------------------------------------------
    // Game mechanics
    // ------------------------------------------------------------------------

    // Returns the set of all legal actions available on the board
    ActionSet possible_actions() const;

    // Applies a move to the board if it is valid
    void make_move(Action action);

    // ------------------------------------------------------------------------
    // Game evaluation
    // ------------------------------------------------------------------------

    // Returns true if the board is in the initial (empty) state
    bool initial() const;

    // Counts how many times a given symbol appears on the board
    int count_turns(char c) const;

    // Returns the player who has the next turn (X or O)
    char player() const;

    // Returns the winner of the game (X, O, or VOID if none)
    char winner() const;

    // Returns true if the game is over (win or draw)
    bool terminal() const;

    // Returns the score of the board:
    //  +1 if X wins
    //  -1 if O wins
    //   0 otherwise
    int scoring() const;
};

#endif
