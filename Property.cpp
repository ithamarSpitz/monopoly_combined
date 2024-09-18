#include "Property.h"
#include "Player.h"

void Property::landedOn(Player& player) {
    if (owner && owner != &player) {
        int rentToPay = getRent();
        player.removeMoney(rentToPay);
        owner->addMoney(rentToPay);
    }
}