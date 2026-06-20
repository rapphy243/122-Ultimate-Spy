#ifndef SPRITES_H
#define SPRITES_H

#include <random>
#include <iostream>

class Sprite {
    protected:
        char icon;
        int x;
        int y;

    public:
        Sprite() {
            icon = ' ';
            x = 0;
            y = 0;
        }
        Sprite(char i, int xPos, int yPos) {
            x = xPos;
            y = yPos;
            icon = i;
        }
        char getIcon() {
            return icon;
        }
        int getX() {
            return x;
        }
        int getY() {
            return y;
        }
        int setX(int newX) {
            x = newX;
        }
        int setY(int newY) {
            y = newY;
        }
        friend std::ostream &operator<<(std::ostream &out, const Sprite &s) {
            out << s.icon;
            return out;
        }
};

class Wall : public Sprite {
    public:
        Wall(int xPos, int yPos) {
            icon = '#';
            x = xPos;
            y = yPos;
        }
};

class Goal : public Sprite {
    public:
        Goal(int xPos, int yPos) {
            icon = '$';
            x = xPos;
            y = yPos;
        }
};

class Guard : public Sprite {
    private:
    char randDirection() {
        switch (static_cast<short>(std::rand() % 4)) {
        case 0:
            return '^';
        case 1:
            return '>';
        case 2:
            return 'v';
        default:
            return '<';
        }
    }

    public:
        Guard(int xPos, int yPos) {
            icon = randDirection();
            x = xPos;
            y = yPos;
        }
};

class Spy : public Sprite {
    public:
        Spy(int xPos, int yPos) {
            icon = '@';
            x = xPos;
            y = yPos;
        }
};

#endif
