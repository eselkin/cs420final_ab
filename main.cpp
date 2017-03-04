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
    srand(time(NULL));
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
    orderOfSuccession->push_back(regex("(?:XX)(-)(?:X)|(?:X)(-)(?:XX)")); // win
    orderOfSuccession->push_back(regex("(-)(?:OOO)|(?:OOO)(-)")); // block win
    orderOfSuccession->push_back(regex("(?:OO)(-)(?:O)|(?:O)(-)(?:OO)")); // block win
    orderOfSuccession->push_back(regex("(--)(?:XX)(-)|(-)(?:XX)(--)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(--)(?:OO)(-)|(-)(?:OO)(--)")); // again with loss // matching 5
    orderOfSuccession->push_back(regex("(-)(?:XX)(-)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(-)(?:OO)(-)")); // more losses
    orderOfSuccession->push_back(regex("(-)(?:XX)|(?:XX)(-)|(?:X)(-)(?:X)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(-)(?:OO)|(?:OO)(-)|(?:O)(-)(?:O)")); // further reduce chance of losing
    orderOfSuccession->push_back(regex("(-)(?:X)|(?:X)(-)"));
    orderOfSuccession->push_back(regex("(-)(?:O)|(?:O)(-)"));

    //    string move;
    //    prompt("Enter your move? ", move);
    //    while(validate(move, game, BOARD_SIZE) == nullptr) {
    //        prompt("Enter your move? ", move);
    //    }
    //    action act = *validate(move, game, BOARD_SIZE);
    game->makeMove('E', 4, 'X');
    game->makeMove('D', 4, 'O');
    game->makeMove('E', 3, 'X');
    game->makeMove('E', 5, 'O');
    game->makeMove('E', 2, 'X');
    game->makeMove('E', 1, 'O');
    game->makeMove('D', 3, 'X');
    game->makeMove('F', 3, 'O');
    game->makeMove('C', 3, 'X');
    game->makeMove('B', 3, 'O');
    game->makeMove('D', 2, 'X');
    game->makeMove('F', 2, 'O');
    game->makeMove('F', 1, 'X');
    game->makeMove('F', 5, 'O');
    A_B_SearchClass* a_b_searchClass = new A_B_SearchClass(game, orderOfSuccession, chrono::seconds(timeLimit));
    cout << *game;
    cout << "Computer played: " << a_b_searchClass->getBestAction().first << a_b_searchClass->getBestAction().second << endl;

    return 0;
}

