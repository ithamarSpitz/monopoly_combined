// Jail.cpp
#include "Jail.h"
#include <iostream>

Jail::Jail(const std::string& name) : Square(name) {}

void Jail::landedOn(Player& player) {
    if (player.isInJail()) {
        
        if (player.useGetOutOfJailCard()) {
            player.releaseFromJail();
            return;
        }
        
        char choice;
        std::cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            player.removeMoney(JAIL_FEE);
            player.releaseFromJail();
        } else {
            player.incrementTurnsInJail();
            if (player.getTurnsInJail() >= MAX_TURNS_IN_JAIL) {
                player.removeMoney(JAIL_FEE);
                player.releaseFromJail();
            }
        }
    }
}