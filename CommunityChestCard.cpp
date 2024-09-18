#include "CommunityChestCard.h"
#include <iostream>

CollectFromEachPlayerCard::CollectFromEachPlayerCard(int amount, const std::string& desc)
    : Card(desc+". Each player should give you $10"), amount(amount) {}

void CollectFromEachPlayerCard::execute(Player& player, Game& game) {
    std::cout << player.getName() << " " << getDescription() << std::endl;
    int totalCollected = 0;
    for (const auto& otherPlayer : game.getPlayers()) {
        if (otherPlayer.get() != &player) {
            if (otherPlayer->getMoney() >= amount) {
                otherPlayer->removeMoney(amount);
                player.addMoney(amount);
                totalCollected += amount;
                std::cout << otherPlayer->getName() << " pays $" << amount << " to " << player.getName() << std::endl;
            } else {
                std::cout << otherPlayer->getName() << " is bankrupt and cannot pay." << std::endl;
                game.transferAssets(*otherPlayer, player);
                game.removePlayer(otherPlayer.get());
            }
        }
    }
    std::cout << player.getName() << " collected a total of $" << totalCollected << std::endl;
}