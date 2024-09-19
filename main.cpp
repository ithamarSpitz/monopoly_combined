#include <QApplication>
#include <iostream>
#include "gamehandler.h"
#include "Game.h"

int main(int argc, char *argv[]) {
    // Initialize Qt application
    QApplication app(argc, argv);
    
    // Initialize game handler and game logic


    GameHandler gameHandler;
    int answer = gameHandler.askQuestion({"2","3","4","5","6","7","8"}).front()-'0';
    Game game(answer); // Example: 4 players

    std::vector<std::vector<int>> playerPositions = {{}};
    for (int i = 0; i < answer; i++){
        playerPositions[0].push_back(i);
    }
    // Setup the game board in the UI
    gameHandler.setupGame(game);
    gameHandler.updatePlayerPositions(playerPositions);
    gameHandler.playTurn(game);

    // Start the Qt event loop
    return app.exec();
}
