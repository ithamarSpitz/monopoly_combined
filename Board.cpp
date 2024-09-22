#include "Board.h"
#include "ChanceSquare.h"
#include "CommunityChestSquare.h"
#include "UtilityProperty.h"
#include <stdexcept>
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>

// Default constructor for the Board class
// Initializes the game board by calling initializeBoard()
Board::Board() {
    initializeBoard();
}

// Method to initialize the Monopoly game board
// Creates and adds all the squares to the board in the correct order
void Board::initializeBoard() {
    // Add the "Go" square as the starting point of the board
    squares.push_back(std::make_unique<GoSquare>("Go"));

    // Add the first property: Mediterranean Avenue
    // Parameters: name, cost, color group, house cost, and rent array for 0-5 houses
    squares.push_back(std::make_unique<Street>("Mediterranean Avenue", 60, ColorGroup::Brown, 50, 
        std::array<int, 6>{2, 10, 30, 90, 160, 250}));

    // Add a Community Chest square
    squares.push_back(std::make_unique<CommunityChestSquare>("Community Chest"));

    // Add the second Brown property: Baltic Avenue
    squares.push_back(std::make_unique<Street>("Baltic Avenue", 60, ColorGroup::Brown, 50, 
        std::array<int, 6>{4, 20, 60, 180, 320, 450}));

    // Add the Income Tax square
    squares.push_back(std::make_unique<Tax>("Income Tax", 200));

    // Add the first Railroad: Reading Railroad
    squares.push_back(std::make_unique<UtilityProperty>("Reading Railroad", 200));

    // Add the first Light Blue property: Oriental Avenue
    squares.push_back(std::make_unique<Street>("Oriental Avenue", 100, ColorGroup::LightBlue, 50, 
        std::array<int, 6>{6, 30, 90, 270, 400, 550}));

    // Add a Chance square
    squares.push_back(std::make_unique<ChanceSquare>("Chance"));

    // Add the second Light Blue property: Vermont Avenue
    squares.push_back(std::make_unique<Street>("Vermont Avenue", 100, ColorGroup::LightBlue, 50, 
        std::array<int, 6>{6, 30, 90, 270, 400, 550}));

    // Add the third Light Blue property: Connecticut Avenue
    squares.push_back(std::make_unique<Street>("Connecticut Avenue", 120, ColorGroup::LightBlue, 50, 
        std::array<int, 6>{8, 40, 100, 300, 450, 600}));

    // Add the Jail square
    squares.push_back(std::make_unique<Jail>("Jail"));

    // Add the first Pink property: St. Charles Place
    squares.push_back(std::make_unique<Street>("St. Charles Place", 140, ColorGroup::Pink, 100, 
        std::array<int, 6>{10, 50, 150, 450, 625, 750}));

    // Add the Electric Company utility
    squares.push_back(std::make_unique<UtilityProperty>("Electric Company", 150));

    // Add the second Pink property: States Avenue
    squares.push_back(std::make_unique<Street>("States Avenue", 140, ColorGroup::Pink, 100, 
        std::array<int, 6>{10, 50, 150, 450, 625, 750}));

    // Add the third Pink property: Virginia Avenue
    squares.push_back(std::make_unique<Street>("Virginia Avenue", 160, ColorGroup::Pink, 100, 
        std::array<int, 6>{12, 60, 180, 500, 700, 900}));

    // Add the second Railroad: Pennsylvania Railroad
    squares.push_back(std::make_unique<UtilityProperty>("Pennsylvania Railroad", 200));

    // Add the first Orange property: St. James Place
    squares.push_back(std::make_unique<Street>("St. James Place", 180, ColorGroup::Orange, 100, 
        std::array<int, 6>{14, 70, 200, 550, 750, 950}));

    // Add another Community Chest square
    squares.push_back(std::make_unique<CommunityChestSquare>("Community Chest"));

    // Add the second Orange property: Tennessee Avenue
    squares.push_back(std::make_unique<Street>("Tennessee Avenue", 180, ColorGroup::Orange, 100, 
        std::array<int, 6>{14, 70, 200, 550, 750, 950}));

    // Add the third Orange property: New York Avenue
    squares.push_back(std::make_unique<Street>("New York Avenue", 200, ColorGroup::Orange, 100, 
        std::array<int, 6>{16, 80, 220, 600, 800, 1000}));

    // Add the Free Parking square
    squares.push_back(std::make_unique<FreeParking>("Free Parking"));

    // Add the first Red property: Kentucky Avenue
    squares.push_back(std::make_unique<Street>("Kentucky Avenue", 220, ColorGroup::Red, 150, 
        std::array<int, 6>{18, 90, 250, 700, 875, 1050}));

    // Add another Chance square
    squares.push_back(std::make_unique<ChanceSquare>("Chance"));

    // Add the second Red property: Indiana Avenue
    squares.push_back(std::make_unique<Street>("Indiana Avenue", 220, ColorGroup::Red, 150, 
        std::array<int, 6>{18, 90, 250, 700, 875, 1050}));

    // Add the third Red property: Illinois Avenue
    squares.push_back(std::make_unique<Street>("Illinois Avenue", 240, ColorGroup::Red, 150, 
        std::array<int, 6>{20, 100, 300, 750, 925, 1100}));

    // Add the third Railroad: B. & O. Railroad
    squares.push_back(std::make_unique<UtilityProperty>("B. & O. Railroad", 200));

    // Add the first Yellow property: Atlantic Avenue
    squares.push_back(std::make_unique<Street>("Atlantic Avenue", 260, ColorGroup::Yellow, 150, 
        std::array<int, 6>{22, 110, 330, 800, 975, 1150}));

    // Add the second Yellow property: Ventnor Avenue
    squares.push_back(std::make_unique<Street>("Ventnor Avenue", 260, ColorGroup::Yellow, 150, 
        std::array<int, 6>{22, 110, 330, 800, 975, 1150}));

    // Add the Water Works utility
    squares.push_back(std::make_unique<UtilityProperty>("Water Works", 150));

    // Add the third Yellow property: Marvin Gardens
    squares.push_back(std::make_unique<Street>("Marvin Gardens", 280, ColorGroup::Yellow, 150, 
        std::array<int, 6>{24, 120, 360, 850, 1025, 1200}));

    // Add the Go To Jail square
    squares.push_back(std::make_unique<GoToJail>("Go To Jail"));

    // Add the first Green property: Pacific Avenue
    squares.push_back(std::make_unique<Street>("Pacific Avenue", 300, ColorGroup::Green, 200, 
        std::array<int, 6>{26, 130, 390, 900, 1100, 1275}));

    // Add the second Green property: North Carolina Avenue
    squares.push_back(std::make_unique<Street>("North Carolina Avenue", 300, ColorGroup::Green, 200, 
        std::array<int, 6>{26, 130, 390, 900, 1100, 1275}));

    // Add another Community Chest square
    squares.push_back(std::make_unique<CommunityChestSquare>("Community Chest"));

    // Add the third Green property: Pennsylvania Avenue
    squares.push_back(std::make_unique<Street>("Pennsylvania Avenue", 320, ColorGroup::Green, 200, 
        std::array<int, 6>{28, 150, 450, 1000, 1200, 1400}));

    // Add the fourth Railroad: Short Line
    squares.push_back(std::make_unique<UtilityProperty>("Short Line", 200));

    // Add the final Chance square
    squares.push_back(std::make_unique<ChanceSquare>("Chance"));

    // Add the first Dark Blue property: Park Place
    squares.push_back(std::make_unique<Street>("Park Place", 350, ColorGroup::DarkBlue, 200, 
        std::array<int, 6>{35, 175, 500, 1100, 1300, 1500}));

    // Add the Luxury Tax square
    squares.push_back(std::make_unique<Tax>("Luxury Tax", 100));

    // Add the last and most expensive property: Boardwalk
    squares.push_back(std::make_unique<Street>("Boardwalk", 400, ColorGroup::DarkBlue, 200, 
        std::array<int, 6>{50, 200, 600, 1400, 1700, 2000}));
    
    // Populate the colorGroups map
    // This groups streets by their color for easier management of monopolies
    for (const auto& square : squares) {
        // Attempt to cast the square to a Street pointer
        Street* street = dynamic_cast<Street*>(square.get());
        if (street) {
            // If successful, add the street to its corresponding color group
            colorGroups[street->getColorGroup()].push_back(street);
        }
    }
}

