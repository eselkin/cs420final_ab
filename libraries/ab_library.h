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
    regex *win, *loss, *tie;
    // cutoff
    chrono::seconds timeLimit = std::chrono::seconds(30);
    chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    // constructor
    A_B_SearchClass(state* s, vector<regex>* orderOfSuccession, chrono::seconds timeLimit = chrono::seconds(30), char minchar = 'O', char maxchar = 'X') {
        this->timeLimit = timeLimit;
        this->MIN_CHAR = minchar; // this way we don't need to switch around the formula whichever user goes first
        this->MAX_CHAR = maxchar;
        this->orderOfSuccession = orderOfSuccession;
        action bestAction = ab_search(s);
        s->makeMove(bestAction.first, bestAction.second, maxchar);
        win = new regex(string({maxchar, maxchar, maxchar, maxchar}));
        loss = new regex(string({minchar, minchar, minchar, minchar}));
    }

    value max(const value &a, const value &b) {
        return a._value > b._value ? a : b;
    }
    value min(const value &a, const value &b) {
        return a._value < b._value ? a : b;
    }

private :
    char MIN_CHAR, MAX_CHAR;
    int MAX_DEPTH;
    vector<regex>* orderOfSuccession;
    // the actual search begins here!
    action ab_search(state* s) {
        this->startTime = chrono::high_resolution_clock::now(); // actually evaluate beginning time
        this->MAX_DEPTH = 1;
        value val(0.0, nullptr);
        chrono::microseconds cycle_duration = chrono::microseconds(0);
        chrono::microseconds total_duration = chrono::microseconds(0);
        while(MAX_DEPTH < 100 && (total_duration + cycle_duration < timeLimit)) {
            int depth = 1;
            total_duration += cycle_duration;
            auto cycle_time = chrono::high_resolution_clock::now(); // actually evaluate beginning time
            value alpha(numeric_limits<double>::lowest(), nullptr);
            value beta(numeric_limits<double>::max(), nullptr);
            val = max_value(s, alpha, beta, depth);
            cycle_duration = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now()-cycle_time);
            MAX_DEPTH++;
        }
        return val._state->getActionTakenToGetHere();
    }

    value max_value(state* s, value alpha, value beta, int& depth) {
        if (cutoff_test(s, depth)) return eval(s);
        value val(numeric_limits<double>::lowest(), s);
        // This is an action even though it's a state b/c we've taken the action to get to the successor
        if (s != NULL) {
            for (state *successor : s->getOrderedSuccessors(MAX_CHAR, *orderOfSuccession)) {
                val = max(val, min_value(successor, alpha, beta, depth));
                if (val >= beta) return val;
                alpha = max(alpha, val);
            }
        }
        return val;
    }

    value min_value(state* s, value alpha, value beta, int& depth) {
        depth++; // turn depth not ply depth
        if (cutoff_test(s, depth)) return eval(s);
        value val(numeric_limits<double>::max(), s);
        if (s != NULL) {
            for (state *successor : s->getOrderedSuccessors(MIN_CHAR, *orderOfSuccession)) {
                val = min(val, max_value(successor, alpha, beta, depth));
                if (val <= alpha) return val;
                beta = min(beta, val);
            }
        }
        return val;
    }

    bool cutoff_test(state* s, int depth) {
        if (depth > MAX_DEPTH)
            return true;
        auto testTime = std::chrono::high_resolution_clock::now();
        auto timeDifference = std::chrono::duration_cast<std::chrono::seconds>(testTime - this->startTime);
        return timeDifference >= this->timeLimit;
    }

    value eval(state* s) {
        return value(s->getValue(), s);
    }
};
#endif // AB_LIBRARY_H
