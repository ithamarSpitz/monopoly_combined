#ifndef COMMUNITY_CHEST_CARD_H
#define COMMUNITY_CHEST_CARD_H

#include "Card.h"
#include "Player.h"
#include "Game.h"

// Represents a Community Chest card that collects money from each player
class CollectFromEachPlayerCard : public Card {
public:
    // Constructor: Sets up the card with an amount and description
    CollectFromEachPlayerCard(int amount, const std::string& desc);
    
    // Implements the card's action
    void execute(Player& player, Game& game) override;

private:
    int amount;  // Amount to collect from each player
};

#endif // COMMUNITY_CHEST_CARD_H