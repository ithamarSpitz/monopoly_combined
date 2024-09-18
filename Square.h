#ifndef SQUARE_H
#define SQUARE_H

#include <string>

class Player; // Forward declaration

class Square {
private:
    std::string name;

public:
    Square(const std::string& name);
    virtual ~Square() = default;
    
    std::string getName() const;
    virtual void landedOn(Player& player) = 0; // Pure virtual function
};

#endif // SQUARE_H