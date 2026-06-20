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
            int spyX = spy->getX();
            int spyY = spy->getY();
            switch (direction) {
                case 'w':
                    return grid[spyX - 1][spyY]->getIcon() != '#';
                case 'a':
                    return grid[spyX][spyY - 1]->getIcon() != '#';
                case 's':
                    return grid[spyX + 1][spyY]->getIcon() != '#';
                case 'd':
                    return grid[spyX][spyY + 1]->getIcon() != '#';
                default:
                    return false;
            }
        }
    public:
        Map1() {
            spy = new Spy(5, 1);
            guard.push_back(new Guard(1, 5));
            // Walls
            for (int i = 0; i < 6; i++) {
                addSprite(i, 0, new Wall(i, 0));
                addSprite(i, 6, new Wall(i, 6));
            }
            for (int i = 1; i < 6; i++) {
                addSprite(0, i, new Wall(0, i));
                addSprite(5, i, new Wall(5, i));
            }

            // Goal
            addSprite(4, 5, new Goal(4, 5));

            for (Guard* g : guard) {
                addSprite(g->getX(), g->getY(), g);
            }
            // Spy
            addSprite(spy->getX(), spy->getY(), spy);
        }
        bool moveSpy(char direction) {
            if (isValidMove(direction)) {
                char icon;
                int spyX = spy->getX();
                int spyY = spy->getY();
                grid[spyX][spyY] = nullptr; // Remove the spy from the current location in grid
                switch (direction) { // Change the location of the spy object
                    case 'w':
                        spy->setX(spyX - 1);
                        break;
                    case 'a':
                        spy->setY(spyY - 1);
                        break;
                    case 's':
                        spy->setX(spyX + 1);
                        break;
                    case 'd':
                        spy->setY(spyY + 1);
                        break;
                }
                icon = grid[spy->getX()][spy->getY()]->getIcon(); // Get the icon of the new location in grid
                if (icon == '$') { // Check if the new location is the goal
                    std::cout << "You win!\n";
                    exit(0);
                } else if (icon == '^' || icon == '>' || icon == 'v' || icon == '<') { // Check if the new location is a guard
                    std::cout << "You lose!\n";
                    exit(0);
                }
                grid[spy->getX()][spy->getY()] = spy; // Place the spy in the new location in grid
                return true;
            }
            return false; // Invalid move, get program to ask for input again
        }

};
#endif
