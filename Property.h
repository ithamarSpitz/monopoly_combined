#ifndef PROPERTY_H
#define PROPERTY_H

#include "Square.h"
#include <string>

class Property : public Square {
protected:
    int price;
    int rent;
    Player* owner;

public:
    Property(const std::string& name, int price, int rent) 
        : Square(name), price(price), rent(rent), owner(nullptr) {}

    virtual int getRent() const { return rent; }
    int getPrice() const { return price; }
    Player* getOwner() const { return owner; }
    void setOwner(Player* newOwner) { owner = newOwner; }

    virtual void landedOn(Player& player) override;
};

#endif // PROPERTY_H