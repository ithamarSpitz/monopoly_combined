#include "GoSquare.h"
#include <iostream>

GoSquare::GoSquare(const std::string& name) : Square(name) {}

void GoSquare::landedOn(Player& player) {
    std::cout << player.getName() << " landed on Go. Collecting $200!" << std::endl;
    player.addMoney(200);
}