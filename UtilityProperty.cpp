#include "UtilityProperty.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <set>

// Calculates and returns the rent for the utility property
int UtilityProperty::getRent() const {
    // Get the current player from the game
    const Player& player = getOwner()->getGame()->getCurrentPlayer();
    Game* game = player.getGame();
    
    // Get the last dice roll from the game
    int lastRoll = game->getLastRoll();
    
    // Calculate the base rent (10 times the dice roll)
    int rent = lastRoll * 10;

    // Define a set of railroad names
    std::set<std::string> rails = {"Reading Railroad", "B. & O. Railroad", "Pennsylvania Railroad", "Short Line"};

    // Check if this property is a railroad
    if (rails.count(getName())) {
        int owned_cnt = 0;
        
        // Count how many railroads the owner has
        for (auto & prop : getOwner()->getProperties()) {
            owned_cnt += rails.count(prop->getName());
        }
        
        // Calculate railroad rent based on the number of railroads owned
        rent = owned_cnt * 50;
    }

    return rent;  
}

// Handles the action when a player lands on this utility property
void UtilityProperty::landedOn(Player& player) {
    // Check if the property is owned by another player
    if (getOwner() != nullptr && getOwner() != &player) {
        // Calculate and pay rent
        int rent = getRent();
        player.removeMoney(rent);
        getOwner()->addMoney(rent);
    }
}