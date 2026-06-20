#ifndef GRID_H
#define GRID_H

#include "sprites.h"


class Grid {
    protected:
        Sprite* grid[10][10];

    public:
        Grid() {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    grid[i][j] = nullptr;
                }
            }
        }

        void addSprite(int x, int y, Sprite* s) {
            grid[x][y] = s;
        }

        void printGrid() {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    if (grid[i][j] == nullptr) {
                        std::cout << ' ';
                    } else {
                        std::cout << *grid[i][j];
                    }
                }
                std::cout << std::endl;
            }
        }
};

/*
########
#    v #
#>  #  #
#   #  #
#@  # $#
########
*/
class Map1 : public Grid {
    private:
        Spy* spy;
        std::vector<Guard*> guard;
        bool isValidMove(char direction) {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    if (grid[i][j]->getIcon() == '@') {
                        switch (direction) {
                            case 'w':
                                return grid[i-1][j]->getIcon() != '#';
                            case 'a':
                                return grid[i][j - 1]->getIcon() != '#';
                            case 's':
                                return grid[i + 1][j]->getIcon() != '#';
                            case 'd':
                                return grid[i][j+1]->getIcon() != '#';
                        }
                    }
                }
            }
        }
    public:
        Map1() {
            // Walls
            for (int i = 0; i < 10; i++) {
                addSprite(0, i, new Wall());
                addSprite(9, i, new Wall());
                addSprite(i, 0, new Wall());
                addSprite(i, 9, new Wall());
            }
            addSprite(2, 3, new Wall());
            addSprite(3, 3, new Wall());
            addSprite(4, 3, new Wall());
            addSprite(5, 3, new Wall());
            addSprite(6, 3, new Wall());

            // Goal
            addSprite(8, 8, new Goal());

            // Guard
            addSprite(1, 5, new Guard());

            // Spy
            addSprite(5, 1, new Spy());
        }
        void moveSpy(char direction) {
            if (isValidMove(direction)) {
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 10; j++) {
                        if (grid[i][j]->getIcon() == '@') {
                            switch (direction) {
                                case 'w':
                                    std::swap(grid[i][j], grid[i-1][j]);
                                    break;
                                case 'a':
                                    std::swap(grid[i][j], grid[i][j - 1]);
                                    break;
                                case 's':
                                    std::swap(grid[i][j], grid[i + 1][j]);
                                    break;
                                case 'd':
                                    std::swap(grid[i][j], grid[i][j+1]);
                                    break;
                            }
                        }
                    }
                }
            }
        }

};
#endif
