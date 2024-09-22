#include "gamehandler.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QObject>
#include <QApplication>
#include <QTimer>

// Define sqColors as a vector of QColor objects for board squares
std::vector<QColor> sqColors = {
    QColor(165, 42, 42),   // brown
    QColor(135, 206, 250),   // light blue
    QColor(255, 192, 203),   // pink
    QColor(255, 165, 0), // orange
    QColor(139, 0, 0), // dark red
    QColor(154, 205, 50), // yellowgreen
    QColor( 0, 128, 0), // green
    QColor( 0, 0, 139),  // darkblue
    QColor(150, 150, 150)  // Orange
};

// Define nameColors as a vector of strings for player colors
std::vector<std::string> nameColors = {
        "blue", "yellow", "cyan",
        "magenta", "gray", "darkGreen", "darkRed","red"
};

// Constructor for GameHandler class
GameHandler::GameHandler() 
    : window(new QWidget),
      customWidget(new CustomWidget(11,9)),
      squareTitles(std::vector<std::string>{""}),
      squareLetters(std::vector<std::vector<int>>{})  // Initialize squareLetters
{
    // Create and set up the main window
    QWidget* window = new QWidget;
    window->setWindowTitle("Monopoly Game");
    QVBoxLayout* mainLayout = new QVBoxLayout(window);
    mainLayout->addWidget(customWidget);

    window->setLayout(mainLayout);
    window->resize(1000, 600);
    window->show();
}

// Function to add two squares to the game board
bool GameHandler::addTwoSquares(Game& game){
    // Ask user if they want to add squares
    std::string answer =  askQuestion({"add squares?", "no"});
    if(answer == "add squares?"){
        // Define valid locations for new squares
        std::vector<int> validLocations = {0, 9, 20, 29};
        int location1, location2;
        
        // Ask user to choose locations for new squares
        answer = askQuestion({
            "locatios " + std::to_string(validLocations[0]) + " and " + std::to_string(validLocations[2]),
            "locatios " + std::to_string(validLocations[0]) + " and " + std::to_string(validLocations[3]),
            "locatios " + std::to_string(validLocations[1]) + " and " + std::to_string(validLocations[2]),
            "locatios " + std::to_string(validLocations[1]) + " and " + std::to_string(validLocations[3])
            });
        
        // Determine chosen locations
        if(answer.find(std::to_string(validLocations[0])) != std::string::npos){
            location1 = validLocations[0];
        }else{
            location1 = validLocations[1];
        }
        if(answer.find(std::to_string(validLocations[2])) != std::string::npos){
            location2 = validLocations[2];
        }else{
            location2 = validLocations[3];
        }

        // Define possible square types
        std::vector<std::string>types{"Street", "ChanceSquare", "CommunityChestSquare", "UtilityProperty"};
        std::string question = "first square is a ";
        std::string type1;
        
        // Ask user to choose type for first square
        answer = askQuestion({
            question+types[0],
            question+types[1],
            question+types[2],
            question+types[3]
            });
        for (int i = 0; i < 4; i++){ if(answer.find(types[i]) != std::string::npos) { type1 = types[i]; } }
        
        // Ask user to choose type for second square
        question = "second square is a ";
        std::string type2;
        answer = askQuestion({
            question+types[0],
            question+types[1],
            question+types[2],
            question+types[3]
            });
        for (int i = 0; i < 4; i++){ if(answer.find(types[i]) != std::string::npos) { type2 = types[i]; } }
        
        // Add the two new squares to the game board
        game.getBoard().addTwoSquares(type1, type2, location1, location2, 
                                  "New " + type1 + " 1", "New " + type2 + " 2", 100, 100);
        return true;
    }
    return false;
}

