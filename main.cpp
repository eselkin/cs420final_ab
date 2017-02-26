#include <iostream>
#include "libraries/ab_library.h"
#include "libraries/timing_library.h"
#include "libraries/state.h"

using namespace std;

int main(int argc, char *argv[])
{
    int dim = 8;
    char** testConstructor = new char* [dim];
    for (int i = 0; i < dim; i++){
        testConstructor[i] = new char[dim]();
        for (int j = 0; j < dim; j++){
            testConstructor[i][j] = char('-'+i-j);
        }
    }
    state* test1 = new state(testConstructor, dim); // uses state(int)
    cout << *test1 << endl << endl;
    cout << test1->makeMove('B', 2, 'O') << endl << endl;
    cout << test1->makeMove('B', 2, 'X') << endl << endl;
    cout << test1->makeMove('H', 8, 'X') << endl << endl;
    state* test2 = new state(*test1);
    cout << *test2;

    delete test1;
    delete test2;

    return 0;
}
