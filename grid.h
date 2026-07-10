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

    Sprite *cell;
    switch (direction) {
    case 'w':
        cell = grid[spyX - 1][spyY];
        return cell == nullptr || !cell->blocksMovement();
    case 'a':
        cell = grid[spyX][spyY - 1];
        return cell == nullptr || !cell->blocksMovement();
    case 's':
        cell = grid[spyX + 1][spyY];
        return cell == nullptr || !cell->blocksMovement();
    case 'd':
        cell = grid[spyX][spyY + 1];
        return cell == nullptr || !cell->blocksMovement();
    default:
        return false;
    }
}

class Grid {
protected:
    Board board;
    Spy *spy;
    std::vector<Guard *> guard;
    bool gameOver;
    void addSprite(int x, int y, Sprite *s) {
        board[x][y] = s;
    }

public:
    Grid() {
        board = Board(10, std::vector<Sprite *>(10, nullptr));
        spy = nullptr;
        guard = std::vector<Guard *>();
        gameOver = false;
    }

    void printGrid() {
        for (auto &row : board) {
            for (Sprite *cell : row) {
                if (cell == nullptr) {
                    std::cout << ' ';
                }
                else {
                    std::cout << cell->getIcon();
                }
            }
            std::cout << std::endl;
        }
    }
    bool isGameOver() {
        return gameOver;
    }
    void setGameOver() {
        gameOver = true;
    }

