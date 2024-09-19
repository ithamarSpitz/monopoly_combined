#ifndef CARD_H
#define CARD_H

#include <string>

class Game;
class Player;

// Base class for all card types in the game
class Card {
protected:
    std::string description;  // Holds the text description of the card

public:
    // Constructor: Initializes the card with a description
    Card(const std::string& desc) : description(desc) {}
    
    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~Card() = default;

    // Pure virtual function to be implemented by derived classes
    // Defines the action to be taken when the card is drawn
    virtual void execute(Player& player, Game& game) = 0;
    
    // Getter for the card's description
    std::string getDescription() const { return description; }
};

#endif // CARD_H