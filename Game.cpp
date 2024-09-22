#include "Game.h"
#include "ChanceCard.h"
#include "CommunityChestCard.h"
#include "Property.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <typeinfo>

/**
 * Constructor for the Game class.
 * Initializes the game with the specified number of players.
 * Creates player objects and initializes the game cards.
 * @param numPlayers The number of players in the game.
 */
Game::Game(int numPlayers) : currentPlayerIndex(0), gameOver(false) {
    for (int i = 0; i < numPlayers; ++i) {
        players.push_back(std::make_unique<Player>("Player " + std::to_string(i + 1), &board, this));
    }
    initializeCards();
}

/**
 * Initializes the Chance and Community Chest cards for the game.
 * Creates various types of cards and adds them to their respective decks.
 * Shuffles both decks after initialization.
 */
void Game::initializeCards() {
    // Initialize Chance cards
    chanceCards.push_back(std::make_unique<AdvanceToCard>("Boardwalk"));
    chanceCards.push_back(std::make_unique<AdvanceToCard>("Go"));
    chanceCards.push_back(std::make_unique<AdvanceToCard>("Illinois Avenue"));
    chanceCards.push_back(std::make_unique<AdvanceToCard>("St. Charles Place"));
    chanceCards.push_back(std::make_unique<CollectCard>(50, "Bank pays you dividend"));
    chanceCards.push_back(std::make_unique<GetOutOfJailCard>());
    chanceCards.push_back(std::make_unique<GoToJailCard>());
    chanceCards.push_back(std::make_unique<PayCard>(15, "Speeding fine"));

    // Initialize Community Chest cards
    communityChestCards.push_back(std::make_unique<AdvanceToCard>("Go"));
    communityChestCards.push_back(std::make_unique<CollectCard>(200, "Bank error in your favor"));
    communityChestCards.push_back(std::make_unique<PayCard>(50, "Doctor's fee"));
    communityChestCards.push_back(std::make_unique<CollectCard>(50, "From sale of stock"));
    communityChestCards.push_back(std::make_unique<GetOutOfJailCard>());
    communityChestCards.push_back(std::make_unique<GoToJailCard>());
    communityChestCards.push_back(std::make_unique<CollectFromEachPlayerCard>(10, "It is your birthday"));

    // Shuffle the cards using a random device and the Mersenne Twister engine
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(chanceCards.begin(), chanceCards.end(), g);
    std::shuffle(communityChestCards.begin(), communityChestCards.end(), g);
}

/**
 * Starts a new turn for a player.
 * Resets the last drawn card to prepare for the new turn.
 */
void Game::startTurn() {
    lastDrawnCard.reset();
}

/**
 * Ends the current player's turn and prepares for the next player.
 * Checks for bankruptcy and advances to the next player.
 * @param player The player whose turn is ending.
 */
void Game::endTurn(Player& player) {
    checkBankruptcy(player); 
    // Move to the next player for the next turn
    currentPlayerIndex++;
    if (static_cast<size_t>(currentPlayerIndex) >= players.size()) {
        currentPlayerIndex = 0;
    }
}

/**
 * Simulates rolling the dice for a player.
 * @param player The player rolling the dice.
 * @return A vector containing the values of the two dice rolls.
 */
std::vector<int> Game::roll(Player& player){
        int roll1 = dice.roll();
        int roll2 = dice.roll();
        lastRoll = roll1 + roll2;
        return std::vector<int>{roll1, roll2};
}

/**
 * Processes a player's turn, including movement and landing on a square.
 * Handles passing GO and triggering square-specific actions.
 * @param player The player whose turn is being processed.
 */
void Game::processPlayerTurn(Player& player) {
        // Move the player
        int oldPosition = player.getPosition();
        player.moveBy(getLastRoll());
        int newPosition = player.getPosition();

        // Check if the player passed GO and award $200 if they did
        if (newPosition < oldPosition) {
            player.addMoney(200);
        }

        // Land on a new square and trigger its action
        Square* currentSquare = board.getSquare(newPosition);
        if (currentSquare) {
            currentSquare->landedOn(player);
        } else {
            std::cerr << "Error: Invalid square position!" << std::endl;
        }
}

/**
 * Checks for consecutive doubles and sends the player to jail if necessary.
 * @param player The player who rolled the dice.
 * @param roll1 The value of the first die.
 * @param roll2 The value of the second die.
 */
void Game::checkConsecutiveDoubles(Player& player, int roll1, int roll2) {
    static int consecutiveDoubles = 0;

    if (dice.isDouble(roll1, roll2)) {
        consecutiveDoubles++;
        if (consecutiveDoubles == 3) {
            sendPlayerToJail(player);
            consecutiveDoubles = 0;
        }
    } else {
        consecutiveDoubles = 0;
    }
}

/**
 * Sends a player to jail.
 * Updates the player's status and moves them to the Jail square.
 * @param player The player being sent to jail.
 */
void Game::sendPlayerToJail(Player& player) {
    player.goToJail();
    player.moveToPosition(getBoard().getSquareIndexByName("Jail"));
}

