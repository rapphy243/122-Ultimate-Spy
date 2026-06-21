#ifndef GRID_H
#define GRID_H

#include <random>
#include "sprites.h"

bool isValidMove(char direction, Sprite* grid[10][10], const Spy& spy) {
    int spyX = spy.getX();
    int spyY = spy.getY();
    switch (direction)
    {
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

bool moveSpy(char direction, Sprite* grid[10][10], Spy* spy) {
    if (isValidMove(direction, grid, *spy)){
        char icon;
        int spyX = spy->getX();
        int spyY = spy->getY();
        grid[spyX][spyY] = nullptr; // Remove the spy from the current location in grid
        switch (direction)
        { // Change the location of the spy object
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
        if (icon == '$')
        { // Check if the new location is the goal
            std::cout << "You win!\n";
            exit(0);
        }
        else if (icon == '^' || icon == '>' || icon == 'v' || icon == '<')
        { // Check if the new location is a guard
            std::cout << "You lose!\n";
            exit(0);
        }
        grid[spy->getX()][spy->getY()] = spy; // Place the spy in the new location in grid
        return true;
    }
    return false; // Invalid move, get program to ask for input again
}

void moveGuard(Guard* guard, Sprite* grid[10][10], Spy* spy) {
    char direction = guard->getIcon();
    int guardX = guard->getX();
    int guardY = guard->getY();
    char nextIcon;
    switch (direction) {
    case '^':
        nextIcon = grid[guardX - 1][guardY]->getIcon();
        if (nextIcon == ' ') {
            if (rand() % 2) {
                guard->setIcon(randDirection());
            } else {
                grid[guardX][guardY] = nullptr;
                guard->setX(guardX - 1);
            }
        } else if (nextIcon == '@') {
            std::cout << "You lose!\n";
            exit(0);
        } else {
            guard->setIcon(randDirection());
        }
        break;
    case '>':
        nextIcon = grid[guardX][guardY + 1]->getIcon();
        if (nextIcon == ' ') {
            if (rand() % 2) {
                guard->setIcon(randDirection());
            } else {
                grid[guardX][guardY] = nullptr;
                guard->setY(guardY + 1);
            }
        } else if (nextIcon == '@') {
            std::cout << "You lose!\n";
            exit(0);
        } else {
            guard->setIcon(randDirection());
        }
        break;
    case 'v':
        nextIcon = grid[guardX + 1][guardY]->getIcon();
        if (nextIcon == ' ') {
            if (rand() % 2) {
                guard->setIcon(randDirection());
            } else {
                grid[guardX][guardY] = nullptr;
                guard->setX(guardX + 1);
            }
        } else if (nextIcon == '@') {
            std::cout << "You lose!\n";
            exit(0);
        } else {
            guard->setIcon(randDirection());
        }
        break;
    case '<':
        nextIcon = grid[guardX][guardY - 1]->getIcon();
        if (nextIcon == ' ') {
            if (rand() % 2) {
                guard->setIcon(randDirection());
            } else {
                grid[guardX][guardY] = nullptr;
                guard->setY(guardY - 1);
            }
        } else if (nextIcon == '@') {
            std::cout << "You lose!\n";
            exit(0);
        } else {
            guard->setIcon(randDirection());
        }
        break;
    }
}

class Grid {
    protected:
        Sprite* grid[10][10];
        Spy *spy;
        std::vector<Guard*> guard;
        void addSprite(int x, int y, Sprite *s) {
            grid[x][y] = s;
        }

    public:
        Grid() {
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    grid[i][j] = nullptr;
                }
            }
            spy = nullptr;
            guard = {};
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
        Spy* getSpy() {
            return spy;
        }
        std::vector<Guard*> getGuards() {
            return guard;
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
    public:
        Map1() {
            spy = new Spy(4, 1);
            guard.push_back(new Guard(1, 5));
            guard.push_back(new Guard(2, 1));
            // Walls
            for (int i = 0; i < 6; i++) { // Top and bottom walls
                addSprite(i, 0, new Wall(i, 0));
                addSprite(i, 8, new Wall(i, 8));
            }
            for (int i = 1; i < 8; i++) { // Left and right walls
                addSprite(0, i, new Wall(0, i));
                addSprite(5, i, new Wall(5, i));
            }
            // Inner walls
            addSprite(2, 4, new Wall(2, 4));
            addSprite(3, 4, new Wall(3, 4));
            addSprite(4, 4, new Wall(4, 4));

            // Goal
            addSprite(4, 7, new Goal(4, 7));

            // Guards
            for (Guard* g : guard) {
                addSprite(g->getX(), g->getY(), g);
            }

            // Spy
            addSprite(spy->getX(), spy->getY(), spy);
        }
};

#endif
