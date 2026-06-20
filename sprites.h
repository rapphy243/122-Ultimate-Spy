#ifndef SPRITES_H
#define SPRITES_H

#include <random>
#include <iostream>

class Sprite {
    protected:
        char icon;

    public:
        Sprite();
        Sprite(char i) {
        icon = i;
    };
    char getIcon() {
        return icon;
    }
    friend std::ostream &operator<<(std::ostream &out, const Sprite &s){
        out << s.icon;
        return out;
    }
};

class Wall : public Sprite {
    public:
        Wall() {
            icon = '#';
        }
};

class Goal : public Sprite {
    public:
        Goal() {
            icon = '$';
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
        Guard() {
            icon = randDirection();
        }
};

class Spy : public Sprite {
    public:
        Spy() {
            icon = '@';
        }
};

#endif