// Function to set up the game
void GameHandler::setupGame(Game& game) {
    int counter = 0;
    // Add squares to the game board
    while (addTwoSquares(game)){
        counter++;
    }
    // If squares were added, update the custom widget
    if(counter > 0){
        customWidget = new CustomWidget(11+counter,9);
        QVBoxLayout* mainLayout = new QVBoxLayout(window);
        mainLayout->addWidget(customWidget);
        window->setLayout(mainLayout);
        window->show();
    }
    
    // Initialize vectors for colors and titles
    std::vector<int> colors = {};
    std::vector<std::string> titles = {};

    // Get square names and colors from the game board
    auto streetColors  = game.getBoard().getSquareNamesAndColors();
    for (const auto& [name, color] : streetColors) {
        titles.push_back(name);
        colors.push_back(color);
    }

    // Set up square titles and letters
    squareTitles = titles;
    QStringList qTitles;
    for (const auto& title : titles) {
        qTitles << QString::fromStdString(title);
    }

    for (size_t i = 0; i < colors.size(); i++) {
        squareLetters.push_back(std::vector<int>{-1, 0});
    }
    
    // Set up square colors
    QMap<int, QColor> squareColors;
    for (int i = 0; i < static_cast<int>(colors.size()); ++i) {
        squareColors[i] = sqColors[colors[i]];
    }

    // Update custom widget with colors and titles
    customWidget->setSquareColors(squareColors);
    customWidget->setTitles(qTitles);

    QApplication::processEvents();
}

// Function to update player positions on the board
void GameHandler::updatePlayerPositions(const std::vector<std::vector<int>>& playerPositions) {
    QMap<int, QList<int>> qtPlayerPositions;
    for (int i = 0; i < static_cast<int>(playerPositions.size()); ++i) {
        qtPlayerPositions[i] = QList<int>::fromVector(QVector<int>::fromStdVector(playerPositions[i]));
    }

    customWidget->updatePlayerPositions(qtPlayerPositions);
    QApplication::processEvents();
}

// Function to update player details in the UI
void GameHandler::updatePlayerDetails(Player& currentPlayer) {
    const std::string& name = currentPlayer.getName();
    int playerNum = currentPlayer.getName().back() - '1';
    const std::string& color = nameColors[playerNum];
    int money = currentPlayer.getMoney();
    int cards = currentPlayer.getGetOutOfJailCards();
    customWidget->setPlayerDetails(
        QString::fromStdString(name),
        QString::fromStdString(color),
        money,
        cards
    );    
    QApplication::processEvents();
}

// Function to update property ownership indicators on the board
void GameHandler::updateLetter(Player& player) {
    for (const auto& property : player.getProperties()) {
        Street* street = dynamic_cast<Street*>(property);
        if (street) {  // Check if the property is a Street
            std::string name = street->getName();
            for (size_t i = 0; i < squareTitles.size(); i++) {
                if (squareTitles[i] == name) {
                    int playerNum = player.getName().back() - '1';
                    if (street->getHasHotel()) {
                        squareLetters[i] = std::vector<int>{playerNum, 5};
                    } else {
                        squareLetters[i] = std::vector<int>{playerNum, street->getNumHouses()};
                    }
                    break;  // We found the matching square, no need to continue the inner loop
                }
            }
        }
    }

    // Update the widget with the new squareLetters
    QMap<int, QList<int>> qtLetters;
    for (int i = 0; i < player.getGame()->getBoard().getSize(); ++i) {
        qtLetters[i] = QList<int>{squareLetters[i][0], squareLetters[i][1]};
    }
    customWidget->setSquareLetters(qtLetters);
}

// Function to ask a question to the user with multiple choice options
std::string GameHandler::askQuestion(const std::vector<std::string>& options) {
    QDialog dialog;
    dialog.setWindowTitle("Player Decision");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    QLabel* label = new QLabel("Please select an option:");
    layout->addWidget(label);

    QComboBox* questionComboBox = new QComboBox;
    for (const auto& option : options) {
        questionComboBox->addItem(QString::fromStdString(option));
    }
    layout->addWidget(questionComboBox);

    QPushButton* submitButton = new QPushButton("Submit");
    layout->addWidget(submitButton);

    std::string selectedOption;

    QObject::connect(submitButton, &QPushButton::clicked, [&]() {
        selectedOption = questionComboBox->currentText().toStdString();
        dialog.accept();
    });

    dialog.exec();
    return selectedOption;
}

// Function to show a popup message to the user
void GameHandler::popup(const std::string& text) {
    QDialog dialog;
    dialog.setWindowTitle("Player Decision");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    QLabel* label = new QLabel(QString::fromStdString(text));
    layout->addWidget(label);
    

    QPushButton* submitButton = new QPushButton("OK");
    layout->addWidget(submitButton);

    std::string selectedOption;

    QObject::connect(submitButton, &QPushButton::clicked, [&]() {
        dialog.accept();
    });

    dialog.exec();
}

