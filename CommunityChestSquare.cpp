#include "CommunityChestSquare.h"
#include "Player.h"
#include "Game.h"
#include <iostream>

void CommunityChestSquare::landedOn(Player& player) {
    Game* game = player.getGame();
    if (game) {
        game->drawCommunityChestCard(player);
    } else {
        std::cerr << "Error: Game pointer is null" << std::endl;
    }
}