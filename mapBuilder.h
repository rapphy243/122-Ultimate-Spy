#ifndef MAPBUILDER_H
#define MAPBUILDER_H

#include <vector>
#include <fstream>
#include "grid.h"

class CustomMap : public Grid {
    private: 
        std::vector<Sprite*> sprites;
        std::vector<SharedState*> sharedStates;

    public:
        CustomMap() {
            board = Board(10, std::vector<Sprite *>(10, nullptr));
            spy = nullptr;
            guard = std::vector<Guard *>();
            gameOver = false;
        }
        bool loadMap(const std::string& filename) {
            return true;
        }
        bool saveMap(const std::string& filename) {
            std::ofstream file(filename);

            if (!file.is_open()) {
                std::cerr << "Error: Could not open file for writing: " << filename << "\n";
                return false;
            }

            file << board.size() << " " << board[0].size() << "\n";

            for (Sprite* s : sprites) {
                if (s != nullptr) {
                    char icon = s->getIcon();
                    if (icon == '#') {
                        file << "#" << " " << s->getX() << " " << s->getY() << "\n";
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
                    else if (icon == '$') {
                        file << "$" << " " << s->getX() << " " << s->getY() << "\n";
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
                }
            }
            file.close();
            return true;
        }
        void addSprite(int x, int y, Sprite *s) {
            board[x][y] = s;
            sprites.push_back(s);
        }
        void removeSprite(int x, int y) {
            Sprite* s = board[x][y];
            if (s != nullptr) {
                auto it = std::find(sprites.begin(), sprites.end(), s);
                if (it != sprites.end()) {
                    delete *it;
                    sprites.erase(it);
                }
                board[x][y] = nullptr;
            }
        }
        ~CustomMap() {
            for (Sprite* s : sprites) {
                delete s;
            }
            sprites.clear();
        }
};

#endif