#ifndef GUARD_H
#define GUARD_H

#include <string>
#include <iostream>

class Guard {
    private:
        char direction;
    public:
        Guard() {
            direction = '>';
        };

        char getDirection() {
            return direction;
        }

        friend std::ostream& operator<<(std::ostream& out, const Guard& g) {
            out << g.direction;
            return out;
        }
};

#endif
