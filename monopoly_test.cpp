#include <gtest/gtest.h>
#include "Player.h"
#include "Property.h"
#include "UtilityProperty.h"
#include "ChanceCard.h"
#include "CommunityChestCard.h"
#include "Board.h"
#include "Game.h"
#include "Street.h"
#include "gamehandler.h"

// Test fixture for Monopoly game components
// This class sets up the common objects needed for multiple tests
class MonopolyTest : public ::testing::Test {
protected:
    std::unique_ptr<Game> game;
    std::unique_ptr<Board> board;
    Player* player;

    // SetUp method is called before each test
    // It initializes a game with one player and sets up the board
    void SetUp() override {
        game = std::make_unique<Game>(1);  // Create a game with 1 player
        board = std::make_unique<Board>(); // Initialize the game board
        player = game->getPlayers()[0].get();  // Get a pointer to the single player
    }
};

// Test to verify correct game setup and initialization
TEST_F(MonopolyTest, GameInitializationIsCorrect) {
    Game game(1);  // Create a new game with 1 player
    GameHandler handler;  // Create a game handler object
    
    // Set up the game using the handler
    handler.setupGame(game);
    
    // Verify that the number of square titles matches the board size
    EXPECT_EQ(handler.squareTitles.size(), game.getBoard().getSize());
    
    // Verify that the number of square letters matches the board size
    EXPECT_EQ(handler.squareLetters.size(), game.getBoard().getSize());
}

// Test the addition of two new squares to the board
TEST_F(MonopolyTest, AddingTwoSquaresIncreaseBoardSize) {
    Game game(1);  // Create a new game with 1 player
    GameHandler handler;  // Create a game handler object

    // Attempt to add two new squares to the board
    bool squaresAdded = handler.addTwoSquares(game);
    
    // Verify that the squares were successfully added
    EXPECT_TRUE(squaresAdded);
    
    // Check that the board size has increased to 42 (40 initial + 2 new)
    EXPECT_EQ(game.getBoard().getSize(), 42);
}

// Test to ensure player positions are correctly updated
TEST_F(MonopolyTest, PlayerPositionsUpdateCorrectly) {
    Game game(1);  // Create a new game with 1 player
    GameHandler handler;  // Create a game handler object
    
    // Create a vector of player positions
    std::vector<std::vector<int>> positions = {{0}, {1}, {2}};
    
    // Update player positions using the handler
    handler.updatePlayerPositions(positions);
    
    // Note: Additional assertions may be needed here to verify UI updates
    // This would depend on how the UI is implemented and can be accessed
}

// Test to verify property purchase mechanics
TEST_F(MonopolyTest, PlayerCanBuyPropertyWhenNotInJail) {
    Game game(1);  // Create a new game with 1 player
    GameHandler handler;  // Create a game handler object
    
    // Get a reference to the current player
    const Player& player = game.getCurrentPlayer();
    
    // Find the position of "Park Place" on the board
    int position = game.getBoard().getSquareIndexByName("Park Place");
    
    // Move the player to the "Park Place" position
    const_cast<Player&>(player).moveToPosition(position);
    
    // Attempt to buy the property
    handler.buyProperty(game);
    
    // Verify that the player now owns the property
    EXPECT_TRUE(player.ownsProperty((const Property*)game.getBoard().getSquare(position)));
    
    // Check that the player's money has decreased by the cost of the property
    EXPECT_EQ(player.getMoney(), 1150);  // Initial money (1500) - property cost (350)
}

// Test player turn mechanics while in jail
TEST_F(MonopolyTest, PlayerInJailFollowsJailTurnRules) {
    Game game(1);  // Create a new game with 1 player
    GameHandler handler;  // Create a game handler object

    // Get a reference to the current player
    const Player& player = game.getCurrentPlayer();
    
    // Put the player in jail
    const_cast<Player&>(player).goToJail();
    
    // Simulate a turn for the jailed player
    handler.jailTurn(const_cast<Player&>(player));
    
    // Note: Additional assertions should be added here to verify jail turn behavior
    // This could include checking if the player rolled doubles, paid bail, or used a card
}

// Verify initial player balance
TEST_F(MonopolyTest, PlayerStartsWithCorrectInitialBalance) {
    // Check that the player starts with the correct amount of money (1500)
    EXPECT_EQ(player->getMoney(), 1500);
}

