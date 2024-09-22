#include "Player.h"
#include "Property.h"
#include "Board.h"
#include "Game.h"
#include <algorithm>

// Constructor for the Player class
// Initializes a player with a name, initial money (1500), starting position (0),
// and sets up references to the game board and game object
Player::Player(const std::string& name, const Board* board, Game* game) 
    : name(name), money(1500), position(0), inJail(false), turnsInJail(0), getOutOfJailCards(0), board(board), game(game) {}

// Destructor for the Player class
// Removes the player's ownership from all properties they own
Player::~Player() {
    for (Property* property : properties) {
        property->setOwner(nullptr);
    }
}

// Moves the player by a specified number of spaces
// Uses the board's normalizePosition method to handle wrapping around the board
void Player::moveBy(int spaces) {
    position = board->normalizePosition(position + spaces);
}

// Moves the player to a specific position on the board
// Also uses normalizePosition to ensure the position is valid
void Player::moveToPosition(int newPosition) {
    position = board->normalizePosition(newPosition);
}

// Returns the player's current position on the board
int Player::getPosition() const {
    return board->normalizePosition(position);
}

// Adds money to the player's balance
void Player::addMoney(int amount) {
    money += amount;
}

// Removes money from the player's balance
void Player::removeMoney(int amount) {
    money -= amount;
}

// Allows the player to buy a property
// Adds the property to the player's list and sets the player as the owner
void Player::buyProperty(Property* property) {
    properties.push_back(property);
    property->setOwner(this);
}

// Checks if the player owns a specific property
bool Player::ownsProperty(const Property* property) const {
    return std::find(properties.begin(), properties.end(), property) != properties.end();
}

// Sends the player to jail
// Sets the inJail flag and resets the turnsInJail counter
void Player::goToJail() {
    inJail = true;
    turnsInJail = 0;
}

// Checks if the player is currently in jail
bool Player::isInJail() const {
    return inJail;
}

// Releases the player from jail
// Resets both the inJail flag and turnsInJail counter
void Player::releaseFromJail() {
    inJail = false;
    turnsInJail = 0;
}

// Returns the player's current money balance
int Player::getMoney() const {
    return money;
}

// Returns the player's name
const std::string& Player::getName() const {
    return name;
}

// Adds a "Get Out of Jail Free" card to the player's inventory
void Player::addGetOutOfJailCard() {
    getOutOfJailCards++;
}

// Attempts to use a "Get Out of Jail Free" card
// Returns true if successful (card was available), false otherwise
bool Player::useGetOutOfJailCard() {
    if (getOutOfJailCards > 0) {
        getOutOfJailCards--;
        return true;
    }
    return false;
}

// Increments the number of turns the player has spent in jail
void Player::incrementTurnsInJail() {
    turnsInJail++;
}

// Returns the number of turns the player has spent in jail
int Player::getTurnsInJail() const {
    return turnsInJail;
}

// Returns a constant reference to the player's list of owned properties
const std::vector<Property*>& Player::getProperties() const {
    return properties;
}

// Clears all properties owned by the player
// This could be used when the player goes bankrupt or for game reset
void Player::clearProperties() {
    properties.clear();
}

// Adds a property to the player's list of owned properties
// This is different from buyProperty as it doesn't set the owner of the property
void Player::addProperty(Property* property) {
    properties.push_back(property);
}

// Returns the number of "Get Out of Jail Free" cards the player has
int Player::getGetOutOfJailCards() const {
    return getOutOfJailCards;
}

// Checks if this player owns the property that another player has landed on
bool Player::ownsPropertyLandedOn(const Player& otherPlayer) const {
    // Get the square the other player has landed on
    const Square* landedSquare = board->getSquare(otherPlayer.getPosition());
    // Try to cast it to a Property object
    const Property* landedProperty = dynamic_cast<const Property*>(landedSquare);
    
    // If it's a property and this player owns it, return true
    if (landedProperty && landedProperty->getOwner() == this) {
        return true;
    }
    return false;
}