#ifndef SPY_H
#define SPY_H

#include <string>
#include <iostream>

class Spy {
    private: 
        
    public:
        Spy();
        
        bool moveDown() {

        }
        
        friend std::ostream& operator<<(std::ostream& out, const Spy& p) {
            out << "@";
            return out;
        }
};

#endif
