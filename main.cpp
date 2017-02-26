#include <iostream>
#include <sstream>
#include "libraries/state.h"
#include "libraries/ab_library.h"
#include "libraries/timing_library.h"
#include "libraries/testing_library.h"
#include "libraries/extras_library.h"

using namespace std;
typedef pair<char, int> action;
#define BOARD_SIZE 8

int main(int argc, char *argv[])
{
    test_state(BOARD_SIZE);
    state *game = new state();
    int timeLimit;
    bool humanFirst;
    prompt("Time limit for a computer move is? ", timeLimit);
    prompt("Human first? " , humanFirst);

    cout << "Time limit for computer is: " << timeLimit << " seconds" << endl;
    cout << "Human will go " <<  (humanFirst ? "first" : "second") << endl;

    cout << *game;
    if (humanFirst) {
        string move;
        prompt("Enter your move? ", move);
        while(validate(move, game, BOARD_SIZE) == nullptr) {
            prompt("Enter your move? ", move);
        }
        action act = *validate(move, game, BOARD_SIZE);
        game->makeMove(act.first, act.second, 'X');
    }
    cout << *game;

    return 0;
}

