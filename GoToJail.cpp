#include "GoToJail.h"
#include "Game.h"  // Add this line
#include <iostream>

GoToJail::GoToJail(const std::string& name) : Square(name) {}

void GoToJail::landedOn(Player& player) {
    player.goToJail();
    player.moveToPosition(player.getGame()->getBoard().getSquareIndexByName("Jail"));
}