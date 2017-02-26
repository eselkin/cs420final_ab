#include <iostream>
#include "libraries/state.h"
#include "libraries/ab_library.h"
#include "libraries/timing_library.h"
#include "libraries/testing_library.h"

void test_state(int);

using namespace std;

int main(int argc, char *argv[])
{
    test_state(8);

    return 0;
}

