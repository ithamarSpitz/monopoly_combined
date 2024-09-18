// FreeParking.cpp
#include "FreeParking.h"
#include <iostream>

FreeParking::FreeParking(const std::string& name) : Square(name) {}

void FreeParking::landedOn(Player& player) {
    std::cout << player.getName() << " landed on " << getName() << ". Enjoy your free parking!" << std::endl;
}