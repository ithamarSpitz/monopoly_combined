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
    GameHandler();
    void setupGame(Game& game);
    void updatePlayerPositions(const std::vector<std::vector<int>>& playerPositions);
    void updatePlayerDetails(Player& player);
    void updateLetter(Player& player);
    std::string askQuestion(const std::vector<std::string>& options);
    void playTurn(Game& game);
    void build(Game& game);
    void buildHouse(Player& player);
    void buildHotel(Player& player);
    void popup(const std::string& text);
    void buyProperty(Game& game);
    void jailTurn(Player& player);
    bool addTwoSquares(Game& game);
    void updateAllPlayersProperties(const Game& game);

    QWidget* window;
    CustomWidget* customWidget;
    std::vector<std::string> squareTitles;  // Changed from reference to regular member
    std::vector<std::vector<int>> squareLetters;
};

#endif  // GAMEHANDLER_H
