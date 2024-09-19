#ifndef CHANCE_CARD_H
#define CHANCE_CARD_H

#include "Card.h"

// Base class for Chance cards, inheriting from Card
class ChanceCard : public Card {
public:
    // Constructor: Initializes a Chance card with a description
    ChanceCard(const std::string& desc) : Card(desc) {}
    
    // Pure virtual function to be implemented by derived classes
    virtual void execute(Player& player, Game& game) override = 0;
};

// Specific Chance card types

// Card that advances player to a specific location
class AdvanceToCard : public ChanceCard {
private:
    std::string destination;  // The destination square name
public:
    // Constructor: Sets up the card with a destination
    AdvanceToCard(const std::string& dest) : ChanceCard("Advance to " + dest), destination(dest) {}
    void execute(Player& player, Game& game) override;
};

// Card that gives money to the player
class CollectCard : public ChanceCard {
private:
    int amount;  // Amount of money to collect
public:
    // Constructor: Sets up the card with an amount and reason
    CollectCard(int amt, const std::string& reason) : ChanceCard(reason + ". Collect $" + std::to_string(amt)), amount(amt) {}
    void execute(Player& player, Game& game) override;
};

// Card that requires the player to pay money
class PayCard : public ChanceCard {
private:
    int amount;  // Amount of money to pay
public:
    // Constructor: Sets up the card with an amount and reason
    PayCard(int amt, const std::string& reason) : ChanceCard(reason + ". Pay $" + std::to_string(amt)), amount(amt) {}
    void execute(Player& player, Game& game) override;
};

// Card that gives the player a Get Out of Jail Free card
class GetOutOfJailCard : public ChanceCard {
public:
    // Constructor: Sets up the Get Out of Jail Free card
    GetOutOfJailCard() : ChanceCard("Get Out of Jail Free") {}
    void execute(Player& player, Game& game) override;
};

// Card that sends the player to Jail
class GoToJailCard : public ChanceCard {
public:
    // Constructor: Sets up the Go to Jail card
    GoToJailCard() : ChanceCard("Go to Jail. Go directly to Jail, do not pass Go, do not collect $200") {}
    void execute(Player& player, Game& game) override;
};

#endif // CHANCE_CARD_H