#ifndef COMMUNITY_CHEST_SQUARE_H
#define COMMUNITY_CHEST_SQUARE_H

#include "Square.h"

class CommunityChestSquare : public Square {
public:
    CommunityChestSquare(const std::string& name) : Square(name) {}
    void landedOn(Player& player) override;
};

#endif // COMMUNITY_CHEST_SQUARE_H