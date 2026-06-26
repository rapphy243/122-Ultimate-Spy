#ifndef SPRITES_H
#define SPRITES_H

#include <random>
#include <iostream>

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
        char getIcon() const {
            return icon;
        }
        int getX() const {
            return x;
        }
        int getY() const {
            return y;
        }
        char setIcon(char newIcon) {
            icon = newIcon;
            return icon;
        }
        int setX(int newX) {
            x = newX;
            return x;
        }
        int setY(int newY) {
            y = newY;
            return y;
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
    public:
        Guard(int xPos, int yPos) {
            icon = randDirection();
            x = xPos;
            y = yPos;
        }
        Guard(int xPos, int yPos, char dir) {
            icon = dir;
            x = xPos;
            y = yPos;
        }
        void oppositeDirection() {
            switch (icon) {
            case '^':
                icon = 'v';
                break;
            case 'v':
                icon = '^';
                break;
            case '<':
                icon = '>';
                break;
            case '>':
                icon = '<';
                break;
            default:
                icon = '^';
            }
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
