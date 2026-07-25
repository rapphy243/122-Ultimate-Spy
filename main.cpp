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

int readIntInRange(const string& text, int min, int max) {
    int value;
    while (true) {
        cout << text;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(INF_FLAG, '\n');
            return value;
        }
        cin.clear();
        cin.ignore(INF_FLAG, '\n');
        cout << "Invalid input. Try again.\n";
    }
}

SharedState* getOrCreateGroup(vector<SharedState*>& groups, const string& name) {
    for (SharedState* state : groups) {
        if (state != nullptr && state->getName() == name) {
            return state;
        }
    }

    SharedState* newState = new SharedState(name);
    groups.push_back(newState);
    return newState;
}

void printEditor(Grid& customMap, const string& name, int rows, int cols) {
    cout << clearAndGoHome;
    cout << "Level Editor: " << name << "\n\n";

    cout << "    ";
    for (int col = 0; col < cols; ++col) {
        if (col < 10) cout << col << "  ";
        else cout << col << " ";
    }
    cout << "\n";

    for (int row = 0; row < rows; ++row) {
        if (row < 10) cout << " " << row << " ";
        else cout << row << " ";

        for (int col = 0; col < cols; ++col) {
            cout << " " << getCellIcon(customMap.getBoard(), row, col) << " ";
        }
        cout << "\n";
    }

    cout << "\n1) Wall\n";
    cout << "2) Goal\n";
    cout << "3) Spy\n";
    cout << "4) Guard\n";
    cout << "5) Area Guard\n";
    cout << "6) Switch\n";
    cout << "7) Door\n";
    cout << "8) Inspect cell\n";
    cout << "9) Save and quit\n";
}

void playCustomMap() {
    CustomMap customMap = CustomMap();
    string filename;

    cout << "Do you have a custom map to load? (Y/N): ";
    char loadChoice;
    cin >> loadChoice;
    cin.ignore(INF_FLAG, '\n');
    if (tolower(loadChoice) == 'y') {
        cout << "Enter the filename of the custom map to play: ";
        getline(cin, filename);
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".lvl") {
            filename += ".lvl";
        }
        if (!customMap.loadMap(filename)) {
            cout << "Failed to load the custom map.\n";
            return;
        }
        playMap(customMap);

    }
    else {
        cout << "Creating new custom map.\n";
        cout << "Enter the level name: ";
        string levelName;
        getline(cin, levelName);

        int rows = readIntInRange("Enter number of rows: ", 5, 101);
        int cols = readIntInRange("Enter number of columns: ", 5, 101);
        customMap.setDimensions(rows, cols);

        vector<SharedState*> groups;
        bool spyPlaced = false;
        int spyRow = -1;
        int spyCol = -1;

        while (true) {
            printEditor(customMap, levelName, rows, cols);

            string choice;
            cout << "Choose an option: ";
            getline(cin, choice);

            // I dislike this here but basically skip asking rows/cols
            if (choice == "8" || choice == "inspect") {
                inspectMap(customMap);
                continue; // Skip rest of loop
            }
            else if (choice == "9" || choice == "save") {
                if (levelName.size() < 4 || levelName.substr(levelName.size() - 4) != ".lvl") {
                    levelName += ".lvl";
                }

                if (customMap.saveMap(levelName)) {
                    cout << "Level saved as " << levelName << "\n";
                }
                else {
                    cout << "Failed to save the level.\n";
                }
                break;
            }

            int row = readIntInRange("Row: ", 0, rows - 1);
            int col = readIntInRange("Column: ", 0, cols - 1);

            if (row == 0 || col == 0 || row == rows - 1 || col == cols - 1) {
                cout << "The border is reserved for walls.\n";
            }

            customMap.removeSprite(row, col);

            if (choice == "1") {
                customMap.addSprite(row, col, new Wall(row, col));
            }
            else if (choice == "2") {
                customMap.addSprite(row, col, new Goal(row, col));
            }
            else if (choice == "3") {
                if (spyPlaced) {
                    customMap.removeSprite(spyRow, spyCol);
                }
                customMap.addSprite(row, col, new Spy(row, col));
                spyPlaced = true;
                spyRow = row;
                spyCol = col;
            }
            else if (choice == "4" || choice == "5") {
                char direction{};
                while (true) {
                    cout << "Direction (^, >, v, <): ";
                    cin >> direction;
                    cin.ignore(INF_FLAG, '\n');

                    if (direction == '^' || direction == '>' || direction == 'v' || direction == '<') {
                        break;
                    }
                    cout << "Invalid direction.\n";
                }

                if (choice == "4") {
                    customMap.addSprite(row, col, new Guard(row, col, direction));
                }
                else {
                    customMap.addSprite(row, col, new AreaGuard(row, col, direction));
                }
            }
            else if (choice == "6" || choice == "7") {
                string groupName;
                cout << "Enter group name: ";
                getline(cin, groupName);

                SharedState *state = getOrCreateGroup(groups, groupName);

                if (choice == "6") {
                    customMap.addSprite(row, col, new Switch(row, col, state));
                    customMap.getSwitches().push_back(dynamic_cast<Switch*>(customMap.getBoard()[row][col]));
                }
                else {
                    customMap.addSprite(row, col, new Door(row, col, state));
                    customMap.getDoors().push_back(dynamic_cast<Door*>(customMap.getBoard()[row][col]));
                }
            }
            else {
                cout << "Invalid selection.\n";
            }
        }
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
