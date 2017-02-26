#ifndef EXTRAS_LIBRARY_H
#define EXTRAS_LIBRARY_H

template <typename T>
void prompt(string dialog, T& outputTo) {
    string input;
    cout << dialog;
    getline(cin, input);
    if (typeid(T) == typeid(bool)) {
        outputTo = input[0] == 'Y' || input[0] == 'y';
    } else {
        stringstream ss(input);
        ss >> outputTo;
    }
}
#endif //EXTRAS_LIBRARY_H