// Function to handle property buying
void GameHandler::buyProperty(Game& game) {
    Player& player = const_cast<Player&>(game.getCurrentPlayer()); // Remove const
    const Square* landedSquare = game.getBoard().getSquare(player.getPosition());
    Property* property = dynamic_cast<Property*>(const_cast<Square*>(landedSquare));
    
    if (property) {
        if(!property->getOwner()) {
            // If property is unowned, ask if player wants to buy
            std::string prompt = "Wanna buy " + property->getName() + " for $" + std::to_string(property->getPrice()) + "?";
            std::vector<std::string> options{prompt, "No, thanks"};
            std::string choice = askQuestion(options);
            
            if (choice == options[0]) {
                if (player.getMoney() >= property->getPrice()) {
                    // Player buys the property
                    player.removeMoney(property->getPrice());
                    property->setOwner(&player);
                    player.buyProperty(property);
                    std::string answer = player.getName() + " bought " + property->getName() + " for $" + std::to_string(property->getPrice());
                    popup(answer);
                } else {
                    popup(player.getName() + " doesn't have enough money to buy.");
                }
            }
        } else if(property->getOwner()->getName() != player.getName()) {
            // If property is owned by another player, pay rent
            popup(player.getName() + " should pay to " + property->getOwner()->getName() + " $" + std::to_string(property->getRent()) + " for rent." );
        } else {
            // If property is owned by the current player
            popup(player.getName() + " owns " + property->getName());
        }
    } else {
        // If it's a tax square
        Tax* tax = dynamic_cast<Tax*>(const_cast<Square*>(landedSquare));
        if(tax){
            popup(player.getName() + " should pay $" + std::to_string(tax->getAmount()) + " for tax." );
        }
    }
}// Function to handle building a house
void GameHandler::buildHouse(Player& player) {
    // Get a list of streets where the player can build houses
    std::vector<Street*> buildableStreets;
    for (auto& property : player.getProperties()) {
        Street* street = dynamic_cast<Street*>(property);
        if (street && street->canBuildHouse(player)) {
            buildableStreets.push_back(street);
        }
    }

    if (buildableStreets.empty()) {
        popup("You can't build any houses at the moment.");
        return;
    }    

    // Ask the player which street to build on
    std::vector<std::string> options;
    for (size_t i = 0; i < buildableStreets.size(); ++i) {
        options.push_back(buildableStreets[i]->getName());
    }

    std::string choice = askQuestion(options);       
    
    // Build the house on the chosen street
    for (size_t i = 0; i < buildableStreets.size(); ++i) {
        if(buildableStreets[i]->getName() == choice){
            Street* selectedStreet = buildableStreets[i];
            try {
                selectedStreet->buildHouse(player);
                popup("House built on " + selectedStreet->getName());
            } catch (const std::runtime_error& e) {
                std::string err(e.what());
                popup("Error: " + err);
            } 
            return;
        }
    }
    popup("Invalid choice.\n");
}

// Function to handle building a hotel
void GameHandler::buildHotel(Player& player) {
    // Get a list of streets where the player can build hotels
    std::vector<Street*> buildableStreets;
    for (auto& property : player.getProperties()) {
        Street* street = dynamic_cast<Street*>(property);
        if (street && street->canBuildHotel(player)) {
            buildableStreets.push_back(street);
        }
    }

    if (buildableStreets.empty()) {
        popup("You can't build any hotels at the moment.");
        return;
    }    

    // Ask the player which street to build on
    std::vector<std::string> options;
    for (const auto& street : buildableStreets) {
        options.push_back(street->getName() + " (Houses: " + std::to_string(street->getNumHouses()) + ")");
    }

    std::string choice = askQuestion(options);

    // Build the hotel on the chosen street
    for (auto* street : buildableStreets) {
        if (street->getName() == choice.substr(0, choice.find(" (Houses:"))) {
            try {
                street->buildHotel(player);
                popup("Hotel built on " + street->getName());
            } catch (const std::runtime_error& e) {
                popup("Error: " + std::string(e.what()));
            }
            return;
        }
    }
    
    popup("Invalid choice.");
}

