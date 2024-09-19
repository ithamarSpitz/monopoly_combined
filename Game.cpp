#include "Game.h"
#include "ChanceCard.h"
#include "CommunityChestCard.h"
#include "Property.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <typeinfo>


Game::Game(int numPlayers) : currentPlayerIndex(0), gameOver(false) {
    for (int i = 0; i < numPlayers; ++i) {
        players.push_back(std::make_unique<Player>("Player " + std::to_string(i + 1), &board, this));
    }
    initializeCards();
}

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

    // Shuffle the cards
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(chanceCards.begin(), chanceCards.end(), g);
    std::shuffle(communityChestCards.begin(), communityChestCards.end(), g);
}

void Game::startTurn() {
    lastDrawnCard.reset();
}

void Game::endTurn(Player& player) {
    checkBankruptcy(player); 
    // Move to the next player for the next turn
    currentPlayerIndex++;
    if (static_cast<size_t>(currentPlayerIndex) >= players.size()) {
        currentPlayerIndex = 0;
    }
}

std::vector<int> Game::roll(Player& player){
        int roll1 = dice.roll();
        int roll2 = dice.roll();
        lastRoll = roll1 + roll2;
        return std::vector<int>{roll1, roll2};
}

void Game::processPlayerTurn(Player& player) {
        // Move the player
        int oldPosition = player.getPosition();
        player.moveBy(getLastRoll());
        int newPosition = player.getPosition();

        // Check if the player passed GO
        if (newPosition < oldPosition) {
            player.addMoney(200);
        }

        // Land on a new square
        Square* currentSquare = board.getSquare(newPosition);
        if (currentSquare) {
            currentSquare->landedOn(player);
        } else {
            std::cerr << "Error: Invalid square position!" << std::endl;
        }
}

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

void Game::sendPlayerToJail(Player& player) {
    player.goToJail();
    player.moveToPosition(getBoard().getSquareIndexByName("Jail"));
}

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

bool Game::isGameOver() const {
    return gameOver || players.size() == 1 || std::any_of(players.begin(), players.end(),
        [](const auto& player) { return player->getMoney() >= 4000; });
}

Player* Game::findCreditor(const Player& bankruptPlayer) {
    for (const auto& p : players) {
        if (p.get() != &bankruptPlayer && p->ownsPropertyLandedOn(bankruptPlayer)) {
            return p.get();
        }
    }
    return nullptr;
}

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

void Game::returnPropertiesToBank(Player& player) {
    const auto& properties = player.getProperties();
    for (auto* property : properties) {
        property->setOwner(nullptr);
    }
    player.clearProperties();
}

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

bool Game::canBuild(Player& player) {
    for (auto& property : player.getProperties()) {
        Street* street = dynamic_cast<Street*>(property);
        if (street && (street->canBuildHouse(player) || street->canBuildHotel(player))) {
            return true;
        }
    }
    return false;
}

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

const Player& Game::getCurrentPlayer() const {
    return *players[currentPlayerIndex];
}

void Game::drawChanceCard(Player& player) {
    if (chanceCards.empty()) {
        initializeCards();
    }
    
    lastDrawnCard = std::move(chanceCards.back());
    chanceCards.pop_back();
    
    lastDrawnCard->execute(player, *this);
}

void Game::drawCommunityChestCard(Player& player) {
    if (communityChestCards.empty()) {
        initializeCards();
    }
    
    lastDrawnCard = std::move(communityChestCards.back());
    communityChestCards.pop_back();
    
    lastDrawnCard->execute(player, *this);
}

std::string Game::getLastDrawnCardName() const {
    if (lastDrawnCard) {
        return lastDrawnCard->getDescription();
    }
    return "";
}