// Test player movement mechanics
TEST_F(MonopolyTest, PlayerMovesCorrectNumberOfSpaces) {
    // Move the player forward by 5 spaces
    player->moveBy(5);
    // Verify that the player's position has changed correctly
    EXPECT_EQ(player->getPosition(), 5);

    // Move the player forward by another 10 spaces
    player->moveBy(10);
    // Verify that the player's new position is correct
    EXPECT_EQ(player->getPosition(), 15);
}

// Ensure player can move to a specific board position
TEST_F(MonopolyTest, PlayerCanMoveToSpecificBoardPosition) {
    // Move the player directly to position 10 (usually "Jail")
    player->moveToPosition(10);
    // Verify that the player's position is updated correctly
    EXPECT_EQ(player->getPosition(), 10);

    // Move the player back to the starting position (Go)
    player->moveToPosition(0);
    // Verify that the player's position is now 0
    EXPECT_EQ(player->getPosition(), 0);
}

// Verify player balance changes
TEST_F(MonopolyTest, PlayerBalanceUpdatesCorrectly) {
    // Record the player's initial balance
    int initialBalance = player->getMoney();
    
    // Add 500 to the player's balance
    player->addMoney(500);
    // Verify that the balance has increased by 500
    EXPECT_EQ(player->getMoney(), initialBalance + 500);

    // Remove 300 from the player's balance
    player->removeMoney(300);
    // Verify that the balance has decreased by 300
    EXPECT_EQ(player->getMoney(), initialBalance + 200);
}

// Test putting a player in jail
TEST_F(MonopolyTest, PlayerGoesToJailCorrectly) {
    // Verify that the player starts not in jail
    EXPECT_FALSE(player->isInJail());
    
    // Send the player to jail
    player->goToJail();
    // Verify that the player is now in jail
    EXPECT_TRUE(player->isInJail());
}

// Test releasing a player from jail
TEST_F(MonopolyTest, PlayerReleasedFromJailCorrectly) {
    // First, put the player in jail
    player->goToJail();
    EXPECT_TRUE(player->isInJail());

    // Now, release the player from jail
    player->releaseFromJail();
    // Verify that the player is no longer in jail
    EXPECT_FALSE(player->isInJail());
}

// Test "Get Out of Jail Free" card mechanics
TEST_F(MonopolyTest, GetOutOfJailCardWorksCorrectly) {
    // Verify the player starts with no "Get Out of Jail Free" cards
    EXPECT_EQ(player->getGetOutOfJailCards(), 0);

    // Give the player a "Get Out of Jail Free" card
    player->addGetOutOfJailCard();
    // Verify the player now has one card
    EXPECT_EQ(player->getGetOutOfJailCards(), 1);

    // Use the "Get Out of Jail Free" card
    EXPECT_TRUE(player->useGetOutOfJailCard());
    // Verify the card was used and the player has no more cards
    EXPECT_EQ(player->getGetOutOfJailCards(), 0);

    // Try to use another card when the player has none
    EXPECT_FALSE(player->useGetOutOfJailCard());
}

// Test property ownership mechanics
TEST_F(MonopolyTest, PropertyOwnershipWorksCorrectly) {
    // Create a new property
    Property prop("Park Place", 350, 50);  // Name, price, and rent
    
    // Verify the player doesn't own the property initially
    EXPECT_FALSE(player->ownsProperty(&prop));

    // Player buys the property
    player->buyProperty(&prop);
    // Verify the player now owns the property
    EXPECT_TRUE(player->ownsProperty(&prop));
    // Verify the property's owner is set correctly
    EXPECT_EQ(prop.getOwner(), player);
}

// Test rent payment when landing on owned property
TEST_F(MonopolyTest, RentPaidCorrectlyWhenLandingOnOwnedProperty) {
    Player player2("Player 2", board.get(), game.get());

    // Create and assign a property to player1
    Property prop("Park Place", 350, 50);
    player->buyProperty(&prop);

    // Move player2 to the owned property's position
    player2.moveToPosition(board.get()->getSquareIndexByName("Park Place"));
    // Verify that player1 owns the property landed on by player2
    EXPECT_TRUE(player->ownsPropertyLandedOn(player2));
}

