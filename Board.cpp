#include "Board.h"
#include "ChanceSquare.h"
#include "CommunityChestSquare.h"
#include "UtilityProperty.h"
#include <stdexcept>
#include <iostream>

Board::Board() {
    initializeBoard();
}

void Board::initializeBoard() {
    squares.push_back(std::make_unique<GoSquare>("Go"));
    squares.push_back(std::make_unique<Street>("Mediterranean Avenue", 60, ColorGroup::Brown, 50, 
        std::array<int, 6>{2, 10, 30, 90, 160, 250}));
    squares.push_back(std::make_unique<CommunityChestSquare>("Community Chest"));
    squares.push_back(std::make_unique<Street>("Baltic Avenue", 60, ColorGroup::Brown, 50, 
        std::array<int, 6>{4, 20, 60, 180, 320, 450}));
    squares.push_back(std::make_unique<Tax>("Income Tax", 200));
    squares.push_back(std::make_unique<UtilityProperty>("Reading Railroad", 200));
    squares.push_back(std::make_unique<Street>("Oriental Avenue", 100, ColorGroup::LightBlue, 50, 
        std::array<int, 6>{6, 30, 90, 270, 400, 550}));
    squares.push_back(std::make_unique<ChanceSquare>("Chance"));
    squares.push_back(std::make_unique<Street>("Vermont Avenue", 100, ColorGroup::LightBlue, 50, 
        std::array<int, 6>{6, 30, 90, 270, 400, 550}));
    squares.push_back(std::make_unique<Street>("Connecticut Avenue", 120, ColorGroup::LightBlue, 50, 
        std::array<int, 6>{8, 40, 100, 300, 450, 600}));
    squares.push_back(std::make_unique<Jail>("Jail"));
    squares.push_back(std::make_unique<Street>("St. Charles Place", 140, ColorGroup::Pink, 100, 
        std::array<int, 6>{10, 50, 150, 450, 625, 750}));
    squares.push_back(std::make_unique<UtilityProperty>("Electric Company", 150));
    squares.push_back(std::make_unique<Street>("States Avenue", 140, ColorGroup::Pink, 100, 
        std::array<int, 6>{10, 50, 150, 450, 625, 750}));
    squares.push_back(std::make_unique<Street>("Virginia Avenue", 160, ColorGroup::Pink, 100, 
        std::array<int, 6>{12, 60, 180, 500, 700, 900}));
    squares.push_back(std::make_unique<UtilityProperty>("Pennsylvania Railroad", 200));
    squares.push_back(std::make_unique<Street>("St. James Place", 180, ColorGroup::Orange, 100, 
        std::array<int, 6>{14, 70, 200, 550, 750, 950}));
    squares.push_back(std::make_unique<CommunityChestSquare>("Community Chest"));
    squares.push_back(std::make_unique<Street>("Tennessee Avenue", 180, ColorGroup::Orange, 100, 
        std::array<int, 6>{14, 70, 200, 550, 750, 950}));
    squares.push_back(std::make_unique<Street>("New York Avenue", 200, ColorGroup::Orange, 100, 
        std::array<int, 6>{16, 80, 220, 600, 800, 1000}));
    squares.push_back(std::make_unique<FreeParking>("Free Parking"));
    squares.push_back(std::make_unique<Street>("Kentucky Avenue", 220, ColorGroup::Red, 150, 
        std::array<int, 6>{18, 90, 250, 700, 875, 1050}));
    squares.push_back(std::make_unique<ChanceSquare>("Chance"));
    squares.push_back(std::make_unique<Street>("Indiana Avenue", 220, ColorGroup::Red, 150, 
        std::array<int, 6>{18, 90, 250, 700, 875, 1050}));
    squares.push_back(std::make_unique<Street>("Illinois Avenue", 240, ColorGroup::Red, 150, 
        std::array<int, 6>{20, 100, 300, 750, 925, 1100}));
    squares.push_back(std::make_unique<UtilityProperty>("B. & O. Railroad", 200));
    squares.push_back(std::make_unique<Street>("Atlantic Avenue", 260, ColorGroup::Yellow, 150, 
        std::array<int, 6>{22, 110, 330, 800, 975, 1150}));
    squares.push_back(std::make_unique<Street>("Ventnor Avenue", 260, ColorGroup::Yellow, 150, 
        std::array<int, 6>{22, 110, 330, 800, 975, 1150}));
    squares.push_back(std::make_unique<UtilityProperty>("Water Works", 150));
    squares.push_back(std::make_unique<Street>("Marvin Gardens", 280, ColorGroup::Yellow, 150, 
        std::array<int, 6>{24, 120, 360, 850, 1025, 1200}));
    squares.push_back(std::make_unique<GoToJail>("Go To Jail"));
    squares.push_back(std::make_unique<Street>("Pacific Avenue", 300, ColorGroup::Green, 200, 
        std::array<int, 6>{26, 130, 390, 900, 1100, 1275}));
    squares.push_back(std::make_unique<Street>("North Carolina Avenue", 300, ColorGroup::Green, 200, 
        std::array<int, 6>{26, 130, 390, 900, 1100, 1275}));
    squares.push_back(std::make_unique<CommunityChestSquare>("Community Chest"));
    squares.push_back(std::make_unique<Street>("Pennsylvania Avenue", 320, ColorGroup::Green, 200, 
        std::array<int, 6>{28, 150, 450, 1000, 1200, 1400}));
    squares.push_back(std::make_unique<UtilityProperty>("Short Line", 200));
    squares.push_back(std::make_unique<ChanceSquare>("Chance"));
    squares.push_back(std::make_unique<Street>("Park Place", 350, ColorGroup::DarkBlue, 200, 
        std::array<int, 6>{35, 175, 500, 1100, 1300, 1500}));
    squares.push_back(std::make_unique<Tax>("Luxury Tax", 100));
    squares.push_back(std::make_unique<Street>("Boardwalk", 400, ColorGroup::DarkBlue, 200, 
        std::array<int, 6>{50, 200, 600, 1400, 1700, 2000}));
    
    for (const auto& square : squares) {
        Street* street = dynamic_cast<Street*>(square.get());
        if (street) {
            colorGroups[street->getColorGroup()].push_back(street);
        }
    }
}

int Board::getSquareIndexByName(const std::string& name) const {
    for (size_t i = 0; i < squares.size(); ++i) {
        if (squares[i]->getName() == name) {
            return static_cast<int>(i);
        }
    }
    return -1; // Return -1 if not found
}


void Board::addSquare(int position, std::unique_ptr<Square> square) {
    if (position < 0 || position > static_cast<int>(squares.size())) {
        throw std::out_of_range("Invalid position for new square");
    }
    squares.insert(squares.begin() + position, std::move(square));
}

Square* Board::getSquare(int position) const {
    return squares[normalizePosition(position)].get();
}

int Board::getSize() const {
    return squares.size();
}

int Board::normalizePosition(int position) const {
    return position % squares.size();
}

const std::vector<Street*>& Board::getStreetsInColorGroup(ColorGroup group) const {
    auto it = colorGroups.find(group);
    if (it == colorGroups.end()) {
        throw std::runtime_error("Invalid color group");
    }
    return it->second;
}
