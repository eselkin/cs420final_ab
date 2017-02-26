#ifndef AB_LIBRARY_H
#define AB_LIBRARY_H

#include "state.h"
#include <limits>
#include <chrono>
using namespace std;

class A_B_SearchClass {
public:
    typedef pair<char, int> action;

    // cutoff
    chrono::seconds timeLimit = std::chrono::seconds(30);
    chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    // constructor
    A_B_SearchClass(state* s, chrono::seconds timeLimit = chrono::seconds(30)) {
        this->timeLimit = timeLimit;
        action bestAction = ab_search(s);
    }

private :
    // the actual search begins here!
    action ab_search(state* s) {
        this->startTime = std::chrono::high_resolution_clock::now(); // actually evaluate beginning time
        double value = max_value(s, numeric_limits<double>::lowest(), numeric_limits<double>::max());
        return *new action();
    }

    double max_value(state* s, double alpha, double beta) {
        if(cutoff_test(s)) return eval(s);
        double val = numeric_limits<double>::lowest();
        for (action act : s->getOrderedActions('X')) {
            state* nextMove =  new state(*s);
            nextMove->makeMove(act.first, act.second, 'X'); // ignore the bool, shouldn't ever need that
            val = max(val, min_value(nextMove, alpha, beta));
            if (val >= beta) return val;
            alpha = max(alpha, val);
        }
        return val;
    }

    double min_value(state* s, double alpha, double beta) {
        if (cutoff_test(s)) return eval(s);
        double val = numeric_limits<double>::max();
        for (action act: s->getOrderedActions('O')) {
            state* nextMove = new state(*s);
            nextMove->makeMove(act.first, act.second, 'O');
            val = min(val, max_value(nextMove, alpha, beta));
            if (val <= alpha) return val;
            beta = min(beta, val);
        }
        return val;
    }

    bool cutoff_test(state* s) {
        auto testTime = std::chrono::high_resolution_clock::now();
        auto timeDifference = std::chrono::duration_cast<std::chrono::seconds>(testTime - this->startTime);
        return timeDifference >= this->timeLimit;
    }

    double eval(state* s) {
        // THE TRICKY FUNCTION! TODO XXX
        return 0;
    }
};
#endif // AB_LIBRARY_H