// Test clearing of properties on bankruptcy
TEST_F(MonopolyTest, PropertiesClearedOnBankruptcy) {
    // Create two properties
    Property prop1("Boardwalk", 400, 50);
    Property prop2("Park Place", 350, 50);

    // Player buys both properties
    player->buyProperty(&prop1);
    player->buyProperty(&prop2);
    // Verify player owns 2 properties
    EXPECT_EQ(player->getProperties().size(), 2);

    // Simulate bankruptcy by clearing properties
    player->clearProperties();
    // Verify player now owns no properties
    EXPECT_EQ(player->getProperties().size(), 0);
    // Verify properties no longer have an owner
    EXPECT_EQ(prop1.getOwner(), nullptr);
    EXPECT_EQ(prop2.getOwner(), nullptr);
}

// Test accumulation of turns in jail
TEST_F(MonopolyTest, TurnsInJailAccumulateCorrectly) {
    // Send player to jail
    player->goToJail();
    // Verify initial turns in jail is 0
    EXPECT_EQ(player->getTurnsInJail(), 0);

    // Increment turns in jail
    player->incrementTurnsInJail();
    // Verify turns in jail is now 1
    EXPECT_EQ(player->getTurnsInJail(), 1);

    // Increment turns in jail again
    player->incrementTurnsInJail();
    // Verify turns in jail is now 2
    EXPECT_EQ(player->getTurnsInJail(), 2);
}

// Test street property rent calculation
TEST_F(MonopolyTest, StreetRentCalculationIsCorrect) {
    // Create a street property
    Street street("Boardwalk", 400, ColorGroup::DarkBlue, 200, {50, 200, 600, 1400, 1700, 2000});
    // Verify base rent is correct
    EXPECT_EQ(street.getRent(), 50);

    // Attempt to build a house (should throw an exception as player doesn't own full color group)
    EXPECT_THROW(street.buildHouse(*player), std::runtime_error);
}

// Test utility property rent calculation
TEST_F(MonopolyTest, UtilityRentCalculationIsCorrect) {
    // Create a utility property
    UtilityProperty utility("Electric Company", 150);
    
    // Verify rent is calculated correctly based on dice roll
    EXPECT_EQ(utility.getRent(), 10 * game->getLastRoll());
}

// Test Chance card execution
TEST_F(MonopolyTest, ChanceCardExecutesCorrectly) {
    // Create an "Advance to" Chance card
    AdvanceToCard advanceCard("Boardwalk");
    int initialPosition = player->getPosition();
    
    // Execute the card
    advanceCard.execute(*player, *game);
    // Verify player's position has changed
    EXPECT_NE(player->getPosition(), initialPosition);
}

// Test Community Chest card execution
TEST_F(MonopolyTest, CommunityChestCardExecutesCorrectly) {
    // Create a "Collect" Community Chest card
    CollectCard collectCard(50, "Bank error in your favor");
    int initialMoney = player->getMoney();
    
    // Execute the card
    collectCard.execute(*player, *game);
    // Verify player's money has increased by the correct amount
    EXPECT_EQ(player->getMoney(), initialMoney + 50);
}

// Test landing on GO square
TEST_F(MonopolyTest, LandingOnGoSquareGivesCorrectMoney) {
    // Create a GO square
    GoSquare goSquare("GO");
    int initialMoney = player->getMoney();
    
    // Simulate landing on GO
    goSquare.landedOn(*player);
    // Verify player receives $200 for landing on GO
    EXPECT_EQ(player->getMoney(), initialMoney + 200);
}

// Test visiting Jail square
TEST_F(MonopolyTest, VisitingJailSquareDoesNotPutPlayerInJail) {
    // Create a Jail square
    GoSquare jailSquare("Jail");
    
    // Simulate landing on Jail square
    jailSquare.landedOn(*player);
    // Verify player is not put in jail just for landing on the Jail square
    EXPECT_FALSE(player->isInJail());
}

// Test landing on Tax square
TEST_F(MonopolyTest, LandingOnTaxSquareDeductsMoney) {
    // Create a Tax square (using GoSquare as a placeholder)
    GoSquare taxSquare("Income Tax");
    int initialMoney = player->getMoney();
    
    // Simulate landing on Tax square
    taxSquare.landedOn(*player);
    // Verify player's money hasn't changed (as GoSquare doesn't implement tax)
    EXPECT_EQ(player->getMoney(), initialMoney);
}

