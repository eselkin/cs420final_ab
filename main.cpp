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
    //test_state(BOARD_SIZE);
    state *game = new state();
    int timeLimit;
    bool humanFirst;
    prompt("Time limit for a computer move is? ", timeLimit);
    prompt("Human first? " , humanFirst);

    cout << "Time limit for computer is: " << timeLimit << " seconds" << endl;
    cout << "Human will go " <<  (humanFirst ? "first" : "second") << endl;

    cout << *game;
    vector<regex>* orderOfSuccession = new vector<regex>();
    orderOfSuccession->push_back(regex("(-)(?:XXX)|(?:XXX)(-)")); // win
    orderOfSuccession->push_back(regex("(-)(?:OOO)|(?:OOO)(-)")); // block win
    orderOfSuccession->push_back(regex("(-)(?:XX)|(?:XX)(-)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(-)(?:OO)|(?:OO)(-)")); // further reduce chance of losing
    orderOfSuccession->push_back(regex("(-)(?:X)|(?:X)(-)"));
    orderOfSuccession->push_back(regex("(-)(?:O)|(?:O)(-)"));

    //    string move;
    //    prompt("Enter your move? ", move);
    //    while(validate(move, game, BOARD_SIZE) == nullptr) {
    //        prompt("Enter your move? ", move);
    //    }
    //    action act = *validate(move, game, BOARD_SIZE);

    game->makeMove('C', 5, 'X');
    game->makeMove('D', 5, 'O');
    game->makeMove('D', 4, 'X');
    game->makeMove('C', 4, 'O');
    game->makeMove('E', 4, 'X');
    game->makeMove('E', 5, 'O');
    game->makeMove('F', 5, 'X');
    game->makeMove('F', 4, 'O');
    game->makeMove('E', 3, 'X');
    game->makeMove('D', 3, 'O');
    game->makeMove('F', 3, 'X');
    game->makeMove('C', 3, 'O');
    game->makeMove('C', 2, 'X');
    game->makeMove('D', 6, 'O');
    game->makeMove('B', 3, 'X');
    game->makeMove('E', 6, 'O');
    game->makeMove('D', 7, 'X');

    A_B_SearchClass* a_b_searchClass = new A_B_SearchClass(game, orderOfSuccession, chrono::seconds(timeLimit));
    cout << *game;
    cout << "Computer played: " << a_b_searchClass->getBestAction().first << a_b_searchClass->getBestAction().second << endl;

    return 0;
}

