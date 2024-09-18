// In CommunityChestCard.h
#ifndef COMMUNITY_CHEST_CARD_H
#define COMMUNITY_CHEST_CARD_H

#include "Card.h"
#include "Player.h"
#include "Game.h"

class CollectFromEachPlayerCard : public Card {
public:
    CollectFromEachPlayerCard(int amount, const std::string& desc);
    void execute(Player& player, Game& game) override;

private:
    int amount;
};

#endif // COMMUNITY_CHEST_CARD_H

