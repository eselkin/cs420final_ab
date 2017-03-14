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
    regex wonO = regex("OOOO");
    regex wonX = regex("XXXX");
    orderOfSuccession->push_back(regex("(-)(?:XXX)")); // win
    orderOfSuccession->push_back(regex("(?:XXX)(-)")); // win
    orderOfSuccession->push_back(regex("(?:XX)(-)(?:X)")); // win
    orderOfSuccession->push_back(regex("(?:X)(-)(?:XX)")); // win
    orderOfSuccession->push_back(regex("(-)(?:OOO)")); // block win
    orderOfSuccession->push_back(regex("(?:OOO)(-)")); // block win 5
    orderOfSuccession->push_back(regex("(?:OO)(-)(?:O)")); // block win
    orderOfSuccession->push_back(regex("(?:O)(-)(?:OO)")); // block win
    orderOfSuccession->push_back(regex("(-)(?:X)(-)(?:X)(-)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(-)(?:O)(-)(?:O)(-)")); // lessen chance of winning
    orderOfSuccession->push_back(regex("(--)(?:XX)(-)")); // improve chance of winning 10
    orderOfSuccession->push_back(regex("(-)(?:XX)(--)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(--)(?:OO)(-)")); // again with loss // matching 5
    orderOfSuccession->push_back(regex("(-)(?:OO)(--)")); // again with loss // matching 5
    orderOfSuccession->push_back(regex("(--)(?:X)(--)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(--)(?:O)(--)")); // this is a dangerous situation 15
    orderOfSuccession->push_back(regex("(-)(?:XX)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(?:XX)(-)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(?:X)(-)(?:X)")); // improve chance of winning
    orderOfSuccession->push_back(regex("(-)(?:OO)")); //
    orderOfSuccession->push_back(regex("(?:OO)(-)")); //
    orderOfSuccession->push_back(regex("(?:O)(-)(?:O)"));
    orderOfSuccession->push_back(regex("(-)(?:X)"));
    orderOfSuccession->push_back(regex("(?:X)(-)"));
    orderOfSuccession->push_back(regex("(-)(?:O)"));
    orderOfSuccession->push_back(regex("(?:O)(-)"));

    vector<regex> *minOrderOfSuccession = new vector<regex>();
    minOrderOfSuccession->push_back((*orderOfSuccession)[4]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[5]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[6]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[7]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[0]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[1]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[2]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[3]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[9]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[8]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[12]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[13]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[10]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[11]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[12]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[13]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[15]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[14]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[19]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[20]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[21]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[16]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[17]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[18]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[24]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[25]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[22]);
    minOrderOfSuccession->push_back((*orderOfSuccession)[23]);

    // otherwise move is random
    while(!game->isOver()) {
        string move;
        if (humanFirst) {
            prompt("Enter your move? ", move);
            while (validate(move, game, BOARD_SIZE) == nullptr) {
                prompt("Enter your move? ", move);
            }
            action act = *validate(move, game, BOARD_SIZE);
            game->makeMove(act.first, act.second, 'O');
        } else {
            A_B_SearchClass *a_b_searchClass = new A_B_SearchClass(game, orderOfSuccession, minOrderOfSuccession, chrono::seconds(timeLimit));
            cout << *game;
            cout << "Computer played: " << a_b_searchClass->getBestAction().first
                 << a_b_searchClass->getBestAction().second
                 << endl;
        }
        humanFirst = !humanFirst;
    }
    return 0;
}

