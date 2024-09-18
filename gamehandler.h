#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include "customwidget.h"
#include <vector>
#include <string>
#include <QColor>
#include <QString>
#include <QDialog>
#include "Game.h"

class GameHandler {
public:
    static const int NUM_SQUARES = 40;  
    GameHandler();
    void setupGame(const std::vector<int>& colors, const std::vector<std::string>& titles);
    void updatePlayerPositions(const std::vector<std::vector<int>>& playerPositions);
    void updatePlayerDetails(Player& player);
    void updateLetters(const std::vector<std::vector<int>>& letters);
    void updateLetter(Player& player);
    std::string askQuestion(const std::vector<std::string>& options);
    void playTurn(Game& game);
    void build(Game& game);
    void buildHouse(Player& player);
    void buildHotel(Player& player);
    void popup(const std::string& text);
    void buyProperty(Game& game);
    void jailTurn(Player& player);
    void updateAllPlayersProperties(const Game& game);

private:
    CustomWidget* customWidget;
    std::vector<std::vector<int>> squareLetters;
    std::vector<std::string> squareTitles;  // Changed from reference to regular member
};

#endif  // GAMEHANDLER_H
