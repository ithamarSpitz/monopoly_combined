#include <gtest/gtest.h>
#include "Player.h"
#include "Property.h"
#include "UtilityProperty.h"
#include "ChanceCard.h"
#include "CommunityChestCard.h"
#include "Board.h"
#include "Game.h"
#include "Street.h"

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
    EXPECT_EQ(game.getPlayers().size(), 1);
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
    Square* square2 = board.getSquare(11);
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