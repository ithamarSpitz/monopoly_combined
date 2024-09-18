#ifndef GO_SQUARE_H
#define GO_SQUARE_H

#include "Square.h"
#include "Player.h"

class GoSquare : public Square {
public:
    GoSquare(const std::string& name);
    void landedOn(Player& player) override;
};

#endif // GO_SQUARE_H