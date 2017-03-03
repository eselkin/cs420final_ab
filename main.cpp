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
    game->makeMove('F', 5, 'X');
    game->makeMove('E', 5, 'O');
    game->makeMove('E', 4, 'X');
    game->makeMove('F', 4, 'O');
    game->makeMove('D', 5, 'X');
    game->makeMove('D', 4, 'O');
    game->makeMove('E', 6, 'X');
    game->makeMove('C', 4, 'O');
    game->makeMove('B', 4, 'X');
    game->makeMove('C', 5, 'O');
    game->makeMove('C', 3, 'X');
    game->makeMove('D', 3, 'O');
    game->makeMove('D', 2, 'X');
    game->makeMove('E', 3, 'O');
    game->makeMove('F', 3, 'X');
    game->makeMove('C', 6, 'O');
    game->makeMove('C', 7, 'X');
    game->makeMove('D', 6, 'O');
    game->makeMove('B', 6, 'X');
    game->makeMove('B', 5, 'O');
    game->makeMove('A', 5, 'X');
    game->makeMove('F', 7, 'O');
    game->makeMove('F', 6, 'X');
    game->makeMove('E', 7, 'O');
    game->makeMove('D', 7, 'X');
    game->makeMove('G', 6, 'O');
    game->makeMove('G', 7, 'X');
    game->makeMove('A', 7, 'O');
    game->makeMove('B', 7, 'X');
    game->makeMove('B', 2, 'O');
    game->makeMove('B', 8, 'X');
    game->makeMove('B', 3, 'O');
    game->makeMove('B', 1, 'X');
    game->makeMove('D', 1, 'O');
    game->makeMove('H', 6, 'X');
    game->makeMove('E', 1, 'O');
    game->makeMove('C', 1, 'X');
    game->makeMove('G', 4, 'O');


//
//
    //game->makeMove('E', 4, 'X');
//    game->makeMove('C', 4, 'X');
//    game->makeMove('D', 4, 'O');
//    game->makeMove('D', 3, 'X');
//    game->makeMove('D', 5, 'O');
//    game->makeMove('C', 3, 'X');
//    game->makeMove('D', 6, 'O');
//    game->makeMove('D', 7, 'X');
//    game->makeMove('F', 3, 'O');
//    game->makeMove('C', 2, 'X');
//    game->makeMove('C', 5, 'O');
//
//    game->makeMove('E', 5, 'X');
//    game->makeMove('D', 5, 'O');
//    game->makeMove('D', 4, 'X');
//    game->makeMove('E', 4, 'O');
//    game->makeMove('E', 6, 'X');
//    game->makeMove('D', 7, 'O');
//    game->makeMove('C', 4, 'X');
//    game->makeMove('E', 7, 'O');
//    game->makeMove('C', 7, 'X');
//    game->makeMove('C', 5, 'O');
//    game->makeMove('F', 7, 'X');
//    game->makeMove('D', 6, 'O');
//    game->makeMove('D', 8, 'X');
//    game->makeMove('A', 4, 'O');
//    game->makeMove('B', 5, 'X');
//    game->makeMove('A', 5, 'O');
//    game->makeMove('A', 3, 'X');
//    game->makeMove('A', 6, 'O');
//    game->makeMove('A', 7, 'X');
//    game->makeMove('F', 4, 'O');
//    game->makeMove('G', 4, 'X');
//    game->makeMove('F', 5, 'O');
//    game->makeMove('F', 3, 'X');
//    game->makeMove('B', 6, 'O');
//    game->makeMove('C', 6, 'X');
//    game->makeMove('E', 3, 'O');
//    game->makeMove('E', 2, 'X');
//    game->makeMove('D', 3, 'O');
//    game->makeMove('C', 3, 'X');
//    game->makeMove('C', 2, 'O');
//    game->makeMove('F', 6, 'X');
//    game->makeMove('G', 3, 'O');
//    game->makeMove('F', 8, 'X');
//    game->makeMove('E', 8, 'O');
//    game->makeMove('G', 6, 'X');
//    game->makeMove('H', 6, 'O');
//    game->makeMove('B', 4, 'X');
//    game->makeMove('G', 5, 'O');
//    game->makeMove('H', 5, 'X');
//    game->makeMove('G', 2, 'O');
//    game->makeMove('G', 1, 'X');
//    game->makeMove('B', 3, 'O');
//    game->makeMove('F', 1, 'X');
//    game->makeMove('D', 1, 'O');
//    game->makeMove('C', 8, 'X');
//    game->makeMove('D', 2, 'O');
//    game->makeMove('B', 2, 'X');
//    game->makeMove('H', 3, 'O');
//    game->makeMove('B', 8, 'X');
//    game->makeMove('A', 8, 'O');
//    game->makeMove('E', 1, 'X');
//    game->makeMove('H', 1, 'O');
//    game->makeMove('A', 2, 'X');
//    game->makeMove('H', 2, 'O');
    A_B_SearchClass* a_b_searchClass = new A_B_SearchClass(game, orderOfSuccession, chrono::seconds(timeLimit));
    cout << *game;
    cout << "Computer played: " << a_b_searchClass->getBestAction().first << a_b_searchClass->getBestAction().second << endl;

    return 0;
}

