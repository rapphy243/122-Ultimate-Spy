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
        virtual std::string getType() const = 0;
        virtual std::string getDescription() const = 0;
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
        std::string getType() const  {
            return "Wall";
        }
        std::string getDescription() const {
            return "A wall that blocks movement.";
        }
};

class Goal : public Sprite {
    public:
        Goal(int xPos, int yPos) {
            icon = '$';
            x = xPos;
            y = yPos;
        }
        std::string getType() const  {
            return "Goal";
        }
        std::string getDescription() const {
            return "The destination the spy must reach.";
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
        std::string getType() const  {
            return "Basic Guard";
        }
        std::string getDescription() const {
            return "A guard that patrols along a line.";
        }
         virtual void turn() {
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

class AreaGuard : public Guard {
    public:
        AreaGuard(int xPos, int yPos) : Guard(xPos, yPos) {

        }
        AreaGuard(int xPos, int yPos, char dir) : Guard(xPos, yPos, dir) {
            
        }
        std::string getType() const  {
            return "Area Guard";
        }
        std::string getDescription() const {
            return "A guard that patrols within a area.";
        }
        void turn() {
            switch (icon) {
            case '^':
                icon = '>';
                break;
            case '>':
                icon = 'v';
                break;
            case 'v':
                icon = '<';
                break;
            case '<':
                icon = '^';
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
        std::string getType() const  {
            return "Spy";
        }
        std::string getDescription() const {
            return "The player.";
        }
};

class SharedState {
    private:
        bool state;
    public:
        SharedState() {
            state = false;
        }
        bool getState() const {
            return state;
        }
        void toggleState() {
            state = !state;
        }
};

class Door : public Sprite {
    protected:
        char previousIcon; // Stores open/closed state
        SharedState* color;
    public:
        Door(int xPos, int yPos, SharedState* c) {
            icon = '▤';
            previousIcon = ' ';
            color = c;
            x = xPos;
            y = yPos;
        }
        std::string getType() const  {
            return "Door";
        }
        std::string getDescription() const {
            return "A door that can be opened by stepping on a switch.";
        }
        void toggleDoor() {
            char temp = icon;
            icon = previousIcon;
            previousIcon = temp;
        }
};

class Switch : public Sprite {
     protected:
        char previousIcon;
        SharedState* color;
    public:
        Switch(int xPos, int yPos, SharedState* c) {
            icon = ' ';
            previousIcon = ' ';
            color = c;
            x = xPos;
            y = yPos;
        }
        std::string getType() const  {
            return "Switch";
        }
        std::string getDescription() const {
            return "A switch that can open a door when stepped on.";
        }
        void toggleDoor() {
            char temp = icon;
            icon = previousIcon;
            previousIcon = temp;
        }
};

#endif
