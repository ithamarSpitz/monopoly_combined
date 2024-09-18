#include "Player.h"
#include "Property.h"
#include "Board.h"
#include "Game.h"
#include <algorithm>

Player::Player(const std::string& name, const Board* board, Game* game) 
    : name(name), money(1500), position(0), inJail(false), turnsInJail(0), getOutOfJailCards(0), board(board), game(game) {}

Player::~Player() {
    for (Property* property : properties) {
        property->setOwner(nullptr);
    }
}

void Player::moveBy(int spaces) {
    position = board->normalizePosition(position + spaces);
}

void Player::moveToPosition(int newPosition) {
    position = board->normalizePosition(newPosition);
}

int Player::getPosition() const {
    return board->normalizePosition(position);
}

void Player::addMoney(int amount) {
    money += amount;
}

void Player::removeMoney(int amount) {
    money -= amount;
}

void Player::buyProperty(Property* property) {
    properties.push_back(property);
    property->setOwner(this);
}

bool Player::ownsProperty(const Property* property) const {
    return std::find(properties.begin(), properties.end(), property) != properties.end();
}

void Player::goToJail() {
    inJail = true;
    turnsInJail = 0;
}

bool Player::isInJail() const {
    return inJail;
}

void Player::releaseFromJail() {
    inJail = false;
    turnsInJail = 0;
}

int Player::getMoney() const {
    return money;
}

const std::string& Player::getName() const {
    return name;
}

void Player::addGetOutOfJailCard() {
    getOutOfJailCards++;
}

bool Player::useGetOutOfJailCard() {
    if (getOutOfJailCards > 0) {
        getOutOfJailCards--;
        return true;
    }
    return false;
}

void Player::incrementTurnsInJail() {
    turnsInJail++;
}

int Player::getTurnsInJail() const {
    return turnsInJail;
}

const std::vector<Property*>& Player::getProperties() const {
    return properties;
}

void Player::clearProperties() {
    properties.clear();
}

void Player::addProperty(Property* property) {
    properties.push_back(property);
}

int Player::getGetOutOfJailCards() const {
    return getOutOfJailCards;
}

bool Player::ownsPropertyLandedOn(const Player& otherPlayer) const {
    const Square* landedSquare = board->getSquare(otherPlayer.getPosition());
    const Property* landedProperty = dynamic_cast<const Property*>(landedSquare);
    
    if (landedProperty && landedProperty->getOwner() == this) {
        return true;
    }
    return false;
}