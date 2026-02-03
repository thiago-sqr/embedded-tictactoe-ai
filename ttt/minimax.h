#ifndef MINIMAX_H
#define MINIMAX_H

// ============================================================================
//  Minimax algorithm
// ----------------------------------------------------------------------------
//  This header is responsible for choosing the best move for the AI player
//  in a Tic-Tac-Toe game, given the current state of the GameBoard.
// ============================================================================

#include "actions.h"
#include "board.h"

const int INF = 1000;

/**
 * @brief Computes the optimal action for the current player using
 *        the minimax algorithm.
 *
 * @param board The current game board state.
 * @return The best Action for the current player.
 */
Action minimax(const GameBoard& board);

/**
 * @brief Evaluates the maximum value achievable from this board state.
 *        This function corresponds to the MAX player (X).
 *
 * @param board The current game board state.
 * @return The evaluated score of the board.
 */
int max_value(const GameBoard& board);

/**
 * @brief Evaluates the minimum value achievable from this board state.
 *        This function corresponds to the MIN player (O).
 *
 * @param board The current game board state.
 * @return The evaluated score of the board.
 */
int min_value(const GameBoard& board);

#endif
