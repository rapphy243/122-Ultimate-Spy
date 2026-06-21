#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include "sprites.h"

using Board = std::vector<std::vector<Sprite *>>;

char getCellIcon(Board &grid, int x, int y) {
    Sprite* cell = grid[x][y];
    return cell == nullptr ? ' ' : cell->getIcon();
}

bool isValidMove(char direction, Board &grid, Spy &spy) {
    int spyX = spy.getX();
    int spyY = spy.getY();
    switch (direction) {
    case 'w':
        return getCellIcon(grid, spyX - 1, spyY) != '#';
    case 'a':
        return getCellIcon(grid, spyX, spyY - 1) != '#';
    case 's':
        return getCellIcon(grid, spyX + 1, spyY) != '#';
    case 'd':
        return getCellIcon(grid, spyX, spyY + 1) != '#';
    default:
        return false;
    }
}

bool moveSpy(char direction, Board &grid, Spy *spy) {
    if (isValidMove(direction, grid, *spy)) {
        int spyX = spy->getX();
        int spyY = spy->getY();
        int nextX = spyX;
        int nextY = spyY;

        switch (direction) {
        case 'w':
            nextX = spyX - 1;
            break;
        case 'a':
            nextY = spyY - 1;
            break;
        case 's':
            nextX = spyX + 1;
            break;
        case 'd':
            nextY = spyY + 1;
            break;
        }

        char icon = getCellIcon(grid, nextX, nextY);
        if (icon == '$') {
            std::cout << "You win!\n";
            exit(0);
        }
        else if (icon == '^' || icon == '>' || icon == 'v' || icon == '<') {
            std::cout << "You lose!\n";
            exit(0);
        }

        grid[spyX][spyY] = nullptr;
        spy->setX(nextX);
        spy->setY(nextY);
        grid[nextX][nextY] = spy;
        return true;
    }
    return false;
}

void moveGuard(Guard *guard, Board &grid, Spy *spy) {
    char direction = guard->getIcon();
    int guardX = guard->getX();
    int guardY = guard->getY();
    char nextIcon;

    switch (direction) {
    case '^':
        nextIcon = getCellIcon(grid, guardX - 1, guardY);
        if (nextIcon == ' ') {
            if (rand() % 2) {
                guard->setIcon(randDirection());
            }
            else {
                grid[guardX][guardY] = nullptr;
                guard->setX(guardX - 1);
                guard->setY(guardY);
                grid[guardX - 1][guardY] = guard;
            }
        }
        else if (nextIcon == '@') {
            std::cout << "You lose!\n";
            exit(0);
        }
        else {
            guard->setIcon(randDirection());
        }
        break;
    case '>':
        nextIcon = getCellIcon(grid, guardX, guardY + 1);
        if (nextIcon == ' ') {
            if (rand() % 2) {
                guard->setIcon(randDirection());
            }
            else {
                grid[guardX][guardY] = nullptr;
                guard->setX(guardX);
                guard->setY(guardY + 1);
                grid[guardX][guardY + 1] = guard;
            }
        }
        else if (nextIcon == '@') {
            std::cout << "You lose!\n";
            exit(0);
        }
        else {
            guard->setIcon(randDirection());
        }
        break;
    case 'v':
        nextIcon = getCellIcon(grid, guardX + 1, guardY);
        if (nextIcon == ' ') {
            if (rand() % 2) {
                guard->setIcon(randDirection());
            }
            else {
                grid[guardX][guardY] = nullptr;
                guard->setX(guardX + 1);
                guard->setY(guardY);
                grid[guardX + 1][guardY] = guard;
            }
        }
        else if (nextIcon == '@') {
            std::cout << "You lose!\n";
            exit(0);
        }
        else {
            guard->setIcon(randDirection());
        }
        break;
    case '<':
        nextIcon = getCellIcon(grid, guardX, guardY - 1);
        if (nextIcon == ' ') {
            if (rand() % 2) {
                guard->setIcon(randDirection());
            }
            else {
                grid[guardX][guardY] = nullptr;
                guard->setX(guardX);
                guard->setY(guardY - 1);
                grid[guardX][guardY - 1] = guard;
            }
        }
        else if (nextIcon == '@') {
            std::cout << "You lose!\n";
            exit(0);
        }
        else {
            guard->setIcon(randDirection());
        }
        break;
    }
}

class Grid {
    protected:
        Board grid;
        Spy *spy;
        std::vector<Guard *> guard;

        void addSprite(int x, int y, Sprite *s) {
            grid[x][y] = s;
        }

    public:
        Grid() {
           grid = Board(10, std::vector<Sprite*>(10, nullptr));
           spy = nullptr;
           guard = std::vector<Guard *>();
        }

        void printGrid() {
            for (auto &row : grid) {
               for (Sprite *cell : row) {
                   if (cell == nullptr) {
                        std::cout << ' ';
                    }
                    else {
                        std::cout << *cell;
                    }
                }
                std::cout << std::endl;
            }
        }   

        Board& getGrid() {
            return grid;
        }
        Spy* getSpy() {
            return spy;
        }
        std::vector<Guard*>& getGuards()  {
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
