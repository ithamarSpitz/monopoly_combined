#ifndef CHANCE_SQUARE_H
#define CHANCE_SQUARE_H

#include "Square.h"

// Represents a Chance square on the Monopoly board
class ChanceSquare : public Square {
public:
    // Constructor: Initializes a Chance square with a name
    ChanceSquare(const std::string& name) : Square(name) {}
    
    // Overridden function to handle when a player lands on this square
    void landedOn(Player& player) override;
};

#endif // CHANCE_SQUARE_H