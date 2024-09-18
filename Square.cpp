#include "Square.h"

Square::Square(const std::string& name) : name(name) {}

std::string Square::getName() const {
    return name;
}

// Note: We've removed the landedOn implementation from here