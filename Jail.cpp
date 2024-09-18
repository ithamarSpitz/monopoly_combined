// Jail.cpp
#include "Jail.h"
#include <iostream>

Jail::Jail(const std::string& name) : Square(name) {}

void Jail::landedOn(Player& player) {
    if (player.isInJail()) {
        std::cout << player.getName() << " is in jail." << std::endl;
        
        if (player.useGetOutOfJailCard()) {
            std::cout << player.getName() << " used a Get Out of Jail Free card!" << std::endl;
            player.releaseFromJail();
            return;
        }
        
        std::cout << "Do you want to pay $" << JAIL_FEE << " to get out of jail? (y/n): ";
        char choice;
        std::cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            player.removeMoney(JAIL_FEE);
            std::cout << player.getName() << " paid $" << JAIL_FEE << " to get out of jail." << std::endl;
            player.releaseFromJail();
        } else {
            player.incrementTurnsInJail();
            if (player.getTurnsInJail() >= MAX_TURNS_IN_JAIL) {
                std::cout << player.getName() << " has been in jail for 3 turns and must pay $" << JAIL_FEE << std::endl;
                player.removeMoney(JAIL_FEE);
                player.releaseFromJail();
            } else {
                std::cout << player.getName() << " stays in jail." << std::endl;
            }
        }
    } else {
        std::cout << player.getName() << " is just visiting the jail." << std::endl;
    }
}