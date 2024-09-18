#include "UtilityProperty.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <set>

int UtilityProperty::getRent() const{
    const Player& player = getOwner()->getGame()->getCurrentPlayer();
    Game* game = player.getGame();
    int lastRoll = game->getLastRoll();
    int rent = lastRoll * 10;
    std::set<std::string> rails = {"Reading Railroad", "B. & O. Railroad", "Pennsylvania Railroad", "Short Line"};
    if(rails.count(getName())){
        int owned_cnt =0;
        for (auto & prop : getOwner()->getProperties()) {
            owned_cnt += rails.count(prop->getName());
        }
        rent = owned_cnt*50;
    }
    return rent;  
}

void UtilityProperty::landedOn(Player& player) {
    std::cout << player.getName() << " landed on " << getName() << std::endl;
    if (getOwner() != nullptr && getOwner() != &player) {
        // Calculate and pay rent
        int rent = getRent();
        player.removeMoney(rent);
        getOwner()->addMoney(rent);

    } else {
        std::cout << player.getName() << " owns this utility." << std::endl;
    }
}