// Method to get the index of a square by its name
// Returns -1 if the square is not found
int Board::getSquareIndexByName(const std::string& name) const {
    // Iterate through all squares on the board
    for (size_t i = 0; i < squares.size(); ++i) {
        // If the current square's name matches the given name
        if (squares[i]->getName() == name) {
            // Return the index as an integer
            return static_cast<int>(i);
        }
    }
    // If no match is found, return -1
    return -1;
}

// Method to add a new square to the board at a specific position
// Throws an out_of_range exception if the position is invalid
void Board::addSquare(int position, std::unique_ptr<Square> square) {
    // Check if the position is valid (between 0 and the current board size)
    if (position < 0 || position > static_cast<int>(squares.size())) {
        throw std::out_of_range("Invalid position for new square");
    }
    // Insert the new square at the specified position
    squares.insert(squares.begin() + position, std::move(square));
}

void Board::addTwoSquares(const std::string& type1, const std::string& type2,
                          int location1, int location2,
                          const std::string& name1, const std::string& name2,
                          int price1, int price2) {
    // Define valid insertion locations
    std::vector<int> validLocations = {0, 9, 20, 29};

    // Validate the provided location
    if (std::find(validLocations.begin(), validLocations.end(), location1) == validLocations.end()) {
        throw std::invalid_argument("Invalid location1. Must be 0, 9, 20, or 29.");
    }    
    if (std::find(validLocations.begin(), validLocations.end(), location2) == validLocations.end()) {
        throw std::invalid_argument("Invalid location2. Must be 0, 9, 20, or 29.");
    }

    // Lambda function to create a square based on its type
    auto createSquare = [](const std::string& type, const std::string& name, int price, int location) -> std::unique_ptr<Square> {
        if (type == "Street") {
            // Using adjusted values for color group, house cost, and rent array based on the specific requirements of the new street
            std::vector<ColorGroup> color =  {ColorGroup::Brown, ColorGroup::LightBlue, ColorGroup::Red, ColorGroup::Yellow};
            return std::make_unique<Street>(name, price, color[(int)(location/9)], 50, std::array<int, 6>{2, 10, 30, 90, 160, 250});
        } else if (type == "ChanceSquare") {
            return std::make_unique<ChanceSquare>(name);
        } else if (type == "UtilityProperty") {
            return std::make_unique<UtilityProperty>(name, price);
        } else if (type == "CommunityChestSquare") {
            return std::make_unique<CommunityChestSquare>(name);
        } else {
            throw std::invalid_argument("Invalid square type.");
        }
    };

    // Create the two new squares
    auto square1 = createSquare(type1, name1, price1, location1);
    auto square2 = createSquare(type2, name2, price2, location2);

    // Insert the new squares at the specified location
    squares.insert(squares.begin() + location1 + 1, std::move(square1));
    squares.insert(squares.begin() + location2 + 2, std::move(square2));

    // Update colorGroups if new streets were added
    // This maintains the integrity of the colorGroups map for property management
    if (type1 == "Street") {
        Street* street = dynamic_cast<Street*>(squares[location2 + 2].get());
        if (street) {
            colorGroups[location1 == 0 ? ColorGroup::Red : ColorGroup::Yellow].push_back(street);
        }
    }
    if (type2 == "Street") {
        Street* street = dynamic_cast<Street*>(squares[location2 + 2].get());
        if (street) {
            colorGroups[location2 == 20 ? ColorGroup::Brown : ColorGroup::LightBlue].push_back(street);
        }
    }
}

