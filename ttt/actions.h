#ifndef ACTIONS_H
#define ACTIONS_H

// =========================================================
// Action
// ---------------------------------------------------------
// An Action represents a move on the game board.
// It is defined by a pair of coordinates (i, j),
// corresponding to a board position that is not occupied.
//
// Two actions are considered equal if they have the same
// coordinates.
// =========================================================

struct Action {
    int i;  // Row index
    int j;  // Column index

    // Compares two actions for equality.
    // return true if both actions have the same coordinates.
    bool operator==(const Action& other) const;
};

// This can be returned when no valid action exists
const Action NONE = Action{-1, -1};

// =========================================================
// ActionSet
// ---------------------------------------------------------
// A lightweight set for Action objects.
// It is intentionally minimal and tailored for the game,
// supporting only the operations required by the minimax
// algorithm.
//
// Internally, it uses a fixed-size array, since the maximum
// number of possible actions in a 3x3 board is known (9).
// =========================================================

class ActionSet {
private:
    static constexpr int MAX = 9;  // Maximum number of actions
    Action vector[MAX];            // Storage for actions
    int stored = 0;                // Number of stored actions

public:

    // Checks whether the set is empty.
    bool is_empty() const;

    // Checks whether an action is contained in the set.
    bool contains(const Action& action) const;

    // Inserts an action into the set if it is not present.
    void insert(Action action);

    // Iterator support (non-const).
    Action* begin();
    Action* end();

    // Iterator support (const).
    const Action* begin() const;
    const Action* end() const;
};

#endif
