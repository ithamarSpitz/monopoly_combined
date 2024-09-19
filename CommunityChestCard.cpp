#include "CommunityChestCard.h"
#include <iostream>

CollectFromEachPlayerCard::CollectFromEachPlayerCard(int amount, const std::string& desc)
    : Card(desc+". Each player should give you $10"), amount(amount) {}

void CollectFromEachPlayerCard::execute(Player& player, Game& game) {
    int totalCollected = 0;
    for (const auto& otherPlayer : game.getPlayers()) {
        if (otherPlayer.get() != &player) {
            if (otherPlayer->getMoney() >= amount) {
                otherPlayer->removeMoney(amount);
                player.addMoney(amount);
                totalCollected += amount;
            } else {
                game.transferAssets(*otherPlayer, player);
                game.removePlayer(otherPlayer.get());
            }
        }
    }
}