#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include <map>
#include "Square.h"
#include "Property.h"
#include "Street.h"
#include "Jail.h"
#include "FreeParking.h"
#include "GoSquare.h"
#include "GoToJail.h"
#include "Tax.h"

class Board {
private:
    std::vector<std::unique_ptr<Square>> squares;
    std::map<ColorGroup, std::vector<Street*>> colorGroups;

public:
    Board();
    void initializeBoard();
    void addSquare(int position, std::unique_ptr<Square> square);
    void addTwoSquares(const std::string& type1, const std::string& type2,
                       int location1, int location2,
                       const std::string& name1, const std::string& name2,
                       int price1, int price2);
    Square* getSquare(int position) const;
    int getSize() const;
    int normalizePosition(int position) const;
    int getSquareIndexByName(const std::string& name) const;
    const std::vector<Street*>& getStreetsInColorGroup(ColorGroup group) const;
    std::vector<std::pair<std::string, int>> getSquareNamesAndColors() const;

};

#endif // BOARD_H