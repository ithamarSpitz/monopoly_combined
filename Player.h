#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

class Property;
class Board;
class Game;

class Player {
private:
    std::string name;
    int money;
    int position;
    bool inJail;
    int turnsInJail;
    std::vector<Property*> properties;
    int getOutOfJailCards;
    const Board* board;
    Game* game;

public:
    Player(const std::string& name, const Board* board, Game* game);
    ~Player();
    void moveBy(int spaces);
    void moveToPosition(int newPosition);
    void addMoney(int amount);
    void removeMoney(int amount);
    void buyProperty(Property* property);
    bool ownsProperty(const Property* property) const;
    void goToJail();
    bool isInJail() const;
    void releaseFromJail();
    int getMoney() const;
    int getPosition() const;
    const std::string& getName() const;
    void addGetOutOfJailCard();
    bool useGetOutOfJailCard();
    void incrementTurnsInJail();
    int getTurnsInJail() const;
    Game* getGame() const { return game; }
    const std::vector<Property*>& getProperties() const;
    void clearProperties();
    void addProperty(Property* property);
    int getGetOutOfJailCards() const;
    bool ownsPropertyLandedOn(const Player& otherPlayer) const;

};

#endif