#include "actions.h"

// --------------------------------------------------------------------------
// Action
// --------------------------------------------------------------------------

bool Action::operator==(const Action& other) const {
    return i == other.i && j == other.j;
}

// --------------------------------------------------------------------------
// ActionSet
// --------------------------------------------------------------------------

bool ActionSet::is_empty() const { return stored == 0; }
    
bool ActionSet::contains(const Action& action) const {
    for (int i = 0; i < stored; i++) {
        if (action == vector[i]) {
            return true;
        }
    }
    return false;
}

void ActionSet::insert(Action action) {
    if (!contains(action) && stored < MAX) {
        vector[stored++] = action;
    }
}

Action* ActionSet::begin() { return vector; }

Action* ActionSet::end() { return vector + stored; }

const Action* ActionSet::begin() const { return vector; }

const Action* ActionSet::end() const { return vector + stored; }
