// Dice.h
#ifndef DICE_H
#define DICE_H

#include <random>

class Dice {
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

public:
    Dice();
    int roll();
    bool isDouble(int roll1, int roll2) const;
};

#endif