#include "ChanceSquare.h"
#include "Player.h"
#include "Game.h"
#include <iostream>

void ChanceSquare::landedOn(Player& player) {
    std::cout << player.getName() << " landed on Chance" << std::endl;
    Game* game = player.getGame();
    if (game) {
        game->drawChanceCard(player);
    } else {
        std::cerr << "Error: Game pointer is null" << std::endl;
    }
}