/**
 * Checks if a player is bankrupt and handles the bankruptcy process if necessary.
 * Transfers assets to a creditor or returns properties to the bank.
 * @param player The player being checked for bankruptcy.
 */
void Game::checkBankruptcy(Player& player) {
    if (player.getMoney() < 0) {
        Player* creditor = findCreditor(player);
        if (creditor) {
            transferAssets(player, *creditor);
        } else {
            returnPropertiesToBank(player);
        }

        removePlayer(&player);
    }
}

/**
 * Checks if the game is over based on various conditions.
 * @return true if the game is over, false otherwise.
 */
bool Game::isGameOver() const {
    return gameOver || players.size() == 1 || std::any_of(players.begin(), players.end(),
        [](const auto& player) { return player->getMoney() >= 4000; });
}

/**
 * Finds the creditor of a bankrupt player.
 * @param bankruptPlayer The player who has gone bankrupt.
 * @return A pointer to the creditor player, or nullptr if no creditor is found.
 */
Player* Game::findCreditor(const Player& bankruptPlayer) {
    for (const auto& p : players) {
        if (p.get() != &bankruptPlayer && p->ownsPropertyLandedOn(bankruptPlayer)) {
            return p.get();
        }
    }
    return nullptr;
}

/**
 * Transfers assets from a bankrupt player to a creditor.
 * Moves money and properties from one player to another.
 * @param fromPlayer The bankrupt player.
 * @param toPlayer The creditor player.
 */
void Game::transferAssets(Player& fromPlayer, Player& toPlayer) {
    int remainingMoney = fromPlayer.getMoney();
    if (remainingMoney > 0) {
        toPlayer.addMoney(remainingMoney);
    }

    const auto& properties = fromPlayer.getProperties();
    for (auto* property : properties) {
        property->setOwner(&toPlayer);
        toPlayer.addProperty(property);
    }
    fromPlayer.clearProperties();
}

/**
 * Returns all properties of a bankrupt player to the bank.
 * Clears ownership of all properties owned by the player.
 * @param player The bankrupt player whose properties are being returned.
 */
void Game::returnPropertiesToBank(Player& player) {
    const auto& properties = player.getProperties();
    for (auto* property : properties) {
        property->setOwner(nullptr);
    }
    player.clearProperties();
}

/**
 * Removes a player from the game.
 * Adjusts the current player index if necessary and checks for game over condition.
 * @param playerToRemove Pointer to the player to be removed.
 */
void Game::removePlayer(Player* playerToRemove) {
    auto it = std::find_if(players.begin(), players.end(),
                           [playerToRemove](const std::unique_ptr<Player>& p) { return p.get() == playerToRemove; });

    if (it != players.end()) {
        std::string name = playerToRemove->getName();
        int index = std::distance(players.begin(), it);
        players.erase(it);

        if (currentPlayerIndex >= index) {
            currentPlayerIndex--;
        }
        if (players.size() == 1) {
            gameOver = true;
        }
    }
}

/**
 * Checks if a player can build houses or hotels on any of their properties.
 * @param player The player to check for building eligibility.
 * @return true if the player can build, false otherwise.
 */
bool Game::canBuild(Player& player) {
    for (auto& property : player.getProperties()) {
        Street* street = dynamic_cast<Street*>(property);
        if (street && (street->canBuildHouse(player) || street->canBuildHotel(player))) {
            return true;
        }
    }
    return false;
}

/**
 * Gets the positions of all players on the board.
 * @return A vector of vectors, where each inner vector contains the player indices at that position.
 */
std::vector<std::vector<int>> Game::getPlayerPositions() const {
    std::vector<std::vector<int>> positions(getBoard().getSize());
    for (int j = 0; j < getBoard().getSize(); j++){
        for (int i = 0; i < (int)players.size(); ++i) {
            if(players[i]->getPosition() == j)
                positions[j].push_back(players[i]->getName().back()-'0');
        }
    }

    return positions;
}

/**
 * Gets the current player.
 * @return A constant reference to the current player.
 */
const Player& Game::getCurrentPlayer() const {
    return *players[currentPlayerIndex];
}

/**
 * Draws a Chance card for a player and executes its action.
 * Reinitializes the deck if it's empty.
 * @param player The player drawing the card.
 */
void Game::drawChanceCard(Player& player) {
    if (chanceCards.empty()) {
        initializeCards();
    }
    
    lastDrawnCard = std::move(chanceCards.back());
    chanceCards.pop_back();
    
    lastDrawnCard->execute(player, *this);
}

/**
 * Draws a Community Chest card for a player and executes its action.
 * Reinitializes the deck if it's empty.
 * @param player The player drawing the card.
 */
void Game::drawCommunityChestCard(Player& player) {
    if (communityChestCards.empty()) {
        initializeCards();
    }
    
    lastDrawnCard = std::move(communityChestCards.back());
    communityChestCards.pop_back();
    
    lastDrawnCard->execute(player, *this);
}

/**
 * Gets the description of the last drawn card.
 * @return A string describing the last drawn card, or an empty string if no card was drawn.
 */
std::string Game::getLastDrawnCardName() const {
    if (lastDrawnCard) {
        return lastDrawnCard->getDescription();
    }
    return "";
}