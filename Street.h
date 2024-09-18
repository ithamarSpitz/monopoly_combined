#ifndef STREET_H
#define STREET_H

#include "Property.h"
#include <string>
#include <array>

enum class ColorGroup {
    Brown, LightBlue, Pink, Orange, Red, Yellow, Green, DarkBlue
};

class Street : public Property {
private:
    ColorGroup colorGroup;
    int housePrice;
    std::array<int, 6> rentPrices; // 0: no houses, 1-4: houses, 5: hotel
    int numHouses;
    bool hasHotel;

public:
    Street(const std::string& name, int price, ColorGroup color, int housePrice, 
           const std::array<int, 6>& rents);

    ColorGroup getColorGroup() const { return colorGroup; }
    int getHousePrice() const { return housePrice; }
    int getNumHouses() const { return numHouses; }
    bool getHasHotel() const { return hasHotel; }

    bool canBuildHouse(const Player& player) const;
    void buildHouse(Player& player);
    bool canBuildHotel(const Player& player) const;
    void buildHotel(Player& player);

    int getRent() const override;
    void landedOn(Player& player) override;
};

#endif // STREET_H