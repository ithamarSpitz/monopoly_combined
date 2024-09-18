#ifndef GO_TO_JAIL_H
#define GO_TO_JAIL_H

#include "Square.h"
#include "Player.h"

class GoToJail : public Square {
public:
    GoToJail(const std::string& name);
    void landedOn(Player& player) override;
};

#endif // GO_TO_JAIL_H