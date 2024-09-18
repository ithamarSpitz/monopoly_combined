#ifndef CARD_H
#define CARD_H

#include <string>

class Game;
class Player;

class Card {
protected:
    std::string description;

public:
    Card(const std::string& desc) : description(desc) {}
    virtual ~Card() = default;

    virtual void execute(Player& player, Game& game) = 0;
    std::string getDescription() const { return description; }
};

#endif // CARD_H