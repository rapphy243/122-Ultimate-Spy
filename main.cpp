#include <iostream>
#include <string>
#include <random>
#include "grid.h"

using namespace std;

inline constexpr streamsize INF_FLAG{numeric_limits<streamsize>::max()};
inline const string clearAndGoHome{"\x1B[2J\x1B[H"};

void playMap(Grid selectedMap) {
    Grid map = selectedMap;
    bool gameOver = map.isGameOver();
    map.printGrid();
    while (!gameOver) {
        char input;
        cout << "Enter move (w/a/s/d): ";
        cin >> input;
        if (moveSpy(input, map)) {
            moveGuards(map);
            gameOver = map.isGameOver();
            if (!gameOver) {
                cout << clearAndGoHome;
                map.printGrid();
            }
        }
        else {
            cout << "Invalid move. Try again.\n";
        }
    }
}

void inspectMap(Grid& map) {
    string input;
    cout << "Map Inspection (row, column): ";
}



void menu(char& restartCharacter) {
    string menuSelected;

    cout << "\n\tWelcome to Ultra-Spy!\n\n";
    cout << "\n\tSelect a level:\n\n";
    cout << "\t1) Level 1\n";
    cout << "\t2) Level 2\n";
    cout << "\t3) Level 3\n";
    cout << "\t4) Quit\n\n";
    cout << "Your selection: ";
    getline(cin, menuSelected);
    cout << clearAndGoHome;

    if (menuSelected == "1" || menuSelected == "Level 1") {
        playMap(Map1());
    }
    else if (menuSelected == "2" || menuSelected == "Level 2") {
        playMap(Map2());
    }
    else if (menuSelected == "3" || menuSelected == "Level 3") {
        playMap(Map3());
    }
    else if (menuSelected == "4" || menuSelected == "Quit") {
        restartCharacter = 'n';
    }
    else {
        cout << "Invalid selection. Please try again.\n";
        menu(restartCharacter);
    }

    if (!(restartCharacter == 'n')) {
        cout << "Go to menu? (Y/N): ";
        cin >> restartCharacter;
        cin.ignore(INF_FLAG, '\n');
        cout << clearAndGoHome;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    bool isTest = false;
    char restartCharacter;
    if (isTest) {
        playMap(Map3());
    }
    else {
        do {
            menu(restartCharacter);
        } while (restartCharacter != 'n');
    }
}
