// Jail.h
#ifndef JAIL_H
#define JAIL_H

#include "Square.h"
#include "Player.h"

class Jail : public Square {
public:
    static constexpr int JAIL_FEE = 50;
    static constexpr int MAX_TURNS_IN_JAIL = 3;

    Jail(const std::string& name);
    void landedOn(Player& player) override;
};

#endif