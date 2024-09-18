#ifndef TAX_H
#define TAX_H

#include "Square.h"
#include "Player.h"

class Tax : public Square {
private:
    int amount;

public:
    Tax(const std::string& name, int taxAmount);
    int getAmount() {return amount;}
    void landedOn(Player& player) override;
};

#endif // TAX_H