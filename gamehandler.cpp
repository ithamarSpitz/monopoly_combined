#include "gamehandler.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QObject>
#include <QApplication>
#include <QTimer>

// Define sqColors as a vector of QColor objects
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
std::vector<std::string> nameColors = {
        "blue", "yellow", "cyan",
        "magenta", "gray", "darkGreen", "darkRed","red"
};

GameHandler::GameHandler() 
    : window(new QWidget),
      customWidget(new CustomWidget(11,9)),
      squareTitles(std::vector<std::string>{""}),
      squareLetters(std::vector<std::vector<int>>{})  // Initialize squareLetters
{
    QWidget* window = new QWidget;
    window->setWindowTitle("Monopoly Game");
    QVBoxLayout* mainLayout = new QVBoxLayout(window);
    mainLayout->addWidget(customWidget);

    window->setLayout(mainLayout);
    window->resize(1000, 600);
    window->show();
}

bool GameHandler::addTwoSquares(Game& game){

    std::string answer =  askQuestion({"add squares?", "no"});
    if(answer == "add squares?"){
        std::vector<int> validLocations = {0, 9, 20, 29};
        int location1, location2;
        answer = askQuestion({
            "locatios " + std::to_string(validLocations[0]) + " and " + std::to_string(validLocations[2]),
            "locatios " + std::to_string(validLocations[0]) + " and " + std::to_string(validLocations[3]),
            "locatios " + std::to_string(validLocations[1]) + " and " + std::to_string(validLocations[2]),
            "locatios " + std::to_string(validLocations[1]) + " and " + std::to_string(validLocations[3])
            });
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

        std::vector<std::string>types{"Street", "ChanceSquare", "CommunityChestSquare", "UtilityProperty"};
        std::string question = "first square is a ";
        std::string type1;
        answer = askQuestion({
            question+types[0],
            question+types[1],
            question+types[2],
            question+types[3]
            });
        for (int i = 0; i < 4; i++){ if(answer.find(types[i]) != std::string::npos) { type1 = types[i]; } }
        question = "second square is a ";
        std::string type2;
        answer = askQuestion({
            question+types[0],
            question+types[1],
            question+types[2],
            question+types[3]
            });
        for (int i = 0; i < 4; i++){ if(answer.find(types[i]) != std::string::npos) { type2 = types[i]; } }
        
        game.getBoard().addTwoSquares(type1, type2, location1, location2, 
                                  "New " + type1 + " 1", "New " + type2 + " 2", 100, 100);
        return true;
    }
    return false;
}

void GameHandler::setupGame(Game& game) {
    int counter = 0;
    while (addTwoSquares(game)){
        counter++;
    }
    if(counter > 0){
        customWidget = new CustomWidget(11+counter,9);
        QVBoxLayout* mainLayout = new QVBoxLayout(window);
        mainLayout->addWidget(customWidget);
        window->setLayout(mainLayout);
        window->show();
    }
    std::vector<int> colors = {};
    std::vector<std::string> titles = {};

    auto streetColors  = game.getBoard().getSquareNamesAndColors();
    for (const auto& [name, color] : streetColors) {
        titles.push_back(name);
        colors.push_back(color);
    }

    squareTitles = titles;
    QStringList qTitles;
    for (const auto& title : titles) {
        qTitles << QString::fromStdString(title);
    }

    for (size_t i = 0; i < colors.size(); i++) {
        squareLetters.push_back(std::vector<int>{-1, 0});
    }
    

    QMap<int, QColor> squareColors;
    for (int i = 0; i < static_cast<int>(colors.size()); ++i) {
        squareColors[i] = sqColors[colors[i]];
    }

    customWidget->setSquareColors(squareColors);
    customWidget->setTitles(qTitles);

    QApplication::processEvents();
}

void GameHandler::updatePlayerPositions(const std::vector<std::vector<int>>& playerPositions) {
    QMap<int, QList<int>> qtPlayerPositions;
    for (int i = 0; i < static_cast<int>(playerPositions.size()); ++i) {
        qtPlayerPositions[i] = QList<int>::fromVector(QVector<int>::fromStdVector(playerPositions[i]));
    }

    customWidget->updatePlayerPositions(qtPlayerPositions);
    QApplication::processEvents();
}

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

void GameHandler::updateLetters(const std::vector<std::vector<int>>& letters) {

    
    squareLetters.resize(letters.size(), std::vector<int>{-1, 0});
    for (size_t i = 0; i < letters.size() && i < squareLetters.size(); ++i) {
        if (letters[i].size() >= 2) {
            squareLetters[i] = std::vector<int>{letters[i][0], letters[i][1]};
        }
    }
    
    QMap<int, QList<int>> qtLetters;
    for (int i = 0; i < static_cast<int>(squareLetters.size()); ++i) {
        qtLetters[i] = QList<int>{squareLetters[i][0], squareLetters[i][1]};
    }
    customWidget->setSquareLetters(qtLetters);
}

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

