#ifndef CHANCE_SQUARE_H
#define CHANCE_SQUARE_H

#include "Square.h"

class ChanceSquare : public Square {
public:
    ChanceSquare(const std::string& name) : Square(name) {}
    void landedOn(Player& player) override;
};

#endif // CHANCE_SQUARE_H