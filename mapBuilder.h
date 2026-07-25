#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include <vector>
#include <fstream>
#include <sstream>
#include "grid.h"

class CustomMap : public Grid {
    private: 
        std::vector<Sprite*> userAddedSprites;
        std::vector<SharedState*> sharedStates;

    public:
        CustomMap() {
            board = Board(10, std::vector<Sprite*>(10, nullptr));
            spy = nullptr;
            guard = std::vector<Guard*>();
            gameOver = false;
        }
        bool loadMap(const std::string& filename) {
            std::ifstream file(filename);

            if (!file.is_open()) {
                std::cerr << "Error: Could not open file for reading: " << filename << "\n";
                return false;
            }

            int rows, cols;
            file >> rows >> cols;
            board = Board(rows, std::vector<Sprite*>(cols, nullptr));

            std::string line;
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    std::istringstream iss(line);
                    char icon;
                    iss >> icon;
                    if (icon == '#') {
                        int x, y;
                        iss >> x >> y;
                        addSprite(x, y, new Wall(x, y));
                    }
                    else if (icon == '$') {
                        int x, y;
                        iss >> x >> y;
                        addSprite(x, y, new Goal(x, y));
                    }
                    else if (icon == '@') {
                        int x, y;
                        iss >> x >> y;
                        spy = new Spy(x, y);
                        addSprite(x, y, spy);
                    }
                    else if (icon == '^' || icon == '>' || icon == 'v' || icon == '<') {
                        char type;
                        int x, y;
                        iss >> type >> x >> y;
                        if (type == 'G') {
                            Guard* g = new Guard(x, y, icon);
                            guard.push_back(g);
                            addSprite(x, y, g);
                        } else if (type == 'A') {
                            AreaGuard* ag = new AreaGuard(x, y, icon);
                            guard.push_back(ag);
                            addSprite(x, y, ag);
                        }
                    }
                    else if (icon == 'D') {
                        std::string linkName;
                        int x, y;
                        iss >> linkName >> x >> y;

                        SharedState* link = nullptr;
                        for (SharedState* state : sharedStates) {
                            if (state->getName() == linkName) {
                                link = state;
                                break;
                            }
                        }

                        if (link == nullptr) {
                            link = new SharedState(linkName);
                            sharedStates.push_back(link);
                        }

                        Door* d = new Door(x, y, link);
                        addSprite(x, y, d);
                        doors.push_back(d);
                    }
                    else if (icon == 'S') {
                        std::string linkName;
                        int x, y;
                        iss >> linkName >> x >> y;

                        SharedState* link = nullptr;
                        for (SharedState* state : sharedStates) {
                            if (state->getName() == linkName) {
                                link = state;
                                break;
                            }
                        }

                        if (link == nullptr) {
                            link = new SharedState(linkName);
                            sharedStates.push_back(link);
                        }

                        Switch* s = new Switch(x, y, link);
                        addSprite(x, y, s);
                        switches.push_back(s);
                    }
                    else {
                        std::cerr << "Error: Unknown icon '" << icon << "' in map file.\n";
                        return false;
                    }
                }
            }
            
            return true;
        }
        bool saveMap(const std::string& filename) {
            std::ofstream file(filename);

            if (!file.is_open()) {
                std::cerr << "Error: Could not open file for writing: " << filename << "\n";
                return false;
            }

            file << board.size() << " " << board[0].size() << "\n";

            for (Sprite* s : userAddedSprites) {
                if (s != nullptr) {
                    char icon = s->getIcon();
                    if (icon == '#') {
                        file << "#" << " " << s->getX() << " " << s->getY() << "\n";
                    }
                    else if (icon == '$') {
                        file << "$" << " " << s->getX() << " " << s->getY() << "\n";
                    }
                    else if (icon == '@') {
                        file << icon << " " << s->getX() << " " << s->getY() << "\n";
                    }
                    else if (icon == '^' || icon == '>' || icon == 'v' || icon == '<') {
                        if (s->getType() == "Area Guard") {
                            file << icon << " " << "A" << " " << s->getX() << " " << s->getY() << " " << icon << "\n";
                        } else {
                            file << icon << " " << "G" << " " << s->getX() << " " << s->getY() << " " << icon << "\n";
                        }
                    }
                    else if (icon == 'D') {
                        Door* door = dynamic_cast<Door*>(s);
                        if (door != nullptr) {
                            SharedState* link = door->getSharedState();
                            if (link != nullptr) {
                                file << "D" << " " << link->getName() << " " << s->getX() << " " << s->getY() << "\n";
                            }
                        }
                    }
                    else if (icon == 'S') {
                        Switch* sw = dynamic_cast<Switch*>(s);
                        if (sw != nullptr) {
                            SharedState* link = sw->getSharedState();
                            if (link != nullptr) {
                                file << "S" << " " << link->getName() << " " << s->getX() << " " << s->getY() << "\n";
                            }
                        }
                    }
                    else {
                        return false; // There is something that is not supposed to be in the map.
                    }
                }
            }

            file.close();
            return true;
        }
        void setDimensions(int rows, int cols) {
            board = Board(rows, std::vector<Sprite*>(cols, nullptr));
            //Additionally fill the board with walls on the edges
            for (int i = 0; i < rows; ++i) {
                addSprite(i, 0, new Wall(i, 0));
                addSprite(i, cols - 1, new Wall(i, cols - 1));
            }
            for (int j = 0; j < cols; ++j) {
                addSprite(0, j, new Wall(0, j));
                addSprite(rows - 1, j, new Wall(rows - 1, j));
            }
        }
        void addSprite(int x, int y, Sprite *s) {
            board[x][y] = s;
            userAddedSprites.push_back(s);
        }
        void removeSprite(int x, int y) {
            Sprite* s = board[x][y];
            if (s != nullptr) {
                auto it = std::find(userAddedSprites.begin(), userAddedSprites.end(), s);
                if (it != userAddedSprites.end()) {
                    delete *it;
                    userAddedSprites.erase(it);
                }
                board[x][y] = nullptr;
            }
        }
        std::vector<Switch*>& getSwitches() {
            return switches;
        }
        std::vector<Door*>& getDoors() {
            return doors;
        }
        ~CustomMap() {
            for (Sprite* s : userAddedSprites) {
                delete s;
            }
            userAddedSprites.clear();
        }
};

#endif