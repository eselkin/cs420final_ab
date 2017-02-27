#ifndef EXTRAS_LIBRARY_H
#define EXTRAS_LIBRARY_H

#include <regex>
#include <locale>

using namespace std;

typedef pair<char, int> action;

template <typename T>
void prompt(string dialog, T& outputTo) {
    string input;
    while (input == "") {
        cout << dialog;
        getline(cin, input);
    }
    if (typeid(T) == typeid(bool)) {
        outputTo = input[0] == 'Y' || input[0] == 'y';
    } else {
        stringstream ss(input);
        ss >> outputTo;
    }
}

action* validate(string move, state* s, int dim) {
    stringstream regexString;
    regexString << "^([A-" << char('A'+dim-1) << "]{1})([1-" << dim << "]{1})$";
    regex moveRegex(regexString.str());
    smatch moveMatch;
    if (regex_search(move, moveMatch, moveRegex)) {
        action* act = new pair<char, int>(toupper(moveMatch[1].str()[0]), atoi(moveMatch[2].str().c_str()));
        state scopy(*s);
        if (scopy.makeMove(act->first, act->second, 'X')) return act; // doesn't matter what type it is will return false if not a '-'
    }
    return nullptr;
}

#endif //EXTRAS_LIBRARY_H