// Method to get a pointer to a square at a specific position
// Uses normalizePosition to handle wrapping around the board
Square* Board::getSquare(int position) const {
    // Return a pointer to the square at the normalized position
    return squares[normalizePosition(position)].get();
}

// Method to get the total number of squares on the board
int Board::getSize() const {
    // Return the size of the squares vector
    return squares.size();
}

// Method to normalize a position to ensure it's within the board's bounds
// Handles cases where the position exceeds the board size (e.g., after rolling dice)
int Board::normalizePosition(int position) const {
    // Use the modulo operator to wrap the position around the board
    return position % squares.size();
}

// Method to get a vector of Street pointers for a specific color group
// Throws a runtime_error if the color group is invalid
const std::vector<Street*>& Board::getStreetsInColorGroup(ColorGroup group) const {
    // Try to find the color group in the colorGroups map
    auto it = colorGroups.find(group);
    // If the color group is not found, throw an exception
    if (it == colorGroups.end()) {
        throw std::runtime_error("Invalid color group");
    }
    // Return the vector of Street pointers for the found color group
    return it->second;
}


    /**
     * @brief Get a vector of all square names and their color values in board order.
     * @return Vector of pairs, each containing a square name and its color value.
     * @note Non-Street squares are assigned a default color value of 8.
     */

std::vector<std::pair<std::string, int>> Board::getSquareNamesAndColors() const {
    std::vector<std::pair<std::string, int>> squareInfo;
    squareInfo.reserve(squares.size());  // Reserve space for efficiency
    
    // Iterate through all squares on the board
    for (const auto& square : squares) {
        // Attempt to cast the square to a Street pointer
        Street* street = dynamic_cast<Street*>(square.get());
        
        if (street) {
            // If it's a street, use its name and color group
            squareInfo.emplace_back(street->getName(), static_cast<int>(street->getColorGroup()));
        } else {
            // For non-Street squares, use the square's name and default color value of 8
            squareInfo.emplace_back(square->getName(), 8);
        }
    }
    
    return squareInfo;
}