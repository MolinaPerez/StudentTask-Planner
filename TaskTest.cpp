#include "TASK.h"

#include <iostream>
using namespace std;

int main() {
    Task t1(1, "Study DS", "Review linked lists", "COMP2501", 1, "2026-04-20");

    t1.show();

    t1.checkedComplete();

    cout << "\nAfter completing:\n";
    t1.show();

    return 0;
}

//g++ TaskTets.cpp TASK.cpp -o program
// IN TERMINAL ./program
