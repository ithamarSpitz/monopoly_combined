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
    Square* getSquare(int position) const;
    int getSize() const;
    int normalizePosition(int position) const;
    int getSquareIndexByName(const std::string& name) const;
    const std::vector<Street*>& getStreetsInColorGroup(ColorGroup group) const;
};

#endif // BOARD_H