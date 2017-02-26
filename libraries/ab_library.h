#ifndef AB_LIBRARY_H
#define AB_LIBRARY_H

#include "state.h"
#include <limits>
#include <chrono>
using namespace std;

struct value {
    value(double value, state* state_ptr){this->_value = value; this->_state = state_ptr;};
    double _value;
    state* _state;
    friend bool operator <=(const value &a, const value &b) {return a._value <= b._value;}
    friend bool operator >=(const value &a, const value &b) {return a._value >= b._value;}
};

class A_B_SearchClass {
public:
    typedef pair<char, int> action;

    // cutoff
    chrono::seconds timeLimit = std::chrono::seconds(30);
    chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    // constructor
    A_B_SearchClass(state* s, chrono::seconds timeLimit = chrono::seconds(30), char minchar = 'O', char maxchar = 'X') {
        this->timeLimit = timeLimit;
        this->MIN_CHAR = minchar;
        this->MAX_CHAR = maxchar;
        action bestAction = ab_search(s);
    }

    value max(const value &a, const value &b) {
        return a._value > b._value ? a : b;
    }
    value min(const value &a, const value &b) {
        return a._value < b._value ? a : b;
    }

private :
    char MIN_CHAR, MAX_CHAR;
    // the actual search begins here!
    action ab_search(state* s) {
        this->startTime = chrono::high_resolution_clock::now(); // actually evaluate beginning time
        int depth = 1;
        value alpha(numeric_limits<double>::lowest(), nullptr);
        value beta(numeric_limits<double>::max(), nullptr);
        value val = max_value(s, alpha, beta, depth);
        return val._state->getActionTakenToGetHere();
    }

    value max_value(state* s, value alpha, value beta, int& depth) {
        if(cutoff_test(s, depth)) return eval(s);
        value val(numeric_limits<double>::lowest(), s);
        // This is an action even though it's a state b/c we've taken the action to get to the successor
        for (state* successor : s->getOrderedSuccessors(MAX_CHAR)) {
            val = max(val, min_value(successor, alpha, beta, depth));
            if (val >= beta) return val;
            alpha = max(alpha, val);
        }
        return val;
    }

    value min_value(state* s, value alpha, value beta, int& depth) {
        depth++; // turn depth not ply depth
        if (cutoff_test(s, depth)) return eval(s);
        value val(numeric_limits<double>::max(), s);
        for (state* successor : s->getOrderedSuccessors(MIN_CHAR)) {
            val = min(val, max_value(successor, alpha, beta, depth));
            if (val <= alpha) return val;
            beta = min(beta, val);
        }
        return val;
    }

    bool cutoff_test(state* s, int depth) {
        // I don't think we need depth, what we need is whether there are moves available
        auto testTime = std::chrono::high_resolution_clock::now();
        auto timeDifference = std::chrono::duration_cast<std::chrono::seconds>(testTime - this->startTime);
        return timeDifference >= this->timeLimit;
    }

    value eval(state* s) {
        return value(getValueOfState(s), s);
    }

    double getValueOfState(state* s) {
        // THE TRICKY FUNCTION! TODO XXX HAS TO BE FAST AND SHOULD PROBABLY BE A PROBABILITY 0.000-1.000
    }
};
#endif // AB_LIBRARY_H