// Function to handle building houses or hotels
void GameHandler::build(Game& game){
    Player& currentPlayer = const_cast<Player&>(game.getCurrentPlayer());
    
    // Check if player is on Free Parking
    if(game.getBoard().getSquare(currentPlayer.getPosition())->getName() == "Free Parking"){
        popup("Free Parking!");
        return;
    } 

    // Allow player to build while they are able to
    while(game.canBuild(currentPlayer)) {
        std::string answer = askQuestion({"Pass","Build House", "Build Hotel"});
        if (answer == "Build House") {
            buildHouse(currentPlayer);
        } else if (answer == "Build Hotel") {
            buildHotel(currentPlayer);
        } else if (answer == "Pass") break;
        updateLetter(currentPlayer);
    }
}

// Function to handle a player's turn while in jail
void GameHandler::jailTurn(Player& player) {
    if (player.useGetOutOfJailCard()) {
        // Player uses Get Out of Jail Free card if available
        popup(player.getName() + " used a Get Out of Jail Free card!");
        player.releaseFromJail();
    } else {
        // Ask player if they want to pay to get out of jail
        std::string answer = askQuestion({"Wanna pay $50 to get out?", "Nah, I'm good"});

        if (answer == "Wanna pay $50 to get out?") {
            player.removeMoney(50);
            player.releaseFromJail();
        } else {
            // Player stays in jail
            player.incrementTurnsInJail();
            if (player.getTurnsInJail() >= 3) {
                // Player must pay after 3 turns in jail
                popup(player.getName() + " has been in jail for 3 turns and must pay $50.");
                player.removeMoney(50);
                player.releaseFromJail();
            } 
        }
    }
}

// Function to update the properties owned by all players in the UI
void GameHandler::updateAllPlayersProperties(const Game& game) {
    QMap<QString, QStringList> allProperties;
    
    for (const auto& player : game.getPlayers()) {
        QStringList playerProperties;
        for (const auto& property : player->getProperties()) {
            playerProperties.append(QString::fromStdString(property->getName()));
        }
        allProperties[QString::fromStdString(player->getName())] = playerProperties;
    }

    customWidget->setAllPlayersProperties(allProperties);
}

// Function to play a turn for a player
void GameHandler::playTurn(Game& game) {
    do {
        game.startTurn();

        Player& currentPlayer = const_cast<Player&>(game.getCurrentPlayer());
        updatePlayerDetails(currentPlayer);

        // Roll dice
        std::vector<int> dice = game.roll(currentPlayer);
        int roll1 = dice[0];
        int roll2 = dice[1];
        std::string rollmsg(currentPlayer.getName() + " rolled " + std::to_string(roll1) + " and " + std::to_string(roll2));

        game.checkConsecutiveDoubles(currentPlayer, roll1, roll2);

        if (currentPlayer.isInJail()) {
            // Handle jail turn
            jailTurn(currentPlayer);
            popup(rollmsg);
        } else {
            // Move player and process turn
            rollmsg += ".\n" + currentPlayer.getName() + " is mooving to: " + game.getBoard().getSquare(currentPlayer.getPosition()+roll1+roll2)->getName();
            popup(rollmsg);
            game.processPlayerTurn(currentPlayer);
            if(game.getLastDrawnCardName() != ""){
                popup(game.getLastDrawnCardName());
            }
        }

        // Update UI elements
        updatePlayerDetails(currentPlayer);
        updatePlayerPositions(game.getPlayerPositions());
        updateAllPlayersProperties(game);
        
        // Handle property buying and building
        buyProperty(game);
        build(game);

        // Check if turn ends
        if (roll1 != roll2 || currentPlayer.isInJail()) {
            game.endTurn(currentPlayer);
        } else if (roll1 == roll2 && !currentPlayer.isInJail()){
            popup("You have one more turn!");
        }

        // Check for game over after each turn
        if (game.isGameOver()) {
            break;
        }
    } while (true);

    // Game is over, show popup and close the program
    QDialog gameOverDialog;
    gameOverDialog.setWindowTitle("Game Over");
    QVBoxLayout* layout = new QVBoxLayout(&gameOverDialog);
    QLabel* label = new QLabel("Game Over!");
    layout->addWidget(label);
    QPushButton* okButton = new QPushButton("OK");
    layout->addWidget(okButton);
    QObject::connect(okButton, &QPushButton::clicked, [&]() {
        gameOverDialog.accept();
        std::exit(0); // Forcefully exit the application
    });
    gameOverDialog.exec(); // Make sure to call exec() to show the dialog
}