void GameHandler::buyProperty(Game& game) {
    Player& player = const_cast<Player&>(game.getCurrentPlayer()); // Remove const
    const Square* landedSquare = game.getBoard().getSquare(player.getPosition());
    Property* property = dynamic_cast<Property*>(const_cast<Square*>(landedSquare));
    
    if (property) {
        if(!property->getOwner()) {
            std::string prompt = "Wanna buy " + property->getName() + " for $" + std::to_string(property->getPrice()) + "?";
            std::vector<std::string> options{prompt, "No, thanks"};
            std::string choice = askQuestion(options);
            
            if (choice == options[0]) {
                if (player.getMoney() >= property->getPrice()) {
                    player.removeMoney(property->getPrice());
                    property->setOwner(&player);
                    player.buyProperty(property);
                    std::string answer = player.getName() + " bought " + property->getName() + " for $" + std::to_string(property->getPrice());
                    popup(answer);
                } else {
                    popup(player.getName() + " doesn't have enough money to buy.");
                }
            }
        }else if(property->getOwner()->getName() != player.getName()){
            popup(player.getName() + " should pay to " + property->getOwner()->getName() + " $" + std::to_string(property->getRent()) + " for rent." );
        }else{
            popup(player.getName() + " owns " + property->getName());
        }
    }else{
        Tax* tax = dynamic_cast<Tax*>(const_cast<Square*>(landedSquare));
        if(tax){
            popup(player.getName() + " should pay $" + std::to_string(tax->getAmount()) + " for tax." );
        }
    }
}

void GameHandler::buildHouse(Player& player) {
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
    std::vector<std::string> options;
    for (size_t i = 0; i < buildableStreets.size(); ++i) {
        options.push_back( buildableStreets[i]->getName());
    }

    std::string choice = askQuestion(options);       
    
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

void GameHandler::buildHotel(Player& player) {
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

    std::vector<std::string> options;
    for (const auto& street : buildableStreets) {
        options.push_back(street->getName() + " (Houses: " + std::to_string(street->getNumHouses()) + ")");
    }

    std::string choice = askQuestion(options);

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

void GameHandler::build(Game& game){

    Player& currentPlayer = const_cast<Player&>(game.getCurrentPlayer());
    if(game.getBoard().getSquare(currentPlayer.getPosition())->getName() == "Free Parking"){
        popup("Free Parking!");
        return;
    } 
    while(game.canBuild(currentPlayer)) {
            std::string answer = askQuestion({"Pass","Build House", "Build Hotel"});
            if (answer == "Build House") {
                buildHouse(currentPlayer);
            } else if (answer == "Build Hotel") {
                buildHotel(currentPlayer);
            }else if (answer == "Pass") break;
            updateLetter(currentPlayer);
    }
}

void GameHandler::jailTurn(Player& player) {
    if (player.useGetOutOfJailCard()) {
        popup(player.getName() + " used a Get Out of Jail Free card!");
        player.releaseFromJail();
    } else {
        std::string answer = askQuestion({"Wanna pay $50 to get out?", "Nah, I'm good"});

        if (answer == "Wanna pay $50 to get out?") {
            player.removeMoney(50);
            player.releaseFromJail();
        } else {
            player.incrementTurnsInJail();
            if (player.getTurnsInJail() >= 3) {
                popup(player.getName() + " has been in jail for 3 turns and must pay $50.");
                player.removeMoney(50);
                player.releaseFromJail();
            } 
        }
    }
}

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

void GameHandler::playTurn(Game& game) {
    do {
        game.startTurn();

        Player& currentPlayer = const_cast<Player&>(game.getCurrentPlayer());
        updatePlayerDetails(currentPlayer);
        std::vector<int> dice = game.roll(currentPlayer);
        int roll1 = dice[0];
        int roll2 = dice[1];
        std::string rollmsg(currentPlayer.getName() + " rolled " + std::to_string(roll1) + " and " + std::to_string(roll2));

        game.checkConsecutiveDoubles(currentPlayer, roll1, roll2);

        if (currentPlayer.isInJail()) {
            jailTurn(currentPlayer);
            popup(rollmsg);
        } else {
            rollmsg += ".\n" + currentPlayer.getName() + " is mooving to: " + game.getBoard().getSquare(currentPlayer.getPosition()+roll1+roll2)->getName();
            popup(rollmsg);
            game.processPlayerTurn(currentPlayer);
            if(game.getLastDrawnCardName() != ""){
                popup(game.getLastDrawnCardName());
            }
        }

        updatePlayerDetails(currentPlayer);
        updatePlayerPositions(game.getPlayerPositions());
        updateAllPlayersProperties(game);
        buyProperty(game);
        build(game);

        if (roll1 != roll2 || currentPlayer.isInJail()) {
            game.endTurn(currentPlayer);
        }else if (roll1 == roll2 && !currentPlayer.isInJail()){
            popup("you have one more turn!");
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