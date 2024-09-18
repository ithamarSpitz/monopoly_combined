#ifndef CHANCE_CARD_H
#define CHANCE_CARD_H

#include "Card.h"

class ChanceCard : public Card {
public:
    ChanceCard(const std::string& desc) : Card(desc) {}
    virtual void execute(Player& player, Game& game) override = 0;
};

// Specific Chance card types
class AdvanceToCard : public ChanceCard {
private:
    std::string destination;
public:
    AdvanceToCard(const std::string& dest) : ChanceCard("Advance to " + dest), destination(dest) {}
    void execute(Player& player, Game& game) override;
};

class CollectCard : public ChanceCard {
private:
    int amount;
public:
    CollectCard(int amt, const std::string& reason) : ChanceCard(reason + ". Collect $" + std::to_string(amt)), amount(amt) {}
    void execute(Player& player, Game& game) override;
};

class PayCard : public ChanceCard {
private:
    int amount;
public:
    PayCard(int amt, const std::string& reason) : ChanceCard(reason + ". Pay $" + std::to_string(amt)), amount(amt) {}
    void execute(Player& player, Game& game) override;
};

class GetOutOfJailCard : public ChanceCard {
public:
    GetOutOfJailCard() : ChanceCard("Get Out of Jail Free") {}
    void execute(Player& player, Game& game) override;
};

class GoToJailCard : public ChanceCard {
public:
    GoToJailCard() : ChanceCard("Go to Jail. Go directly to Jail, do not pass Go, do not collect $200") {}
    void execute(Player& player, Game& game) override;
};

#endif // CHANCE_CARD_H