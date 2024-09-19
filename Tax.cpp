#include "Tax.h"
#include <iostream>

Tax::Tax(const std::string& name, int taxAmount) : Square(name), amount(taxAmount) {}

void Tax::landedOn(Player& player) {
    player.removeMoney(amount);
}