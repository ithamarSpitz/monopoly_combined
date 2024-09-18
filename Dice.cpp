#include "Dice.h"
#include <chrono>

Dice::Dice() : rng(std::chrono::steady_clock::now().time_since_epoch().count()), dist(1, 6) {}

int Dice::roll() {
    return dist(rng);
}

bool Dice::isDouble(int roll1, int roll2) const {
    return roll1 == roll2;
}