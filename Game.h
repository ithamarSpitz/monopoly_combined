// Game.h
#ifndef GAME_H
#define GAME_H
#include <vector>
#include <memory>
#include <string>
#include "Player.h"
#include "Board.h"
#include "Dice.h"
#include "Card.h"

class Game {
private:
    std::vector<std::unique_ptr<Player>> players;
    Board board;
    Dice dice;
    int currentPlayerIndex;
    bool gameOver;
    std::vector<std::unique_ptr<Card>> chanceCards;
    std::vector<std::unique_ptr<Card>> communityChestCards;
    int lastRoll;
    std::unique_ptr<Card> lastDrawnCard;  // New class variable

    void initializeCards();
    void returnPropertiesToBank(Player& player);
    Player* findCreditor(const Player& bankruptPlayer);

public:
    Game(int numPlayers);
    bool isGameOver() const;
    void startTurn();
    void endTurn(Player& player);
    void processPlayerTurn(Player& player);
    void sendPlayerToJail(Player& player);
    void drawChanceCard(Player& player);
    void drawCommunityChestCard(Player& player);
    void removePlayer(Player* playerToRemove);
    void transferAssets(Player& fromPlayer, Player& toPlayer);
    bool canBuild(Player& player);
    std::vector<std::vector<int>> getPlayerPositions() const;
    const Player& getCurrentPlayer() const;
    const Board& getBoard() const { return board; }
    Board& getBoard() { return board; }
    const std::vector<std::unique_ptr<Player>>& getPlayers() const { return players; }
    int getLastRoll() const { return lastRoll; }
    std::vector<int> roll(Player& player);
    void checkConsecutiveDoubles(Player& player, int roll1, int roll2);
    void checkBankruptcy(Player& player);
    std::string getLastDrawnCardName() const;
};

#endif  // GAME_H