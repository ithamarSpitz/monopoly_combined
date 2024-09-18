#ifndef UTILITY_PROPERTY_H
#define UTILITY_PROPERTY_H

#include "Property.h"
#include "Game.h"
#include<iostream>

class UtilityProperty : public Property {
public:
    UtilityProperty(const std::string& name, int price) 
        : Property(name, price, 0) {} // Rent is calculated dynamically

    void landedOn(Player& player) override;

    int getRent() const override;
};

#endif // UTILITY_PROPERTY_H