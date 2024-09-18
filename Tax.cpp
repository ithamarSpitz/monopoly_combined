#include "Tax.h"
#include <iostream>

Tax::Tax(const std::string& name, int taxAmount) : Square(name), amount(taxAmount) {}

void Tax::landedOn(Player& player) {
    std::cout << player.getName() << " landed on " << getName() << ". Paying $" << amount << " in taxes." << std::endl;
    player.removeMoney(amount);
}