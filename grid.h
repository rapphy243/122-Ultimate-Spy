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
            switch (direction) {
                case 'w':
                    return grid[spy->getX() - 1][spy->getY()] == nullptr || grid[spy->getX() - 1][spy->getY()]->getIcon() != '#';
                case 'a':
                    return grid[spy->getX()][spy->getY() - 1] == nullptr || grid[spy->getX()][spy->getY() - 1]->getIcon() != '#';
                case 's':
                    return grid[spy->getX() + 1][spy->getY()] == nullptr || grid[spy->getX() + 1][spy->getY()]->getIcon() != '#';
                case 'd':
                    return grid[spy->getX()][spy->getY() + 1] == nullptr || grid[spy->getX()][spy->getY() + 1]->getIcon() != '#';
                default:
                    return false;
            }
        }
    public:
        Map1() {
            spy = new Spy(5, 1);
            // Walls
            for (int i = 0; i < 10; i++)
            {
                addSprite(0, i, new Wall(0, i));
                addSprite(9, i, new Wall(9, i));
                addSprite(i, 0, new Wall(i, 0));
                addSprite(i, 9, new Wall(i, 9));
            }
            addSprite(2, 3, new Wall(2, 3));
            addSprite(3, 3, new Wall(3, 3));
            addSprite(4, 3, new Wall(4, 3));
            addSprite(5, 3, new Wall(5, 3));
            addSprite(6, 3, new Wall(6, 3));

            // Goal
            addSprite(8, 8, new Goal(8, 8));

            // Guard
            addSprite(1, 5, new Guard(1, 5));

            // Spy
            addSprite(spy->getX(), spy->getY(), spy);
        }
        void moveSpy(char direction) {
            if (isValidMove(direction)) {
                // move spy
                grid[spy->getX()][spy->getY()] = nullptr;
                switch (direction) {
                    case 'w':
                        spy->setX(spy->getX() - 1);
                        break;
                    case 'a':
                        spy->setY(spy->getY() - 1);
                        break;
                    case 's':
                        spy->setX(spy->getX() + 1);
                        break;
                    case 'd':
                        spy->setY(spy->getY() + 1);
                        break;
                }
                // check if spy is on goal
                if (grid[spy->getX()][spy->getY()] != nullptr && grid[spy->getX()][spy->getY()]->getIcon() == '$') {
                    std::cout << "You win!" << std::endl;
                    exit(0);
                }
                if
                grid[spy->getX()][spy->getY()] = spy;
            }
        }

};
#endif
