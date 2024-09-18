#include <QApplication>
#include <iostream>
#include "gamehandler.h"
#include "Game.h"

int main(int argc, char *argv[]) {
    // Initialize Qt application
    QApplication app(argc, argv);
    
    // Initialize game handler and game logic
    std::vector<int> colors = {
        8,0,8,0,8,8,1,8,1,1,8,
        2,8,2,2,8,3,8,3,3,
        8,4,8,4,4,8,5,5,8,5,8,
        6,6,8,6,8,8,7,8,7};
    std::vector<std::string> titles = {
        "Go", "Mediterranean Avenue", "Community Chest", "Baltic Avenue", 
        "Income Tax", "Reading Railroad", "Oriental Avenue", "Chance",
        "Vermont Avenue", "Connecticut Avenue", "Jail", "St. Charles Place",
        "Electric Company", "States Avenue", "Virginia Avenue", "Pennsylvania Railroad",
        "St. James Place", "Community Chest", "Tennessee Avenue", "New York Avenue",
        "Free Parking", "Kentucky Avenue", "Chance", "Indiana Avenue", 
        "Illinois Avenue", "B. & O. Railroad", "Atlantic Avenue", "Ventnor Avenue",
        "Water Works", "Marvin Gardens", "Go To Jail", "Pacific Avenue", 
        "North Carolina Avenue", "Community Chest", "Pennsylvania Avenue", "Short Line",
        "Chance", "Park Place", "Luxury Tax", "Boardwalk"
    };

    GameHandler gameHandler;
    int answer = gameHandler.askQuestion({"2","3","4","5","6","7","8"}).front()-'0';
    Game game(answer); // Example: 4 players

    std::vector<std::vector<int>> playerPositions = {{}};
    for (int i = 0; i < answer; i++){
        playerPositions[0].push_back(i);
    }
    // Setup the game board in the UI
    gameHandler.setupGame(colors, titles);
    gameHandler.updatePlayerPositions(playerPositions);
    gameHandler.playTurn(game);

    // Start the Qt event loop
    return app.exec();
}
