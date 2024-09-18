#include "Street.h"
#include "Player.h"
#include "Game.h"
#include "Board.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

Street::Street(const std::string& name, int price, ColorGroup color, int housePrice, 
               const std::array<int, 6>& rents)
    : Property(name, price, rents[0]), colorGroup(color), housePrice(housePrice),
      rentPrices(rents), numHouses(0), hasHotel(false) {}

bool Street::canBuildHouse(const Player& player) const {
    if (owner != &player) return false;
    if (numHouses >= 4 || hasHotel) return false;

    // Check if player owns all streets in the color group
    const Board& board = player.getGame()->getBoard();
    const auto& streets = board.getStreetsInColorGroup(colorGroup);
    bool ownsAllStreets = std::all_of(streets.begin(), streets.end(),
        [&player](const Street* street) { return street->getOwner() == &player; });
    if (!ownsAllStreets) return false;

    // Check if the number of houses is balanced across the color group
    return std::all_of(streets.begin(), streets.end(),
        [this](const Street* street) { return street->getNumHouses() >= this->numHouses; });
}

void Street::buildHotel(Player& player) {
    if (!canBuildHotel(player)) {
        throw std::runtime_error("Cannot build hotel on this street");
    }

    int hotelPrice = housePrice + 100;
    if (player.getMoney() < hotelPrice) {
        throw std::runtime_error("Not enough money to build hotel");
    }

    player.removeMoney(hotelPrice);
    numHouses = 0;
    hasHotel = true;
}

void Street::buildHouse(Player& player) {
    if (!canBuildHouse(player)) {
        throw std::runtime_error("Cannot build house on this street");
    }

    if (player.getMoney() < housePrice) {
        throw std::runtime_error("Not enough money to build house");
    }

    player.removeMoney(housePrice);
    numHouses++;
}

bool Street::canBuildHotel(const Player& player) const {
    if (owner != &player || numHouses < 4 || hasHotel) return false;

    // Check if all streets in the color group have 4 houses
    const Board& board = player.getGame()->getBoard();
    const auto& streets = board.getStreetsInColorGroup(colorGroup);
    return std::all_of(streets.begin(), streets.end(),
        [](const Street* street) { return street->getNumHouses() == 4; });
}

int Street::getRent() const {
    if (hasHotel) {
        return rentPrices[5];
    } else if (numHouses > 0) {
        return rentPrices[numHouses];
    } else {
        return rentPrices[0];
    }
}

void Street::landedOn(Player& player) {
    if (owner && owner != &player) {
        int rentToPay = getRent();
        std::cout << player.getName() << " landed on " << getName() << " owned by " << owner->getName() << std::endl;
        std::cout << "Rent to pay: $" << rentToPay << std::endl;

        player.removeMoney(rentToPay);
        owner->addMoney(rentToPay);
        std::cout << player.getName() << " paid $" << rentToPay << " to " << owner->getName() << std::endl;

    } 
    // else 
    // if (!owner) {
    //     // Offer to buy the property
    //     std::cout << getName() << " is unowned. Would " << player.getName() << " like to buy it for $" << getPrice() << "? (y/n): ";
    //     char choice;
    //     std::cin >> choice;

    //     if (choice == 'y' || choice == 'Y') {
    //         if (player.getMoney() >= getPrice()) {
    //             player.removeMoney(getPrice());
    //             setOwner(&player);
    //             player.buyProperty(this);
    //             std::cout << player.getName() << " bought " << getName() << " for $" << getPrice() << std::endl;
    //         } else {
    //             std::cout << player.getName() << " doesn't have enough money to buy " << getName() << std::endl;
    //         }
    //     }
    // }
}