#include <iostream>
#include <string>
#include <random>
#include "grid.h"
#include "mapBuilder.h"

using namespace std;

inline constexpr streamsize INF_FLAG{numeric_limits<streamsize>::max()};
inline const string clearAndGoHome{"\x1B[2J\x1B[H"};


void inspectMap(Grid& map) {
    char openParenthesis{}, comma{}, closedParenthesis{};
    int x, y;
    cout << "Map Inspection (row, column): ";
    cin >> ws;
    if (ispunct(cin.peek())) { // Remove any punct
        cin >> openParenthesis;
    }
    cin >> x >> ws; // Set X then remove whitespace
    if (cin.fail()) {
        cin.clear();
        cin.ignore(INF_FLAG, '\n');
        cout << "Invalid input.\n";
        return;
    }
    if (ispunct(cin.peek())) {
        cin >> comma;
    }
    cin >> y; // Set Y (Using ws eats the \n if there is nothing after)
    if (cin.fail()) {
        cin.clear();
        cin.ignore(INF_FLAG, '\n');
        cout << "Invalid input.\n";
        return;
    }
    while ((isspace(cin.peek()) && cin.peek() != '\n')) {
        cin.ignore();
    }
    if (ispunct(cin.peek())) {
        cin >> closedParenthesis;
    }
    cin.ignore(INF_FLAG, '\n');
    if (x < 0 || x >= map.getBoard().size() || y < 0 || y >= map.getBoard()[0].size()) {
        cout << "Coordinates are out of bounds!\n";
    }
    else {
        char icon = getCellIcon(map.getBoard(), x, y);
        if (icon == ' ') {
            cout << "The cell at (" << x << ", " << y << ") is empty.\n";
        }
        else {
            Sprite* sprite = map.getBoard()[x][y];
            cout << "The cell at (" << x << ", " << y << ") contains a "
                << sprite->getType() << ": " << sprite->getDescription() << "\n";
        }
    }
    cin.clear();
}

void playMap(Grid selectedMap) {
    Grid map = selectedMap;
    bool gameOver = map.isGameOver();
    map.printGrid();
    while (!gameOver) {
        string input;
        cout << "Enter move (w/a/s/d) or inspect: ";
        cin >> input;
        if (input == "inspect") {
            inspectMap(map);
            map.printGrid();
        }
        else {
            char move = input[0];
            if (moveSpy(move, map)) {
                moveGuards(map);
                map.fixSwitchandDoors();
                gameOver = map.isGameOver();
            if (!gameOver) {
                cout << clearAndGoHome;
                map.printGrid();
            }
            }
            else {
                cout << "Invalid input. Try again.\n";
            }
        }
    }
}

void playCustomMap() {
    CustomMap customMap;
    string filename;

    cout << "Do you have a custom map to load? (Y/N): ";
    char loadChoice;
    cin >> loadChoice;
    cin.ignore(INF_FLAG, '\n');
    if (tolower(loadChoice) == 'y') {
        cout << "Enter the filename of the custom map to play (including extension .lvl): ";
        getline(cin, filename);
        if (!customMap.loadMap(filename)) {
            cout << "Failed to load the custom map.\n";
            return;
        }
        playMap(customMap);

    }
    else {
        cout << "Creating new custom map.\n";
        cout << "Please enter the dimensions of the map (rows columns): ";
        int rows, cols;
        cin >> rows >> cols;
        cin.ignore(INF_FLAG, '\n');
        customMap = CustomMap();
        customMap.setDimensions(rows, cols);
        
        
    }
}

void menu(char& restartCharacter) {
    string menuSelected;

    cout << "\n\tWelcome to Ultra-Spy!\n\n";
    cout << "\n\tSelect a level:\n\n";
    cout << "\t1) Level 1\n";
    cout << "\t2) Level 2\n";
    cout << "\t3) Level 3\n";
    cout << "\t4) Level 4\n";
    cout << "\t5) Custom Map\n";
    cout << "\t6) Quit\n\n";
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
    else if (menuSelected == "4" || menuSelected == "Level 4") {
        playMap(Map4());
    }
    else if (menuSelected == "5" || menuSelected == "Custom Map") {
       playCustomMap();
    }
    else if (menuSelected == "6" || menuSelected == "Quit") {
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
        playMap(Map4());
    }
    else {
        do {
            menu(restartCharacter);
        } while (restartCharacter != 'n');
    }
}
