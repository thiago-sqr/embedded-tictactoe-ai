#include "minimax.h"
#include "actions.h"
#include "board.h"

// --------------------------------------------------------------------------
// Utility functions
// --------------------------------------------------------------------------

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

// --------------------------------------------------------------------------
// Minimax entry point
// --------------------------------------------------------------------------

Action minimax(const GameBoard& board) {

    // No move possible if the game is already over
    if (board.terminal())
        return NONE;

    Action best_action = NONE;

    // MAX player (X) tries to maximize the score
    if (board.player() == X) {
        int best_score = -INF;

        for (Action action : board.possible_actions()) {
            GameBoard next = board;
            next.make_move(action);

            int score = min_value(next);
            if (score > best_score) {
                best_score = score;
                best_action = action;
            }
        }
    }
    // MIN player (O) tries to minimize the score
    else {
        int best_score = INF;

        for (Action action : board.possible_actions()) {
            GameBoard next = board;
            next.make_move(action);

            int score = max_value(next);
            if (score < best_score) {
                best_score = score;
                best_action = action;
            }
        }
    }

    return best_action;
}

int max_value(const GameBoard& board) {

    // Terminal state: return the final score
    if (board.terminal())
        return board.scoring();

    int best_score = -INF;

    // Try all possible actions and choose the maximum value
    for (Action action : board.possible_actions()) {
        GameBoard next = board;
        next.make_move(action);

        best_score = max(best_score, min_value(next));
    }

    return best_score;
}

int min_value(const GameBoard& board) {

    // Terminal state: return the final score
    if (board.terminal())
        return board.scoring();

    int best_score = INF;

    // Try all possible actions and choose the minimum value
    for (Action action : board.possible_actions()) {
        GameBoard next = board;
        next.make_move(action);

        best_score = min(best_score, max_value(next));
    }

    return best_score;
}


