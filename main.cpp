#include <iostream>
#include <random>
#include "grid.h"

using namespace std;

int main() {
    Grid map1 = Map1();
    Spy *spy = map1.getSpy();
    vector<Guard *> guards = map1.getGuards();
    vector<vector<Sprite*>>& grid = map1.getGrid();

    map1.printGrid();
    while (true) {
        char input;
        cout << "Enter move (w/a/s/d): ";
        cin >> input;
        if (moveSpy(input, grid, spy)) {
            for (Guard *g : guards)
            {
                moveGuard(g, grid, spy);
            }
            map1.printGrid();
        }
        else {
            cout << "Invalid move. Try again.\n";
        }
    }

    return 0;
}
