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
    vector<regex>* orderOfSuccession = new vector<regex>();
    char typeChar = 'X', altChar = 'O';
    if (humanFirst) {
        typeChar = 'O';
        altChar = 'X';
    }
    stringstream conv;
    conv << "(-)(?:" << typeChar << typeChar << typeChar << ")|(?:"<< typeChar << typeChar << typeChar << ")(-)";
    orderOfSuccession->push_back(regex(conv.str())); // win
    conv.str("");
    conv.flush();
    conv << "(-)(?:" << altChar << altChar << altChar << ")|(?:"<< altChar << altChar << altChar << ")(-)";
    orderOfSuccession->push_back(regex(conv.str())); // block win
    conv.str("");
    conv.flush();
    conv << "(-)(?:" << typeChar << typeChar << ")|(?:"<< typeChar << typeChar << ")(-)";
    orderOfSuccession->push_back(regex(conv.str())); // improve chance of winning
    conv.str("");
    conv.flush();
    conv << "(-)(?:" << altChar << altChar << ")|(?:"<< altChar << altChar << ")(-)";
    orderOfSuccession->push_back(regex(conv.str())); // further reduce chance of losing
    conv.str("");
    conv.flush();
    conv << "(-)(?:" << typeChar << ")|(?:" << typeChar << ")(-)";
    orderOfSuccession->push_back(regex(conv.str()));
    conv.str("");
    conv.flush();
    conv << "(-)(?:" << altChar << ")|(?:" << altChar << ")(-)";
    orderOfSuccession->push_back(regex(conv.str()));
    conv.str("");
    conv.flush();

    string move;
    prompt("Enter your move? ", move);
    while(validate(move, game, BOARD_SIZE) == nullptr) {
        prompt("Enter your move? ", move);
    }
    action act = *validate(move, game, BOARD_SIZE);
    //game->makeMove(act.first, act.second, 'X');
    game->makeMove('A', 1, 'X');
    game->makeMove('A', 2, 'X');
    game->makeMove('A', 3, 'X');
    game->makeMove('C', 5, 'X');
    game->makeMove('D', 5, 'X');
    game->makeMove('E', 1, 'X');
    A_B_SearchClass* a_b_searchClass = new A_B_SearchClass(game, orderOfSuccession);

    cout << *game;

    return 0;
}

