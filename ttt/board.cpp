#include "board.h"

// --------------------------------------------------------------------------
// Construction and initialization
// --------------------------------------------------------------------------

GameBoard::GameBoard() {
    clear();
}

void GameBoard::clear() {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            board[i][j] = VOID;
}

// --------------------------------------------------------------------------
// Operators
// --------------------------------------------------------------------------

char GameBoard::operator()(int i, int j) const {
    return board[i][j];
}

bool GameBoard::operator==(const GameBoard& other) const {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] != other.board[i][j])
                return false;

    return true;
}


bool GameBoard::operator!=(const GameBoard& other) const {
    return !(*this == other);
}


GameBoard& GameBoard::operator=(const GameBoard& other) {
    if (this == &other)
        return *this;

    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            board[i][j] = other.board[i][j];

    return *this;
}

// --------------------------------------------------------------------------
// Game mechanics
// --------------------------------------------------------------------------

ActionSet GameBoard::possible_actions() const {
    ActionSet actions;

    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] == VOID)
                actions.insert(Action{i, j});

    return actions;
}

void GameBoard::make_move(Action action) {
    ActionSet actions = possible_actions();

    if (!actions.contains(action)) return;

    board[action.i][action.j] = player();
}

// --------------------------------------------------------------------------
// Game evaluation
// --------------------------------------------------------------------------

bool GameBoard::initial() const {
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] != VOID)
                return false;

    return true;
}

int GameBoard::count_turns(char c) const {
    int count = 0;

    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (board[i][j] == c)
                ++count;

    return count;
}

char GameBoard::player() const {
    if (initial())
        return X;

    int xs = count_turns(X);
    int os = count_turns(O);

    return (xs > os) ? O : X;
}

char GameBoard::winner() const {
    // Rows and columns
    for (int i = 0; i < SIZE; ++i)
    {
        if (board[i][0] != VOID &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];

        if (board[0][i] != VOID &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }

    // Diagonals
    if (board[0][0] != VOID &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];

    if (board[0][2] != VOID &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];

    return VOID;
}

bool GameBoard::terminal() const {
    return (winner() != VOID) || possible_actions().is_empty();
}

int GameBoard::scoring() const {
    char win = winner();

    if (win == X) return 1;
    if (win == O) return -1;

    return 0;
}