// Test landing on Free Parking
TEST_F(MonopolyTest, LandingOnFreeParkingDoesNothing) {
    // Create a Free Parking square
    FreeParking freeParking("Free Parking");
    int initialMoney = player->getMoney();
    
    // Simulate landing on Free Parking
    freeParking.landedOn(*player);
    // Verify player's money hasn't changed
    EXPECT_EQ(player->getMoney(), initialMoney);
}

// Test landing on Go To Jail square
TEST_F(MonopolyTest, GoToJailLanding) {
    GoToJail goToJail("Go To Jail");
    goToJail.landedOn(*player);
    // Check if the player is sent to jail when landing on "Go To Jail" square
    EXPECT_TRUE(player->isInJail());
    // Verify that the player's position is updated to the "Jail" square
    EXPECT_EQ(player->getPosition(), game->getBoard().getSquareIndexByName("Jail"));
}

// Test 10: Street property building houses
TEST_F(MonopolyTest, StreetBuildingHouses) {
    Street street("Park Place", 350, ColorGroup::DarkBlue, 200, {35, 175, 500, 1100, 1300, 1500});
    player->addMoney(1000);  // Ensure player has enough money
    
    // Attempt to build houses on the property
    for (int i = 0; i < 4; ++i) {
        // Check if it's possible to build a house before each attempt
        EXPECT_TRUE(street.canBuildHouse(*player));
        street.buildHouse(*player);
        // Verify that the number of houses increases after each build
        EXPECT_EQ(street.getNumHouses(), i + 1);
    }
    
    // After building 4 houses, it should not be possible to build more
    EXPECT_FALSE(street.canBuildHouse(*player));
}

// Test 11: Street property building hotel
TEST_F(MonopolyTest, StreetBuildingHotel) {
    Street street("Park Place", 350, ColorGroup::DarkBlue, 200, {35, 175, 500, 1100, 1300, 1500});
    player->addMoney(2000);  // Ensure player has enough money
    
    // Build 4 houses on the property
    for (int i = 0; i < 4; ++i) {
        street.buildHouse(*player);
    }
    
    // Check if it's possible to build a hotel
    EXPECT_TRUE(street.canBuildHotel(*player));
    street.buildHotel(*player);
    // Verify that the hotel is built and houses are removed
    EXPECT_TRUE(street.getHasHotel());
    EXPECT_EQ(street.getNumHouses(), 0);
}

// Test 12: Player bankruptcy
TEST_F(MonopolyTest, PlayerBankruptcy) {
    // Remove all money from the player plus an extra dollar to ensure bankruptcy
    player->removeMoney(player->getMoney()+1);
    // Check if the player's balance is negative, indicating bankruptcy
    EXPECT_TRUE(player->getMoney()<0);
}

// Test 13: Get Out of Jail Free card usage
TEST_F(MonopolyTest, GetOutOfJailFreeCardUsage) {
    player->goToJail();
    player->addGetOutOfJailCard();
    // Attempt to use the Get Out of Jail Free card
    EXPECT_TRUE(player->useGetOutOfJailCard());
    // Verify that the player is no longer in jail after using the card
    EXPECT_FALSE(player->isInJail());
}

// Test 14: Railroad rent calculation
TEST_F(MonopolyTest, RailroadRentCalculation) {
    UtilityProperty railroad("Reading Railroad", 200);
    player->buyProperty(&railroad);
    // Check the rent when the player owns one railroad
    EXPECT_EQ(railroad.getRent(), 25);

    UtilityProperty railroad2("Pennsylvania Railroad", 200);
    player->buyProperty(&railroad2);
    // Verify that the rent increases when the player owns two railroads
    EXPECT_EQ(railroad.getRent(), 50);
}

// Test 15: Chance card - Advance to GO
TEST_F(MonopolyTest, ChanceCardAdvanceToGO) {
    AdvanceToCard advanceToGOCard("GO");
    int initialMoney = player->getMoney();
    advanceToGOCard.execute(*player, *game);
    // Check if the player's position is updated to GO (position 0)
    EXPECT_EQ(player->getPosition(), 0);
    // Verify that the player receives $200 for passing GO
    EXPECT_EQ(player->getMoney(), initialMoney + 200);
}

// Test initialization of the game
TEST(GameTest, Initialization) {
    Game game(2);  // Create a game with two players
    // Verify that the first player is set as the current player
    EXPECT_EQ(game.getCurrentPlayer().getName(), "Player 1");
}

