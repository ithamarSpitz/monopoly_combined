#include "GoSquare.h"
#include <iostream>

GoSquare::GoSquare(const std::string& name) : Square(name) {}

void GoSquare::landedOn(Player& player) {
    player.addMoney(200);
}