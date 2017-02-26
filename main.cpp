#include <iostream>
#include <sstream>
#include "libraries/state.h"
#include "libraries/ab_library.h"
#include "libraries/timing_library.h"
#include "libraries/testing_library.h"

template <typename T>
void prompt(string dialog, T& outputTo);

using namespace std;

int main(int argc, char *argv[])
{
    test_state(8);
    state *game = new state();
    int timeLimit;
    bool humanFirst;
    prompt("Time limit for a computer move is? ", timeLimit);
    cout << "Time limit for computer is: " << timeLimit << " seconds" << endl;
    prompt("Human first?" , humanFirst);
    cout << "Human will go " <<  (humanFirst ? "first" : "second") << endl;
    return 0;
}

template <typename T>
void prompt(string dialog, T& outputTo) {
    string input;
    cout << dialog;
    getline(cin, input);
    if (typeid(T) == typeid(bool)) {
        outputTo = input[0] == 'Y' || input == 'y';
    } else {
        stringstream ss(input);
        ss >> outputTo;
    }
}