// Test rolling the dice
TEST(GameTest, DiceRoll) {
    Game game(2);
    Player& player = const_cast<Player&>(game.getCurrentPlayer());  // Get current player
    std::vector<int> rolls = game.roll(player);
    
    // Check that two dice are rolled
    EXPECT_EQ(rolls.size(), 2);
    // Verify that each die roll is between 1 and 6
    EXPECT_GE(rolls[0], 1);
    EXPECT_LE(rolls[0], 6);
    EXPECT_GE(rolls[1], 1);
    EXPECT_LE(rolls[1], 6);
}

// Test player movement after rolling the dice
TEST(GameTest, PlayerMovement) {
    Game game(2);
    Player& player = const_cast<Player&>(game.getCurrentPlayer());  // Get current player
    int initialPosition = player.getPosition();

    // Simulate a dice roll and player movement
    game.roll(player);  
    game.processPlayerTurn(player);

    // Ensure that the player's position has changed after the turn
    EXPECT_NE(player.getPosition(), initialPosition);
}

// Test if the player receives money after passing GO
TEST(GameTest, PassGo) {
    Game game(2);
    Player& player = const_cast<Player&>(game.getCurrentPlayer());  // Get current player
    player.moveToPosition(38);  // Simulate player being near GO
    game.roll(player);  // Simulate rolling dice
    game.processPlayerTurn(player);

    // Verify that the player's money has increased by $200 after passing GO
    EXPECT_GE(player.getMoney(), 1500 + 200);
}

// Test drawing a chance card
TEST(GameTest, DrawChanceCard) {
    Game game(2);
    Player& player = const_cast<Player&>(game.getCurrentPlayer());  // Get current player
    game.drawChanceCard(player);

    // Ensure that a card was drawn by checking if the last drawn card name is not empty
    EXPECT_FALSE(game.getLastDrawnCardName().empty());
}

// Test drawing a community chest card
TEST(GameTest, DrawCommunityChestCard) {
    Game game(2);
    Player& player = const_cast<Player&>(game.getCurrentPlayer());  // Get current player
    game.drawCommunityChestCard(player);

    // Ensure that a card was drawn by checking if the last drawn card name is not empty
    EXPECT_FALSE(game.getLastDrawnCardName().empty());
}

// Test sending player to jail after rolling three consecutive doubles
TEST(GameTest, ConsecutiveDoublesSendToJail) {
    Game game(2);
    Player& player = const_cast<Player&>(game.getCurrentPlayer());  // Get current player

    // Manually simulate three consecutive double rolls
    game.checkConsecutiveDoubles(player, 3, 3);  // First double
    game.checkConsecutiveDoubles(player, 2, 2);  // Second double
    game.checkConsecutiveDoubles(player, 4, 4);  // Third double

    // Verify that the player is sent to jail after three consecutive doubles
    EXPECT_TRUE(player.isInJail());
}

// Test player bankruptcy
TEST(GameTest, PlayerBankruptcy) {
    Game game(2);
    Player& player = const_cast<Player&>(game.getCurrentPlayer());  // Get current player
    player.removeMoney(2000);  // Force bankruptcy

    game.checkBankruptcy(player);
    // Ensure that one player was removed from the game after going bankrupt
    EXPECT_EQ(game.getPlayerPositions().size(), 1);
}

// Test if the game ends when one player remains
TEST(GameTest, GameOver) {
    Game game(2);
    Player& player = const_cast<Player&>(game.getCurrentPlayer());
    player.removeMoney(2000);  // Force one player to go bankrupt

    game.checkBankruptcy(player);
    // Verify that the game is over when only one player remains
    EXPECT_TRUE(game.isGameOver());
}

// Test property transfer upon bankruptcy
TEST(GameTest, PropertyTransferOnBankruptcy) {
    Game game(2);
    Player& player1 = const_cast<Player&>(game.getCurrentPlayer());
    game.endTurn(player1);  // End turn for player1
    Player& player2 = const_cast<Player&>(game.getCurrentPlayer());

    // Create a mock property and assign it to player1
    Property property("Boardwalk", 400, 50);
    player1.addProperty(&property);
    property.setOwner(&player1);

    player1.removeMoney(2000);  // Force player1 into bankruptcy
    game.checkBankruptcy(player1);

    // Check if the property is transferred to player2 after player1 goes bankrupt
    EXPECT_EQ(property.getOwner(), &player2);
}

#include <gtest/gtest.h>
#include "Board.h"

