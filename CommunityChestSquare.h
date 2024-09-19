#ifndef COMMUNITY_CHEST_SQUARE_H
#define COMMUNITY_CHEST_SQUARE_H

#include "Square.h"

// Represents a Community Chest square on the Monopoly board
class CommunityChestSquare : public Square {
public:
    // Constructor: Initializes a Community Chest square with a name
    CommunityChestSquare(const std::string& name) : Square(name) {}
    
    // Overridden function to handle when a player lands on this square
    void landedOn(Player& player) override;
};

#endif // COMMUNITY_CHEST_SQUARE_H