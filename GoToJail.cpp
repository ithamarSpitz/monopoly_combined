#include "GoToJail.h"
#include <iostream>

GoToJail::GoToJail(const std::string& name) : Square(name) {}

void GoToJail::landedOn(Player& player) {
    std::cout << player.getName() << " landed on Go To Jail. Moving to Jail!" << std::endl;
    player.goToJail();
    player.moveToPosition(10); // Assuming Jail is at position 10
}