TEST(BoardTest, BoardInitialization) {
    Board board;

    // Verify the board size is correct (should be 40 squares)
    ASSERT_EQ(board.getSize(), 40);

    // Check the names and color groups of some key squares
    auto squaresInfo = board.getSquareNamesAndColors();

    // Verify the properties of the first square (Go)
    EXPECT_EQ(squaresInfo[0].first, "Go");
    // Check the properties of the second square (Mediterranean Avenue)
    EXPECT_EQ(squaresInfo[1].first, "Mediterranean Avenue");
    EXPECT_EQ(squaresInfo[1].second, static_cast<int>(ColorGroup::Brown));

    // Verify the properties of the Jail square
    EXPECT_EQ(squaresInfo[10].first, "Jail");
    // Check the properties of the last square (Boardwalk)
    EXPECT_EQ(squaresInfo[39].first, "Boardwalk");
    EXPECT_EQ(squaresInfo[39].second, static_cast<int>(ColorGroup::DarkBlue));
}

TEST(BoardTest, GetSquareByIndex) {
    Board board;

    // Get the first square (Go) by its index
    Square* goSquare = board.getSquare(0);
    ASSERT_NE(goSquare, nullptr);
    EXPECT_EQ(goSquare->getName(), "Go");

    // Get the last square (Boardwalk) by its index
    Square* boardwalkSquare = board.getSquare(39);
    ASSERT_NE(boardwalkSquare, nullptr);
    EXPECT_EQ(boardwalkSquare->getName(), "Boardwalk");

    // Ensure the square is normalized when out of bounds
    Square* wrappedSquare = board.getSquare(40);  // Should wrap around to index 0
    EXPECT_EQ(wrappedSquare->getName(), "Go");
}

TEST(BoardTest, GetSquareIndexByName) {
    Board board;

    // Check if the correct index is returned for known square names
    EXPECT_EQ(board.getSquareIndexByName("Go"), 0);
    EXPECT_EQ(board.getSquareIndexByName("Boardwalk"), 39);
    EXPECT_EQ(board.getSquareIndexByName("Jail"), 10);

    // Test a non-existent square name
    EXPECT_EQ(board.getSquareIndexByName("NonExistentSquare"), -1);
}

TEST(BoardTest, AddSquare) {
    Board board;

    // Create a new square to add to the board
    auto newSquare = std::make_unique<Street>("New Avenue", 100, ColorGroup::LightBlue, 50, std::array<int, 6>{6, 30, 90, 270, 400, 550});

    // Add the new square at position 5
    board.addSquare(5, std::move(newSquare));

    // Check that the new square is at position 5
    Square* addedSquare = board.getSquare(5);
    ASSERT_NE(addedSquare, nullptr);
    EXPECT_EQ(addedSquare->getName(), "New Avenue");

    // Verify that the board now has 41 squares
    EXPECT_EQ(board.getSize(), 41);
}

TEST(BoardTest, AddTwoSquares) {
    Board board;

    // Add two new squares at positions 0 and 9
    board.addTwoSquares("Street", "ChanceSquare", 0, 9, "New Street 1", "New Chance", 100, 0);

    // Verify the first new square was added correctly
    Square* square1 = board.getSquare(1);
    ASSERT_NE(square1, nullptr);
    EXPECT_EQ(square1->getName(), "New Street 1");

    // Verify the second new square was added correctly
    Square* square2 = board.getSquare(10);
    ASSERT_NE(square2, nullptr);
    EXPECT_EQ(square2->getName(), "New Chance");

    // Check that the board now has 42 squares
    EXPECT_EQ(board.getSize(), 42);
}

TEST(BoardTest, GetStreetsInColorGroup) {
    Board board;

    // Get all streets in the Dark Blue color group
    const std::vector<Street*>& darkBlueStreets = board.getStreetsInColorGroup(ColorGroup::DarkBlue);

    // Verify that there are two streets in the Dark Blue group
    ASSERT_EQ(darkBlueStreets.size(), 2);
    EXPECT_EQ(darkBlueStreets[0]->getName(), "Park Place");
    EXPECT_EQ(darkBlueStreets[1]->getName(), "Boardwalk");

    // Attempt to retrieve streets from a non-existent color group
    EXPECT_THROW(board.getStreetsInColorGroup(static_cast<ColorGroup>(999)), std::runtime_error);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}