Here is the updated README:

---

# Monopoly Game Project

## Overview
This project is a C++ implementation of a Monopoly-like board game. It includes core components such as players, a game board, dice, and various types of squares like properties, chance, and community chest cards. A Qt-based GUI is used to represent the game board and provide interaction between players.

### Features
- **Custom Board**: The game board consists of various square types such as properties, chance, community chest, tax, and more.
- **Player Management**: Players can move across the board, buy properties, build houses/hotels, and face various in-game events like taxes, jail, and cards.
- **Card System**: Chance and community chest cards affect gameplay by advancing players, giving bonuses, or imposing penalties.
- **Qt GUI**: The game board and player interactions are represented using a Qt-based custom widget for visual representation.

## Project Structure

### 1. **Game Logic**
- **Board** (`Board.h`, `Board.cpp`): Manages the overall game board, initializing squares (properties, tax, jail, etc.) and handling player interactions with the board.
  - Example: `initializeBoard()` sets up the game board with various types of squares.
- **Game** (`Game.h`, `Game.cpp`): Central class controlling the game flow. It handles player turns, rolling dice, checking for game over, and managing cards.
  - Example: `processPlayerTurn()` processes the movement and actions of the current player.
- **Player** (`Player.h`, `Player.cpp`): Manages the state of each player, including their money, position, properties owned, and status (e.g., whether they are in jail).
- **Dice** (`Dice.h`, `Dice.cpp`): Provides a mechanism to roll two dice for player movement.
  
### 2. **Squares**
- **Square** (`Square.h`, `Square.cpp`): Base class for all types of squares on the board.
- **Street** (`Street.h`, `Street.cpp`): Represents property squares that can be bought by players, built with houses and hotels, and charged rent.
- **Special Squares**: Other specific types of squares include:
  - `ChanceSquare` (`ChanceSquare.h`, `ChanceSquare.cpp`)
  - `CommunityChestSquare` (`CommunityChestSquare.h`, `CommunityChestSquare.cpp`)
  - `GoSquare` (`GoSquare.h`, `GoSquare.cpp`)
  - `FreeParking` (`FreeParking.h`, `FreeParking.cpp`)
  - `GoToJail` (`GoToJail.h`, `GoToJail.cpp`)
  - `Tax` (`Tax.h`, `Tax.cpp`): Represents tax squares that deduct money from the player landing on it.

### 3. **Cards**
- **Card** (`Card.h`, `Card.cpp`): Base class for Chance and Community Chest cards, each of which can affect players differently.
- **Chance Cards** (`ChanceCard.h`, `ChanceCard.cpp`): Includes specific cards like `AdvanceToCard`, `CollectCard`, `PayCard`, and more.
- **Community Chest Cards** (`CommunityChestCard.h`, `CommunityChestCard.cpp`): Functions similarly to Chance cards but has different effects.

### 4. **GUI (Qt-based)**
- **Custom Widget** (`customwidget.h`, `customwidget.cpp`): The graphical component displaying the board, player positions, and properties.
  - Example: `paintEvent()` draws the game board and updates the display during gameplay.
- **GameHandler** (`gamehandler.h`, `gamehandler.cpp`): Manages the interaction between the game logic and the GUI. It updates the game state, processes player decisions, and handles events like buying properties or drawing cards.
  - Example: `setupGame()` initializes the board with squares, player positions, and other visual elements.

### 5. **Utility Classes**
- **Dice**: Random number generation for dice rolls.
- **UtilityProperty**: A type of property that functions differently than standard street properties.

## Class Hierarchy

1. **Board Class**: 
   - Manages squares and interactions between the player and the board.
   - Stores a vector of `std::unique_ptr<Square>` representing different types of squares.

2. **Square Class**: 
   - Base class for all square types. Each square type (e.g., `Street`, `ChanceSquare`) inherits from this.
   
3. **Player Class**: 
   - Handles player information (position, money, properties).
   
4. **Game Class**: 
   - Central controller of the game flow and player actions.
   
5. **Card Class**: 
   - Base class for all cards. `ChanceCard` and `CommunityChestCard` inherit from this.

6. **CustomWidget Class (Qt)**: 
   - Renders the board visually and manages player interaction via Qt.

## How to Run

### Prerequisites:
You will need to install **Qt** and **Google Test (gtest)** to run this project and its tests.

1. **Qt Installation**:
   - Install Qt by following the official [Qt installation guide](https://doc.qt.io/qt-6/gettingstarted.html).
   - Alternatively, use a package manager:
     - On Ubuntu:
       ```terminal
       sudo apt-get install qt6-base-dev
       ```
   - Ensure `qmake` is in your system path:
     ```terminal
     export PATH=$PATH:/path/to/qt/bin
     ```

2. **Google Test Installation**:
   - To install Google Test:
     - On Ubuntu:
       ```terminal
       sudo apt-get install libgtest-dev
       sudo apt-get install cmake
       cd /usr/src/gtest
       sudo cmake CMakeLists.txt
       sudo make
       sudo cp *.a /usr/lib
       ```

### Build Instructions:

1. **Building the Project**:

     ```terminal
     make
     ```

2. **Running the Application**:
   - Execute the compiled binary to start the game:
     ```bash
     ./monopoly
     ```

3. **Running Tests**:
   - To run the Google Test suite:
     ```terminal
     make tests
     ```

## Game Flow

1. **Initialize Game**:
   - A `Game` object is created with the number of players.
   - The board and cards are initialized.
   
2. **Player Turn**:
   - Each player rolls the dice to move around the board.
   - Depending on the square landed on, the player can buy properties, pay rent, draw cards, or go to jail.
   
3. **Game End**:
   - The game ends when one player achieves a set condition (e.g., monopoly or bankruptcy).
