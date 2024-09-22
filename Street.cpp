#include "Street.h"
#include "Player.h"
#include "Game.h"
#include "Board.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

// Constructor for the Street class
// Initializes a street with its name, price, color group, house price, and an array of rent prices
Street::Street(const std::string& name, int price, ColorGroup color, int housePrice, 
               const std::array<int, 6>& rents)
    : Property(name, price, rents[0]), colorGroup(color), housePrice(housePrice),
      rentPrices(rents), numHouses(0), hasHotel(false) {}

// Checks if a player can build a house on this street
bool Street::canBuildHouse(const Player& player) const {
    // Check if the player owns this street and if it doesn't already have max houses or a hotel
    if (owner != &player) return false;
    if (numHouses >= 4 || hasHotel) return false;

    // Check if the player owns all streets in the color group
    const Board& board = player.getGame()->getBoard();
    const auto& streets = board.getStreetsInColorGroup(colorGroup);
    bool ownsAllStreets = std::all_of(streets.begin(), streets.end(),
        [&player](const Street* street) { return street->getOwner() == &player; });
    if (!ownsAllStreets) return false;

    // Check if the number of houses is balanced across the color group
    // This ensures even development across the color group
    return std::all_of(streets.begin(), streets.end(),
        [this](const Street* street) { return street->getNumHouses() >= this->numHouses; });
}

// Builds a hotel on the street
void Street::buildHotel(Player& player) {
    // Check if a hotel can be built and if the player has enough money
    if (!canBuildHotel(player)) {
        throw std::runtime_error("Cannot build hotel on this street");
    }
    int hotelPrice = housePrice + 100;
    if (player.getMoney() < hotelPrice) {
        throw std::runtime_error("Not enough money to build hotel");
    }

    // Remove money from player, reset house count, and set hotel flag
    player.removeMoney(hotelPrice);
    numHouses = 0;
    hasHotel = true;
}

// Builds a house on the street
void Street::buildHouse(Player& player) {
    // Check if a house can be built and if the player has enough money
    if (!canBuildHouse(player)) {
        throw std::runtime_error("Cannot build house on this street");
    }
    if (player.getMoney() < housePrice) {
        throw std::runtime_error("Not enough money to build house");
    }

    // Remove money from player and increment house count
    player.removeMoney(housePrice);
    numHouses++;
}

// Checks if a hotel can be built on this street
bool Street::canBuildHotel(const Player& player) const {
    // Check if the player owns this street, if it has 4 houses, and no hotel yet
    if (owner != &player || numHouses < 4 || hasHotel) return false;

    // Check if all streets in the color group have 4 houses
    // This ensures even development across the color group before building hotels
    const Board& board = player.getGame()->getBoard();
    const auto& streets = board.getStreetsInColorGroup(colorGroup);
    return std::all_of(streets.begin(), streets.end(),
        [](const Street* street) { return street->getNumHouses() == 4; });
}

// Calculates and returns the current rent for the street
int Street::getRent() const {
    if (hasHotel) {
        return rentPrices[5];  // Hotel rent
    } else if (numHouses > 0) {
        return rentPrices[numHouses];  // Rent based on number of houses
    } else {
        return rentPrices[0];  // Base rent
    }
}

// Handles the action when a player lands on this street
void Street::landedOn(Player& player) {
    // If the street is owned by another player, collect rent
    if (owner && owner != &player) {
        int rentToPay = getRent();
        player.removeMoney(rentToPay);
        owner->addMoney(rentToPay);
    } 
    // If unowned or owned by the current player, no action is taken here
}