    Board& getBoard() {
        return board;
    }
    Spy* getSpy() {
        return spy;
    }
    std::vector<Guard*>& getGuards() {
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
            guard.push_back(new Guard(1, 5, 'v'));
            guard.push_back(new Guard(2, 1, '>'));
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

class Map2 : public Grid {
public:
    Map2() {
        spy = new Spy(8, 1);
        guard.push_back(new Guard(5, 5, '>'));
        guard.push_back(new Guard(3, 5, '>'));
        // Walls
        for (int i = 0; i < 10; i++) { // Top and bottom walls
            addSprite(i, 0, new Wall(i, 0));
            addSprite(i, 9, new Wall(i, 9));
        }
        for (int i = 1; i < 9; i++) { // Left and right walls
            addSprite(0, i, new Wall(0, i));
            addSprite(9, i, new Wall(9, i));
        }
        for (int i = 2; i < 8; i++) { // Inner walls
            addSprite(2, i, new Wall(2, i));
            addSprite(4, i, new Wall(4, i));
            addSprite(7, i, new Wall(6, i));
        }
        addSprite(4, 8, new Wall(4, 8));
        addSprite(7, 1, new Wall(6, 1));

        // Goal
        addSprite(1, 8, new Goal(1, 8));

        // Guards
        for (Guard* g : guard) {
            addSprite(g->getX(), g->getY(), g);
        }

        // Spy
        addSprite(spy->getX(), spy->getY(), spy);
    }
};

class Map3 : public Grid {
public:
    Map3() {
        board = Board(11, std::vector<Sprite *>(11, nullptr));
        spy = new Spy(9, 1);
        guard.push_back(new AreaGuard(4, 5, '>'));
        guard.push_back(new Guard(2, 8, '^'));
        // Walls
        for (int i = 0; i < 11; i++) { // Top and bottom walls
            addSprite(i, 0, new Wall(i, 0));
            addSprite(i, 10, new Wall(i, 10));
        }
        for (int i = 1; i < 10; i++) { // Left and right walls
            addSprite(0, i, new Wall(0, i));
            addSprite(10, i, new Wall(10, i));
        }
        for (int i = 1; i < 8; i++) {
            addSprite(7, i, new Wall(7, i));
        }
        for (int i = 3; i < 7; i++) {
            addSprite(i, 7, new Wall(i, 7));
        }
        for (int i = 3; i < 7; i++) {
            addSprite(3, i, new Wall(3, i));
        }

        // Goal
        addSprite(5, 5, new Goal(5, 5));

        // Guards
        for (Guard* g : guard) {
            addSprite(g->getX(), g->getY(), g);
        }

        // Spy
        addSprite(spy->getX(), spy->getY(), spy);
    }
};

class Map4 : public Grid {
public:
    Map4() {
        board = Board(11, std::vector<Sprite *>(11, nullptr));
        spy = new Spy(9, 1);
        guard.push_back(new AreaGuard(5, 1, '^'));
        guard.push_back(new AreaGuard(5, 9, 'v'));
        // Walls
        for (int i = 0; i < 11; i++) { // Top and bottom walls
            addSprite(i, 0, new Wall(i, 0));
            addSprite(i, 10, new Wall(i, 10));
        }
        for (int i = 1; i < 10; i++) { // Left and right walls
            addSprite(0, i, new Wall(0, i));
            addSprite(10, i, new Wall(10, i));
        }

        for (int i = 1; i < 10; i++) { 
            if (i == 5) {
                i++;
            }
            addSprite(2, i, new Wall(2, i));
            addSprite(8, i, new Wall(8, i));
        }

        //Button 1
        SharedState* sharedState1 = new SharedState();
        addSprite(9, 9, new Switch(9, 9, sharedState1));
        addSprite(2, 5, new Door(2, 5, sharedState1));
        addSprite(8, 5, new Door(8, 5, sharedState1));

        SharedState *sharedState2 = new SharedState();
        addSprite(1, 1, new Switch(1, 1, sharedState2));
        addSprite(4, 1, new Switch(4, 1, sharedState2));
        addSprite(1, 6, new Door(1, 6, sharedState2));
        // Goal
        addSprite(1, 9, new Goal(1, 9));

        // // Guards
        for (Guard* g : guard) {
            addSprite(g->getX(), g->getY(), g);
        }

        // Spy
        addSprite(spy->getX(), spy->getY(), spy);
    }
};

bool moveSpy(char direction, Grid& map) {
    Board &board = map.getBoard();
    Spy *spy = map.getSpy();
    if (isValidMove(direction, board, *spy)) {
        int spyX = spy->getX();
        int spyY = spy->getY();
        int dx = 0;
        int dy = 0;

        switch (direction) {
        case 'w':
            dx = -1;
            break;
        case 'a':
            dy = -1;
            break;
        case 's':
            dx = 1;
            break;
        case 'd':
            dy = 1;
            break;
        }

        int nextX = spyX + dx;
        int nextY = spyY + dy;

        Sprite* target = board[nextX][nextY];

        if (target != nullptr) {
            if (target->getType() == "Switch") {
                Switch *switchSprite = dynamic_cast<Switch *>(target);
                switchSprite->toggleDoor();
                nextY -= dy;
                nextX -= dx;
            }
            else if (target->getType() == "Door") {
                Door *doorSprite = dynamic_cast<Door *>(target);
                if (doorSprite->blocksMovement())
                {
                    return false; // Can't move into a closed door
                }
                else
                { // Go past door if it's open
                    nextY += dy;
                    nextX += dx;
                }
            }
        }

        char icon = getCellIcon(board, nextX, nextY);
        if (icon == '$') {
            std::cout << "You win!\n";
            map.setGameOver();
        }
        else if (icon == '^' || icon == '>' || icon == 'v' || icon == '<') {
            std::cout << "You lose!\n";
            map.setGameOver();
        }

        board[spyX][spyY] = nullptr;
        spy->setX(nextX);
        spy->setY(nextY);
        board[nextX][nextY] = spy;
        return true;
    }
    return false;
}

void moveGuards(Grid& map) {
    std::vector<Guard *> guards = map.getGuards();
    Board &grid = map.getBoard();
    for (Guard *guard : guards) {
        int guardX = guard->getX();
        int guardY = guard->getY();
        char direction = guard->getIcon();
        int dx = 0;
        int dy = 0;

        switch (direction) {
        case '^':
            dx = -1;
            break;
        case '>':
            dy = 1;
            break;
        case 'v':
            dx = 1;
            break;
        case '<':
            dy = -1;
            break;
        }

        int nextX = guardX + dx;
        int nextY = guardY + dy;

        Sprite *target = grid[nextX][nextY];
        if (target != nullptr && target->getType() == "Switch")  {
            Switch *switchSprite = dynamic_cast<Switch *>(target);
            switchSprite->toggleDoor();
            nextX += dx;
            nextY += dy;
        }

        int visionX = guardX + dx;
        int visionY = guardY + dy;

        while (getCellIcon(grid, visionX, visionY) != '#') {
            if (getCellIcon(grid, visionX, visionY) == '@') {
                std::cout << "You lose!\n";
                map.setGameOver();
            }
            visionX += dx;
            visionY += dy;
        }

        if (getCellIcon(grid, nextX, nextY) != ' ') {
            guard->turn();

            dx = 0;
            dy = 0;

            switch (guard->getIcon()) {
            case '^':
                dx = -1;
                break;
            case '>':
                dy = 1;
                break;
            case 'v':
                dx = 1;
                break;
            case '<':
                dy = -1;
                break;
            }

            visionX = guardX + dx;
            visionY = guardY + dy;
            while (getCellIcon(grid, visionX, visionY) != '#') {
                if (getCellIcon(grid, visionX, visionY) == '@') {
                    std::cout << "You lose!\n";
                    map.setGameOver();
                    break;
                }
                visionX += dx;
                visionY += dy;
            }

            nextX = guardX + dx;
            nextY = guardY + dy;

            // If still blocked, stay in place
            if (getCellIcon(grid, nextX, nextY) != ' ') {
                nextX = guardX;
                nextY = guardY;
            }
        }

        grid[guardX][guardY] = nullptr;
        guard->setX(nextX);
        guard->setY(nextY);
        grid[nextX][nextY] = guard;
    }
}

#endif
