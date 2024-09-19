#include "ChanceSquare.h"
#include "Player.h"
#include "Game.h"
#include <iostream>

void ChanceSquare::landedOn(Player& player) {
    Game* game = player.getGame();
    if (game) {
        game->drawChanceCard(player);
    } else {
        std::cerr << "Error: Game pointer is null" << std::endl;
    }
}