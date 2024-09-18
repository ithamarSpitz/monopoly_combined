#include "ChanceCard.h"
#include "CommunityChestCard.h"
#include "Player.h"
#include "Game.h"
#include "Board.h"
#include "Card.h"
#include <iostream>

void AdvanceToCard::execute(Player& player, Game& game) {
    int currentPosition = player.getPosition();
    int newPosition = game.getBoard().getSquareIndexByName(destination);
    
    if (newPosition == -1) {
        std::cout << "Error: Destination square '" << destination << "' not found." << std::endl;
        return;
    }
    
    if (newPosition < currentPosition) {
        std::cout << player.getName() << " passes GO and collects $200" << std::endl;
        player.addMoney(200);
    }
    player.moveToPosition(newPosition);
    std::cout << player.getName() << " advances to " << destination << std::endl;
    
    // Land on the new square
    game.getBoard().getSquare(newPosition)->landedOn(player);
}

void CollectCard::execute(Player& player, Game& game) {
    (void)game;
    player.addMoney(amount);
    std::cout << player.getName() << " collects $" << amount << std::endl;
}

void PayCard::execute(Player& player, Game& game) {
    (void)game;
    player.removeMoney(amount);
    std::cout << player.getName() << " pays $" << amount << std::endl;
}

void GetOutOfJailCard::execute(Player& player, Game& game) {
    (void)game;
    player.addGetOutOfJailCard();
    std::cout << player.getName() << " receives a Get Out of Jail Free card" << std::endl;
}

void GoToJailCard::execute(Player& player, Game& game) {
    game.sendPlayerToJail(player);
}