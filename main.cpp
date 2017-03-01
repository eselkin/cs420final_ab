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
    char typeChar = 'X', altChar = 'O';
    if (humanFirst) {
        typeChar = 'O';
        altChar = 'X';
    }
    cout << " typechar: " << typeChar << endl << endl;
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
    game->makeMove('E', 5, 'X');
    game->makeMove('D', 5, 'O');
    game->makeMove('E', 4, 'X');
    game->makeMove('E', 3, 'O');
    game->makeMove('F', 4, 'X');
    game->makeMove('D', 4, 'O');
    game->makeMove('E', 6, 'X');
    game->makeMove('E', 7, 'O');
    game->makeMove('G', 4, 'X');
    game->makeMove('H', 4, 'O');
    game->makeMove('D', 6, 'X');
    game->makeMove('D', 3, 'O');
    game->makeMove('D', 2, 'X');
    game->makeMove('C', 3, 'O');
    game->makeMove('B', 3, 'X');
    A_B_SearchClass* a_b_searchClass = new A_B_SearchClass(game, orderOfSuccession, chrono::seconds(timeLimit), typeChar, altChar);
    cout << *game;
    cout << "Computer played: " << a_b_searchClass->getBestAction().first << a_b_searchClass->getBestAction().second << endl;

    return 0;
}

