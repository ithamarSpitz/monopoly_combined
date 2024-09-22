// FreeParking.h
#ifndef FREE_PARKING_H
#define FREE_PARKING_H

#include "Square.h"
#include "Player.h"

class FreeParking : public Square {
public:
    FreeParking(const std::string& name) : Square(name) {};
    void landedOn(Player& player) override {};
};

#endif