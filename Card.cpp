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
        return;
    }
    
    if (newPosition < currentPosition) {
        player.addMoney(200);
    }
    player.moveToPosition(newPosition);
    
    // Land on the new square
    game.getBoard().getSquare(newPosition)->landedOn(player);
}

void CollectCard::execute(Player& player, Game& game) {
    (void)game;
    player.addMoney(amount);
}

void PayCard::execute(Player& player, Game& game) {
    (void)game;
    player.removeMoney(amount);
}

void GetOutOfJailCard::execute(Player& player, Game& game) {
    (void)game;
    player.addGetOutOfJailCard();
}

void GoToJailCard::execute(Player& player, Game& game) {
    game.sendPlayerToJail(player);
}